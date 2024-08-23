/* Float ceil function, sparc64 version.
   Copyright (C) 2016-2022 Free Software Foundation, Inc.
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

#define NO_MATH_REDIRECT
#include <sparc-ifunc.h>
#include <math.h>
#include <libm-alias-float.h>

extern __typeof (ceilf) __ceilf_vis3 attribute_hidden;
extern __typeof (ceilf) __ceilf_generic attribute_hidden;

sparc_libm_ifunc (__ceilf,
		  hwcap & HWCAP_SPARC_VIS3
		  ? __ceilf_vis3
		  : __ceilf_generic);
libm_alias_float (__ceil, ceil)
