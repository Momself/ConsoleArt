
/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2011  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2011  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
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

#ifndef OPENCV_FLANN_HIERARCHICAL_CLUSTERING_INDEX_H_
#define OPENCV_FLANN_HIERARCHICAL_CLUSTERING_INDEX_H_

#include <algorithm>
#include <map>
#include <cassert>
#include <limits>
#include <cmath>

#include "general.h"
#include "nn_index.h"
#include "dist.h"
#include "matrix.h"
#include "result_set.h"
#include "heap.h"
#include "allocator.h"
#include "random.h"
#include "saving.h"


namespace cvflann
{

struct HierarchicalClusteringIndexParams : public IndexParams
{
    HierarchicalClusteringIndexParams(int branching = 32,
                                      flann_centers_init_t centers_init = FLANN_CENTERS_RANDOM,
                                      int trees = 4, int leaf_size = 100)
    {
        (*this)["algorithm"] = FLANN_INDEX_HIERARCHICAL;
        // The branching factor used in the hierarchical clustering
        (*this)["branching"] = branching;
        // Algorithm used for picking the initial cluster centers
        (*this)["centers_init"] = centers_init;
        // number of parallel trees to build
        (*this)["trees"] = trees;
        // maximum leaf size
        (*this)["leaf_size"] = leaf_size;
    }
};


/**
 * Hierarchical index
 *
 * Contains a tree constructed through a hierarchical clustering
 * and other information for indexing a set of points for nearest-neighbour matching.
 */
template <typename Distance>
class HierarchicalClusteringIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

private:


    typedef void (HierarchicalClusteringIndex::* centersAlgFunction)(int, int*, int, int*, int&);

    /**
     * The function used for choosing the cluster centers.
     */
    centersAlgFunction chooseCenters;



    /**
     * Chooses the initial centers in the k-means clustering in a random manner.
     *
     * Params:
     *     k = number of centers
     *     vecs = the dataset of points
     *     indices = indices in the dataset
     *     indices_length = length of indices vector
     *
     */
    void chooseCentersRandom(int k, int* dsindices, int indices_length, int* centers, int& centers_length)
    {
        UniqueRandom r(indices_length);

        int index;
        for (index=0; index<k; ++index) {
            bool duplicate = true;
            int rnd;
            while (duplicate) {
                duplicate = false;
                rnd = r.next();
                if (rnd<0) {
                    centers_length = index;
                    return;
                }

                centers[index] = dsindices[rnd];

                for (int j=0; j<index; ++j) {
                    DistanceType sq = distance(dataset[centers[index]], dataset[centers[j]], dataset.cols);
                    if (sq<1e-16) {
                        duplicate = true;
                    }
                }
            }
        }

        centers_length = index;
    }


    /**
     * Chooses the initial centers in the k-means using Gonzales' algorithm
     * so that the centers are spaced apart from each other.
     *
     * Params:
     *     k = number of centers
     *     vecs = the dataset of points
     *     indices = indices in the dataset
     * Returns:
     */
    void chooseCentersGonzales(int k, int* dsindices, int indices_length, int* centers, int& centers_length)
    {
        int n = indices_length;

        int rnd = rand_int(n);
        assert(rnd >=0 && rnd < n);

        centers[0] = dsindices[rnd];

        int index;
        for (index=1; index<k; ++index) {

            int best_index = -1;
            DistanceType best_val = 0;
            for (int j=0; j<n; ++j) {
                DistanceType dist = distance(dataset[centers[0]],dataset[dsindices[j]],dataset.cols);
                for (int i=1; i<index; ++i) {
                    DistanceType tmp_dist = distance(dataset[centers[i]],dataset[dsindices[j]],dataset.cols);
                    if (tmp_dist<dist) {
                        dist = tmp_dist;
                    }
                }
                if (dist>best_val) {
                    best_val = dist;
                    best_index = j;
                }
            }
            if (best_index!=-1) {
                centers[index] = dsindices[best_index];
            }
            else {
                break;
            }
        }
        centers_length = index;
    }


    /**
     * Chooses the initial centers in the k-means using the algorithm
     * proposed in the KMeans++ paper:
     * Arthur, David; Vassilvitskii, Sergei - k-means++: The Advantages of Careful Seeding
     *
     * Implementation of this function was converted from the one provided in Arthur's code.
     *
     * Params:
     *     k = number of centers
     *     vecs = the dataset of points
     *     indices = indices in the dataset
     * Returns:
     */
    void chooseCentersKMeanspp(int k, int* dsindices, int indices_length, int* centers, int& centers_length)
    {
        int n = indices_length;

        double currentPot = 0;
        DistanceType* closestDistSq = new DistanceType[n];

        // Choose one random center and set the closestDistSq values
        int index = rand_int(n);
        assert(index >=0 && index < n);
        centers[0] = dsindices[index];

        // Computing distance^2 will have the advantage of even higher probability further to pick new centers
        // far from previous centers (and this complies to "k-means++: the advantages of careful seeding" article)
        for (int i = 0; i < n; i++) {
            closestDistSq[i] = distance(dataset[dsindices[i]], dataset[dsindices[index]], dataset.cols);
            closestDistSq[i] = ensureSquareDistance<Distance>( closestDistSq[i] );
            currentPot += closestDistSq[i];
        }


        const int numLocalTries = 1;

        // Choose each center
        int centerCount;
        for (centerCount = 1; centerCount < k; centerCount++) {

            // Repeat several trials
            double bestNewPot = -1;
            int bestNewIndex = 0;
            for (int localTrial = 0; localTrial < numLocalTries; localTrial++) {

                // Choose our center - have to be slightly careful to return a valid answer even accounting
                // for possible rounding errors
                double randVal = rand_double(currentPot);
                for (index = 0; index < n-1; index++) {
                    if (randVal <= closestDistSq[index]) break;
                    else randVal -= closestDistSq[index];
                }

                // Compute the new potential
                double newPot = 0;
                for (int i = 0; i < n; i++) {
                    DistanceType dist = distance(dataset[dsindices[i]], dataset[dsindices[index]], dataset.cols);
                    newPot += std::min( ensureSquareDistance<Distance>(dist), closestDistSq[i] );
                }

                // Store the best result
                if ((bestNewPot < 0)||(newPot < bestNewPot)) {
                    bestNewPot = newPot;
                    bestNewIndex = index;
                }
            }

            // Add the appropriate center
            centers[centerCount] = dsindices[bestNewIndex];
            currentPot = bestNewPot;
            for (int i = 0; i < n; i++) {
                DistanceType dist = distance(dataset[dsindices[i]], dataset[dsindices[bestNewIndex]], dataset.cols);