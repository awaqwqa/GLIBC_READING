/* Multiple versions of memchr. AARCH64 version.
   Copyright (C) 2018-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* Define multiple versions only for the definition in libc.  */

#if IS_IN (libc)
/* Redefine memchr so that the compiler won't complain about the type
   mismatch with the IFUNC selector in strong_alias, below.  */
# undef memchr
# define memchr __redirect_memchr
# include <string.h>
# include <init-arch.h>

extern __typeof (__redirect_memchr) __memchr;

extern __typeof (__redirect_memchr) __memchr_generic attribute_hidden;
extern __typeof (__redirect_memchr) __memchr_nosimd attribute_hidden;

libc_ifunc (__memchr,
	    ((IS_EMAG (midr)
	       ? __memchr_nosimd
	       : __memchr_generic)));

# undef memchr
strong_alias (__memchr, memchr);
#endif
