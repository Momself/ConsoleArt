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

#ifndef OPENCV_VIDEOIO_HPP
#define OPENCV_VIDEOIO_HPP

#include "opencv2/core.hpp"

/**
  @defgroup videoio Video I/O

  @brief Read and write video or images sequence with OpenCV

  ### See also:
  - @ref videoio_overview
  - Tutorials: @ref tutorial_table_of_content_videoio
  @{
    @defgroup videoio_flags_base Flags for video I/O
    @defgroup videoio_flags_others Additional flags for video I/O API backends
    @defgroup videoio_c C API for video I/O
    @defgroup videoio_ios iOS glue for video I/O
    @defgroup videoio_winrt WinRT glue for video I/O
  @}
*/

////////////////////////////////// video io /////////////////////////////////

typedef struct CvCapture CvCapture;
typedef struct CvVideoWriter CvVideoWriter;

namespace cv
{

//! @addtogroup videoio
//! @{

//! @addtogroup videoio_flags_base
//! @{


/** @brief %VideoCapture API backends identifier.

Select preferred API for a capture object.
To be used in the VideoCapture::VideoCapture() constructor or VideoCapture::open()

@note Backends are available only if they have been built with your OpenCV binaries.
See @ref videoio_overview for more information.
*/
enum VideoCaptureAPIs {
       CAP_ANY          = 0,            //!< Auto detect == 0
       CAP_VFW          = 200,          //!< Video For Windows (platform native)
       CAP_V4L          = 200,          //!< V4L/V4L2 capturing support via libv4l
       CAP_V4L2         = CAP_V4L,      //!< Same as CAP_V4L
       CAP_FIREWIRE     = 300,          //!< IEEE 1394 drivers
       CAP_FIREWARE     = CAP_FIREWIRE, //!< Same as CAP_FIREWIRE
       CAP_IEEE1394     = CAP_FIREWIRE, //!< Same as CAP_FIREWIRE
       CAP_DC1394       = CAP_FIREWIRE, //!< Same as CAP_FIREWIRE
       CAP_CMU1394      = CAP_FIREWIRE, //!< Same as CAP_FIREWIRE
       CAP_QT           = 500,          //!< QuickTime
       CAP_UNICAP       = 600,          //!< Unicap drivers
       CAP_DSHOW        = 700,          //!< DirectShow (via videoInput)
       CAP_PVAPI        = 800,          //!< PvAPI, Prosilica GigE SDK
       CAP_OPENNI       = 900,          //!< OpenNI (for Kinect)
       CAP_OPENNI_ASUS  = 910,          //!< OpenNI (for Asus Xtion)
       CAP_ANDROID      = 1000,         //!< Android - not used
       CAP_XIAPI        = 1100,         //!< XIMEA Camera API
       CAP_AVFOUNDATION = 1200,         //!< AVFoundation framework for iOS (OS X Lion will have the same API)
       CAP_GIGANETIX    = 1300,         //!< Smartek Giganetix GigEVisionSDK
       CAP_MSMF         = 1400,         //!< Microsoft Media Foundation (via videoInput)
       CAP_WINRT        = 1410,         //!< Microsoft Windows Runtime using Media Foundation
       CAP_INTELPERC    = 1500,         //!< Intel Perceptual Computing SDK
       CAP_OPENNI2      = 1600,         //!< OpenNI2 (for Kinect)
       CAP_OPENNI2_ASUS = 1610,         //!< OpenNI2 (for Asus Xtion and Occipital Structure sensors)
       CAP_GPHOTO2      = 1700,         //!< gPhoto2 connection
       CAP_GSTREAMER    = 1800,         //!< GStreamer
       CAP_FFMPEG       = 1900,         //!< Open and record video file or stream using the FFMPEG library
       CAP_IMAGES       = 2000,         //!< OpenCV Image Sequence (e.g. img_%02d.jpg)
       CAP_ARAVIS       = 2100,         //!< Aravis SDK
       CAP_OPENCV_MJPEG = 2200,         //!< Built-in OpenCV MotionJPEG codec
       CAP_INTEL_MFX    = 2300          //!< Intel MediaSDK
     };

/** @brief %VideoCapture generic properties identifier.

 Reading / writing properties involves many layers. Some unexpected result might happens along this chain.
 Effective behaviour depends from device hardware, driver and API Backend.
 @sa videoio_flags_others, VideoCapture::get(), VideoCapture::set()
*/
enum VideoCaptureProperties {
       CAP_PROP_POS_MSEC       =0, //!< Current position of the video file in milliseconds.
       CAP_PROP_POS_FRAMES     =1, //!< 0-based index of the frame to be decoded/captured next.
       CAP_PROP_POS_AVI_RATIO  =2, //!< Relative position of the video file: 0=start of the film, 1=end of the film.
       CAP_PROP_FRAME_WIDTH    =3, //!< Width of the frames in the video stream.
       CAP_PROP_FRAME_HEIGHT   =4, //!< Height of the frames in the video stream.
       CAP_PROP_FPS            =5, //!< Frame rate.
       CAP_PROP_FOURCC         =6, //!< 4-character code of codec. see VideoWriter::fourcc .
       CAP_PROP_FRAME_COUNT    =7, //!< Number of frames in the video file.
       CAP_PROP_FORMAT         =8, //!< Format of the %Mat objects returned by VideoCapture::retrieve().
       CAP_PROP_MODE           =9, //!< Backend-specific value indicating the current capture mode.
       CAP_PROP_BRIGHTNESS    =10, //!< Brightness of the image (only for those cameras that support).
       CAP_PROP_CONTRAST      =11, //!< Contrast of the image (only for cameras).
       CAP_PROP_SATURATION    =12, //!< Saturation of the image (only for cameras).
       CAP_PROP_HUE           =13, //!< Hue of the image (only for cameras).
       CAP_PROP_GAIN          =14, //!< Gain of the image (only for those cameras that support).
       CAP_PROP_EXPOSURE      =15, //!< Exposure (only for those cameras that support).
       CAP_PROP_CONVERT_RGB   =16, //!< Boolean flags indicating whether images should be converted to RGB.
       CAP_PROP_WHITE_BALANCE_BLUE_U =17, //!< Currently unsupported.
       CAP_PROP_RECTIFICATION =18, //!< Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently).
       CAP_PROP_MONOCHROME    =19,
       CAP_PROP_SHARPNESS     =20,
       CAP_PROP_AUTO_EXPOSURE =21, //!< DC1394: exposure control done by camera, user can adjust reference level using this feature.
       CAP_PROP_GAMMA         =22,
       CAP_PROP_TEMPERATURE   =23,
       CAP_PROP_TRIGGER       =24,
       CAP_PROP_TRIGGER_DELAY =25,
       CAP_PROP_WHITE_BALANCE_RED_V =26,
       CAP_PROP_ZOOM          =27,
       CAP_PROP_FOCUS         =28,
       CAP_PROP_GUID          =29,
       CAP_PROP_ISO_SPEED     =30,
       CAP_PROP_BACKLIGHT     =32,
       CAP_PROP_PAN           =33,
       CAP_PROP_TILT          =34,
       CAP_PROP_ROLL          =35,
       CAP_PROP_IRIS          =36,
       CAP_PROP_SETTINGS      =37, //!< Pop up video/camera filter dialog (note: only supported by DSHOW backend currently. The property value is ignored)
       CAP_PROP_BUFFERSIZE    =38,
       CAP_PROP_AUTOFOCUS     =39,
       CAP_PROP_SAR_NUM       =40, //!< Sample aspect ratio: num/den (num)
       CAP_PROP_SAR_DEN       =41, //!< Sample aspect ratio: num/den (den)
#ifndef CV_DOXYGEN
       CV__CAP_PROP_LATEST
#endif
     };


/** @brief Generic camera output modes identifier.
@note Currently, these are supported through the libv4l backend only.
*/
enum VideoCaptureModes {
       CAP_MODE_BGR  = 0, //!< BGR24 (default)
       CAP_MODE_RGB  = 1, //!< RGB24
       CAP_MODE_GRAY = 2, //!< Y8
       CAP_MODE_YUYV = 3  //!< YUYV
     };

/** @brief %VideoWriter generic properties identifier.
 @sa VideoWriter::get(), VideoWriter::set()
*/
enum VideoWriterProperties {
  VIDEOWRITER_PROP_QUALITY = 1,    //!< Current quality (0..100%) of the encoded videostream. Can be adjusted dynamically in some codecs.
  VIDEOWRITER_PROP_FRAMEBYTES = 2, //!< (Read-only): Size of just encoded video frame. Note that the encoding order may be different from representation order.
  VIDEOWRITER_PROP_NSTRIPES = 3    /