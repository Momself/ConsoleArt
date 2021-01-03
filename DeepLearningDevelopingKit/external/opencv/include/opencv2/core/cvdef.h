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

#ifndef OPENCV_CORE_CVDEF_H
#define OPENCV_CORE_CVDEF_H

//! @addtogroup core_utils
//! @{

#if !defined CV_DOXYGEN && !defined CV_IGNORE_DEBUG_BUILD_GUARD
#if (defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))) || \
    (defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_DEBUG_PEDANTIC))
// Guard to prevent using of binary incompatible binaries / runtimes
// https://github.com/opencv/opencv/pull/9161
#define CV__DEBUG_NS_BEGIN namespace debug_build_guard {
#define CV__DEBUG_NS_END }
namespace cv { namespace debug_build_guard { } using namespace debug_build_guard; }
#endif
#endif

#ifndef CV__DEBUG_NS_BEGIN
#define CV__DEBUG_NS_BEGIN
#define CV__DEBUG_NS_END
#endif


#ifdef __OPENCV_BUILD
#include "cvconfig.h"
#endif

#ifndef __CV_EXPAND
#define __CV_EXPAND(x) x
#endif

#ifndef __CV_CAT
#define __CV_CAT__(x, y) x ## y
#define __CV_CAT_(x, y) __CV_CAT__(x, y)
#define __CV_CAT(x, y) __CV_CAT_(x, y)
#endif


// undef problematic defines sometimes defined by system headers (windows.h in particular)
#undef small
#undef min
#undef max
#undef abs
#undef Complex

#include <limits.h>
#include "opencv2/core/hal/interface.h"

#if defined __ICL
#  define CV_ICC   __ICL
#elif defined __ICC
#  define CV_ICC   __ICC
#elif defined __ECL
#  define CV_ICC   __ECL
#elif defined __ECC
#  define CV_ICC   __ECC
#elif defined __INTEL_COMPILER
#  define CV_ICC   __INTEL_COMPILER
#endif

#ifndef CV_INLINE
#  if defined __cplusplus
#    define CV_INLINE static inline
#  elif defined _MSC_VER
#    define CV_INLINE __inline
#  else
#    define CV_INLINE static
#  endif
#endif

#if defined CV_DISABLE_OPTIMIZATION || (defined CV_ICC && !defined CV_ENABLE_UNROLLED)
#  define CV_ENABLE_UNROLLED 0
#else
#  define CV_ENABLE_UNROLLED 1
#endif

#ifdef __GNUC__
#  define CV_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#  define CV_DECL_ALIGNED(x) __declspec(align(x))
#else
#  define CV_DECL_ALIGNED(x)
#endif

/* CPU features and intrinsics support */
#define CV_CPU_NONE             0
#define CV_CPU_MMX              1
#define CV_CPU_SSE              2
#define CV_CPU_SSE2             3
#define CV_CPU_SSE3             4
#define CV_CPU_SSSE3            5
#define CV_CPU_SSE4_1           6
#define CV_CPU_SSE4_2           7
#define CV_CPU_POPCNT           8
#define CV_CPU_FP16             9
#define CV_CPU_AVX              10
#define CV_CPU_AVX2             11
#define CV_CPU_FMA3             12

#define CV_CPU_AVX_512F         13
#define CV_CPU_AVX_512BW        14
#define CV_CPU_AVX_512CD        15
#define CV_CPU_AVX_512DQ        16
#define CV_CPU_AVX_512ER        17
#define CV_CPU_AVX_512IFMA512   18 // deprecated
#define CV_CPU_AVX_512IFMA      18
#define CV_CPU_AVX_512PF        19
#define CV_CPU_AVX_512VBMI      20
#define CV_CPU_AVX_512VL        21

#define CV_CPU_NEON   100

#define CV_CPU_VSX 200

// CPU features groups
#define CV_CPU_AVX512_SKX       256

// when adding to this list remember to update the following enum
#define CV_HARDWARE_MAX_FEATURE 512

/** @brief Available CPU features.
*/
enum CpuFeatures {
    CPU_MMX             = 1,
    CPU_SSE             = 2,
    CPU_SSE2            = 3,
    CPU_SSE3            = 4,
    CPU_SSSE3           = 5,
    CPU_SSE4_1          = 6,
    CPU_SSE4_2          = 7,
    CPU_POPCNT          = 8,
    CPU_FP16            = 9,
    CPU_AVX             = 10,
    CPU_AVX2            = 11,
    CPU_FMA3            = 12,

    CPU_AVX_512F        = 13,
    CPU_AVX_512BW       = 14,
    CPU_AVX_512CD       = 15,
    CPU_AVX_512DQ       = 16,
    CPU_AVX_512ER       = 17,
    CPU_AVX_512IFMA512  = 18, // deprecated
    CPU_AVX_512IFMA     = 18,
    CPU_AVX_512PF       = 19,
    CPU_AVX_512VBMI     = 20,
    CPU_AVX_512VL       = 21,

    CPU_NEON            = 100,

    CPU_VSX             = 200,

    CPU_AVX512_SKX      = 256, //!< Skylake-X with AVX-512F/CD/BW/DQ/VL

    CPU_MAX_FEATURE     = 512  // see CV_HARDWARE_MA