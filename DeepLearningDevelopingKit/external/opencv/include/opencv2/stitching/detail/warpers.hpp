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

#ifndef OPENCV_STITCHING_WARPERS_HPP
#define OPENCV_STITCHING_WARPERS_HPP

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv_modules.hpp"

namespace cv {
namespace detail {

//! @addtogroup stitching_warp
//! @{

/** @brief Rotation-only model image warper interface.
 */
class CV_EXPORTS RotationWarper
{
public:
    virtual ~RotationWarper() {}

    /** @brief Projects the image point.

    @param pt Source point
    @param K Camera intrinsic parameters
    @param R Camera rotation matrix
    @return Projected point
     */
    virtual Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R) = 0;

    /** @brief Builds the projection maps according to the given camera data.

    @param src_size Source image size
    @param K Camera intrinsic parameters
    @param R Camera rotation matrix
    @param xmap Projection map for the x axis
    @param ymap Projection map for the y axis
    @return Projected image minimum bounding box
     */
    virtual Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) = 0;

    /** @brief Projects the image.

    @param src Source image
    @param K Camera intrinsic parameters
    @param R Camera rotation matrix
    @param interp_mode Interpolation mode
    @param border_mode Border extrapolation mode
    @param dst Projected image
    @return Project image top-left corner
     *