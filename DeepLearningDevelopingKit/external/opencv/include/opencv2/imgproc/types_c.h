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
    sigma= sigma1 and spatial sigma= sigma2. If size1=0, the aperture square side is set to
    cvRound(sigma2\*1.5)\*2+1. See cv::bilateralFilter */
    CV_BILATERAL =4
};

/** Filters used in pyramid decomposition */
enum
{
    CV_GAUSSIAN_5x5 = 7
};

/** Special filters */
enum
{
    CV_SCHARR =-1,
    CV_MAX_SOBEL_KSIZE =7
};

/** Constants for color conversion */
enum
{
    CV_BGR2BGRA    =0,
    CV_RGB2RGBA    =CV_BGR2BGRA,

    CV_BGRA2BGR    =1,
    CV_RGBA2RGB    =CV_BGRA2BGR,

    CV_BGR2RGBA    =2,
    CV_RGB2BGRA    =CV_BGR2RGBA,

    CV_RGBA2BGR    =3,
    CV_BGRA2RGB    =CV_RGBA2BGR,

    CV_BGR2RGB     =4,
    CV_RGB2BGR     =CV_BGR2RGB,

    CV_BGRA2RGBA   =5,
    CV_RGBA2BGRA   =CV_BGRA2RGBA,

    CV_BGR2GRAY    =6,
    CV_RGB2GRAY    =7,
    CV_GRAY2BGR    =8,
    CV_GRAY2RGB    =CV_GRAY2BGR,
    CV_GRAY2BGRA   =9,
    CV_GRAY2RGBA   =CV_GRAY2BGRA,
    CV_BGRA2GRAY   =10,
    CV_RGBA2GRAY   =11,

    CV_BGR2BGR565  =12,
    CV_RGB2BGR565  =13,
    CV_BGR5652BGR  =14,
    CV_BGR5652RGB  =15,
    CV_BGRA2BGR565 =16,
    CV_RGBA2BGR565 =17,
    CV_BGR5652BGRA =18,
    CV_BGR5652RGBA =19,

    CV_GRAY2BGR565 =20,
    CV_BGR5652GRAY =21,

    CV_BGR2BGR555  =22,
    CV_RGB2BGR555  =23,
    CV_BGR5552BGR  =24,
    CV_BGR5552RGB  =25,
    CV_BGRA2BGR555 =26,
    CV_RGBA2BGR555 =27,
    CV_BGR5552BGRA =28,
    CV_BGR5552RGBA =29,

    CV_GRAY2BGR555 =30,
    CV_BGR5552GRAY =31,

    CV_BGR2XYZ     =32,
    CV_RGB2XYZ     =33,
    CV_XYZ2BGR     =34,
    CV_XYZ2RGB     =35,

    CV_BGR2YCrCb   =36,
    CV_RGB2YCrCb   =37,
    CV_YCrCb2BGR   =38,
    CV_YCrCb2RGB   =39,

    CV_BGR2HSV     =40,
    CV_RGB2HSV     =41,

    CV_BGR2Lab     =44,
    CV_RGB2Lab     =45,

    CV_BayerBG2BGR =46,
    CV_BayerGB2BGR =47,
    CV_BayerRG2BGR =48,
    CV_BayerGR2BGR =49,

    CV_BayerBG2RGB =CV_BayerRG2BGR,
    CV_BayerGB2RGB =CV_BayerGR2BGR,
    CV_BayerRG2RGB =CV_BayerBG2BGR,
    CV_BayerGR2RGB =CV_BayerGB2BGR,

    CV_BGR2Luv     =50,
    CV_RGB2Luv     =51,
    CV_BGR2HLS     =52,
    CV_RGB2HLS     =53,

    CV_HSV2BGR     =54,
    CV_HSV2RGB     =55,

    CV_Lab2BGR     =56,
    CV_Lab2RGB     =57,
    CV_Luv2BGR     =58,
    CV_Luv2RGB     =59,
    CV_HLS2BGR     =60,
    CV_HLS2RGB     =61,

    CV_BayerBG2BGR_VNG =62,
    CV_BayerGB2BGR_VNG =63,
    CV_BayerRG2BGR_VNG =64,
    CV_BayerGR2BGR_VNG =65,

    CV_BayerBG2RGB_VNG =CV_BayerRG2BGR_VNG,
    CV_BayerGB2RGB_VNG =CV_BayerGR2BGR_VNG,
    CV_BayerRG2RGB_VNG =CV_BayerBG2BGR_VNG,
    CV_BayerGR2RGB_VNG =CV_BayerGB2BGR_VNG,

    CV_BGR2HSV_FULL = 66,
    CV_RGB2HSV_FULL = 67,
    CV_BGR2HLS_FULL = 68,
    CV_RGB2HLS_FULL = 69,

    CV_HSV2BGR_FULL = 70,
    CV_HSV2RGB_FULL = 71,
    CV_HLS2BGR_FULL = 72,
    CV_HLS2RGB_FULL = 73,

