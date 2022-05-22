/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

#ifndef OPENCV_VIDEOIO_H
#define OPENCV_VIDEOIO_H

#include "opencv2/core/core_c.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
  @addtogroup videoio_c
  @{
*/

/****************************************************************************************\
*                         Working with Video Files and Cameras                           *
\****************************************************************************************/

/** @brief "black box" capture structure

In C++ use cv::VideoCapture
*/
typedef struct CvCapture CvCapture;

/** @brief start capturing frames from video file
*/
CVAPI(CvCapture*) cvCreateFileCapture( const char* filename );

/** @brief start capturing frames from video file. allows specifying a preferred API to use
*/
CVAPI(CvCapture*) cvCreateFileCaptureWithPreference( const char* filename , int apiPreference);

enum
{
    CV_CAP_ANY      =0,     // autodetect

    CV_CAP_MIL      =100,   // MIL proprietary drivers

    CV_CAP_VFW      =200,   // platform native
    CV_CAP_V4L      =200,
    CV_CAP_V4L2     =200,

    CV_CAP_FIREWARE =300,   // IEEE 1394 drivers
    CV_CAP_FIREWIRE =300,
    CV_CAP_IEEE1394 =300,
    CV_CAP_DC1394   =300,
    CV_CAP_CMU1394  =300,

    CV_CAP_STEREO   =400,   // TYZX proprietary drivers
    CV_CAP_TYZX     =400,
    CV_TYZX_LEFT    =400,
    CV_TYZX_RIGHT   =401,
    CV_TYZX_COLOR   =402,
    CV_TYZX_Z       =403,

    CV_CAP_QT       =500,   // QuickTime

    CV_CAP_UNICAP   =600,   // Unicap drivers

    CV_CAP_DSHOW    =700,   // DirectShow (via videoInput)
    CV_CAP_MSMF     =1400,  // Microsoft Media Foundation (via videoInput)

    CV_CAP_PVAPI    =800,   // PvAPI, Prosilica GigE SDK

    CV_CAP_OPENNI   =900,   // OpenNI (for Kinect)
    CV_CAP_OPENNI_ASUS =910,   // OpenNI (for Asus Xtion)

    CV_CAP_ANDROID  =1000,  // Android - not used
    CV_CAP_ANDROID_BACK =CV_CAP_ANDROID+99, // Android back camera - not used
    CV_CAP_ANDROID_FRONT =CV_CAP_ANDROID+98, // Android front camera - not used

    CV_CAP_XIAPI    =1100,   // XIMEA Camera API

    CV_CAP_AVFOUNDATION = 1200,  // AVFoundation framework for iOS (OS X Lion will have the same API)

    CV_CAP_GIGANETIX = 1300,  // Smartek Giganetix GigEVisionSDK

    CV_CAP_INTELPERC = 1500, // Intel Perceptual Computing

    CV_CAP_OPENNI2 = 1600,   // OpenNI2 (for Kinect)
    CV_CAP_GPHOTO2 = 1700,
    CV_CAP_GSTREAMER = 1800, // GStreamer
    CV_CAP_FFMPEG = 1900,    // FFMPEG
    CV_CAP_IMAGES = 2000,    // OpenCV Image Sequence (e.g. img_%02d.jpg)

    CV_CAP_ARAVIS = 2100     // Aravis GigE SDK
};

/** @brief start capturing frames from camera: index = camera_index + domain_offset (CV_CAP_*)
*/
CVAPI(CvCapture*) cvCreateCameraCapture( int index );

/** @brief grab a frame, return 1 on success, 0 on fail.

  this function is thought to be fast
*/
CVAPI(int) cvGrabFrame( CvCapture* capture );

/** @brief get the frame grabbed with cvGrabFrame(..)

  This function may apply some frame processing like
  frame decompression, flipping etc.
  @warning !!!DO NOT RELEASE or MODIFY the retrieved frame!!!
*/
CVAPI(IplImage*) cvRetrieveFrame( CvCapture* capture, int streamIdx CV_DEFAULT(0) );

/** @brief Just a combination of cvGrabFrame and cvRetrieveFrame

  @warning !!!DO NOT RELEASE or MODIFY the retrieved frame!!!
*/
CVAPI(IplImage*) cvQueryFrame( CvCapture* capture );

/** @brief stop capturing/reading and free resources
*/
CVAPI(void) cvReleaseCapture( CvCapture** capture );

enum
{
    // modes of the controlling registers (can be: auto, manual, auto single push, absolute Latter allowed with any other mode)
    // every feature can have only one mode turned on at a time
    CV_CAP_PROP_DC1394_OFF         = -4,  //turn the feature off (not controlled manually nor automatically)
    CV_CAP_PROP_DC1394_MODE_MANUAL = -3, //set automatically when a value of the feature is set by the user
    CV_CAP_PROP_DC1394_MODE_AUTO = -2,
    CV_CAP_PROP_DC1394_MODE_ONE_PUSH_AUTO = -1,
    CV_CAP_PROP_POS_MSEC       =0,
    CV_CAP_PROP_POS_FRAMES     =1,
    CV_CAP_PROP_POS_AVI_RATIO  =2,
    CV_CAP_PROP_FRAME_WIDTH    =3,
    CV_CAP_PROP_FRAME_HEIGHT   =4,
    CV_CAP_PROP_FPS            =5,
    CV_CAP_PROP_FOURCC         =6,
    CV_CAP_PROP_FRAME_COUNT    =7,
    CV_CAP_PROP_FORMAT         =8,
    CV_CAP_PROP_MODE           =9,
    CV_CAP_PROP_BRIGHTNESS    =10,
    CV_CAP_PROP_CONTRAST      =11,
    CV_CAP_PROP_SATURATION    =12,
    CV_CAP_PROP_HUE           =13,
    CV_CAP_PROP_GAIN          =14,
    CV_CAP_PROP_EXPOSURE      =15,
    CV_CAP_PROP_CONVERT_RGB   =16,
    CV_CAP_PROP_WHITE_BALANCE_BLUE_U =17,
    CV_CAP_PROP_RECTIFICATION =18,
    CV_CAP_PROP_MONOCHROME    =19,
    CV_CAP_PROP_SHARPNESS     =20,
    CV_CAP_PROP_AUTO_EXPOSURE =21, // exposure control done by camera,
                                   // user can adjust reference level
                                   // using this feature
    CV_CAP_PROP_GAMMA         =22,
    CV_CAP_PROP_TEMPERATURE   =23,
    CV_CAP_PROP_TRIGGER       =24,
    CV_CAP_PROP_TRIGGER_DELAY =25,
    CV_CAP_PROP_WHITE_BALANCE_RED_V =26,
    CV_CAP_PROP_ZOOM          =27,
    CV_CAP_PROP_FOCUS         =28,
    CV_CAP_PROP_GUID          =29,
    CV_CAP_PROP_ISO_SPEED     =30,
    CV_CAP_PROP_MAX_DC1394    =31,
    CV_CAP_PROP_BACKLIGHT     =32,
    CV_CAP_PROP_PAN           =33,
    CV_CAP_PROP_TILT          =34,
    CV_CAP_PROP_ROLL          =35,
    CV_CAP_PROP_IRIS          =36,
    CV_CAP_PROP_SETTINGS      =37,
    CV_CAP_PROP_BUFFERSIZE    =38,
    CV_CAP_PROP_AUTOFOCUS     =39,
    CV_CAP_PROP_SAR_NUM       =40,
    CV_CAP_PROP_SAR_DEN       =41,

    CV_CAP_PROP_AUTOGRAB      =1024, // property for videoio class CvCapture_Android only
    CV_CAP_PROP_SUPPORTED_PREVIEW_SIZES_STRING=1025, // readonly, tricky property, returns cpnst char* indeed
    CV_CAP_PROP_PREVIEW_FORMAT=1026, // readonly, tricky property, returns cpnst char* indeed

    // OpenNI map generators
    CV_CAP_OPENNI_DEPTH_GENERATOR = 1 << 31,
    CV_CAP_OPENNI_IMAGE_GENERATOR = 1 << 30,
    CV_CAP_OPENNI_IR_GENERATOR    = 1 << 29,
    CV_CAP_OPENNI_GENERATORS_MASK = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_OPENNI_IR_GENERATOR,

    // Properties of cameras available through OpenNI interfaces
    CV_CAP_PROP_OPENNI_OUTPUT_MODE     = 100,
    CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH = 101, // in mm
    CV_CAP_PROP_OPENNI_BASELINE        = 102, // in mm
    CV_CAP_PROP_OPENNI_FOCAL_LENGTH    = 103, // in pixels
    CV_CAP_PROP_OPENNI_REGISTRATION    = 104, // flag
    CV_CAP_PROP_OPENNI_REGISTRATION_ON = CV_CAP_PROP_OPENNI_REGISTRATION, // flag that synchronizes the remapping depth map to image map
                                                                          // by changing depth generator's view point (if the flag is "on") or
                                                                          // sets this view point to its normal one (if the flag is "off").
    CV_CAP_PROP_OPENNI_APPROX_FRAME_SYNC = 105,
    CV_CAP_PROP_OPENNI_MAX_BUFFER_SIZE   = 106,
    CV_CAP_PROP_OPENNI_CIRCLE_BUFFER     = 107,
    CV_CAP_PROP_OPENNI_MAX_TIME_DURATION = 108,

    CV_CAP_PROP_OPENNI_GENERATOR_PRESENT = 109,
    CV_CAP_PROP_OPENNI2_SYNC = 110,
    CV_CAP_PROP_OPENNI2_MIRROR = 111,

    CV_CAP_OPENNI_IMAGE_GENERATOR_PRESENT         = CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_OPENNI_GENERATOR_PRESENT,
    CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE     = CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_OPENNI_OUTPUT_MODE,
    CV_CAP_OPENNI_DEPTH_GENERATOR_PRESENT         = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_GENERATOR_PRESENT,
    CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE        = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_BASELINE,
    CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH    = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_FOCAL_LENGTH,
    CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION    = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_REGISTRATION,
    CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION_ON = CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION,
    CV_CAP_OPENNI_IR_GENERATOR_PRESENT            = CV_CAP_OPENNI_IR_GENERATOR + CV_CAP_PROP_OPENNI_GENERATOR_PRESENT,

    // Properties of cameras available through GStreamer interface
    CV_CAP_GSTREAMER_QUEUE_LENGTH           = 200, // default is 1

    // PVAPI
    CV_CAP_PROP_PVAPI_MULTICASTIP           = 300, // ip for anable multicast master mode. 0 for disable multicast
    CV_CAP_PROP_PVAPI_FRAMESTARTTRIGGERMODE = 301, // FrameStartTriggerMode: Determines how a frame is initiated
    CV_CAP_PROP_PVAPI_DECIMATIONHORIZONTAL  = 302, // Horizontal sub-sampling of the image
    CV_CAP_PROP_PVAPI_DECIMATIONVERTICAL    = 303, // Vertical sub-sampling of the image
    CV_CAP_PROP_PVAPI_BINNINGX              = 304, // Horizontal binning factor
    CV_CAP_PROP_PVAPI_BINNINGY              = 305, // Vertical binning factor
    CV_CAP_PROP_PVAPI_PIXELFORMAT           = 306, // Pixel format

    // Properties of cameras available through XIMEA SDK interface
    CV_CAP_PROP_XI_DOWNSAMPLING                                 = 400, // Change image resolution by binning or skipping.
    CV_CAP_PROP_XI_DATA_FORMAT                                  = 401, // Output data format.
    CV_CAP_PROP_XI_OFFSET_X                                     = 402, // Horizontal offset from the origin to the area of interest (in pixels).
    CV_CAP_PROP_XI_OFFSET_Y                                     = 403, // Vertical offset from the origin to the area of interest (in pixels).
    CV_CAP_PROP_XI_TRG_SOURCE                                   = 404, // Defines source of trigger.
    CV_CAP_PROP_XI_TRG_SOFTWARE                                 = 405, // Generates an internal trigger. PRM_TRG_SOURCE must be set to TRG_SOFTWARE.
    CV_CAP_PROP_XI_GPI_SELECTOR                                 = 406, // Selects general purpose input
    CV_CAP_PROP_XI_GPI_MODE                                     = 407, // Set general purpose input mode
    CV_CAP_PROP_XI_GPI_LEVEL                                    = 408, // Get general purpose level
    CV_CAP_PROP_XI_GPO_SELECTOR                                 = 409, // Selects general purpose output
    CV_CAP_PROP_XI_GPO_MODE                                     = 410, // Set general purpose output mode
    CV_CAP_PROP_XI_LED_SELECTOR                                 = 411, // Selects camera signalling LED
    CV_CAP_PROP_XI_LED_MODE                                     = 412, // Define camera signalling LED functionality
    CV_CAP_PROP_XI_MANUAL_WB                                    = 413, // Calculates White Balance(must be called during acquisition)
    CV_CAP_PROP_XI_AUTO_WB                                      = 414, // Automatic white balance
    CV_CAP_PROP_XI_AEAG                                         = 415, // Automatic exposure/gain
    CV_CAP_PROP_XI_EXP_PRIORITY                                 = 416, // Exposure priority (0.5 - exposure 50%, gain 50%).
    CV_CAP_PROP_XI_AE_MAX_LIMIT                                 = 417, // Maximum limit of exposure in AEAG procedure
    CV_CAP_PROP_XI_AG_MAX_LIMIT                                 = 418,  // Maximum limit of gain in AEAG procedure
    CV_CAP_PROP_XI_AEAG_LEVEL                                   = 419, // Average intensity of output signal AEAG should achieve(in %)
    CV_CAP_PROP_XI_TIMEOUT                                      = 420, // Image capture timeout in milliseconds
    CV_CAP_PROP_XI_EXPOSURE                                     = 421, // Exposure time in microseconds
    CV_CAP_PROP_XI_EXPOSURE_BURST_COUNT                         = 422, // Sets the number of times of exposure in one frame.
    CV_CAP_PROP_XI_GAIN_SELECTOR                                = 423, // Gain selector for parameter Gain allows to select different type of gains.
    CV_CAP_PROP_XI_GAIN                                         = 424, // Gain in dB
    CV_CAP_PROP_XI_DOWNSAMPLING_TYPE                            = 426, // Change image downsampling type.
    CV_CAP_PROP_XI_BINNING_SELECTOR                             = 427, // Binning engine selector.
    CV_CAP_PROP_XI_BINNING_VERTICAL                             = 428, // Vertical Binning - number of vertical photo-sensitive cells to combine together.
    CV_CAP_PROP_XI_BINNING_HORIZONTAL                           = 429, // Horizontal Binning - number of horizontal photo-sensitive cells to combine together.
    CV_CAP_PROP_XI_BINNING_PATTERN                              = 430, // Binning pattern type.
    CV_CAP_PROP_XI_DECIMATION_SELECTOR                          = 431, // Decimation engine selector.
    CV_CAP_PROP_XI_DECIMATION_VERTICAL                          = 432, // Vertical Decimation - vertical sub-sampling of the image - reduces the vertical resolution of the 