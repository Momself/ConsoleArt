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

#ifndef OPENCV_CORE_PERSISTENCE_HPP
#define OPENCV_CORE_PERSISTENCE_HPP

#ifndef CV_DOXYGEN
/// Define to support persistence legacy formats
#define CV__LEGACY_PERSISTENCE
#endif

#ifndef __cplusplus
#  error persistence.hpp header must be compiled as C++
#endif

//! @addtogroup core_c
//! @{

/** @brief "black box" representation of the file storage associated with a file on disk.

Several functions that are described below take CvFileStorage\* as inputs and allow the user to
save or to load hierarchical collections that consist of scalar values, standard CXCore objects
(such as matrices, sequences, graphs), and user-defined objects.

OpenCV can read and write data in XML (<http://www.w3c.org/XML>), YAML (<http://www.yaml.org>) or
JSON (<http://www.json.org/>) formats. Below is an example of 3x3 floating-point identity matrix A,
stored in XML and YAML files
using CXCore functions:
XML:
@code{.xml}
    <?xml version="1.0">
    <opencv_storage>
    <A type_id="opencv-matrix">
      <rows>3</rows>
      <cols>3</cols>
      <dt>f</dt>
      <data>1. 0. 0. 0. 1. 0. 0. 0. 1.</data>
    </A>
    </opencv_storage>
@endcode
YAML:
@code{.yaml}
    %YAML:1.0
    A: !!opencv-matrix
      rows: 3
      cols: 3
      dt: f
      data: [ 1., 0., 0., 0., 1., 0., 0., 0., 1.]
@endcode
As it can be seen from the examples, XML uses nested tags to represent hierarchy, while YAML uses
indentation for that purpose (similar to the Python programming language).

The same functions can read and write data in both formats; the particular format is determined by
the extension of the opened file, ".xml" for XML files, ".yml" or ".yaml" for YAML and ".json" for
JSON.
 */
typedef struct CvFileStorage CvFileStorage;
typedef struct CvFileNode CvFileNode;
typedef struct CvMat CvMat;
typedef struct CvMatND CvMatND;

//! @} core_c

#include "opencv2/core/types.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

/** @addtogroup core_xml

XML/YAML/JSON file storages.     {#xml_storage}
=======================
Writing to a file storage.
--------------------------
You can store and then restore various OpenCV data structures to/from XML (<http://www.w3c.org/XML>),
YAML (<http://www.yaml.org>) or JSON (<http://www.json.org/>) formats. Also, it is possible to store
and load arbitrarily complex data structures, which include OpenCV data structures, a