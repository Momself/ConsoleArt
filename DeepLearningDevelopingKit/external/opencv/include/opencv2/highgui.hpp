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
       WINDOW_FREERATIO  = 0x00000100, //!< the image expends as much as it can (no ratio constraint).
       WINDOW_KEEPRATIO  = 0x00000000, //!< the ratio of the image is respected.
       WINDOW_GUI_EXPANDED=0x00000000, //!< status bar and tool bar
       WINDOW_GUI_NORMAL = 0x00000010, //!< old fashious way
    };

//! Flags for cv::setWindowProperty / cv::getWindowProperty
enum WindowPropertyFlags {
       WND_PROP_FULLSCREEN   = 0, //!< fullscreen property    (can be WINDOW_NORMAL or WINDOW_FULLSCREEN).
       WND_PROP_AUTOSIZE     = 1, //!< autosize property      (can be WINDOW_NORMAL or WINDOW_AUTOSIZE).
       WND_PROP_ASPECT_RATIO = 2, //!< window's aspect ration (can be set to WINDOW_FREERATIO or WINDOW_KEEPRATIO).
       WND_PROP_OPENGL       = 3, //!< opengl support.
       WND_PROP_VISIBLE      = 4  //!< checks whether the window exists and is visible
     };

//! Mouse Events see cv::MouseCallback
enum MouseEventTypes {
       EVENT_MOUSEMOVE      = 0, //!< indicates that the mouse pointer has moved over the window.
       EVENT_LBUTTONDOWN    = 1, //!< indicates that the left mouse button is pressed.
       EVENT_RBUTTONDOWN    = 2, //!< indicates that the right mouse button is pressed.
       EVENT_MBUTTONDOWN    = 3, //!< indicates that the middle mouse button is pressed.
       EVENT_LBUTTONUP      = 4, //!< indicates that left mouse button is released.
       EVENT_RBUTTONUP      = 5, //!< indicates that right mouse button is released.
       EVENT_MBUTTONUP      = 6, //!< indicates that middle mouse button is released.
       EVENT_LBUTTONDBLCLK  = 7, //!< indicates that left mouse button is double clicked.
       EVENT_RBUTTONDBLCLK  = 8, //!< indicates that right mouse button is double clicked.
       EVENT_MBUTTONDBLCLK  = 9, //!< indicates that middle mouse button is double clicked.
       EVENT_MOUSEWHEEL     = 10,//!< positive and negative values mean forward and backward scrolling, respectively.
       EVENT_MOUSEHWHEEL    = 11 //!< positive and negative values mean right and left scrolling, respectively.
     };

//! Mouse Event Flags see cv::MouseCallback
enum MouseEventFlags {
       EVENT_FLAG_LBUTTON   = 1, //!< indicates that the left mouse button is down.
       EVENT_FLAG_RBUTTON   = 2, //!< indicates that the right mouse button is down.
       EVENT_FLAG_MBUTTON   = 4, //!< indicates that the middle mouse button is down.
       EVENT_FLAG_CTRLKEY   = 8, //!< indicates that CTRL Key is pressed.
       EVENT_FLAG_SHIFTKEY  = 16,//!< indicates that SHIFT Key is pressed.
       EVENT_FLAG_ALTKEY    = 32 //!< indicates that ALT Key is pressed.
     };

//! Qt font weight
enum QtFontWeights {
        QT_FONT_LIGHT           = 25, //!< Weight of 25
        QT_FONT_NORMAL          = 50, //!< Weight of 50
        QT_FONT_DEMIBOLD        = 63, //!< Weight of 63
        QT_FONT_BOLD            = 75, //!< Weight of 75
        QT_FONT_BLACK           = 87  //!< Weight of 87
     };

//! Qt font style
enum QtFontStyles {
        QT_STYLE_NORMAL         = 0, //!< Normal font.
        QT_STYLE_ITALIC         = 1, //!< Italic font.
        QT_STYLE_OBLIQUE        = 2  //!< Oblique font.
     };

//! Qt "button" type
enum QtButtonTypes {
       QT_PUSH_BUTTON   = 0,    //!< Push button.
       QT_CHECKBOX      = 1,    //!< Checkbox button.
       QT_RADIOBOX      = 2,    //!< Radiobox button.
       QT_NEW_BUTTONBAR = 1024  //!< Button should create a new buttonbar
     };

