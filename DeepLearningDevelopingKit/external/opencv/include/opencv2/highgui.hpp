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
    If you cannot see the control panel, press Ctrl+P or right-click any Qt window and select **D