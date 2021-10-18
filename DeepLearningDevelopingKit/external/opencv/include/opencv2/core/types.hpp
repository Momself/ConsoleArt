
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

#ifndef OPENCV_CORE_TYPES_HPP
#define OPENCV_CORE_TYPES_HPP

#ifndef __cplusplus
#  error types.hpp header must be compiled as C++
#endif

#include <climits>
#include <cfloat>
#include <vector>
#include <limits>

#include "opencv2/core/cvdef.h"
#include "opencv2/core/cvstd.hpp"
#include "opencv2/core/matx.hpp"

namespace cv
{

//! @addtogroup core_basic
//! @{

//////////////////////////////// Complex //////////////////////////////

/** @brief  A complex number class.

  The template class is similar and compatible with std::complex, however it provides slightly
  more convenient access to the real and imaginary parts using through the simple field access, as opposite
  to std::complex::real() and std::complex::imag().
*/
template<typename _Tp> class Complex
{
public:

    //! default constructor
    Complex();
    Complex( _Tp _re, _Tp _im = 0 );

    //! conversion to another data type
    template<typename T2> operator Complex<T2>() const;
    //! conjugation
    Complex conj() const;

    _Tp re, im; //< the real and the imaginary parts
};

typedef Complex<float> Complexf;
typedef Complex<double> Complexd;

template<typename _Tp> class DataType< Complex<_Tp> >
{
public:
    typedef Complex<_Tp> value_type;
    typedef value_type   work_type;
    typedef _Tp          channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
    };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Complex<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Complex<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace


//////////////////////////////// Point_ ////////////////////////////////

/** @brief Template class for 2D points specified by its coordinates `x` and `y`.

An instance of the class is interchangeable with C structures, CvPoint and CvPoint2D32f . There is
also a cast operator to convert point coordinates to the specified type. The conversion from
floating-point coordinates to integer coordinates is done by rounding. Commonly, the conversion
uses this operation for each of the coordinates. Besides the class members listed in the
declaration above, the following operations on points are implemented:
@code
    pt1 = pt2 + pt3;
    pt1 = pt2 - pt3;
    pt1 = pt2 * a;
    pt1 = a * pt2;
    pt1 = pt2 / a;
    pt1 += pt2;
    pt1 -= pt2;
    pt1 *= a;
    pt1 /= a;
    double value = norm(pt); // L2 norm
    pt1 == pt2;
    pt1 != pt2;
@endcode
For your convenience, the following type aliases are defined:
@code
    typedef Point_<int> Point2i;
    typedef Point2i Point;
    typedef Point_<float> Point2f;
    typedef Point_<double> Point2d;
@endcode
Example:
@code
    Point2f a(0.3f, 0.f), b(0.f, 0.4f);
    Point pt = (a + b)*10.f;
    cout << pt.x << ", " << pt.y << endl;
@endcode
*/
template<typename _Tp> class Point_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Point_();
    Point_(_Tp _x, _Tp _y);
    Point_(const Point_& pt);
    Point_(const Size_<_Tp>& sz);
    Point_(const Vec<_Tp, 2>& v);

    Point_& operator = (const Point_& pt);
    //! conversion to another data type
    template<typename _Tp2> operator Point_<_Tp2>() const;

    //! conversion to the old-style C structures
    operator Vec<_Tp, 2>() const;

    //! dot product
    _Tp dot(const Point_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point_& pt) const;
    //! cross-product
    double cross(const Point_& pt) const;
    //! checks whether the point is inside the specified rectangle
    bool inside(const Rect_<_Tp>& r) const;
    _Tp x; //!< x coordinate of the point
    _Tp y; //!< y coordinate of the point
};

typedef Point_<int> Point2i;
typedef Point_<int64> Point2l;
typedef Point_<float> Point2f;
typedef Point_<double> Point2d;
typedef Point2i Point;

