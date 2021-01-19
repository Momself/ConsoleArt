
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

#ifndef OPENCV_CORE_MATRIX_OPERATIONS_HPP
#define OPENCV_CORE_MATRIX_OPERATIONS_HPP

#ifndef __cplusplus
#  error mat.inl.hpp header must be compiled as C++
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif

namespace cv
{
CV__DEBUG_NS_BEGIN


//! @cond IGNORED

//////////////////////// Input/Output Arrays ////////////////////////

inline void _InputArray::init(int _flags, const void* _obj)
{ flags = _flags; obj = (void*)_obj; }

inline void _InputArray::init(int _flags, const void* _obj, Size _sz)
{ flags = _flags; obj = (void*)_obj; sz = _sz; }

inline void* _InputArray::getObj() const { return obj; }
inline int _InputArray::getFlags() const { return flags; }
inline Size _InputArray::getSz() const { return sz; }

inline _InputArray::_InputArray() { init(NONE, 0); }
inline _InputArray::_InputArray(int _flags, void* _obj) { init(_flags, _obj); }
inline _InputArray::_InputArray(const Mat& m) { init(MAT+ACCESS_READ, &m); }
inline _InputArray::_InputArray(const std::vector<Mat>& vec) { init(STD_VECTOR_MAT+ACCESS_READ, &vec); }
inline _InputArray::_InputArray(const UMat& m) { init(UMAT+ACCESS_READ, &m); }
inline _InputArray::_InputArray(const std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_READ, &vec); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + traits::Type<_Tp>::value + ACCESS_READ, &vec); }

#ifdef CV_CXX_STD_ARRAY
template<typename _Tp, std::size_t _Nm> inline
_InputArray::_InputArray(const std::array<_Tp, _Nm>& arr)
{ init(FIXED_TYPE + FIXED_SIZE + STD_ARRAY + traits::Type<_Tp>::value + ACCESS_READ, arr.data(), Size(1, _Nm)); }

template<std::size_t _Nm> inline
_InputArray::_InputArray(const std::array<Mat, _Nm>& arr)
{ init(STD_ARRAY_MAT + ACCESS_READ, arr.data(), Size(1, _Nm)); }
#endif

inline
_InputArray::_InputArray(const std::vector<bool>& vec)
{ init(FIXED_TYPE + STD_BOOL_VECTOR + traits::Type<bool>::value + ACCESS_READ, &vec); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + traits::Type<_Tp>::value + ACCESS_READ, &vec); }

inline
_InputArray::_InputArray(const std::vector<std::vector<bool> >&)
{ CV_Error(Error::StsUnsupportedFormat, "std::vector<std::vector<bool> > is not supported!\n"); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + traits::Type<_Tp>::value + ACCESS_READ, &vec); }

template<typename _Tp, int m, int n> inline
_InputArray::_InputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_READ, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputArray::_InputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_READ, vec, Size(n, 1)); }

template<typename _Tp> inline
_InputArray::_InputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + traits::Type<_Tp>::value + ACCESS_READ, &m); }

inline _InputArray::_InputArray(const double& val)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + CV_64F + ACCESS_READ, &val, Size(1,1)); }

inline _InputArray::_InputArray(const MatExpr& expr)
{ init(FIXED_TYPE + FIXED_SIZE + EXPR + ACCESS_READ, &expr); }

inline _InputArray::_InputArray(const cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_READ, &d_mat); }

inline _InputArray::_InputArray(const std::vector<cuda::GpuMat>& d_mat)
{	init(STD_VECTOR_CUDA_GPU_MAT + ACCESS_READ, &d_mat);}

inline _InputArray::_InputArray(const ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_READ, &buf); }

inline _InputArray::_InputArray(const cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_READ, &cuda_mem); }

inline _InputArray::~_InputArray() {}

inline Mat _InputArray::getMat(int i) const
{
    if( kind() == MAT && i < 0 )
        return *(const Mat*)obj;
    return getMat_(i);
}

inline bool _InputArray::isMat() const { return kind() == _InputArray::MAT; }
inline bool _InputArray::isUMat() const  { return kind() == _InputArray::UMAT; }
inline bool _InputArray::isMatVector() const { return kind() == _InputArray::STD_VECTOR_MAT; }
inline bool _InputArray::isUMatVector() const  { return kind() == _InputArray::STD_VECTOR_UMAT; }
inline bool _InputArray::isMatx() const { return kind() == _InputArray::MATX; }
inline bool _InputArray::isVector() const { return kind() == _InputArray::STD_VECTOR ||
                                                   kind() == _InputArray::STD_BOOL_VECTOR ||
                                                   kind() == _InputArray::STD_ARRAY; }
