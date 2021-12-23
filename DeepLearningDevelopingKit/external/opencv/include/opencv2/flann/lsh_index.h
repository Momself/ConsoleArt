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

#ifndef OPENCV_FLANN_LSH_INDEX_H_
#define OPENCV_FLANN_LSH_INDEX_H_

#include <algorithm>
#include <cassert>
#include <cstring>
#include <map>
#include <vector>

#include "general.h"
#include "nn_index.h"
#include "matrix.h"
#include "result_set.h"
#include "heap.h"
#include "lsh_table.h"
#include "allocator.h"
#include "random.h"
#include "saving.h"

namespace cvflann
{

struct LshIndexParams : public IndexParams
{
    LshIndexParams(unsigned int table_number = 12, unsigned int key_size = 20, unsigned int multi_probe_level = 2)
    {
        (* this)["algorithm"] = FLANN_INDEX_LSH;
        // The number of hash tables to use
        (*this)["table_number"] = table_number;
        // The length of the key in the hash tables
        (*this)["key_size"] = key_size;
        // Number of levels to use in multi-probe (0 for standard LSH)
        (*this)["multi_probe_level"] = multi_probe_level;
    }
};

/**
 * Randomized kd-tree index
 *
 * Contains the k-d trees and other information for indexing a set of points
 * for nearest-neighbor matching.
 */
template<typename Distance>
class LshIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

    /** Constructor
     * @param input_data dataset with the input features
     * @param params parameters passed to the LSH algorithm
     * @param d the distance used
     */
    LshIndex(const Matrix<ElementType>& input_data, const IndexParams& params = LshIndexParams(),
             Distance d = Distance()) :
        dataset_(input_data), index_params_(params), distance_(d)
    {
        // cv::flann::IndexParams sets integer params as 'int', so it is used with get_param
        // in place of 'unsigned int'
        table_number_ = (unsigned int)get_param<int>(index_params_,"table_number",12);
        key_size_ = (unsigned int)get_param<int>(index_params_,"key_size",20);
        multi_probe_level_ = (unsigned int)get_param<int>(index_params_,"multi_probe_level",2);

        feature_size_ = (unsigned)dataset_.cols;
        fill_xor_mask(0, key_size_, multi_probe_level_, xor_masks_);
    }


    LshIndex(const LshIndex&);
    LshIndex& operator=(const LshIndex&);

    /**
     * Builds the index
     */
    void buildIndex()
    {
        tables_.resize(table_number_);
        for (unsigned int i = 0; i < table_number_; ++i