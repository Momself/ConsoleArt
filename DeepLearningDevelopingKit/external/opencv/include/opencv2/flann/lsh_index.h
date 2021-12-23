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
        for (unsigned int i = 0; i < table_number_; ++i) {
            lsh::LshTable<ElementType>& table = tables_[i];
            table = lsh::LshTable<ElementType>(feature_size_, key_size_);

            // Add the features to the table
            table.add(dataset_);
        }
    }

    flann_algorithm_t getType() const
    {
        return FLANN_INDEX_LSH;
    }


    void saveIndex(FILE* stream)
    {
        save_value(stream,table_number_);
        save_value(stream,key_size_);
        save_value(stream,multi_probe_level_);
        save_value(stream, dataset_);
    }

    void loadIndex(FILE* stream)
    {
        load_value(stream, table_number_);
        load_value(stream, key_size_);
        load_value(stream, multi_probe_level_);
        load_value(stream, dataset_);
        // Building the index is so fast we can afford not storing it
        buildIndex();

        index_params_["algorithm"] = getType();
        index_params_["table_number"] = table_number_;
        index_params_["key_size"] = key_size_;
        index_params_["multi_probe_level"] = multi_probe_level_;
    }

    /**
     *  Returns size of index.
     */
    size_t size() const
    {
        return dataset_.rows;
    }

    /**
     * Returns the length of an index feature.
     */
    size_t veclen() const
    {
        return feature_size_;
    }

    /**
     * Computes the index memory usage
     * Returns: memory used by the index
     */
    int usedMemory() const
    {
        return (int)(dataset_.rows * sizeof(int));
    }


    IndexParams getParameters() const
    {
        return index_params_;
    }

    /**
     * \brief Perform k-nearest neighbor search
     * \param[in] queries The query points for which to find the nearest neighbors
     * \param[out] indices The indices of the nearest neighbors found
     * \param[out] dists Distances to the nearest neighbors found
     * \param[in] knn Number of nearest neighbors to return
     * \param[in] params Search parameters
     */
    virtual void knnSearch(const Matrix<ElementType>& queries, Matrix<int>& indices, Matrix<DistanceType>& dists, int knn, const SearchParams& params)
    {
        assert(queries.cols == veclen());
        assert(indices.rows >= queries.rows);
        assert(dists.rows >= queries.rows);
        assert(int(indices.cols) >= knn);
        assert(int(dists.cols) >= knn);


        KNNUniqueResultSet<DistanceType> resultSet(knn);
        for (size_t i = 0; i < queries.rows; i++) {
            resultSet.clear();
            std::fill_n(indices[i], knn, -1);
            std::fill_n(dists[i], knn, std::numeric_limits<DistanceType>::max());
            findNeighbors(resultSet, queries[i], params);
            if (get_param(params,"sorted",true)) resultSet.sortAndCopy(indices[i], dists[i], knn);
            else resultSet.copy(indices[i], dists[i], knn);
        }
    }


    /**
     * Find set of nearest neighbors to vec. Their indices are stored inside
     * the result object.
     *
     * Params:
     *     result = the result object in which the indices of the nearest-neighbors are stored
     *     vec = the vector for which to search the nearest neighbors
     *     maxCheck = the maximum number of restarts (in a best-bin-first manner)
     */
    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& /*searchParams*/)
    {
        getNeighbors(vec, result);
    }

private:
    /** Defines the comparator on score and index
     */
    typedef std::pair<float, unsigned int> ScoreIndexPair;
    struct SortScoreIndexPairOnSecond
    {
        bool operator()(const ScoreIndexPair& left, const ScoreIndexPair& right) const
        {
            return left.second < right.second;
        }
    };

    /** Fills the different xor masks to us