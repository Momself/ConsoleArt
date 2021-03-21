/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
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

#ifndef OPENCV_CORE_OPERATIONS_HPP
#define OPENCV_CORE_OPERATIONS_HPP

#ifndef __cplusplus
#  error operations.hpp header must be compiled as C++
#endif

#include <cstdio>

//! @cond IGNORED

namespace cv
{

////////////////////////////// Matx methods depending on core API /////////////////////////////

namespace internal
{

template<typename _Tp, int m> struct Matx_FastInvOp
{
    bool operator()(const Matx<_Tp, m, m>& a, Matx<_Tp, m, m>& b, int method) const
    {
        Matx<_Tp, m, m> temp = a;

        // assume that b is all 0's on input => make it a unity matrix
        for( int i = 0; i < m; i++ )
            b(i, i) = (_Tp)1;

        if( method == DECOMP_CHOLESKY )
            return Cholesky(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m);

        return LU(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m) != 0;
    }
};

template<typename _Tp> struct Matx_FastInvOp<_Tp, 2>
{
    bool operator()(const Matx<_Tp, 2, 2>& a, Matx<_Tp, 2, 2>& b, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        b(1,1) = a(0,0)*d;
        b(0,0) = a(1,1)*d;
        b(0,1) = -a(0,1)*d;
        b(1,0) = -a(1,0)*d;
        return true;
    }
};

template<typename _Tp> struct Matx_FastInvOp<_Tp, 3>
{
    bool operator()(const Matx<_Tp, 3, 3>& a, Matx<_Tp, 3, 3>& b, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        b(0,0) = (a(1,1) * a(2,2) - a(1,2) * a(2,1)) * d;
        b(0,1) = (a(0,2) * a(2,1) - a(0,1) * a(2,2)) * d;
        b(0,2) = (a(0,1) * a(1,2) - a(0,2) * a(1,1)) * d;

        b(1,0) = (a(1,2) * a(2,0) - a(1,0) * a(2,2)) * d;
        b(1,1) = (a(0,0) * a(2,2) - a(0,2) * a(2,0)) * d;
        b(1,2) = (a(0,2) * a(1,0) - a(0,0) * a(1,2)) * d;

        b(2,0) = (a(1,0) * a(2,1) - a(1,1) * a(2,0)) * d;
        b(2,1) = (a(0,1) * a(2,0) - a(0,0) * a(2,1)) * d;
        b(2,2) = (a(0,0) * a(1,1) - a(0,1) * a(1,0)) * d;
        return true;
    }
};


template<typename _Tp, int m, int n> struct Matx_FastSolveOp
{
    bool operator()(const Matx<_Tp, m, m>& a, const Matx<_Tp, m, n>& b,
                    Matx<_Tp, m, n>& x, int method) const
    {
        Matx<_Tp, m, m> temp = a;
        x = b;
        if( method == DECOMP_CHOLESKY )
            return Cholesky(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n);

        return LU(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n) != 0;
    }
};

template<typename _Tp> struct Matx_FastSolveOp<_Tp, 2, 1>
{
    bool operator()(const Matx<_Tp, 2, 2>& a, const Matx<_Tp, 2, 1>& b,
                    Matx<_Tp, 2, 1>& x, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        x(0) = (b(0)*a(1,1) - b(1)*a(0,1))*d;
        x(1) = (b(1)*a(0,0) - b(0)*a(1,0))*d;
        return true;
    }
};

template<typename _Tp> struct Matx_FastSolveOp<_Tp, 3, 1>
{
    bool operator()(const Matx<_Tp, 3, 3>& a, const Matx<_Tp, 3, 1>& b,
                    Matx<_Tp, 3, 1>& x, int) const
    {
        _Tp d = (_Tp)determinant(a);
        if( d == 0 )
            return false;
        d = 1/d;
        x(0) = d*(b(0)*(a(1,1)*a(2,2) - a(1,2)*a(2,1)) -
                a(0,1)*(b(1)*a(2,2) - a(1,2)*b(2)) +
                a(0,2)*(b(1)*a(2,1) - a(1,1)*b(2)));

        x(1) = d*(a(0,0)*(b(1)*a(2,2) - a(1,2)*b(2)) -
                b(0)*(a(1,0)*a(2,2) - a(1,2)*a(2,0)) +
                a(0,2)*(a(1,0)*b(2) - b(1)*a(2,0)));

        x(2) = d*(a(0,0)*(a(1,1)*b(2) - b(1)*a(2,1)) -
                a(0,1)*(a(1,0)*b(2) - b(1)*a(2,0)) +
                b(0)*(a(1,0)*a(2,1) - a(1,1)*a(2,0)));
        return true;
    }
};

} // internal

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::randu(_Tp a, _Tp b)
{
    Matx<_Tp,m,n> M;
    cv::randu(M, Scalar(a), Scalar(b));
    return M;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp,m,n>::randn(_Tp a, _Tp b)
{
    Matx<_Tp,m,n> M;
    cv::randn(M, Scalar(a), Scalar(b));
    return M;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, n, m> Matx<_Tp, m, n>::inv(int method, bool *p_is_ok /*= NULL*/) const
{
    Matx<_Tp, n, m> b;
    bool ok;
    if( method == DECOMP_LU || method == DECOMP_CHOLESKY )
        ok = cv::internal::Matx_FastInvOp<_Tp, m>()(*this, b, method);
    else
    {
        Mat A(*this, false), B(b, false);
        ok = (invert(A, B, method) != 0);
    }
    if( NULL != p_is_ok ) { *p_is_ok = ok; }
    return ok ? b : Matx<_Tp, n, m>::zeros();
}

template<typename _Tp, int m, int n> template<int l> inline
Matx<_Tp, n, l> Matx<_Tp, m, n>::solve(const Matx<_Tp, m, l>& rhs, int method) const
{
    Matx<_Tp, n, l> x;
    bool ok;
    if( method == DECOMP_LU || method == DECOMP_CHOLESKY )
        ok = cv::internal::Matx_FastSolveOp<_Tp, m, l>()(*this, rhs, x, method);
    else
    {
        Mat A(*this, false), B(rhs, false), X(x, false);
        ok = cv::solve(A, B, X, method);
    }

    return ok ? x : Matx<_Tp, n, l>::zeros();
}



////////////////////////// Augmenting algebraic & logical operations //////////////////////////

#define CV_MAT_AUG_OPERATOR1(op, cvop, A, B) \
    static inline A& operator op (A& a, const B& b) { cvop; return a; }

#define CV_MAT_AUG_OPERATOR(op, cvop, A, B)   \
    CV_MAT_AUG_OPERATOR1(op, cvop, A, B)      \
    CV_MAT_AUG_OPERATOR1(op, cvop, const A, B)

#define CV_MAT_AUG_OPERATOR_T(op, cvop, A, B)                   \
    template<typename _Tp> CV_MAT_AUG_OPERATOR1(op, cvop, A, B) \
    template<typename _Tp> CV_MAT_AUG_OPERATOR1(op, cvop, const A, B)

CV_MAT_AUG_OPERATOR  (+=, cv::add(a,b,a), Mat, Mat)
CV_MAT_AUG_OPERATOR  (+=, cv::add(a,b,a), Mat, Scalar)
CV_MAT_AUG_OPERATOR_T(+=, cv::add(a,b,a), Mat_<_Tp>, Mat)
CV_MAT_AUG_OPERATOR_T(+=, cv::add(a,b,a), Mat_<_Tp>, Scalar)
CV_MAT_AUG_OPERATOR_T(+=, cv::add(a,b,a), Mat_<_Tp>, Mat_<_Tp>)

CV_MAT_AUG_OPERATOR  (-=, cv::subtract(a,b,a), Mat, Mat)
CV_MAT_AUG_OPERATOR  (-=, cv::subtract(a,b,a), Mat, Scalar)
CV_MAT_AUG_OPERATOR_T(-=, cv::subtract(a,b,a), Mat_<_Tp>, Mat)
CV_MAT_AUG_OPERATOR_T(-=, cv::subtract(a,b,a), Mat_<_Tp>, Scalar)
CV_MAT_AUG_OPERATOR_T(-=, cv::subtract(a,b,a), Mat_<_Tp>, Mat_<_Tp>)

CV_MAT_AUG_OPERATOR  (*=, cv::gemm(a, b, 1, Mat(), 0, a, 0), Mat, Mat)
CV_MAT_AUG_OPERATOR_T(*=, cv::gemm(a, b, 1, Mat(), 0, a, 0), Mat_<_Tp>, Mat)
CV_MAT_AUG_OPERATOR_T(*=, cv::gemm(a, b, 1, Mat(), 0, a, 0), Mat_<_Tp>, Mat_<_Tp>)
CV_MAT_AUG_OPERATOR  (*=, a.convertTo(a, -1, b), Mat, double)
CV_MAT_AUG_OPERATOR_T(*=,