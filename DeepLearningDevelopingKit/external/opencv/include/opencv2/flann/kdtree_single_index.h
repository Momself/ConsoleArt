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

#ifndef OPENCV_FLANN_KDTREE_SINGLE_INDEX_H_
#define OPENCV_FLANN_KDTREE_SINGLE_INDEX_H_

#include <algorithm>
#include <map>
#include <cassert>
#include <cstring>

#include "general.h"
#include "nn_index.h"
#include "matrix.h"
#include "result_set.h"
#include "heap.h"
#include "allocator.h"
#include "random.h"
#include "saving.h"

namespace cvflann
{

struct KDTreeSingleIndexParams : public IndexParams
{
    KDTreeSingleIndexParams(int leaf_max_size = 10, bool reorder = true, int dim = -1)
    {
        (*this)["algorithm"] = FLANN_INDEX_KDTREE_SINGLE;
        (*this)["leaf_max_size"] = leaf_max_size;
        (*this)["reorder"] = reorder;
        (*this)["dim"] = dim;
    }
};


/**
 * Randomized kd-tree index
 *
 * Contains the k-d trees and other information for indexing a set of points
 * for nearest-neighbor matching.
 */
template <typename Distance>
class KDTreeSingleIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;


    /**
     * KDTree constructor
     *
     * Params:
     *          inputData = dataset with the input features
     *          params = parameters passed to the kdtree algorithm
     */
    KDTreeSingleIndex(const Matrix<ElementType>& inputData, const IndexParams& params = KDTreeSingleIndexParams(),
                      Distance d = Distance() ) :
        dataset_(inputData), index_params_(params), distance_(d)
    {
        size_ = dataset_.rows;
        dim_ = dataset_.cols;
        root_node_ = 0;
        int dim_param = get_param(params,"dim",-1);
        if (dim_param>0) dim_ = dim_param;
        leaf_max_size_ = get_param(params,"leaf_max_size",10);
        reorder_ = get_param(params,"reorder",true);

        // Create a permutable array of indices to the input vectors.
        vind_.resize(size_);
        for (size_t i = 0; i < size_; i++) {
            vind_[i] = (int)i;
        }
    }

    KDTreeSingleIndex(const KDTreeSingleIndex&);
    KDTreeSingleIndex& operator=(const KDTreeSingleIndex&);

    /**
     * Standard destructor
     */
    ~KDTreeSingleIndex()
    {
        if (reorder_) delete[] data_.data;
    }

    /**
     * Builds the index
     */
    void buildIndex()
    {
        computeBoundingBox(root_bbox_);
        root_node_ = divideTree(0, (int)size_, root_bbox_ );   // construct the tree

        if (reorder_) {
            delete[] data_.data;
            data_ = cvflann::Matrix<ElementType>(new ElementType[size_*dim_], size_, dim_);
            for (size_t i=0; i<size_; ++i) {
                for (size_t j=0; j<dim_; ++j) {
                    data_[i][j] = dataset_[vind_[i]][j];
                }
            }
        }
        else {
            data_ = dataset_;
        }
    }

    flann_algorithm_t getType() const
    {
        return FLANN_INDEX_KDTREE_SINGLE;
    }


    void saveIndex(FILE* stream)
    {
        save_value(stream, size_);
        save_value(stream, dim_);
        save_value(stream, root_bbox_);
        save_value(stream, reorder_);
        save_value(stream, leaf_max_size_);
        save_value(stream, vind_);
        if (reorder_) {
            save_value(stream, data_);
        }
        save_tree(stream, root_node_);
    }


    void loadIndex(FILE* stream)
    {
        load_value(stream, size_);
        load_value(stream, dim_);
        load_value(stream, root_bbox_);
        load_value(stream, reorder_);
        load_value(stream, leaf_max_size_);
        load_value(stream, vind_);
        if (reorder_) {
            load_value(stream, data_);
        }
        else {
            data_ = dataset_;
        }
        load_tree(stream, root_node_);


        index_params_["algorithm"] = getType();
        index_params_["leaf_max_size"] = leaf_max_size_;
        index_params_["reorder"] = reorder_;
    }

