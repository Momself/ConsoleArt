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
        Logger::info("%s", stream.str().c_str());
        Logger::info("----------------------------------------------------\n");
    }

    /**
     *  Saves the index to a stream
     */
    virtual void saveIndex(FILE* stream)
    {
        save_value(stream, (int)bestIndex_->getType());
        bestIndex_->saveIndex(stream);
        save_value(stream, get_param<int>(bestSearchParams_, "checks"));
    }

    /**
     *  Loads the index from a stream
     */
    virtual void loadIndex(FILE* stream)
    {
        int index_type;

        load_value(stream, index_type);
        IndexParams params;
        params["algorithm"] = (flann_algorithm_t)index_type;
        bestIndex_ = create_index_by_type<Distance>(dataset_, params, distance_);
        bestIndex_->loadIndex(stream);
        int checks;
        load_value(stream, checks);
        bestSearchParams_["checks"] = checks;
    }

    /**
     *      Method that searches for nearest-neighbors
     */
    virtual void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams)
    {
        int checks = get_param<int>(searchParams,"checks",FLANN_CHECKS_AUTOTUNED);
        if (checks == FLANN_CHECKS_AUTOTUNED) {
            bestIndex_->findNeighbors(result, vec, bestSearchParams_);
        }
        else {
            bestIndex_->findNeighbors(result, vec, searchParams);
        }
    }


    IndexParams getParameters() const
    {
        return bestIndex_->getParameters();
    }

    SearchParams getSearchParameters() const
    {
        return bestSearchParams_;
    }

    float getSpeedup() const
    {
        return speedup_;
    }


    /**
     *      Number of features in this index.
     */
    virtual size_t size() const
    {
        return bestIndex_->size();
    }

    /**
     *  The length of each vector in this index.
     */
    virtual size_t veclen() const
    {
        return bestIndex_->veclen();
    }

    /**
     * The amount of memory (in bytes) this index uses.
     */
    virtual int usedMemory() const
    {
        return bestIndex_->usedMemory();
    }

    /**
     * Algorithm name
     */
    virtual flann_algorithm_t getType() const
    {
        return FLANN_INDEX_AUTOTUNED;
    }

