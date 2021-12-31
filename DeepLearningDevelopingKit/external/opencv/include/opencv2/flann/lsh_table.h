/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 *
 * THE BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

/***********************************************************************
 * Author: Vincent Rabaud
 *************************************************************************/

#ifndef OPENCV_FLANN_LSH_TABLE_H_
#define OPENCV_FLANN_LSH_TABLE_H_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits.h>
// TODO as soon as we use C++0x, use the code in USE_UNORDERED_MAP
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  define USE_UNORDERED_MAP 1
#else
#  define USE_UNORDERED_MAP 0
#endif
#if USE_UNORDERED_MAP
#include <unordered_map>
#else
#include <map>
#endif
#include <math.h>
#include <stddef.h>

#include "dynamic_bitset.h"
#include "matrix.h"

namespace cvflann
{

namespace lsh
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** What is stored in an LSH bucket
 */
typedef uint32_t FeatureIndex;
/** The id from which we can get a bucket back in an LSH table
 */
typedef unsigned int BucketKey;

/** A bucket in an LSH table
 */
typedef std::vector<FeatureIndex> Bucket;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** POD for stats about an LSH table
 */
struct LshStats
{
    std::vector<unsigned int> bucket_sizes_;
    size_t n_buckets_;
    size_t bucket_size_mean_;
    size_t bucket_size_median_;
    size_t bucket_size_min_;
    size_t bucket_size_max_;
    size_t bucket_size_std_dev;
    /** Each contained vector contains three value: beginning/end for interval, number of elements in the bin
     */
    std::vector<std::vector<unsigned int> > size_histogram_;
};

/** Overload the << operator for LshStats
 * @param out the streams
 * @param stats the stats to display
 * @return the streams
 */
inline std::ostream& operator <<(std::ostream& out, const LshStats& stats)
{
    int w = 20;
    out << "Lsh Table Stats:\n" << std::setw(w) << std::setiosflags(std::ios::right) << "N buckets : "
    << stats.n_buckets_ << "\n" << std::setw(w) << std::setiosflags(std::ios::right) << "mean size : "
    << std::setiosflags(std::ios::left) << stats.bucket_size_mean_ << "\n" << std::setw(w)
    << std::setiosflags(std::ios::right) << "median size : " << stats.bucket_size_median_ << "\n" << std::setw(w)
    << std::setiosflags(std::ios::right) << "min size : " << std::setiosflags(std::ios::left)
    << stats.bucket_size_min_ << "\n" << std::setw(w) << std::setiosfla