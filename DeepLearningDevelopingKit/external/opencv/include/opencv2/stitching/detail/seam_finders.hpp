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

#ifndef OPENCV_STITCHING_SEAM_FINDERS_HPP
#define OPENCV_STITCHING_SEAM_FINDERS_HPP

#include <set>
#include "opencv2/core.hpp"
#include "opencv2/opencv_modules.hpp"

namespace cv {
namespace detail {

//! @addtogroup stitching_seam
//! @{

/** @brief Base class for a seam estimator.
 */
class CV_EXPORTS SeamFinder
{
public:
    virtual ~SeamFinder() {}
    /** @brief Estimates seams.

    @param src Source images
    @param corners Source image top-left corners
    @param masks Source image masks to update
     */
    virtual void find(const std::vector<UMat> &src, const std::vector<Point> &corners,
                      std::vector<UMat> &masks) = 0;
};

/** @brief Stub seam estimator which does nothing.
 */
class CV_EXPORTS NoSeamFinder : public SeamFinder
{
public:
    void find(const std::vector<UMat>&, const std::vector<Point>&, std::vector<UMat>&) {}
};

/** @brief Base class for all pairwise seam estimators.
 */
class CV_EXPORTS PairwiseSeamFinder : public SeamFinder
{
public:
    virtual void find(const std::vector<UMat> &src, const std::vector<Point> &corners,
                      std::vector<UMat> &masks);

protected:
    void run();
    /** @brief Resolves masks intersection of two specified images in the given ROI.

    @param first First image index
    @param second Second image index
    @param roi Region of interest
     */
    virtual void findInPair(size_t first, size_t second, Rect roi) = 0;

    std::vector<UMat> images_;
    std::vector<Size> sizes_;
    std::vector<Point> corners_;
    std::vector<UMat> masks_;
};

/** @brief Voronoi diagram-based seam estimator.
 */
class CV_EXPORTS VoronoiSeamFinder : public PairwiseSeamFinder
{
public:
    virtual void find(const std::vector<UMat> &src, const std::vector<Point> &corners,
                      std::vector<UMat> &masks);
    virtual void find(const std::vector<Size> &size, const std::vector<Point> &corners,
                      std::vector<UMat> &masks);
private:
    void findInPair(size_t first, size_t second, Rect roi);
};


class CV_EXPORTS DpSeamFinder : public SeamFinder
{
public:
    enum CostFunction { COLOR, COLOR_GRAD };

    DpSeamFinder(CostFunction costFunc = COLOR);

    CostFunction costFunction() const { return costFunc_; }
    void setCostFunction(CostFunction val) { costFunc_ = val; }

    virtual void find(const std::vector<UMat> &src, const std::vector<Point> &corners,
                      std::vector<UMat> &masks);

private:
    enum ComponentState
    {
        FIRST = 1, SECOND = 2, INTERS = 4,
        INTERS_FIRST = INTERS | FIRST,
        INTERS_SECOND = INTERS | SECOND
    };

    class ImagePairLess
    {
    public:
        ImagePairLess(const std::vector<Mat> &images, const std::vector<Point> &corners)
            : src_(&images[0]), corners_(&corners[0]) {}

        bool operator() (const std::pair<size_t, size_t> &l, const std::pair<size_t, size_t> &r) const
        {
            Point c1 = corners_[l.first] + Point(src_[l.first].cols / 2, src_[l.first].rows / 2);
 