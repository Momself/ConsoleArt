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

#ifndef OPENCV_CORE_AFFINE3_HPP
#define OPENCV_CORE_AFFINE3_HPP

#ifdef __cplusplus

#include <opencv2/core.hpp>

namespace cv
{

//! @addtogroup core
//! @{

    /** @brief Affine transform
     *
     * It represents a 4x4 homogeneous transformation matrix \f$T\f$
     *
     *  \f[T =
     *  \begin{bmatrix}
     *  R & t\\
     *  0 & 1\\
     *  \end{bmatrix}
     *  \f]
     *
     *  where \f$R\f$ is a 3x3 rotation matrix and \f$t\f$ is a 3x1 translation vector.
     *
     *  You can specify \f$R\f$ either by a 3x3 rotation matrix or by a 3x1 rotation vector,
     *  which is converted to a 3x3 rotation matrix by the Rodrigues formula.
     *
     *  To construct a matrix \f$T\f$ representing first rotation around the axis \f$r\f$ with rotation
     *  angle \f$|r|\f$ in radian (right hand rule) and then translation by the vector \f$t\f$, you can use
     *
     *  @code
     *  cv::Vec3f r, t;
     *  cv::Affine3f T(r, t);
     *  @endcode
     *
     *  If you already have the rotation matrix \f$R\f$, then you can use
     *
     *  @code
     *  cv::Matx33f R;
     *  cv::Affine3f T(R, t);
     *  @endcode
     *
     *  To extract the rotation matrix \f$R\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Matx33f R = T.rotation();
     *  @endcode
     *
     *  To extract the translation vector \f$t\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Vec3f t = T.translation();
     *  @endcode
     *
     *  To extract the rotation vector \f$r\f$ from \f$T\f$, use
     *
     *  @code
     *  cv::Vec3f r = T.rvec();
     *  @endcode
     *
     *  Note that since the mapping from rotation vectors to rotation matrices
     *  is many to one. The returned rotation vector is not necessarily the one
     *  you used before to set the matrix.
     *
     *  If you have two transformations \f$T = T_1 * T_2\f$, use
     *
     *  @code
     *  cv::Affine3f T, T1, T2;
     *  T = T2.concatenate(T1);
     *  @endcode
     *
     *  To get the inverse transform of \f$T\f$, use
     *
     *  @code
     *  cv::Affine3f T, T_inv;
     *  T_inv = T.inv();
     *  @endcode
     *
     */
    template<typename T>
    class Affine3
    {
    public:
        typedef T float_type;
        typedef Matx<float_type, 3, 3> Mat3;
        typedef Matx<float_type, 4, 4> Mat4;
        typedef Vec<float_type, 3> Vec3;

       //! Default constructor. It represents a 4x4 identity matrix.
        Affine3();

        //! Augmented affine matrix
        Affine3(const Mat4& affine);

        /**
         *  The resulting 4x4 matrix is
         *
         *  \f[
         *  \begin{bmatrix}
         *  R & t\\
         *  0 & 1\\
         *  \end{bmatrix}
         *  \f]
         *
         * @param R 3x3 rotation matrix.
         * @param t 3x1 translation vector.
         */
        Affine3(const Mat3& R, const Vec3& t = Vec3::all(0));

        /**
         * Rodrigues vector.
         *
         * The last row of the current matrix is set to [0,0,0,1].
         *
         * @param rvec 3x1 rotation vector. Its direction indicates the rotation axis and its length
         *             indicates the rotation angle in radian (using right hand rule).
         * @param t 3x1 translation vector.
         */
        Affine3(const Vec3& rvec, const Vec3& t = Vec3::all(0));

        /**
         * Combines all constructors above. Supports 4x4, 3x4, 3x3, 1x3, 3x1 sizes of data matrix.
         *
         * The last row of the current matrix is set to [0,0,0,1] when data is not 4x4.
         *
         * @param data 1-channel matrix.
         *             when it is 4x4, it is copied to the current matrix and t is not used.
         *             When it is 3x4, it is copied to the upper part 3x4 of the current matrix and t is not used.
         *             When it is 3x3, it is copied to the upper left 3x3 part of the current matrix.
         *             When it is 3x1 or 1x3, it is treated as a rotation vector and the Rodrigues formula is used
         *                             to compute a 3x3 rotation matrix.
         * @param t 3x1 translation vector. It is used only when data is neither 4x4 nor 3x4.
         */
        explicit Affine3(const Mat& data, const Vec3& t = Vec3::all(0));

        //! From 16-element array
        explicit Affine3(const float_type* vals);

        //! Create an 4x4 identity transform
        static Affine3 Identity();

        /**
         * Rotation matrix.
         *
         * Copy the rotation matrix to the upper left 3x3 part of the current matrix.
         * The remaining elements of the current matrix are not changed.
         *
         * @param R 3x3 rotation matrix.
         *
         */
        void rotation(const Mat3& R);

        /**
         * Rodrigues vector.
         *
         * It sets the upper left 3x3 part of the matrix. The remaining part is unaffected.
         *
         * @param rvec 3x1 rotation vector. The direction indicates the rotation axis and
         *             its length indicates the rotation angle in radian (using the right thumb convention).
         */
        void rotation(const Vec3& rvec);

        /**
         * Combines rotation methods above. Supports 3x3, 1x3, 3x1 sizes of data matrix.
         *
         * It sets the upper left 3x3 part of the matrix. The remaining part is unaffected.
         *
         * @param data 1-channel matrix.
         *             When it is a 3x3 matrix, it sets the upper left 3x3 part of the current matrix.
         *             When it is a 1x3 or 3x1 matrix, it is used as a rotation vector. The Rodrigues formula
         *             is used to compute the rota