template<typename _Tp> class DataType< Point_<_Tp> >
{
public:
    typedef Point_<_Tp>                               value_type;
    typedef Point_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                       channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Point_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Point_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace


//////////////////////////////// Point3_ ////////////////////////////////

/** @brief Template class for 3D points specified by its coordinates `x`, `y` and `z`.

An instance of the class is interchangeable with the C structure CvPoint2D32f . Similarly to
Point_ , the coordinates of 3D points can be converted to another type. The vector arithmetic and
comparison operations are also supported.

The following Point3_\<\> aliases are available:
@code
    typedef Point3_<int> Point3i;
    typedef Point3_<float> Point3f;
    typedef Point3_<double> Point3d;
@endcode
@see cv::Point3i, cv::Point3f and cv::Point3d
*/
template<typename _Tp> class Point3_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Point3_();
    Point3_(_Tp _x, _Tp _y, _Tp _z);
    Point3_(const Point3_& pt);
    explicit Point3_(const Point_<_Tp>& pt);
    Point3_(const Vec<_Tp, 3>& v);

    Point3_& operator = (const Point3_& pt);
    //! conversion to another data type
    template<typename _Tp2> operator Point3_<_Tp2>() const;
    //! conversion to cv::Vec<>
#if OPENCV_ABI_COMPATIBILITY > 300
    template<typename _Tp2> operator Vec<_Tp2, 3>() const;
#else
    operator Vec<_Tp, 3>() const;
#endif

    //! dot product
    _Tp dot(const Point3_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point3_& pt) const;
    //! cross product of the 2 3D points
    Point3_ cross(const Point3_& pt) const;
    _Tp x; //!< x coordinate of the 3D point
    _Tp y; //!< y coordinate of the 3D point
    _Tp z; //!< z coordinate of the 3D point
};

typedef Point3_<int> Point3i;
typedef Point3_<float> Point3f;
typedef Point3_<double> Point3d;

template<typename _Tp> class DataType< Point3_<_Tp> >
{
public:
    typedef Point3_<_Tp>                               value_type;
    typedef Point3_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                        channel_type;

    enum { generic_type = 0,
           channels     = 3,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Point3_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Point3_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 3) }; };
} // namespace

//////////////////////////////// Size_ ////////////////////////////////

/** @brief Template class for specifying the size of an image or rectangle.

The class includes two members called width and height. The structure can be converted to and from
the old OpenCV structures CvSize and CvSize2D32f . The same set of arithmetic and comparison
operations as for Point_ is available.

OpenCV defines the following Size_\<\> aliases:
@code
    typedef Size_<int> Size2i;
    typedef Size2i Size;
    typedef Size_<float> Size2f;
@endcode
*/
template<typename _Tp> class Size_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Size_();
    Size_(_Tp _width, _Tp _height);
    Size_(const Size_& sz);
    Size_(const Point_<_Tp>& pt);

    Size_& operator = (const Size_& sz);
    //! the area (width*height)
    _Tp area() const;
    //! true if empty
    bool empty() const;

    //! conversion of another data type.
    template<typename _Tp2> operator Size_<_Tp2>() const;

    _Tp width; //!< the width
    _Tp height; //!< the height
};

typedef Size_<int> Size2i;
typedef Size_<int64> Size2l;
typedef Size_<float> Size2f;
typedef Size_<double> Size2d;
typedef Size2i Size;

template<typename _Tp> class DataType< Size_<_Tp> >
{
public:
    typedef Size_<_Tp>                               value_type;
    typedef Size_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                      channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Size_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Size_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace

//////////////////////////////// Rect_ ////////////////////////////////

/** @brief Template class for 2D rectangles

described by the following parameters:
-   Coordinates of the top-left corner. This is a default interpretation of Rect_::x and Rect_::y
    in OpenCV. Though, in your algorithms you may count x and y from the bottom-left corner.
-   Rectangle width and height.

OpenCV typically assumes that the top and left boundary of the rectangle are inclusive, while the
right and bottom boundaries are not. For example, the method Rect_::contains returns true if

\f[x  \leq pt.x < x+width,
      y  \leq pt.y < y+height\f]

Virtually every loop over an image ROI in OpenCV (where ROI is specified by Rect_\<int\> ) is
implemented as:
@code
    for(int y = roi.y; y < roi.y + roi.height; y++)
        for(int x = roi.x; x < roi.x + roi.width; x++)
        {
            // ...
        }
@endcode
In addition to the class members, the following operations on rectangles are implemented:
-   \f$\texttt{rect} = \texttt{rect} \pm \texttt{point}\f$ (shifting a rectangle by a certain offset)
-   \f$\texttt{rect} = \texttt{rect} \pm \texttt{size}\f$ (expanding or shrinking a rectangle by a
    certain amount)
-   rect += point, rect -= point, rect += size, rect -= size (augmenting operations)
-   rect = rect1 & rect2 (rectangle intersection)
-   rect = rect1 | rect2 (minimum area rectangle containing rect1 and rect2 )