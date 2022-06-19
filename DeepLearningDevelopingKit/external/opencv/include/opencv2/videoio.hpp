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
  VIDEOWRITER_PROP_NSTRIPES = 3    //!< Number of stripes for parallel encoding. -1 for auto detection.
};

//! @} videoio_flags_base

//! @addtogroup videoio_flags_others
//! @{

/** @name IEEE 1394 drivers
    @{
*/

/** @brief Modes of the IEEE 1394 controlling registers
(can be: auto, manual, auto single push, absolute Latter allowed with any other mode)
every feature can have only one mode turned on at a time
*/
enum { CAP_PROP_DC1394_OFF                = -4, //!< turn the feature off (not controlled manually nor automatically).
       CAP_PROP_DC1394_MODE_MANUAL        = -3, //!< set automatically when a value of the feature is set by the user.
       CAP_PROP_DC1394_MODE_AUTO          = -2,
       CAP_PROP_DC1394_MODE_ONE_PUSH_AUTO = -1,
       CAP_PROP_DC1394_MAX                = 31
     };

//! @} IEEE 1394 drivers

/** @name OpenNI (for Kinect)
    @{
*/

//! OpenNI map generators
enum { CAP_OPENNI_DEPTH_GENERATOR = 1 << 31,
       CAP_OPENNI_IMAGE_GENERATOR = 1 << 30,
       CAP_OPENNI_IR_GENERATOR    = 1 << 29,
       CAP_OPENNI_GENERATORS_MASK = CAP_OPENNI_DEPTH_GENERATOR + CAP_OPENNI_IMAGE_GENERATOR + CAP_OPENNI_IR_GENERATOR
     };

//! Properties of cameras available through OpenNI backend
enum { CAP_PROP_OPENNI_OUTPUT_MODE       = 100,
       CAP_PROP_OPENNI_FRAME_MAX_DEPTH   = 101, //!< In mm
       CAP_PROP_OPENNI_BASELINE          = 102, //!< In mm
       CAP_PROP_OPENNI_FOCAL_LENGTH      = 103, //!< In pixels
       CAP_PROP_OPENNI_REGISTRATION      = 104, //!< Flag that synchronizes the remapping depth map to image map
                                                //!< by changing depth generator's view point (if the flag is "on") or
                                                //!< sets this view point to its normal one (if the flag is "off").
       CAP_PROP_OPENNI_REGISTRATION_ON   = CAP_PROP_OPENNI_REGISTRATION,
       CAP_PROP_OPENNI_APPROX_FRAME_SYNC = 105,
       CAP_PROP_OPENNI_MAX_BUFFER_SIZE   = 106,
       CAP_PROP_OPENNI_CIRCLE_BUFFER     = 107,
       CAP_PROP_OPENNI_MAX_TIME_DURATION = 108,
       CAP_PROP_OPENNI_GENERATOR_PRESENT = 109,
       CAP_PROP_OPENNI2_SYNC             = 110,
       CAP_PROP_OPENNI2_MIRROR           = 111
     };

//! OpenNI shortcuts
enum { CAP_OPENNI_IMAGE_GENERATOR_PRESENT         = CAP_OPENNI_IMAGE_GENERATOR + CAP_PROP_OPENNI_GENERATOR_PRESENT,
       CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE     = CAP_OPENNI_IMAGE_GENERATOR + CAP_PROP_OPENNI_OUTPUT_MODE,
       CAP_OPENNI_DEPTH_GENERATOR_PRESENT         = CAP_OPENNI_DEPTH_GENERATOR + CAP_PROP_OPENNI_GENERATOR_PRESENT,
       CAP_OPENNI_DEPTH_GENERATOR_BASELINE        = CAP_OPENNI_DEPTH_GENERATOR + CAP_PROP_OPENNI_BASELINE,
       CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH    = CAP_OPENNI_DEPTH_GENERATOR + CAP_PROP_OPENNI_FOCAL_LENGTH,
       CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION    = CAP_OPENNI_DEPTH_GENERATOR + CAP_PROP_OPENNI_REGISTRATION,
       CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION_ON = CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION,
       CAP_OPENNI_IR_GENERATOR_PRESENT            = CAP_OPENNI_IR_GENERATOR + CAP_PROP_OPENNI_GENERATOR_PRESENT,
     };

//! OpenNI data given from depth generator
enum { CAP_OPENNI_DEPTH_MAP         = 0, //!< Depth values in mm (CV_16UC1)
       CAP_OPENNI_POINT_CLOUD_MAP   = 1, //!< XYZ in meters (CV_32FC3)
       CAP_OPENNI_DISPARITY_MAP     = 2, //!< Disparity in pixels (CV_8UC1)
       CAP_OPENNI_DISPARITY_MAP_32F = 3, //!< Disparity in pixels (CV_32FC1)
       CAP_OPENNI_VALID_DEPTH_MASK  = 4, //!< CV_8UC1

       CAP_OPENNI_BGR_IMAGE         = 5, //!< Data given from RGB image generator
       CAP_OPENNI_GRAY_IMAGE        = 6, //!< Data given from RGB image generator

       CAP_OPENNI_IR_IMAGE          = 7  //!< Data given from IR image generator
     };

