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

#ifndef OPENCV_DNN_DNN_INL_HPP
#define OPENCV_DNN_DNN_INL_HPP

#include <opencv2/dnn.hpp>

namespace cv {
namespace dnn {
CV__DNN_EXPERIMENTAL_NS_BEGIN

template<typename TypeIter>
DictValue DictValue::arrayInt(TypeIter begin, int size)
{
    DictValue res(Param::INT, new AutoBuffer<int64, 1>(size));
    for (int j = 0; j < size; begin++, j++)
        (*res.pi)[j] = *begin;
    return res;
}

template<typename TypeIter>
DictValue DictValue::arrayReal(TypeIter begin, int size)
{
    DictValue res(Param::REAL, new AutoBuffer<double, 1>(size));
    for (int j = 0; j < size; begin++, j++)
        (*res.pd)[j] = *begin;
    return res;
}

template<typename TypeIter>
DictValue DictValue::arrayString(TypeIter begin, int size)
{
    DictValue res(Param::STRING, new AutoBuffer<String, 1>(size));
    for (int j = 0; j < size; begin++, j++)
        (*res.ps)[j] = *begin;
    return res;
}

template<>
inline DictValue DictValue::get<DictValue>(int idx) const
{
    CV_Assert(idx == -1);
    return *this;
}

template<>
inline int64 DictValue::get<int64>(int idx) const
{
    CV_Assert((idx == -1 && size() == 1) || (idx >= 0 && idx < size()));
    idx = (idx == -1) ? 0 : idx;

    if (type == Param::INT)
    {
        return (*pi)[idx];
    }
    else if (type == Param::REAL)
    {
        double doubleValue = (*pd)[idx];

        double fracpart, intpart;
        fracpart = std::modf(doubleValue, &intpart);
        CV_Assert(fracpart == 0.0);

        return (int64)doubleValue;
    }
    else
    {
        CV_Assert(isInt() || isReal());
        return 0;
    }
}

template<>
inline int DictValue::get<int>(int idx) const
{
    return (int)get<int64>(idx);
}

inline int DictValue::getIntValue(int idx) const
{
    return (int)get<int64>(idx);
}

template<>
inline unsigned DictValue::get<unsigned>(int idx) const
{
    return (unsigned)get<int64>(idx);
}

template<>
inline bool DictValue::get<bool>(int idx) const
{
    return (get<int64>(idx) != 0);
}

template<>
inline double DictValue::get<double>(int idx) const
{
    CV_Assert((idx == -1 && size() == 1) || (idx >= 0 && idx < size()));
    idx = (idx == -1) ? 0 : idx;

    if (type == Param::REAL)
    {
        return (*pd)[idx];
    }
    else if (type == Param::INT)
    {
        return (double)(*pi)[idx];
    }
    else
    {
        CV_Assert(isReal() || isInt());
        return 0;
    }
}

inline double DictValue::getRealValue(int idx) const
{
    return get<double>(idx);
}

template<>
inline float DictValue::get<float>(int idx) const
{
    return (float)get<double>(idx);
}

template<>
inline String DictValue::get<String>(int idx) const
{
    CV_Assert(isString());
    CV_Assert((idx == -1 && ps->size() == 1) || (idx >= 0 && idx < (int)ps->size()));
    return (*ps)[(idx == -1) ? 0 : idx];
}


inline String DictValue::getStringValue(int idx) const
{
    return get<String>(idx);
}

inline void DictValue::