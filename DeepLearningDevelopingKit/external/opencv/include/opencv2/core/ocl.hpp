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
// In no event shall the OpenCV Foundation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_OPENCL_HPP
#define OPENCV_OPENCL_HPP

#include "opencv2/core.hpp"

namespace cv { namespace ocl {

//! @addtogroup core_opencl
//! @{

CV_EXPORTS_W bool haveOpenCL();
CV_EXPORTS_W bool useOpenCL();
CV_EXPORTS_W bool haveAmdBlas();
CV_EXPORTS_W bool haveAmdFft();
CV_EXPORTS_W void setUseOpenCL(bool flag);
CV_EXPORTS_W void finish();

CV_EXPORTS bool haveSVM();

class CV_EXPORTS Context;
class CV_EXPORTS Device;
class CV_EXPORTS Kernel;
class CV_EXPORTS Program;
class CV_EXPORTS ProgramSource;
class CV_EXPORTS Queue;
class CV_EXPORTS PlatformInfo;
class CV_EXPORTS Image2D;

class CV_EXPORTS Device
{
public:
    Device();
    explicit Device(void* d);
    Device(const Device& d);
    Device& operator = (const Device& d);
    ~Device();

    void set(void* d);

    enum
    {
        TYPE_DEFAULT     = (1 << 0),
        TYPE_CPU         = (1 << 1),
        TYPE_GPU         = (1 << 2),
        TYPE_ACCELERATOR = (1 << 3),
        TYPE_DGPU        = TYPE_GPU + (1 << 16),
        TYPE_IGPU        = TYPE_GPU + (1 << 17),
        TYPE_ALL         = 0xFFFFFFFF
    };

    String name() const;
    String extensions() const;
    bool isExtensionSupported(const String& extensionName) const;
    String version() const;
    String vendorName() const;
    String OpenCL_C_Version() const;
    String OpenCLVersion() const;
    int deviceVersionMajor() const;
    int deviceVersionMinor() const;
    String driverVersion() const;
    void* ptr() const;

    int type() const;

    int addressBits() const;
    bool available() const;
    bool compilerAvailable() const;
    bool linkerAvailable() const;

    enum
    {
        FP_DENORM=(1 << 0),
        FP_INF_NAN=(1 << 1),
        FP_ROUND_TO_NEAREST=(1 << 2),
        FP_ROUND_TO_ZERO=(1 << 3),
        FP_ROUND_TO_INF=(1 << 4),
        FP_FMA=(1 << 5),
        FP_SOFT_FLOAT=(1 << 6),
        FP_CORRECTLY_ROUNDED_DIVIDE_SQRT=(1 << 7)
    };
    int doubleFPConfig() const;
    int singleFPConfig() const;
    int halfFPConfig() const;

    bool endianLittle() const;
    bool errorCorrectionSupport() const;

    enum
    {
        EXEC_KERNEL=(1 << 0),
        EXEC_NATIVE_KERNEL=(1 << 1)
    };
    int executionCapabilities() const;

    size_t globalMemCacheSize() const;

    enum
    {
        NO_CACHE=0,
        READ_ONLY_CACHE=1,
        READ_WRITE_CACHE=2
    };
    int globalMemCacheType() const;
    int globalMemCacheLineSize() const;
    size_t globalMemSize() const;

    size_t localMemSize() const;
    enum
    {
        NO_LOCAL_MEM=0,
        LOCAL_IS_LOCAL=1,
        LOCAL_IS_GLOBAL=2
    };
    int localMemType() const;
    bool hostUnifiedMemory() const;

    bool imageSupport() const;

    bool imageFromBufferSupport() 