    CV_LBGR2Lab     = 74,
    CV_LRGB2Lab     = 75,
    CV_LBGR2Luv     = 76,
    CV_LRGB2Luv     = 77,

    CV_Lab2LBGR     = 78,
    CV_Lab2LRGB     = 79,
    CV_Luv2LBGR     = 80,
    CV_Luv2LRGB     = 81,

    CV_BGR2YUV      = 82,
    CV_RGB2YUV      = 83,
    CV_YUV2BGR      = 84,
    CV_YUV2RGB      = 85,

    CV_BayerBG2GRAY = 86,
    CV_BayerGB2GRAY = 87,
    CV_BayerRG2GRAY = 88,
    CV_BayerGR2GRAY = 89,

    //YUV 4:2:0 formats family
    CV_YUV2RGB_NV12 = 90,
    CV_YUV2BGR_NV12 = 91,
    CV_YUV2RGB_NV21 = 92,
    CV_YUV2BGR_NV21 = 93,
    CV_YUV420sp2RGB = CV_YUV2RGB_NV21,
    CV_YUV420sp2BGR = CV_YUV2BGR_NV21,

    CV_YUV2RGBA_NV12 = 94,
    CV_YUV2BGRA_NV12 = 95,
    CV_YUV2RGBA_NV21 = 96,
    CV_YUV2BGRA_NV21 = 97,
    CV_YUV420sp2RGBA = CV_YUV2RGBA_NV21,
    CV_YUV420sp2BGRA = CV_YUV2BGRA_NV21,

    CV_YUV2RGB_YV12 = 98,
    CV_YUV2BGR_YV12 = 99,
    CV_YUV2RGB_IYUV = 100,
    CV_YUV2BGR_IYUV = 101,
    CV_YUV2RGB_I420 = CV_YUV2RGB_IYUV,
    CV_YUV2BGR_I420 = CV_YUV2BGR_IYUV,
    CV_YUV420p2RGB = CV_YUV2RGB_YV12,
    CV_YUV420p2BGR = CV_YUV2BGR_YV12,

    CV_YUV2RGBA_YV12 = 102,
    CV_YUV2BGRA_YV12 = 103,
    CV_YUV2RGBA_IYUV = 104,
    CV_YUV2BGRA_IYUV = 105,
    CV_YUV2RGBA_I420 = CV_YUV2RGBA_IYUV,
    CV_YUV2BGRA_I420 = CV_YUV2BGRA_IYUV,
    CV_YUV420p2RGBA = CV_YUV2RGBA_YV12,
    CV_YUV420p2BGRA = CV_YUV2BGRA_YV12,

    CV_YUV2GRAY_420 = 106,
    CV_YUV2GRAY_NV21 = CV_YUV2GRAY_420,
    CV_YUV2GRAY_NV12 = CV_YUV2GRAY_420,
    CV_YUV2GRAY_YV12 = CV_YUV2GRAY_420,
    CV_YUV2GRAY_IYUV = CV_YUV2GRAY_420,
    CV_YUV2GRAY_I420 = CV_YUV2GRAY_420,
    CV_YUV420sp2GRAY = CV_YUV2GRAY_420,
    CV_YUV420p2GRAY = CV_YUV2GRAY_420,

    //YUV 4:2:2 formats family
    CV_YUV2RGB_UYVY = 107,
    CV_YUV2BGR_UYVY = 108,
    //CV_YUV2RGB_VYUY = 109,
    //CV_YUV2BGR_VYUY = 110,
    CV_YUV2RGB_Y422 = CV_YUV2RGB_UYVY,
    CV_YUV2BGR_Y422 = CV_YUV2BGR_UYVY,
    CV_YUV2RGB_UYNV = CV_YUV2RGB_UYVY,
    CV_YUV2BGR_UYNV = CV_YUV2BGR_UYVY,

    CV_YUV2RGBA_UYVY = 111,
    CV_YUV2BGRA_UYVY = 112,
    //CV_YUV2RGBA_VYUY = 113,
    //CV_YUV2BGRA_VYUY = 114,
    CV_YUV2RGBA_Y422 = CV_YUV2RGBA_UYVY,
    CV_YUV2BGRA_Y422 = CV_YUV2BGRA_UYVY,
    CV_YUV2RGBA_UYNV = CV_YUV2RGBA_UYVY,
    CV_YUV2BGRA_UYNV = CV_YUV2BGRA_UYVY,

    CV_YUV2RGB_YUY2 = 115,
    CV_YUV2BGR_YUY2 = 116,
    CV_YUV2RGB_YVYU = 117,
    CV_YUV2BGR_YVYU = 118,
    CV_YUV2RGB_YUYV = CV_YUV2RGB_YUY2,
    CV_YUV2BGR_YUYV = CV_YUV2BGR_YUY2,
    CV_YUV2RGB_YUNV = CV_YUV2RGB_YUY2,
    CV_YUV2BGR_YUNV = CV_YUV2BGR_YUY2,

