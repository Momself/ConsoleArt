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


#ifndef OPENCV_CORE_C_H
#define OPENCV_CORE_C_H

#include "opencv2/core/types_c.h"

#ifdef __cplusplus
#  ifdef _MSC_VER
/* disable warning C4190: 'function' has C-linkage specified, but returns UDT 'typename'
                          which is incompatible with C

   It is OK to disable it because we only extend few plain structures with
   C++ construrtors for simpler interoperability with C++ API of the library
*/
#    pragma warning(disable:4190)
#  elif defined __clang__ && __clang_major__ >= 3
#    pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup core_c
    @{
*/

/****************************************************************************************\
*          Array allocation, deallocation, initialization and access to elements         *
\****************************************************************************************/

/** `malloc` wrapper.
   If there is no enough memory, the function
   (as well as other OpenCV functions that call cvAlloc)
   raises an error. */
CVAPI(void*)  cvAlloc( size_t size );

/** `free` wrapper.
   Here and further all the memory releasing functions
   (that all call cvFree) take double pointer in order to
   to clear pointer to the data after releasing it.
   Passing pointer to NULL pointer is Ok: nothing happens in this case
*/
CVAPI(void)   cvFree_( void* ptr );
#define cvFree(ptr) (cvFree_(*(ptr)), *(ptr)=0)

/** @brief Creates an image header but does not allocate the image data.

@param size Image width and height
@param depth Image depth (see cvCreateImage )
@param channels Number of channels (see cvCreateImage )
 */
CVAPI(IplImage*)  cvCreateImageHeader( CvSize size, int depth, int channels );

/** @brief Initializes an image header that was previously allocated.

The returned IplImage\* points to the initialized header.
@param image Image header to initialize
@param size Image width and height
@param depth Image depth (see cvCreateImage )
@param channels Number of channels (see cvCreateImage )
@param origin Top-left IPL_ORIGIN_TL or bottom-left IPL_ORIGIN_BL
@param align Alignment for image rows, typically 4 or 8 bytes
 */
CVAPI(IplImage*) cvInitImageHeader( IplImage* image, CvSize size, int depth,
                                   int channels, int origin CV_DEFAULT(0),
                                   int align CV_DEFAULT(4));

/** @brief Creates an image header and allocates the image data.

This function call is equivalent to the following code:
@code
    header = cvCreateImageHeader(size, depth, channels);
    cvCreateData(header);
@endcode
@param size Image width and height
@param depth Bit depth of image elements. See IplImage for valid depths.
@param channels Number of channels per pixel. See IplImage for details. This function only creates
images with interleaved channels.
 */
CVAPI(IplImage*)  cvCreateImage( CvSize size, int depth, int channels );

/** @brief Deallocates an image header.

This call is an analogue of :
@code
    if(image )
    {
        iplDeallocate(*image, IPL_IMAGE_HEADER | IPL_IMAGE_ROI);
        *image = 0;
    }
@endcode
but it does not use IPL functions by default (see the CV_TURN_ON_IPL_COMPATIBILITY macro).
@param image Double pointer to the image header
 */
CVAPI(void)  cvReleaseImageHeader( IplImage** image );

/** @brief Deallocates the image header and the image data.

This call is a shortened form of :
@code
    if(*image )
    {
        cvReleaseData(*image);
        cvReleaseImageHeader(image);
    }
@endcode
@param image Double pointer to the image header
*/
CVAPI(void)  cvReleaseImage( IplImage** image );

/** Creates a copy of IPL image (widthStep may differ) */
CVAPI(IplImage*) cvCloneImage( const IplImage* image );

/** @brief Sets the channel of interest in an IplImage.

If the ROI is set to NULL and the coi is *not* 0, the ROI is allocated. Most OpenCV functions do
*not* support the COI setting, so to process an individual image/matrix channel one may copy (via
cvCopy or cvSplit) the channel to a separate image/matrix, process it and then copy the result
back (via cvCopy or cvMerge) if needed.
@param image A pointer to the image header
@param coi The channel of interest. 0 - all channels are selected, 1 - first channel is selected,
etc. Note that the channel indices become 1-based.
 */
CVAPI(void)  cvSetImageCOI( IplImage* image, int coi );

/** @brief Returns the index of the channel of interest.

Returns the channel of interest of in an IplImage. Returned values correspond to the coi in
cvSetImageCOI.
@param image A pointer to the image header
 */
CVAPI(int)  cvGetImageCOI( const IplImage* image );

/** @brief Sets an image Region Of Interest (ROI) for a given rectangle.

If the original image ROI was NULL and the rect is not the whole image, the ROI structure is
allocated.

Most OpenCV functions support the use of ROI and treat the image rectangle as a separate image. For
example, all of the pixel coordinates are counted from the top-left (or bottom-left) corner of the
ROI, not the original image.
@param image A pointer to the image header
@param rect The ROI rectangle
 */
CVAPI(void)  cvSetImageROI( IplImage* image, CvRect rect );

/** @brief Resets the image ROI to include the entire image and releases the ROI structure.

This produces a similar result to the following, but in addition it releases the ROI structure. :
@code
    cvSetImageROI(image, cvRect(0, 0, image->width, image->height ));
    cvSetImageCOI(image, 0);
@endcode
@param image A pointer to the image header
 */
CVAPI(void)  cvResetImageROI( IplImage* image );

/** @brief Returns the image ROI.

If there is no ROI set, cvRect(0,0,image-\>width,image-\>height) is returned.
@param image A pointer to the image header
 */
CVAPI(CvRect) cvGetImageROI( const IplImage* image );

/** @brief Creates a matrix header but does not allocate the matrix data.

The function allocates a new matrix header and returns a pointer to it. The matrix data can then be
allocated using cvCreateData or set explicitly to user-allocated data via cvSetData.
@param rows Number of rows in the matrix
@param cols Number of columns in the matrix
@param type Type of the matrix elements, see cvCreateMat
 */
CVAPI(CvMat*)  cvCreateMatHeader( int rows, int cols, int type );

#define CV_AUTOSTEP  0x7fffffff

/** @brief Initializes a pre-allocated matrix header.

This function is often used to process raw data with OpenCV matrix functions. For example, the
following code computes the matrix product of two matrices, stored as ordinary arrays:
@code
    double a[] = { 1, 2, 3, 4,
                   5, 6,