//! Supported output modes of OpenNI image generator
enum { CAP_OPENNI_VGA_30HZ  = 0,
       CAP_OPENNI_SXGA_15HZ = 1,
       CAP_OPENNI_SXGA_30HZ = 2,
       CAP_OPENNI_QVGA_30HZ = 3,
       CAP_OPENNI_QVGA_60HZ = 4
     };

//! @} OpenNI

/** @name GStreamer
    @{
*/

enum { CAP_PROP_GSTREAMER_QUEUE_LENGTH = 200 //!< Default is 1
     };

//! @} GStreamer

/** @name PvAPI, Prosilica GigE SDK
    @{
*/

//! PVAPI
enum { CAP_PROP_PVAPI_MULTICASTIP           = 300, //!< IP for enable multicast master mode. 0 for disable multicast.
       CAP_PROP_PVAPI_FRAMESTARTTRIGGERMODE = 301, //!< FrameStartTriggerMode: Determines how a frame is initiated.
       CAP_PROP_PVAPI_DECIMATIONHORIZONTAL  = 302, //!< Horizontal sub-sampling of the image.
       CAP_PROP_PVAPI_DECIMATIONVERTICAL    = 303, //!< Vertical sub-sampling of the image.
       CAP_PROP_PVAPI_BINNINGX              = 304, //!< Horizontal binning factor.
       CAP_PROP_PVAPI_BINNINGY              = 305, //!< Vertical binning factor.
       CAP_PROP_PVAPI_PIXELFORMAT           = 306  //!< Pixel format.
     };

//! PVAPI: FrameStartTriggerMode
enum { CAP_PVAPI_FSTRIGMODE_FREERUN     = 0,    //!< Freerun
       CAP_PVAPI_FSTRIGMODE_SYNCIN1     = 1,    //!< SyncIn1
       CAP_PVAPI_FSTRIGMODE_SYNCIN2     = 2,    //!< SyncIn2
       CAP_PVAPI_FSTRIGMODE_FIXEDRATE   = 3,    //!< FixedRate
       CAP_PVAPI_FSTRIGMODE_SOFTWARE    = 4     //!< Software
     };

//! PVAPI: DecimationHorizontal, DecimationVertical
enum { CAP_PVAPI_DECIMATION_OFF       = 1,    //!< Off
       CAP_PVAPI_DECIMATION_2OUTOF4   = 2,    //!< 2 out of 4 decimation
       CAP_PVAPI_DECIMATION_2OUTOF8   = 4,    //!< 2 out of 8 decimation
       CAP_PVAPI_DECIMATION_2OUTOF16  = 8     //!< 2 out of 16 decimation
     };

//! PVAPI: PixelFormat
enum { CAP_PVAPI_PIXELFORMAT_MONO8    = 1,    //!< Mono8
       CAP_PVAPI_PIXELFORMAT_MONO16   = 2,    //!< Mono16
       CAP_PVAPI_PIXELFORMAT_BAYER8   = 3,    //!< Bayer8
       CAP_PVAPI_PIXELFORMAT_BAYER16  = 4,    //!< Bayer16
       CAP_PVAPI_PIXELFORMAT_RGB24    = 5,    //!< Rgb24
       CAP_PVAPI_PIXELFORMAT_BGR24    = 6,    //!< Bgr24
       CAP_PVAPI_PIXELFORMAT_RGBA32   = 7,    //!< Rgba32
       CAP_PVAPI_PIXELFORMAT_BGRA32   = 8,    //!< Bgra32
     };

//! @} PvAPI

/** @name XIMEA Camera API
    @{
*/

//! Properties of cameras available through XIMEA SDK backend
enum { CAP_PROP_XI_DOWNSAMPLING                                 = 400, //!< Change image resolution by binning or skipping.
       CAP_PROP_XI_DATA_FORMAT                                  = 401, //!< Output data format.
       CAP_PROP_XI_OFFSET_X                                     = 402, //!< Horizontal offset from the origin to the area of interest (in pixels).
       CAP_PROP_XI_OFFSET_Y                                     = 403, //!< Vertical offset from the origin to the area of interest (in pixels).
       CAP_PROP_XI_TRG_SOURCE                                   = 404, //!< Defines source of trigger.
       CAP_PROP_XI_TRG_SOFTWARE                                 = 405, //!< Generates an internal trigger. PRM_TRG_SOURCE must be set to TRG_SOFTWARE.
       CAP_PROP_XI_GPI_SELECTOR                                 = 406, //!< Selects general purpose input.
       CAP_PROP_XI_GPI_MODE                                     = 407, //!< Set general purpose input mode.
       CAP_PROP_XI_GPI_LEVEL                                    = 408, //!< Get general purpose level.
       CAP_PROP_XI_GPO_SELECTOR                                 = 409, //!< Selects general purpose output.
       CAP_PROP_XI_GPO_MODE                                     = 410, //!< Set general purpose output mode.
       CAP_PROP_XI_LED_SELECTOR                                 = 411, //!< Selects camera signalling LED.
       CAP_PROP_XI_LED_MODE                                     = 412, //!< Define camera signalling LED functionality.
       CAP_PROP_XI_MANUAL_WB                                    = 413, //!< Calculates White Balance(must be called during acquisition).
       CAP_PROP_XI_AUTO_WB                                      = 414, //!< Automatic white balance.
       CAP_PROP_XI_AEA