    CV_YUV2RGBA_YUY2 = 119,
    CV_YUV2BGRA_YUY2 = 120,
    CV_YUV2RGBA_YVYU = 121,
    CV_YUV2BGRA_YVYU = 122,
    CV_YUV2RGBA_YUYV = CV_YUV2RGBA_YUY2,
    CV_YUV2BGRA_YUYV = CV_YUV2BGRA_YUY2,
    CV_YUV2RGBA_YUNV = CV_YUV2RGBA_YUY2,
    CV_YUV2BGRA_YUNV = CV_YUV2BGRA_YUY2,

    CV_YUV2GRAY_UYVY = 123,
    CV_YUV2GRAY_YUY2 = 124,
    //CV_YUV2GRAY_VYUY = CV_YUV2GRAY_UYVY,
    CV_YUV2GRAY_Y422 = CV_YUV2GRAY_UYVY,
    CV_YUV2GRAY_UYNV = CV_YUV2GRAY_UYVY,
    CV_YUV2GRAY_YVYU = CV_YUV2GRAY_YUY2,
    CV_YUV2GRAY_YUYV = CV_YUV2GRAY_YUY2,
    CV_YUV2GRAY_YUNV = CV_YUV2GRAY_YUY2,

    // alpha premultiplication
    CV_RGBA2mRGBA = 125,
    CV_mRGBA2RGBA = 126,

    CV_RGB2YUV_I420 = 127,
    CV_BGR2YUV_I420 = 128,
    CV_RGB2YUV_IYUV = CV_RGB2YUV_I420,
    CV_BGR2YUV_IYUV = CV_BGR2YUV_I420,

    CV_RGBA2YUV_I420 = 129,
    CV_BGRA2YUV_I420 = 130,
    CV_RGBA2YUV_IYUV = CV_RGBA2YUV_I420,
    CV_BGRA2YUV_IYUV = CV_BGRA2YUV_I420,
    CV_RGB2YUV_YV12  = 131,
    CV_BGR2YUV_YV12  = 132,
    CV_RGBA2YUV_YV12 = 133,
    CV_BGRA2YUV_YV12 = 134,

    // Edge-Aware Demosaicing
    CV_BayerBG2BGR_EA = 135,
    CV_BayerGB2BGR_EA = 136,
    CV_BayerRG2BGR_EA = 137,
    CV_BayerGR2BGR_EA = 138,

    CV_BayerBG2RGB_EA = CV_BayerRG2BGR_EA,
    CV_BayerGB2RGB_EA = CV_BayerGR2BGR_EA,
    CV_BayerRG2RGB_EA = CV_BayerBG2BGR_EA,
    CV_BayerGR2RGB_EA = CV_BayerGB2BGR_EA,

    CV_BayerBG2BGRA =139,
    CV_BayerGB2BGRA =140,
    CV_BayerRG2BGRA =141,
    CV_BayerGR2BGRA =142,

    CV_BayerBG2RGBA =CV_BayerRG2BGRA,
    CV_BayerGB2RGBA =CV_BayerGR2BGRA,
    CV_BayerRG2RGBA =CV_BayerBG2BGRA,
    CV_BayerGR2RGBA =CV_BayerGB2BGRA,

    CV_COLORCVT_MAX  = 143
};


/** Sub-pixel interpolation methods */
enum
{
    CV_INTER_NN        =0,
    CV_INTER_LINEAR    =1,
    CV_INTER_CUBIC     =2,
    CV_INTER_AREA      =3,
    CV_INTER_LANCZOS4  =4
};

/** ... and other image warping flags */
enum
{
    CV_WARP_FILL_OUTLIERS =8,
    CV_WARP_INVERSE_MAP  =16
};

/** Shapes of a structuring element for morphological operations
@see cv::MorphShapes, cv::getStructuringElement
*/
enum MorphShapes_c
{
    CV_SHAPE_RECT      =0,
    CV_SHAPE_CROSS     =1,
    CV_SHAPE_ELLIPSE   =2,
    CV_SHAPE_CUSTOM    =100 //!< custom structuring element
};

/** Morphological operations */
enum
{
    CV_MOP_ERODE        =0,
    CV_MOP_DILATE       =1,
    CV_MOP_OPEN         =2,
    CV_MOP_CLOSE        =3,
    CV_MOP_GRADIENT     =4,
    CV_MOP_TOPHAT       =5,
    CV_MOP_BLACKHAT     =6
};

/** Spatial and central moments */
typedef struct CvMoments
{
    double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03; /**< spatial moments */
    double  mu20, mu11, mu02, mu30, mu21, mu12, mu03; /**< central moments */
    double  inv_sqrt_m00; /**< m00 != 0 ? 1/sqrt(m00) : 0 */

#ifdef __cp