inline bool _InputArray::isGpuMatVector() const { return kind() == _InputArray::STD_VECTOR_CUDA_GPU_MAT; }

////////////////////////////////////////////////////////////////////////////////////////

inline _OutputArray::_OutputArray() { init(ACCESS_WRITE, 0); }
inline _OutputArray::_OutputArray(int _flags, void* _obj) { init(_flags|ACCESS_WRITE, _obj); }
inline _OutputArray::_OutputArray(Mat& m) { init(MAT+ACCESS_WRITE, &m); }
inline _OutputArray::_OutputArray(std::vector<Mat>& vec) { init(STD_VECTOR_MAT+ACCESS_WRITE, &vec); }
inline _OutputArray::_OutputArray(UMat& m) { init(UMAT+ACCESS_WRITE, &m); }
inline _OutputArray::_OutputArray(std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

#ifdef CV_CXX_STD_ARRAY
template<typename _Tp, std::size_t _Nm> inline
_OutputArray::_OutputArray(std::array<_Tp, _Nm>& arr)
{ init(FIXED_TYPE + FIXED_SIZE + STD_ARRAY + traits::Type<_Tp>::value + ACCESS_WRITE, arr.data(), Size(1, _Nm)); }

template<std::size_t _Nm> inline
_OutputArray::_OutputArray(std::array<Mat, _Nm>& arr)
{ init(STD_ARRAY_MAT + ACCESS_WRITE, arr.data(), Size(1, _Nm)); }
#endif

inline
_OutputArray::_OutputArray(std::vector<bool>&)
{ CV_Error(Error::StsUnsupportedFormat, "std::vector<bool> cannot be an output array\n"); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

inline
_OutputArray::_OutputArray(std::vector<std::vector<bool> >&)
{ CV_Error(Error::StsUnsupportedFormat, "std::vector<std::vector<bool> > cannot be an output array\n"); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + traits::Type<_Tp>::value + ACCESS_WRITE, &m); }

template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_WRITE, vec, Size(n, 1)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

#ifdef CV_CXX_STD_ARRAY
template<typename _Tp, std::size_t _Nm> inline
_OutputArray::_OutputArray(const std::array<_Tp, _Nm>& arr)
{ init(FIXED_TYPE + FIXED_SIZE + STD_ARRAY + traits::Type<_Tp>::value + ACCESS_WRITE, arr.data(), Size(1, _Nm)); }

template<std::size_t _Nm> inline
_OutputArray::_OutputArray(const std::array<Mat, _Nm>& arr)
{ init(FIXED_SIZE + STD_ARRAY_MAT + ACCESS_WRITE, arr.data(), Size(1, _Nm)); }
#endif

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_VECTOR + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_MAT + traits::Type<_Tp>::value + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + traits::Type<_Tp>::value + ACCESS_WRITE, &m); }

template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_WRITE, vec, Size(n, 1)); }

inline _OutputArray::_OutputArray(cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_WRITE, &d_mat); }

inline _OutputArray::_OutputArray(std::vector<cuda::GpuMat>& d_mat)
{	init(STD_VECTOR_CUDA_GPU_MAT + ACCESS_WRITE, &d_mat);}

inline _OutputArray::_OutputArray(ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_WRITE, &buf); }

inline _OutputArray::_OutputArray(cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_WRITE, &cuda_mem); }

inline _OutputArray::_OutputArray(const Mat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_WRITE, &m); }

inline _OutputArray::_OutputArray(const std::vector<Mat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_MAT + ACCESS_WRITE, &vec); }

inline _OutputArray::_OutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_WRITE, &m); }

inline _OutputArray::_OutputArray(const std::vector<UMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_UMAT + ACCESS_WRITE, &vec); }

inline _OutputArray::_OutputArray(const cuda::GpuMat& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_GPU_MAT + ACCESS_WRITE, &d_mat); }


inline _OutputArray::_OutputArray(const ogl::Buffer& buf)
{ init(FIXED_TYPE + FIXED_SIZE + OPENGL_BUFFER + ACCESS_WRITE, &buf); }

inline _OutputArray::_OutputArray(const cuda::HostMem& cuda_mem)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_HOST_MEM + ACCESS_WRITE, &cuda_mem); }

