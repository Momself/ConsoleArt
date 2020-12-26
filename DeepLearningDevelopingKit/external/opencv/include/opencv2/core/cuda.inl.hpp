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

#ifndef OPENCV_CORE_CUDAINL_HPP
#define OPENCV_CORE_CUDAINL_HPP

#include "opencv2/core/cuda.hpp"

//! @cond IGNORED

namespace cv { namespace cuda {

//===================================================================================
// GpuMat
//===================================================================================

inline
GpuMat::GpuMat(Allocator* allocator_)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{}

inline
GpuMat::GpuMat(int rows_, int cols_, int type_, Allocator* allocator_)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{
    if (rows_ > 0 && cols_ > 0)
        create(rows_, cols_, type_);
}

inline
GpuMat::GpuMat(Size size_, int type_, Allocator* allocator_)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{
    if (size_.height > 0 && size_.width > 0)
        create(size_.height, size_.width, type_);
}

inline
GpuMat::GpuMat(int rows_, int cols_, int type_, Scalar s_, Allocator* allocator_)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{
    if (rows_ > 0 && cols_ > 0)
    {
        create(rows_, cols_, type_);
        setTo(s_);
    }
}

inline
GpuMat::GpuMat(Size size_, int type_, Scalar s_, Allocator* allocator_)
    : flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{
    if (size_.height > 0 && size_.width > 0)
    {
        create(size_.height, size_.width, type_);
        setTo(s_);
    }
}

inline
GpuMat::GpuMat(const GpuMat& m)
    : flags(m.flags), rows(m.rows), cols(m.cols), step(m.step), data(m.data), refcount(m.refcount), datastart(m.datastart), dataend(m.dataend), allocator(m.allocator)
{
    if (refcount)
        CV_XADD(refcount, 1);
}

inline
GpuMat::GpuMat(InputArray arr, Allocator* allocator_) :
    flags(0), rows(0), cols(0), step(0), data(0), refcount(0), datastart(0), dataend(0), allocator(allocator_)
{
    upload(arr);
}

inline
GpuMat::~GpuMat()
{
    release();
}

inline
GpuMat& GpuMat::operator =(const GpuMat& m)
{
    if (this != &m)
    {
        GpuMat temp(m);
        swap(temp);
    }

    return *this;
}

inline
void GpuMat::create(Size size_, int type_)
{
    create(size_.height, size_.width, type_);
}

inline
void GpuMat::swap(GpuMat& b)
{
    std::swap(flags, b.flags);
    std::swap(rows, b.rows);
    std::swap(cols, b.cols);
    std::swap(step, b.step);
    std::swap(data, b.data);
    std::swap(datastart, b.datastart);
    std::swap(dataend, b.dataend);
    std::swap(refcount, b.refcount);
    std::swap(allocator, b.allocator);
}

inline
GpuMat GpuMat::clone() const
{
    GpuMat m;
    copyTo(m);
    return m;
}

inline
void GpuMat::copyTo(OutputArray dst, InputArray mask) const
{
    copyTo(dst, mask, Stream::Null());
}

inline
GpuMat& GpuMat::setTo(Scalar s)
{
    return setTo(s, Stream::Null());
}

inline
GpuMat& GpuMat::setTo(Scalar s, InputArray mask)
{
    return setTo(s, mask, Stream::Null());
}

inline
void GpuMat::convertTo(OutputArray dst, int rtype) const
{
    convertTo(dst, rtype, Stream::Null());
}

inline
void GpuMat::convertTo(OutputArray dst, int rtype, double alpha, double beta) const
{
    convertTo(dst, rtype, alpha, beta, Stream::Null());
}

inline
void GpuMat::convertTo(OutputArray dst, int rtype, double alpha, Stream& stream) const
{
    convertTo(dst, rtype, alpha, 0.0, stream);
}

inline
void GpuMat::assignTo(GpuMat& m, int _type) 