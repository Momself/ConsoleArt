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

#ifndef OPENCV_CORE_AFFINE3_HPP
#define OPENCV_CORE_AFFINE3_HPP

#ifdef __cplusplus

#include <opencv2/core.hpp>

namespace cv
{

//! @addtogroup core
//! @{

    /** @brief Affine transform
     *
     * It represents a 4x4 homogeneous transformation matrix \f$T\f$
     *
     *  \f[T =
     *  \begin{bmatrix}
     *  R & t\\
     *  0 & 1\\
     *  \end{bmatrix}
     *  \f]
     *
     *  where \f$R\f$ is a 3x3 rotation matrix and \f$t\f$ is a 3x1 translation vector.
     *
     *  You can specify \f$R\f$ either by a 3x3 rotation matrix or by a 3x1 rotation vector,
     *  which is converted to a 3x3 rotation matrix by the Rodrigues formula.
     *
     *  To construct a matrix \f$T\f$ representing first rotation around the axis \f$r\f$ with rotation
     *  angle \f$|r|\f$ in radian (right hand rule) and then translation by the vector \f$t\f$, you can use
     *
     *  @code
     *  cv::Vec3f r, t;
     *  cv::Affine3f T(r, t);
     *  @endcode
     *
     *  If you already have the rotation matrix \f$R\f$, then you can use
     *
     *  @code
     *  cv::Matx33f R;
     *  cv::Affine3f T(R, t);
     *  @endcode
     *
     *  To extract the rotation matrix \f$R\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Matx33f R = T.rotation();
     *  @endcode
     *
     *  To extract the translation vector \f$t\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Vec3f t = T.translation();
     *  @endcode
     *
     *  To extract the rotation vector \f$r\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Vec3f r = T.rvec();
     *  @endcode
     *
     *  Note that since the mapping from rotation vectors to rotation matrices
     *  is many to one. The returned rotation vector is not necessarily the one
     *  you us