///////////////////////////////////////////////////////////////////////////////////////////

inline _InputOutputArray::_InputOutputArray() { init(ACCESS_RW, 0); }
inline _InputOutputArray::_InputOutputArray(int _flags, void* _obj) { init(_flags|ACCESS_RW, _obj); }
inline _InputOutputArray::_InputOutputArray(Mat& m) { init(MAT+ACCESS_RW, &m); }
inline _InputOutputArray::_InputOutputArray(std::vector<Mat>& vec) { init(STD_VECTOR_MAT+ACCESS_RW, &vec); }
inline _InputOutputArray::_InputOutputArray(UMat& m) { init(UMAT+ACCESS_RW, &m); }
inline _InputOutputArray::_InputOutputArray(std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

#ifdef CV_CXX_STD_ARRAY
template<typename _Tp, std::size_t _Nm> inline
_InputOutputArray::_InputOutputArray(std::array<_Tp, _Nm>& arr)
{ init(FIXED_TYPE + FIXED_SIZE + STD_ARRAY + traits::Type<_Tp>::value + ACCESS_RW, arr.data(), Size(1, _Nm)); }

template<std::size_t _Nm> inline
_InputOutputArray::_InputOutputArray(std::array<Mat, _Nm>& arr)
{ init(STD_ARRAY_MAT + ACCESS_RW, arr.data(), Size(1, _Nm)); }
#endif

inline _InputOutputArray::_InputOutputArray(std::vector<bool>&)
{ CV_Error(Error::StsUnsupportedFormat, "std::vector<bool> cannot be an input/output array\n"); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + traits::Type<_Tp>::value + ACCESS_RW, &m); }

template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_RW, vec, Size(n, 1)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

#ifdef CV_CXX_STD_ARRAY
template<typename _Tp, std::size_t _Nm> inline
_InputOutputArray::_InputOutputArray(const std::array<_Tp, _Nm>& arr)
{ init(FIXED_TYPE + FIXED_SIZE + STD_ARRAY + traits::Type<_Tp>::value + ACCESS_RW, arr.data(), Size(1, _Nm)); }

template<std::size_t _Nm> inline
_InputOutputArray::_InputOutputArray(const std::array<Mat, _Nm>& arr)
{ init(FIXED_SIZE + STD_ARRAY_MAT + ACCESS_RW, arr.data(), Size(1, _Nm)); }
#endif

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_VECTOR + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_MAT + traits::Type<_Tp>::value + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + traits::Type<_Tp>::value + ACCESS_RW, &m); }

template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + traits::Type<_Tp>::value + ACCESS_RW, vec, Size(n, 1)); }

inline _InputOutputArray::_InputOutputArray(cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_RW, &d_mat); }

inline _InputOutputArray::_InputOutputArray(ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_RW, &buf); }

inline _InputOutputArray::_InputOutputArray(cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_RW, &cuda_mem); }

inline _InputOutputArray::_InputOutputArray(const Mat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_RW, &m); }

inline _InputOutputArray::_InputOutputArray(const std::vector<Mat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_MAT + ACCESS_RW, &vec); }

inline _InputOutputArray::_InputOutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_RW, &m); }

inline _InputOutputArray::_InputOutputArray(const std::vector<UMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_UMAT + ACCESS_RW, &vec); }

inline _InputOutputArray::_InputOutputArray(const cuda::GpuMat& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_GPU_MAT + ACCESS_RW, &d_mat); }

inline _InputOutputArray::_InputOutputArray(const std::vector<cuda::GpuMat>& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_CUDA_GPU_MAT + ACCESS_RW, &d_mat);}

template<> inline _InputOutputArray::_InputOutputArray(std::vector<cuda::GpuMat>& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_CUDA_GPU_MAT + ACCESS_RW, &d_mat);}

inline _InputOutputArray::_InputOutputArray(const ogl::Buffer& buf)
{ init(FIXED_TYPE + FIXED_SIZE + OPENGL_BUFFER + ACCESS_RW, &buf); }

inline _InputOutputArray::_InputOutputArray(const cuda::HostMem& cuda_mem)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_HOST_MEM + ACCESS_RW, &cuda_mem); }

CV__DEBUG_NS_END

//////////////////////////////////////////// Mat //////////////////////////////////////////

inline
Mat::Mat()
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows), step(0)
{}

inline
Mat::Mat(int _rows, int _cols, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),