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
#defi