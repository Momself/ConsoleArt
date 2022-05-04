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

#ifndef OPENCV_BACKGROUND_SEGM_HPP
#define OPENCV_BACKGROUND_SEGM_HPP

#include "opencv2/core.hpp"

namespace cv
{

//! @addtogroup video_motion
//! @{

/** @brief Base class for background/foreground segmentation. :

The class is only used to define the common interface for the whole family of background/foreground
segmentation algorithms.
 */
class CV_EXPORTS_W BackgroundSubtractor : public Algorithm
{
public:
    /** @brief Computes a foreground mask.

    @param image Next video frame.
    @param fgmask The output foreground mask as an 8-bit binary image.
    @param learningRate The value between 0 and 1 that indicates how fast the background model is
    learnt. Negative parameter value makes the algorithm to use some automatically chosen learning
    rate. 0 means that the background model is not updated at all, 1 means that the background model
    is completely reinitialized from the last frame.
     */
    CV_WRAP virtual void apply(InputArray image, OutputArray fgmask, double learningRate=-1) = 0;

    /** @brief Computes a background image.

    @param backgroundImage The output background image.

    @note Sometimes the background image can be very blurry, as it contain the average background
    statistics.
     */
    CV_WRAP virtual void getBackgroundImage(OutputArray backgroundImage) const = 0;
};


/** @brief Gaussian Mixture-based Background/Foreground Segmentation Algorithm.

The class implements the Gaussian mixture model background subtraction described in @cite Zivkovic2004
and @cite Zivkovic2006 .
 */
class CV_EXPORTS_W BackgroundSubtractorMOG2 : public BackgroundSubtractor
{
public:
    /** @brief Returns the number of last frames that affect the background model
    */
    CV_WRAP virtual int getHistory() const = 0;
    /** @brief Sets the number of last frames that affect the background model
    */
    CV_WRAP virtual void setHistory(int history) = 0;

    /** @brief Returns the number of gaussian components in the background model
    */
    CV_WRAP virtual int getNMixtures() const = 0;
    /** @brief Sets the number of gaussian components in the background model.

    The model needs to be reinitalized to reserve memory.
    */
    CV_WRAP virtual void setNMixtures(int nmixtures) = 0;//needs reinitialization!

    /** @brief Returns the "background ratio" parameter of the algorithm

    If a foreground pixel keeps semi-constant value for about backgroundRatio\*history frames, it's
    considered background and added to the model as a center of a new component. It corresponds to TB
    parameter in the paper.
     */
    CV_WRAP virtual double getBackgroundRatio() const = 0;
    /** @brief Sets the "background ratio" parameter of the algorithm
    */
    CV_WRAP virtual void setBackgroundRatio(double ratio) = 0;

    /** @brief Returns the variance threshold for the pixel-model match

    The main threshold on the squared Mahalanobis distance to decide if the sample is well described by
    the background model or not. Related to Cthr from the paper.
     */
    CV_WRAP virtual double getVarThreshold() const = 0;
    /** @brief Sets the variance threshold for the pixel-model match
    */
    CV_WRAP virtual void setVarThreshold(double varThreshold) = 0;

    /** @brief Returns the variance threshold for the pixel-model match used for new mixture component generation

    Threshold for the squared Mahalanobis distance that helps decide when a sample is close to the
    existing components (corresponds to Tg in the paper). If a pixel is not close to any component, it
    is considered foreground or added as a new component. 3 sigma =\> Tg=3\*3=9 is default. A smaller Tg
    value generates more components. A higher Tg value may result in a small number of components but
    they can grow too large.
     */
    CV_WRAP virtual double getVarThresholdGen() const = 0;
    /** @brief Sets the variance threshold for the pixel-model match used for new mixture component generation
    */
    CV_WRAP virtual void setVarThresholdGen(double varThresholdGen) = 0;

    /** @brief Returns the initial variance of each gaussian component
    */
    CV_WRAP virtual double getVarInit() const = 0;
    /** @brief Sets the initial variance of each gaussian component
    */
    CV_WRAP virtual void setVarInit(double varInit) = 0;

    CV_WRAP virtual double getVarMin() const = 0;
    CV_WRAP virtual void setVarMin(double varMin) = 0;

    CV_WRAP virtual double getVarMax() const = 0;
    CV_WRAP virtual void setVarMax(double varMax) = 0;

    /** @brief Returns the complexity reduction threshold

    This parameter defines the number of samples needed to accept to prove the component exists. CT=0.05
    is a default value for all the samples. By setting CT=0 you get an algorithm very similar to the
    standard Stauffer&Grimson algorithm.
     */
    CV_WRAP virtual double getComplexityReductionThreshold() const = 0;