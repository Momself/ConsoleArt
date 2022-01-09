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

#ifndef OPENCV_HIGHGUI_HPP
#define OPENCV_HIGHGUI_HPP

#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_IMGCODECS
#include "opencv2/imgcodecs.hpp"
#endif
#ifdef HAVE_OPENCV_VIDEOIO
#include "opencv2/videoio.hpp"
#endif

/**
@defgroup highgui High-level GUI

While OpenCV was designed for use in full-scale applications and can be used within functionally
rich UI frameworks (such as Qt\*, WinForms\*, or Cocoa\*) or without any UI at all, sometimes there
it is required to try functionality quickly and visualize the results. This is what the HighGUI
module has been designed for.

It provides easy interface to:

-   Create and manipulate windows that can display images and "remember" their content (no need to
    handle repaint events from OS).
-   Add trackbars to the windows, handle simple mouse events as well as keyboard commands.

@{
    @defgroup highgui_opengl OpenGL support
    @defgroup highgui_qt Qt New Functions

    ![image](pics/qtgui.png)

    This figure explains new functionality implemented with Qt\* GUI. The new GUI provides a statusbar,
    a toolbar, and a control panel. The control panel can have trackbars and buttonbars attached to it.
    If you cannot see the control panel, press Ctrl+P or right-click any Qt window and select **Display
    properties window**.

    -   To attach a trackbar, the window name parameter must be NULL.

    -   To attach a buttonbar, a button must be created. If the last bar attached to the control panel
        is a buttonbar, the new button is added to the right of the last button. If the last bar
        attached to the control panel is a trackbar, or the control panel is empty, a new buttonbar is
        created. Then, a new button is attached to it.

    See below the example used to generate the figure:
    @code
        int main(int argc, char *argv[])
        {

            int value = 50;
            int value2 = 0;


            namedWindow("main1",WINDOW_NORMAL);
            namedWindow("main2",WINDOW_AUTOSIZE | CV_GUI_NORMAL);
            createTrackbar( "track1", "main1", &value, 255,  NULL);

            String nameb1 = "button1";
            String nameb2 = "button2";

            createButton(nameb1,callbackButton,&nameb1,QT_CHECKBOX,1);
            createButton(nameb2,callbackButton,NULL,QT_CHECKBOX,0);
            createTrackbar( "track2", NULL, &value2, 255, NULL);
            createButton("button5",callbackButton1,NULL,QT_RADIOBOX,0);
            createButton("button6",callbackButton2,NULL,QT_RADIOBOX,1);

            setMouseCallback( "main2",on_mouse,NULL );

            Mat img1 = imread("files/flower.jpg");
            VideoCapture video;
            video.open("files/hockey.avi");

            Mat img2,img3;

            while( waitKey(33) != 27 )
            {
                img1.convertTo(img2,-1,1,value);
                video >> img3;

                imshow("main1",img2);
                imshow("main2",img3);
            }

            destroyAllWindows();

            return 0;
        }
    @endcode


    @defgroup highgui_winrt WinRT support

    This figure explains new functionality implemented with WinRT GUI. The new GUI provides an Image control,
    and a slider panel. Slider panel holds trackbars attached to it.

    Sliders are attached below the image control. Every new slider is added below the previous one.

    See below the example used to generate the figure:
    @code
        void sample_app::MainPage::ShowWindow()
        {
            static cv::String windowName("sample");
            cv::winrt_initContainer(this->cvContainer);
            cv::namedWindow(windowName); // not required

            cv::Mat image = cv::imread("Assets/sample.jpg");
            cv::Mat converted = cv::Mat(image.rows, image.cols, CV_8UC4);
            cv::cvtColor(image, converted, COLOR_BGR2BGRA);
            cv::imshow(windowName, converted); // this will create window if it hasn't been created before

            int state = 42;
            cv::TrackbarCallback callback = [](int pos, void* userdata)
            {
                if (pos == 0) {
                    cv::destroyWindow(windowName);
                }
            };
            cv::TrackbarCallback callbackTwin = [](int pos, void* userdata)
            {
                if (pos >= 70) {
                    cv::destroyAllWindows();
                }
            };
            cv::createTrackbar("Sample trackbar", windowName, &state, 100, callback);
            cv::createTrackbar("Twin brother", windowName, &state, 100, callbackTwin);
        }
    @endcode

    @defgroup highgui_c C API
@}
*/

///////////////////////// graphical user interface //////////////////////////
namespace cv
{

//! @addtogroup highgui
//! @{

//! Flags for cv::namedWindow
enum WindowFlags {
       WINDOW_NORMAL     = 0x00000000, //!< the user can resize the window (no constraint) / also use to switch a fullscreen window to a normal size.
       WINDOW_AUTOSIZE   = 0x00000001, //!< the user cannot resize the window, the size is constrainted by the image displayed.
       WINDOW_OPENGL     = 0x00001000, //!< window with opengl support.

       WINDOW_FULLSCREEN = 1,          //!< change the window to fullscreen.
       WINDOW_FREERATIO  = 