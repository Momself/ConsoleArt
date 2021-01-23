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

#ifndef OPENCV_CORE_MATX_HPP
#define OPENCV_CORE_MATX_HPP

#ifndef __cplusplus
#  error matx.hpp header must be compiled as C++
#endif

#include "opencv2/core/cvdef.h"
#include "opencv2/core/base.hpp"
#include "opencv2/core/traits.hpp"
#include "opencv2/core/saturate.hpp"

#ifdef CV_CXX11
#include <initializer_list>
#endif

namespace cv
{

//! @addtogroup core_basic
//! @{

////////////////////////////// Small Matrix ///////////////////////////

//! @cond IGNORED
struct CV_EXPORTS Matx_AddOp {};
struct CV_EXPORTS Matx_SubOp {};
struct CV_EXPORTS Matx_ScaleOp {};
struct CV_EXPORTS Matx_MulOp {};
struct CV_EXPORTS Matx_DivOp {};
struct CV_EXPORTS Matx_MatMulOp {};
struct CV_EXPORTS Matx_TOp {};
//! @endcond

/** @brief Template class for small matrices whose type and size are known at compilation time

If you need a more flexible type, use Mat . The elements of the matrix M are accessible using the
M(i,j) notation. Most of the common matrix operations (see also @ref MatrixExpressions ) are
available. To do an operation on Matx that is not implemented, you can easily convert the matrix to
Mat and backwards:
@code{.cpp}
    Matx33f m(1, 2, 3,
              4, 5, 6,
              7, 8, 9);
    cout << sum(Mat(m*m.t())) << endl;
@endcode
Except of the plain constructor which takes a list of elements, Matx can be initialized from a C-array:
@code{.cpp}
    float values[] = { 1, 2, 3};
    Matx31f m(values);
@endcode
In case if C++11 features are available, std::initializer_list can be also used to initialize Matx:
@code{.cpp}
    Matx31f m = { 1, 2, 3};
@endcode
 */
template<typename _Tp, int m, int n> class Matx
{
public:
    enum {
           rows     = m,
           cols     = n,
           channels = rows*cols,
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           depth    = traits::Type<_Tp>::value,
           type     = CV_MAKETYPE(depth, channels),
#endif
           shortdim = (m < n ? m : n)
         };

    typedef _Tp                           value_type;
    typedef Matx<_Tp, m, n>               mat_type;
    typedef Matx<_Tp, shortdim, 1> diag_type;

    //! default constructor
  