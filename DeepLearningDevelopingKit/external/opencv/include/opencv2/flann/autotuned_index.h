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
#ifndef OPENCV_FLANN_AUTOTUNED_INDEX_H_
#define OPENCV_FLANN_AUTOTUNED_INDEX_H_

#include <sstream>

#include "general.h"
#include "nn_index.h"
#include "ground_truth.h"
#include "index_testing.h"
#include "sampling.h"
#include "kdtree_index.h"
#include "kdtree_single_index.h"
#include "kmeans_index.h"
#include "composite_index.h"
#include "linear_index.h"
#include "logger.h"

namespace cvflann
{

template<typename Distance>
NNIndex<Distance>* create_index_by_type(const Matrix<typename Distance::ElementType>& dataset, const IndexParams& params, const Distance& distance);


struct AutotunedIndexParams : public IndexParams
{
    AutotunedIndexParams(float target_precision = 0.8, float build_weight = 0.01, float memory_weight = 0, float sample_fraction = 0.1)
    {
        (*this)["algorithm"] = FLANN_INDEX_AUTOTUNED;
        // precision desired (used for autotuning, -1 otherwise)
        (*this)["target_precision"] = target_precision;
        // build tree time weighting factor
        (*this)["build_weight"] = build_weight;
        // index memory weighting factor
        (*this)["memory_weight"] = memory_weight;
        // what fraction of the dataset to use for autotuning
        (*this)["sample_fraction"] = sample_fraction;
    }
};


template <typename Distance>
class AutotunedIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

    AutotunedIndex(const Matrix<ElementType>& inputData, const IndexParams& params = AutotunedIndexParams(), Distance d = Distance()) :
        dataset_(inputData), distance_(d)
    {
        target_precision_ = get_param(params, "target_precision",0.8f);
        build_weight_ =  get_param(params,"build_weight", 0.01f);
        memory_weight_ = get_param(params, "memory_weight", 0.0f);
        sample_fraction_ = get_param(params,"sample_fraction", 0.1f);
        bestIndex_ = NULL;
        speedup_ = 0;
    }

    AutotunedIndex(const AutotunedIndex&);
    AutotunedIndex& operator=(const AutotunedIndex&);

    virtual ~AutotunedIndex()
    {
        if (bestIndex_ != NULL) {
            delete bestIndex_;
            bestIndex_ = NULL;
        }
    }

    /**
     *          Method responsible with building the index.
     */
    virtual void buildIndex()
    {
        std::ostringstream stream;
        bestParams_ = estimateBuildParams();
        print_params(bestParams_, stream);
        Logger::info("----------------------------------------------------\n");
        Logger::info("Autotuned parameters:\n");
        Logger::info("%s", stream.str().c_str());
        Logger::info("----------------------------------------------------\n");

        bestIndex_ = create_index_by_type(dataset_, bestParams_, distance_);
        bestIndex_->buildIndex();
        speedup_ = estimateSearchParams(bestSearchParams_);
        stream.str(std::string());
        print_params(bestSearchParams_, stream);
        Logger::info("----------------------------------------------------\n");
        Logger::info("Search parameters:\n");
 