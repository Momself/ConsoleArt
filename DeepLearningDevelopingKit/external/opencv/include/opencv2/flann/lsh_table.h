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
    << stats.bucket_size_min_ << "\n" << std::setw(w) << std::setiosflags(std::ios::right) << "max size : "
    << std::setiosflags(std::ios::left) << stats.bucket_size_max_;

    // Display the histogram
    out << std::endl << std::setw(w) << std::setiosflags(std::ios::right) << "histogram : "
    << std::setiosflags(std::ios::left);
    for (std::vector<std::vector<unsigned int> >::const_iterator iterator = stats.size_histogram_.begin(), end =
             stats.size_histogram_.end(); iterator != end; ++iterator) out << (*iterator)[0] << "-" << (*iterator)[1] << ": " << (*iterator)[2] << ",  ";

    return out;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Lsh hash table. As its key is a sub-feature, and as usually
 * the size of it is pretty small, we keep it as a continuous memory array.
 * The value is an index in the corpus of features (we keep it as an unsigned
 * int for pure memory reasons, it could be a size_t)
 */
template<typename ElementType>
class LshTable
{
public:
    /** A container of all the feature indices. Optimized for space
     */
#if USE_UNORDERED_MAP
    typedef std::unordered_map<BucketKey, Bucket> BucketsSpace;
#else
    typedef std::map<BucketKey, Bucket> BucketsSpace;
#endif

    /** A container of all the feature indices. Optimized for speed
     */
    typedef std::vector<Bucket> BucketsSpeed;

    /** Default constructor
     */
    LshTable()
    {
        key_size_ = 0;
        feature_size_ = 0;
        speed_level_ = kArray;
    }

    /** Default constructor
     * Create the mask and allocate the memory
     * @param feature_size is the size of the feature (considered as a ElementType[])
     * @param key_size is the number of bits that are turned on in the feature
     */
    LshTable(unsigned int feature_size, unsigned int key_size)
    {
        feature_size_ = feature_size;
        (void)key_size;
        std::cerr << "LSH is not implemented for that type" << std::endl;
        assert(0);
    }

    /** Add a feature to the table
     * @param value the value to store for that feature
     * @param feature the feature itself
     */
    void add(unsigned int value, const ElementType* feature)
    {
        // Add the value to the corresponding bucket
        BucketKey key = (lsh::BucketKey)getKey(feature);

        switch (speed_level_) {
        case kArray:
            // That means we get the buckets from an array
            buckets_speed_[key].push_back(value);
            break;
        case kBitsetHash:
            // That means we can check the bitset for the presence of a key
            key_bitset_.set(key);
            buckets_space_[key].push_back(value);
            break;
        case kHash:
        {
            // That means we have to check for the hash table for the presence of a key
            buckets_space_[key].push_back(value);
            break;
        }
        }
    }

    /** Add a set of features to the table
     * @param dataset the values to store
     */
    void add(Matrix<ElementType> dataset)
    {
#if USE_UNORDERED_MAP
        buckets_space_.rehash((buckets_space_.size() + dataset.rows) * 1.2);
#endif
        // Add the features to the table
        for (unsigned int i = 0; i < dataset.rows; ++i) add(i, dataset[i]);
        // Now that the table is full, optimize it for speed/space
        optimize();
    }

    /** Get a bucket given the key
     * @param key
     * @return
     */
    inline const Bucket* getBucketFromKey(BucketKey key) const
    {
        // Generate other buckets
        switch (speed_level_) {
        case kArray:
            // That means we get the buckets from an array
            return &buckets_speed_[key];
            break;
        case kBitsetHash:
            // That means we can check the bitset for the presence of a key
            if (key_bitset_.test(key)) return &buckets_space_.find(key)-