    /**
     *  Returns size of index.
     */
    size_t size() const
    {
        return size_;
    }

    /**
     * Returns the length of an index feature.
     */
    size_t veclen() const
    {
        return dim_;
    }

    /**
     * Computes the inde memory usage
     * Returns: memory used by the index
     */
    int usedMemory() const
    {
        return (int)(pool_.usedMemory+pool_.wastedMemory+dataset_.rows*sizeof(int));  // pool memory and vind array memory
    }


    /**
     * \brief Perform k-nearest neighbor search
     * \param[in] queries The query points for which to find the nearest neighbors
     * \param[out] indices The indices of the nearest neighbors found
     * \param[out] dists Distances to the nearest neighbors found
     * \param[in] knn Number of nearest neighbors to return
     * \param[in] params Search parameters
     */
    void knnSearch(const Matrix<ElementType>& queries, Matrix<int>& indices, Matrix<DistanceType>& dists, int knn, const SearchParams& params)
    {
        assert(queries.cols == veclen());
        assert(indices.rows >= queries.rows);
        assert(dists.rows >= queries.rows);
        assert(int(indices.cols) >= knn);
        assert(int(dists.cols) >= knn);

        KNNSimpleResultSet<DistanceType> resultSet(knn);
        for (size_t i = 0; i < queries.rows; i++) {
            resultSet.init(indices[i], dists[i]);
            findNeighbors(resultSet, queries[i], params);
        }
    }

    IndexParams getParameters() const
    {
        return index_params_;
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
    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams)
    {
        float epsError = 1+get_param(searchParams,"eps",0.0f);

        std::vector<DistanceType> dists(dim_,0);
        DistanceType distsq = computeInitialDistances(vec, dists);
        searchLevel(result, vec, root_node_, distsq, dists, epsError);
    }

private:


    /*--------------------- Internal Data Structures --------------------------*/
    struct Node
    {
        /**
         * Indices of points in leaf node
         */
        int left, right;
        /**
         * Dimension used for subdivision.
         */
        int divfeat;
        /**
         * The values used for subdivision.
         */
        DistanceType divlow, divhigh;
        /**
         * The child nodes.
         */
        Node* child1, * child2;
    };
    typedef Node* NodePtr;


    struct Interval
    {
        DistanceType low, high;
    };

    typedef std::vector<Interval> BoundingBox;

    typedef BranchStruct<NodePtr, DistanceType> BranchSt;
    typedef BranchSt* Branch;




    void save_tree(FILE* stream, NodePtr tree)
    {
        save_value(stream, *tree);
        if (tree->child1!=NULL) {
            save_tree(stream, tree->child1);
        }
        if (tree->child2!=NULL) {
            save_tree(stream, tree->child2);
        }
    }


    void load_tree(FILE* stream, NodePtr& tree)
    {
        tree = pool_.allocate<Node>();
        load_value(stream, *tree);
        if (tree->child1!=NULL) {
            load_tree(stream, tree->child1);
        }
        if (tree->child2!=NULL) {
            load_tree(stream, tree->child2);
        }
    }


    void computeBoundingBox(BoundingBox& bbox)
    {
        bbox.resize(dim_);
        for (size_t i=0; i<dim_; ++i) {
            bbox[i].low = (DistanceType)dataset_[0][i];
            bbox[i].high = (DistanceType)dataset_[0][i];
        }
        for (size_t k=1; k<dataset_.rows; ++k) {
            for (size_t i=0; i<dim_; ++i) {
                if (dataset_[k][i]<bbox[i].low) bbox[i].low = (DistanceType)dataset_[k][i];
                if (dataset_[k][i]>bbox[i].high) bbox[i].high = (DistanceType)dataset_[k][i];
            }
        }
    }


    /**
     * Create a tree node that subdivides the list of vecs from vind[first]
     * to vind[last].  The routine is called recursively on each sublist.
     * Place a pointer to this new tree node in the location pTree.
     *
     * Params: pTree = the new node to create
     *                  first = index of the first vector
     *                  last = index of the last vector
     */
    NodePtr divideTree(int left, 