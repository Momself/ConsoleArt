/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_HAL_VSX_UTILS_HPP
#define OPENCV_HAL_VSX_UTILS_HPP

#include "opencv2/core/cvdef.h"

#ifndef SKIP_INCLUDES
#   include <assert.h>
#endif

//! @addtogroup core_utils_vsx
//! @{
#if CV_VSX

#define __VSX_S16__(c, v) (c){v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}
#define __VSX_S8__(c, v)  (c){v, v, v, v, v, v, v, v}
#define __VSX_S4__(c, v)  (c){v, v, v, v}
#define __VSX_S2__(c, v)  (c){v, v}

typedef __vector unsigned char vec_uchar16;
#define vec_uchar16_set(...) (vec_uchar16){__VA_ARGS__}
#define vec_uchar16_sp(c)    (__VSX_S16__(vec_uchar16, c))
#define vec_uchar16_c(v)     ((vec_uchar16)(v))
#define vec_uchar16_mx       vec_uchar16_sp(0xFF)
#define vec_uchar16_mn       vec_uchar16_sp(0)
#define vec_uchar16_z        vec_uchar16_mn

typedef __vector signed char vec_char16;
#define vec_char16_set(...) (vec_char16){__VA_ARGS__}
#define vec_char16_sp(c)    (__VSX_S16__(vec_char16, c))
#define vec_char16_c(v)     ((vec_char16)(v))
#define vec_char16_mx       vec_char16_sp(0x7F)
#define vec_char16_mn       vec_char16_sp(-0x7F-1)
#define vec_char16_z        vec_char16_sp(0)

typedef __vector unsigned short vec_ushort8;
#define vec_ushort8_set(...) (vec_ushort8){__VA_ARGS__}
#define vec_ushort8_sp(c)    (__VSX_S8__(vec_ushort8, c))
#define vec_ushort8_c(v)     ((vec_ushort8)(v))
#define vec_ushort8_mx       vec_ushort8_sp(0xFFFF)
#define vec_ushort8_mn       vec_ushort8_sp(0)
#define vec_ushort8_z        vec_ushort8_mn

typedef __vector signed short vec_short8;
#define vec_short8_set(...) (vec_short8){__VA_ARGS__}
#define vec_short8_sp(c)    (__VSX_S8__(vec_short8, c))
#define vec_short8_c(v)     ((vec_short8)(v))
#define vec_short8_mx       vec_short8_sp(0x7FFF)
#define vec_short8_mn       vec_short8_sp(-0x7FFF-1)
#define vec_short8_z        vec_short8_sp(0)

typedef __vector unsigned int vec_uint4;
#define vec_uint4_set(...) (vec_uint4){__VA_ARGS__}
#define vec_uint4_sp(c)    (__VSX_S4__(vec_uint4, c))
#define vec_uint4_c(v)     ((vec_uint4)(v))
#define vec_uint4_mx       vec_uint4_sp(0xFFFFFFFFU)
#define vec_uint4_mn       vec_uint4_sp(0)
#define vec_uint4_z        vec_uint4_mn

typedef __vector signed int vec_int4;
#define vec_int4_set(...)  (vec_int4){__VA_ARGS__}
#define vec_int4_sp(c)     (__VSX_S4__(vec_int4, c))
#define vec_int4_c(v)      ((vec_int4)(v))
#define vec_int4_mx        vec_int4_sp(0x7FFFFFFF)
#define vec_int4_mn        vec_int4_sp(-0x7FFFFFFF-1)
#define vec_int4_z         vec_int4_sp(0)

typedef __vector float vec_float4;
#define vec_float4_set(...)  (vec_float4){__VA_ARGS__}
#define vec_float4_sp(c)     (__VSX_S4__(vec_float4, c))
#define vec_float4_c(v)      ((vec_float4)(v))
#define vec_float4_mx        vec_float4_sp(3.40282347E+38F)
#define vec_float4_mn        vec_float4_sp(1.17549435E-38F)
#define vec_float4_z         vec_float4_sp(0)

typedef __vector unsigned long long vec_udword2;
#define vec_udword2_set(...) (vec_udword2){__VA_ARGS__}
#define vec_udword2_sp(c)    (__VSX_S2__(vec_udword2, c))
#define vec_udword2_c(v)     ((vec_udword2)(v))
#define vec_udword2_mx       vec_udword2_sp(18446744073709551615ULL)
#define vec_udword2_mn       vec_udword2_sp(0)
#define vec_udword2_z        vec_udword2_mn

typedef __vector signed long long vec_dword2;
#define vec_dword2_set(...) (vec_dword2){__VA_ARGS__}
#define vec_dword2_sp(c)    (__VSX_S2__(vec_dword2, c))
#define vec_dword2_c(v)     ((vec_dword2)(v))
#define vec_dword2_mx       vec_dword2_sp(9223372036854775807LL)
#define vec_dword2_mn       vec_dword2_sp(-9223372036854775807LL-1)
#define vec_dword2_z        vec_dword2_sp(0)

typedef  __vector double vec_double2;
#define vec_double2_set(...) (vec_double2){__VA_ARGS__}
#define vec_double2_c(v)     ((vec_double2)(v))
#define vec_double2_sp(c)    (__VSX_S2__(vec_double2, c))
#define vec_double2_mx       vec_double2_sp(1.7976931348623157E+308)
#define vec_double2_mn       vec_double2_sp(2.2250738585072014E-308)
#define vec_double2_z        vec_double2_sp(0)

#define vec_bchar16           __vector __bool char
#define vec_bchar16_set(...) (vec_bchar16){__VA_ARGS__}
#define vec_bchar16_c(v)     ((vec_bchar16)(v))
#define vec_bchar16_f        (__VSX_S16__(vec_bchar16, 0))
#define vec_bchar16_t        (__VSX_S16__(vec_bchar16, 1))

#define vec_bshort8           __vector __bool short
#define vec_bshort8_set(...) (vec_bshort8){__VA_ARGS__}
#define vec_bshort8_c(v)     ((vec_bshort8)(v))
#define vec_bshort8_f        (__VSX_S8__(vec_bshort8, 0))
#define vec_bshort8_t        (__VSX_S8__(vec_bshort8, 1))

#define vec_bint4             __vector __bool int
#define vec_bint4_set(...)   (vec_bint4){__VA_ARGS__}
#define vec_bint4_c(v)       ((vec_bint4)(v))
#define vec_bint4_f          (__VSX_S4__(vec_bint4, 0))
#define vec_bint4_t          (__VSX_S4__(vec_bint4, 1))

#define vec_bdword2            __vector __bool long long
#define vec_bdword2_set(...)  (vec_bdword2){__VA_ARGS__}
#define vec_bdword2_c(v)      ((vec_bdword2)(v))
#define vec_bdword2_f         (__VSX_S2__(vec_bdword2, 0))
#define vec_bdword2_t         (__VSX_S2__(vec_bdword2, 1))


#define VSX_FINLINE(tp) extern inline tp __attribute__((always_inline))

#define VSX_REDIRECT_1RG(rt, rg, fnm, fn2)   \
VSX_FINLINE(rt) fnm(const rg& a) { return fn2(a); }

#define VSX_REDIRECT_2RG(rt, rg, fnm, fn2)   \
VSX_FINLINE(rt) fnm(const rg& a, const rg& b) { return fn2(a, b); }

/*
 * GCC VSX compatibility
**/
#if defined(__GNUG__) && !defined(__clang__)

// inline asm helper
#define VS