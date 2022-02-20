/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
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

#ifndef OPENCV_IMGPROC_TYPES_C_H
#define OPENCV_IMGPROC_TYPES_C_H

#include "opencv2/core/core_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup imgproc_c
  @{
*/

/** Connected component structure */
typedef struct CvConnectedComp
{
    double area;    /**<area of the connected component  */
    CvScalar value; /**<average color of the connected component */
    CvRect rect;    /**<ROI of the component  */
    CvSeq* contour; /**<optional component boundary
                      (the contour might have child contours corresponding to the holes)*/
}
CvConnectedComp;

/** Image smooth methods */
enum SmoothMethod_c
{
    /** linear convolution with \f$\texttt{size1}\times\texttt{size2}\f$ box kernel (all 1's). If
    you want to smooth different pixels with different-size box kernels, you can use the integral
    image that is computed using integral */
    CV_BLUR_NO_SCALE =0,
    /** linear convolution with \f$\texttt{size1}\times\texttt{size2}\f$ box kernel (all
    1's) with subsequent scaling by \f$1/(\texttt{size1}\cdot\texttt{size2})\f$ */
    CV_BLUR  =1,
    /** linear convolution with a \f$\texttt{size1}\times\texttt{size2}\f$ Gaussian kernel */
    CV_GAUSSIAN  =2,
    /** median filter with a \f$\texttt{size1}\times\texttt{size1}\f$ square aperture */
    CV_MEDIAN =3,
    /** bilateral filter with a \f$\texttt{size1}\times\texttt{size1}\f$ square aperture, color
    sigma= sigma1 and spatial sigma= sigma2. If size1=0, the aperture square side is se