/** @brief Callback function for mouse events. see cv::setMouseCallback
@param event one of the cv::MouseEventTypes constants.
@param x The x-coordinate of the mouse event.
@param y The y-coordinate of the mouse event.
@param flags one of the cv::MouseEventFlags constants.
@param userdata The optional parameter.
 */
typedef void (*MouseCallback)(int event, int x, int y, int flags, void* userdata);

/** @brief Callback function for Trackbar see cv::createTrackbar
@param pos current position of the specified trackbar.
@param userdata The optional parameter.
 */
typedef void (*TrackbarCallback)(int pos, void* userdata);

/** @brief Callback function defined to be called every frame. See cv::setOpenGlDrawCallback
@param userdata The optional parameter.
 */
typedef void (*OpenGlDrawCallback)(void* userdata);

/** @brief Callback function for a button created by cv::createButton
@param state current state of the button. It could be -1 for a push button, 0 or 1 for a check/radio box button.
@param userdata The optional parameter.
 */
typedef void (*ButtonCallback)(int state, void* userdata);

/** @brief Creates a window.

The function namedWindow creates a window that can be used as a placeholder for images and
trackbars. Created windows are referred to by their names.

If a window with the same name already exists, the function does nothing.

You can call cv::destroyWindow or cv::destroyAllWindows to close the window and de-allocate any associated
memory usage. For a simple program, you do not really have to call these functions because all the
resources and windows of the application are closed automatically by the operating system upon exit.

@note

Qt backend supports additional flags:
 -   **WINDOW_NORMAL or WINDOW_AUTOSIZE:** WINDOW_NORMAL enables you to resize the
     window, whereas WINDOW_AUTOSIZE adjusts automatically the window size to fit the
     displayed image (see imshow ), and you cannot change the window size manually.
 -   **WINDOW_FREERATIO or WINDOW_KEEPRATIO:** WINDOW_FREERATIO adjusts the image
     with no respect to its ratio, whereas WINDOW_KEEPRATIO keeps the image ratio.
 -   **WINDOW_GUI_NORMAL or WINDOW_GUI_EXPANDED:** WINDOW_GUI_NORMAL is the old way to draw the window
     without statusbar and toolbar, whereas WINDOW_GUI_EXPANDED is a new enhanced GUI.
By default, flags == WINDOW_AUTOSIZE | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED

@param winname Name of the window in the window caption that may be used as a window identifier.
@param flags Flags of the window. The supported flags are: (cv::WindowFlags)
 */
CV_EXPORTS_W void namedWindow(const String& winname, int flags = WINDOW_AUTOSIZE);

/** @brief Destroys the specified window.

The function destroyWindow destroys the window with the given name.

@param winname Name of the window to be destroyed.
 */
CV_EXPORTS_W void destroyWindow(const String& winname);

/** @brief Destroys all of the HighGUI windows.

The function destroyAllWindows destroys all of the opened HighGUI windows.
 */
CV_EXPORTS_W void destroyAllWindows();

CV_EXPORTS_W int startWindowThread();

/** @brief Similar to #waitKey, but returns full key code.

@note

Key code is implementation specific and depends on used backend: QT/GTK/Win32/etc

*/
CV_EXPORTS_W int waitKeyEx(int delay = 0);

/** @brief Waits for a pressed key.

The function waitKey waits for a key event infinitely (when \f$\texttt{delay}\leq 0\f$ ) or for delay
milliseconds, when it is positive. Since the OS has a minimum time between switching threads, the
function will not wait exactly delay ms, it will wait at least delay ms, depending on what else is
running on your computer at that time. It returns the code of the pressed key or -1 if no key was
pressed before the specified time had elapsed.

@note

This function is the only method in HighGUI that can fetch and handle events, so it needs to be
called periodically for normal event processing unless HighGUI is used within an environment that
takes care of event processing.

@note

The function only works if there is at least one HighGUI window created and the window is active.
If there are several HighGUI windows, any of them can be active.

@param delay Delay in milliseconds. 0 is the special value that means "forever".
 */
CV_EXPORTS_W int waitKey(int delay = 0);

/** @brief Displays an image in the specified window.

The function imshow displays an image in the specified window. If the window was created with the
cv::WINDOW_AUTOSIZE flag, the image is shown wit