private:

    struct CostData
    {
        float searchTimeCost;
        float buildTimeCost;
        float memoryCost;
        float totalCost;
        IndexParams params;
    };

    void evaluate_kmeans(CostData& cost)
    {
        StartStopTimer t;
        int checks;
        const int nn = 1;

        Logger::info("KMeansTree using params: max_iterations=%d, branching=%d\n",
                     get_param<int>(cost.params,"iterations"),
                     get_param<int>(cost.params,"branching"));
        KMeansIndex<Distance> kmeans(sampledDataset_, cost.params, distance_);
        // measure index build time
        t.start();
        kmeans.buildIndex();
        t.stop();
        float buildTime = (float)t.value;

        // measure search time
        float searchTime = test_index_precision(kmeans, sampledDataset_, testDataset_, gt_matches_, target_precision_, checks, distance_, nn);

        float datasetMemory = float(sampledDataset_.rows * sampledDataset_.cols * sizeof(float));
        cost.memoryCost = (kmeans.usedMemory() + datasetMemory) / datasetMemory;
        cost.searchTimeCost = searchTime;
        cost.buildTimeCost = buildTime;
        Logger::info("KMeansTree buildTime=%g, searchTime=%g, build_weight=%g\n", buildTime, searchTime, build_weight_);
    }


    void evaluate_kdtree(CostData& cost)
    {
        StartStopTimer t;
        int checks;
        const int nn = 1;

        Logger::info("KDTree using params: trees=%d\n", get_param<int>(cost.params,"trees"));
        KDTreeIndex<Distance> kdtree(sampledDataset_, cost.params, distance_);

        t.start();
        kdtree.buildIndex();
        t.stop();
        float buildTime = (float)t.value;

        //measure search time
        float searchTime = test_index_precision(kdtree, sampledDataset_, testDataset_, gt_matches_, target_precision_, checks, distance_, nn);

        float datasetMemory = float(sampledDataset_.rows * sampledDataset_.cols * sizeof(float));
        cost.memoryCost = (kdtree.usedMemory() + datasetMemory) / datasetMemory;
        cost.searchTimeCost = searchTime;
        cost.buildTimeCost = buildTime;
        Logger::info("KDTree buildTime=%g, searchTime=%g\n", buildTime, searchTime);
    }


    //    struct KMeansSimpleDownhillFunctor {
    //
    //        Autotune& autotuner;
    //        KMeansSimpleDownhillFunctor(Autotune& autotuner_) : autotuner(autotuner_) {}
    //
    //        float operator()(int* params) {
    //
    //            float maxFloat = numeric_limits<float>::max();
    //
    //            if (params[0]<2) return maxFloat;
    //            if (params[1]<0) return maxFloat;
    //
    //            CostData c;
    //            c.params["algorithm"] = KMEANS;
    //            c.params["centers-init"] = CENTERS_RANDOM;
    //            c.params["branching"] = params[0];
    //            c.params["max-iterations"] = params[1];
    //
    //            autotuner.evaluate_kmeans(c);
    //
    //            return c.timeCost;
    //
    //        }
    //    };
    //
    //    struct KDTreeSimpleDownhillFunctor {
    //
    //        Autotune& autotuner;
    //        KDTreeSimpleDownhillFunctor(Autotune& autotuner_) : autotuner(autotuner_) {}
    //
    //        float operator()(int* params) {
    //            float maxFloat = numeric_limits<float>::max();
    //
    //            if (params[0]<1) return maxFloat;
    //
    //            CostData c;
    //            c.params["algorithm"] = KDTREE;
    //            c.params["trees"] = params[0];
    //
    //            autotuner.evaluate_kdtree(c);
    //
    //            return c.timeCost;
    //
    //        }
    //    };



    void optimizeKMeans(std::vector<CostData>& costs)
    {
        Logger::info("KMEANS, Step 1: Exploring parameter space\n");

        // explore kmeans parameters space using combinations of the parameters below
        int maxIterations[] = { 1, 5, 10, 15 };
        int branchingFactors[] = { 16, 32, 64, 128, 256 };

        int kmeansParamSpaceSize = FLANN_ARRAY_LEN(maxIterations) * FLANN_ARRAY_LEN(branchingFactors);
        costs.reserve(costs.size() + kmeansParamSpaceSize);

        // evaluate kmeans for all parameter combinations
        for (size_t i = 0; i < FLANN_ARRAY_LEN(maxIterations); ++i) {
            for (size_t j = 0; j < FLANN_ARRAY_LEN(branchingFactors); ++j) {
                CostData cost;
                cost.params["algorithm"] = FLANN_INDEX_KMEANS;
                cost.params["centers_init"] = FLANN_CENTERS_RANDOM;
                cost.params["iterations"] = maxIterations[i];
                cost.params["branching"] = branchingFactors[j];

                evaluate_kmeans(cost);
                costs.push_back(cost);
            }
        }

        //         Logger::info("KMEANS, Step 2: simplex-downhill optimization\n");
        //
        //         const int n = 2;
        //         // choose initial simplex points as the best parameters so far
        //         int kmeansNMPoints[n*(n+1)];
        //         float kmeansVals[n+1];
        //         for (int i=0;i<n+1;++i) {
        //             kmeansNMPoints[i*n] = (int)kmeansCosts[i].params["branching"];
        //             kmeansNMPoints[i*n+1] = (int)kmeansCosts[i].params["max-iterations"];
        //             kmeansVals[i] = kmeansCosts[i].timeCost;
        //         }
        //         KMeansSimpleDownhillFunctor kmeans_cost_func(*this);
        //         // run optimization
        //         optimizeSimplexDownhill(kmeansNMPoints,n,kmeans_cost_func,kmeansVals);
        //         // store results
        //         for (int i=0;i<n+1;++i) {
        //             kmeansCosts[i].pa