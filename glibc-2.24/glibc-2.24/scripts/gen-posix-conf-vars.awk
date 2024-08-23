# Generate posix-conf-vars-def.h with definitions for CONF_DEF{CONF} for each
# configuration variable that getconf or sysconf may use.  Currently it is
# equipped only to generate such macros for specification macros and for
# SYSCONF macros in the _POSIX namespace.

BEGIN {
  prefix = ""
}

$1 ~ /^#/ || $0 ~ /^\s*$/ {
  next
}

# Begin a new prefix.
$NF == "{" {
  type = $1
  prefix = $2

  if (NF == 4)
    sc_prefix = $3
  else
    sc_prefix = "_SC"

  next
}

$1 == "}" {
  prefix = ""
  type = ""
  sc_prefix = ""
  next
}

{
  if (prefix == "" && type == "" && sc_prefix == "") {
    printf ("Syntax error at %s:%d\n", FILENAME, FNR) > "/dev/stderr"
    exit 1
  }

  # The prefix and variable names are indices and the value indicates what type
  # of variable it is.  The possible options are:
  # CONFSTR: A configuration string
  # SYSCONF: A numeric value
  # SPEC: A specification
  c = prefix "_" $1
  sc_prefixes[c] = sc_prefix
  prefix_conf[c] = type
  conf[c] = $1
}

END {
  print "/* AUTOGENERATED by gen-posix-conf-vars.awk.  DO NOT EDIT.  */\n"

  # Generate macros that specify if a sysconf macro is defined and/or set.
  for (c in prefix_conf) {
    printf "#ifndef _%s\n", c
    printf "# define CONF_DEF_%s CONF_DEF_UNDEFINED\n", c
    # CONFSTR have string values and they are not set or unset.
    if (prefix_conf[c] != "CONFSTR") {
      printf "#else\n"
      printf "# if _%s > 0\n", c
      printf "#  define CONF_DEF_%s CONF_DEF_DEFINED_SET\n", c
      printf "# else\n"
      printf "#  define CONF_DEF_%s CONF_DEF_DEFINED_UNSET\n", c
      printf "# endif\n"
    }
    printf "#endif\n\n"

    # Build a name -> sysconf number associative array to print a C array at
    # the end.
    if (prefix_conf[c] == "SPEC")
      spec[c] = sc_prefixes[c] "_" conf[c]
  }

  # Print the specification array.  Define the macro NEED_SPEC_ARRAY before
  # including posix-conf-vars.h to make it available in the compilation unit.
  print "#if NEED_SPEC_ARRAY"
  print "static const struct { const char *name; int num; } specs[] ="
  print "  {"
  for (s in spec) {
    printf "    { \"%s\", %s },\n", s, spec[s]
  }
  print "  };"
  print "static const size_t nspecs = sizeof (specs) / sizeof (specs[0]);"
  print "#endif"
}