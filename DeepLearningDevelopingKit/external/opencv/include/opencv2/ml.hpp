
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2014, Itseez Inc, all rights reserved.
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

#ifndef OPENCV_ML_HPP
#define OPENCV_ML_HPP

#ifdef __cplusplus
#  include "opencv2/core.hpp"
#endif

#ifdef __cplusplus

#include <float.h>
#include <map>
#include <iostream>

/**
  @defgroup ml Machine Learning

  The Machine Learning Library (MLL) is a set of classes and functions for statistical
  classification, regression, and clustering of data.

  Most of the classification and regression algorithms are implemented as C++ classes. As the
  algorithms have different sets of features (like an ability to handle missing measurements or
  categorical input variables), there is a little common ground between the classes. This common
  ground is defined by the class cv::ml::StatModel that all the other ML classes are derived from.

  See detailed overview here: @ref ml_intro.
 */

namespace cv
{

namespace ml
{

//! @addtogroup ml
//! @{

/** @brief Variable types */
enum VariableTypes
{
    VAR_NUMERICAL    =0, //!< same as VAR_ORDERED
    VAR_ORDERED      =0, //!< ordered variables
    VAR_CATEGORICAL  =1  //!< categorical variables
};

/** @brief %Error types */
enum ErrorTypes
{
    TEST_ERROR = 0,
    TRAIN_ERROR = 1
};

/** @brief Sample types */
enum SampleTypes
{
    ROW_SAMPLE = 0, //!< each training sample is a row of samples
    COL_SAMPLE = 1  //!< each training sample occupies a column of samples
};

/** @brief The structure represents the logarithmic grid range of statmodel parameters.

It is used for optimizing statmodel accuracy by varying model parameters, the accuracy estimate
being computed by cross-validation.
 */
class CV_EXPORTS_W ParamGrid
{
public:
    /** @brief Default constructor */
    ParamGrid();
    /** @brief Constructor with parameters */
    ParamGrid(double _minVal, double _maxVal, double _logStep);

    CV_PROP_RW double minVal; //!< Minimum value of the statmodel parameter. Default value is 0.
    CV_PROP_RW double maxVal; //!< Maximum value of the statmodel parameter. Default value is 0.
    /** @brief Logarithmic step for iterating the statmodel parameter.

    The grid determines the following iteration sequence of the statmodel parameter values:
    \f[(minVal, minVal*step, minVal*{step}^2, \dots,  minVal*{logStep}^n),\f]
    where \f$n\f$ is the maximal index satisfying
    \f[\texttt{minVal} * \texttt{logStep} ^n <  \texttt{maxVal}\f]
    The grid is logarithmic, so logStep must always be greater then 1. Default value is 1.
    */
    CV_PROP_RW double logStep;

    /** @brief Creates a ParamGrid Ptr that can be given to the %SVM::trainAuto method

    @param minVal minimum value of the parameter grid
    @param maxVal maximum value of the parameter grid
    @param logstep Logarithmic step for iterating the statmodel parameter
    */
    CV_WRAP static Ptr<ParamGrid> create(double minVal=0., double maxVal=0., double logstep=1.);
};

/** @brief Class encapsulating training data.

Please note that the class only specifies the interface of training data, but not implementation.
All the statistical model classes in _ml_ module accepts Ptr\<TrainData\> as parameter. In other
words, you can create your own class derived from TrainData and pass smart pointer to the instance
of this class into StatModel::train.

@sa @ref ml_intro_data
 */
class CV_EXPORTS_W TrainData
{
public:
    static inline float missingValue() { return FLT_MAX; }
    virtual ~TrainData();

    CV_WRAP virtual int getLayout() const = 0;
    CV_WRAP virtual int getNTrainSamples() const = 0;
    CV_WRAP virtual int getNTestSamples() const = 0;
    CV_WRAP virtual int getNSamples() const = 0;
    CV_WRAP virtual int getNVars() const = 0;
    CV_WRAP virtual int getNAllVars() const = 0;

    CV_WRAP virtual void getSample(InputArray varIdx, int sidx, float* buf) const = 0;
    CV_WRAP virtual Mat getSamples() const = 0;
    CV_WRAP virtual Mat getMissing() const = 0;

    /** @brief Returns matrix of train samples

    @param layout The requested layout. If it's different from the initial one, the matrix is
        transposed. See ml::SampleTypes.
    @param compressSamples if true, the function returns only the training samples (specified by
        sampleIdx)
    @param compressVars if true, the function returns the shorter training samples, containing only
        the active variables.

    In current implementation the function tries to avoid physical data copying and returns the
    matrix stored inside TrainData (unless the transposition or compression is needed).
     */
    CV_WRAP virtual Mat getTrainSamples(int layout=ROW_SAMPLE,
                                bool compressSamples=true,
                                bool compressVars=true) const = 0;

    /** @brief Returns the vector of responses

    The function returns ordered or the original categorical responses. Usually it's used in
    regression algorithms.
     */
    CV_WRAP virtual Mat getTrainResponses() const = 0;

    /** @brief Returns the vector of normalized categorical responses

    The function returns vector of responses. Each response is integer from `0` to `<number of
    classes>-1`. The actual label value can be retrieved then from the class label vector, see
    TrainData::getClassLabels.
     */
    CV_WRAP virtual Mat getTrainNormCatResponses() const = 0;
    CV_WRAP virtual Mat getTestResponses() const = 0;
    CV_WRAP virtual Mat getTestNormCatResponses() const = 0;
    CV_WRAP virtual Mat getResponses() const = 0;
    CV_WRAP virtual Mat getNormCatResponses() const = 0;
    CV_WRAP virtual Mat getSampleWeights() const = 0;
    CV_WRAP virtual Mat getTrainSampleWeights() const = 0;
    CV_WRAP virtual Mat getTestSampleWeights() const = 0;
    CV_WRAP virtual Mat getVarIdx() const = 0;
    CV_WRAP virtual Mat getVarType() const = 0;
    CV_WRAP Mat getVarSymbolFlags() const;
    CV_WRAP virtual int getResponseType() const = 0;
    CV_WRAP virtual Mat getTrainSampleIdx() const = 0;
    CV_WRAP virtual Mat getTestSampleIdx() const = 0;
    CV_WRAP virtual void getValues(int vi, InputArray sidx, float* values) const = 0;
    virtual void getNormCatValues(int vi, InputArray sidx, int* values) const = 0;
    CV_WRAP virtual Mat getDefaultSubstValues() const = 0;

    CV_WRAP virtual int getCatCount(int vi) const = 0;

    /** @brief Returns the vector of class labels

    The function returns vector of unique labels occurred in the responses.
     */
    CV_WRAP virtual Mat getClassLabels() const = 0;

    CV_WRAP virtual Mat getCatOfs() const = 0;
    CV_WRAP virtual Mat getCatMap() const = 0;

    /** @brief Splits the training data into the training and test parts
    @sa TrainData::setTrainTestSplitRatio
     */
    CV_WRAP virtual void setTrainTestSplit(int count, bool shuffle=true) = 0;

    /** @brief Splits the training data into the training and test parts

    The function selects a subset of specified relative size and then returns it as the training
    set. If the function is not called, all the data is used for training. Please, note that for
    each of TrainData::getTrain\* there is corresponding TrainData::getTest\*, so that the test
    subset can be retrieved and processed as well.
    @sa TrainData::setTrainTestSplit
     */
    CV_WRAP virtual void setTrainTestSplitRatio(double ratio, bool shuffle=true) = 0;
    CV_WRAP virtual void shuffleTrainTest() = 0;

    /** @brief Returns matrix of test samples */
    CV_WRAP Mat getTestSamples() const;

    /** @brief Returns vector of symbolic names captured in loadFromCSV() */
    CV_WRAP void getNames(std::vector<String>& names) const;

    CV_WRAP static Mat getSubVector(const Mat& vec, const Mat& idx);

    /** @brief Reads the dataset from a .csv file and returns the ready-to-use training data.

    @param filename The input file name
    @param headerLineCount The number of lines in the beginning to skip; besides the header, the
        function also skips empty lines and lines staring with `#`
    @param responseStartIdx Index of the first output variable. If -1, the function considers the
        last variable as the response
    @param responseEndIdx Index of the last output variable + 1. If -1, then there is single
        response variable at responseStartIdx.
    @param varTypeSpec The optional text string that specifies the variables' types. It has the
        format `ord[n1-n2,n3,n4-n5,...]cat[n6,n7-n8,...]`. That is, variables from `n1 to n2`
        (inclusive range), `n3`, `n4 to n5` ... are considered ordered and `n6`, `n7 to n8` ... are
        considered as categorical. The range `[n1..n2] + [n3] + [n4..n5] + ... + [n6] + [n7..n8]`
        should cover all the variables. If varTypeSpec is not specified, then algorithm uses the
        following rules:
        - all input variables are considered ordered by default. If some column contains has non-
          numerical values, e.g. 'apple', 'pear', 'apple', 'apple', 'mango', the corresponding
          variable is considered categorical.
        - if there are several output variables, they are all considered as ordered. Error is
          reported when non-numerical values are used.
        - if there is a single output variable, then if its values are non-numerical or are all
          integers, then it's considered categorical. Otherwise, it's considered ordered.
    @param delimiter The character used to separate values in each line.
    @param missch The character used to specify missing measurements. It should not be a digit.
        Although it's a non-numerical value, it surely does not affect the decision of whether the
        variable ordered or categorical.
    @note If the dataset only contains input variables and no responses, use responseStartIdx = -2
        and responseEndIdx = 0. The output variables vector will just contain zeros.
     */
    static Ptr<TrainData> loadFromCSV(const String& filename,
                                      int headerLineCount,
                                      int responseStartIdx=-1,
                                      int responseEndIdx=-1,
                                      const String& varTypeSpec=String(),
                                      char delimiter=',',
                                      char missch='?');

    /** @brief Creates training data from in-memory arrays.

    @param samples matrix of samples. It should have CV_32F type.
    @param layout see ml::SampleTypes.
    @param responses matrix of responses. If the responses are scalar, they should be stored as a
        single row or as a single column. The matrix should have type CV_32F or CV_32S (in the
        former case the responses are considered as ordered by default; in the latter case - as
        categorical)
    @param varIdx vector specifying which variables to use for training. It can be an integer vector
        (CV_32S) containing 0-based variable indices or byte vector (CV_8U) containing a mask of
        active variables.
    @param sampleIdx vector specifying which samples to use for training. It can be an integer
        vector (CV_32S) containing 0-based sample indices or byte vector (CV_8U) containing a mask
        of training samples.
    @param sampleWeights optional vector with weights for each sample. It should have CV_32F type.
    @param varType optional vector of type CV_8U and size `<number_of_variables_in_samples> +
        <number_of_variables_in_responses>`, containing types of each input and output variable. See
        ml::VariableTypes.
     */
    CV_WRAP static Ptr<TrainData> create(InputArray samples, int layout, InputArray responses,
                                 InputArray varIdx=noArray(), InputArray sampleIdx=noArray(),
                                 InputArray sampleWeights=noArray(), InputArray varType=noArray());
};

/** @brief Base class for statistical models in OpenCV ML.
 */
class CV_EXPORTS_W StatModel : public Algorithm
{
public:
    /** Predict options */
    enum Flags {
        UPDATE_MODEL = 1,
        RAW_OUTPUT=1, //!< makes the method return the raw results (the sum), not the class label
        COMPRESSED_INPUT=2,
        PREPROCESSED_INPUT=4
    };

    /** @brief Returns the number of variables in training samples */
    CV_WRAP virtual int getVarCount() const = 0;

    CV_WRAP virtual bool empty() const;

    /** @brief Returns true if the model is trained */
    CV_WRAP virtual bool isTrained() const = 0;
    /** @brief Returns true if the model is classifier */
    CV_WRAP virtual bool isClassifier() const = 0;

    /** @brief Trains the statistical model

    @param trainData training data that can be loaded from file using TrainData::loadFromCSV or
        created with TrainData::create.
    @param flags optional flags, depending on the model. Some of the models can be updated with the
        new training samples, not completely overwritten (such as NormalBayesClassifier or ANN_MLP).
     */
    CV_WRAP virtual bool train( const Ptr<TrainData>& trainData, int flags=0 );

    /** @brief Trains the statistical model

    @param samples training samples
    @param layout See ml::SampleTypes.
    @param responses vector of responses associated with the training samples.
    */
    CV_WRAP virtual bool train( InputArray samples, int layout, InputArray responses );

    /** @brief Computes error on the training or test dataset

    @param data the training data
    @param test if true, the error is computed over the test subset of the data, otherwise it's
        computed over the training subset of the data. Please note that if you loaded a completely
        different dataset to evaluate already trained classifier, you will probably want not to set
        the test subset at all with TrainData::setTrainTestSplitRatio and specify test=false, so
        that the error is computed for the whole new set. Yes, this sounds a bit confusing.
    @param resp the optional output responses.

    The method uses StatModel::predict to compute the error. For regression models the error is
    computed as RMS, for classifiers - as a percent of missclassified samples (0%-100%).
     */
    CV_WRAP virtual float calcError( const Ptr<TrainData>& data, bool test, OutputArray resp ) const;

    /** @brief Predicts response(s) for the provided sample(s)

    @param samples The input samples, floating-point matrix
    @param results The optional output matrix of results.
    @param flags The optional flags, model-dependent. See cv::ml::StatModel::Flags.
     */
    CV_WRAP virtual float predict( InputArray samples, OutputArray results=noArray(), int flags=0 ) const = 0;

    /** @brief Create and train model with default parameters

    The class must implement static `create()` method with no parameters or with all default parameter values
    */
    template<typename _Tp> static Ptr<_Tp> train(const Ptr<TrainData>& data, int flags=0)
    {
        Ptr<_Tp> model = _Tp::create();
        return !model.empty() && model->train(data, flags) ? model : Ptr<_Tp>();
    }
};

/****************************************************************************************\
*                                 Normal Bayes Classifier                                *
\****************************************************************************************/

/** @brief Bayes classifier for normally distributed data.

@sa @ref ml_intro_bayes
 */
class CV_EXPORTS_W NormalBayesClassifier : public StatModel
{
public:
    /** @brief Predicts the response for sample(s).

    The method estimates the most probable classes for input vectors. Input vectors (one or more)
    are stored as rows of the matrix inputs. In case of multiple input vectors, there should be one
    output vector outputs. The predicted class for a single input vector is returned by the method.
    The vector outputProbs contains the output probabilities corresponding to each element of
    result.
     */
    CV_WRAP virtual float predictProb( InputArray inputs, OutputArray outputs,
                               OutputArray outputProbs, int flags=0 ) const = 0;

    /** Creates empty model
    Use StatModel::train to train the model after creation. */
    CV_WRAP static Ptr<NormalBayesClassifier> create();

    /** @brief Loads and creates a serialized NormalBayesClassifier from a file
     *
     * Use NormalBayesClassifier::save to serialize and store an NormalBayesClassifier to disk.
     * Load the NormalBayesClassifier from this file again, by calling this function with the path to the file.
     * Optionally specify the node for the file containing the classifier
     *
     * @param filepath path to serialized NormalBayesClassifier
     * @param nodeName name of node containing the classifier
     */
    CV_WRAP static Ptr<NormalBayesClassifier> load(const String& filepath , const String& nodeName = String());
};

/****************************************************************************************\
*                          K-Nearest Neighbour Classifier                                *
\****************************************************************************************/

/** @brief The class implements K-Nearest Neighbors model

@sa @ref ml_intro_knn
 */
class CV_EXPORTS_W KNearest : public StatModel
{
public:

    /** Default number of neighbors to use in predict method. */
    /** @see setDefaultK */
    CV_WRAP virtual int getDefaultK() const = 0;
    /** @copybrief getDefaultK @see getDefaultK */
    CV_WRAP virtual void setDefaultK(int val) = 0;

    /** Whether classification or regression model should be trained. */
    /** @see setIsClassifier */
    CV_WRAP virtual bool getIsClassifier() const = 0;
    /** @copybrief getIsClassifier @see getIsClassifier */
    CV_WRAP virtual void setIsClassifier(bool val) = 0;

    /** Parameter for KDTree implementation. */
    /** @see setEmax */
    CV_WRAP virtual int getEmax() const = 0;
    /** @copybrief getEmax @see getEmax */
    CV_WRAP virtual void setEmax(int val) = 0;

    /** %Algorithm type, one of KNearest::Types. */
    /** @see setAlgorithmType */
    CV_WRAP virtual int getAlgorithmType() const = 0;
    /** @copybrief getAlgorithmType @see getAlgorithmType */
    CV_WRAP virtual void setAlgorithmType(int val) = 0;

    /** @brief Finds the neighbors and predicts responses for input vectors.

    @param samples Input samples stored by rows. It is a single-precision floating-point matrix of
        `<number_of_samples> * k` size.
    @param k Number of used nearest neighbors. Should be greater than 1.
    @param results Vector with results of prediction (regression or classification) for each input
        sample. It is a single-precision floating-point vector with `<number_of_samples>` elements.
    @param neighborResponses Optional output values for corresponding neighbors. It is a single-
        precision floating-point matrix of `<number_of_samples> * k` size.
    @param dist Optional output distances from the input vectors to the corresponding neighbors. It
        is a single-precision floating-point matrix of `<number_of_samples> * k` size.

    For each input vector (a row of the matrix samples), the method finds the k nearest neighbors.
    In case of regression, the predicted result is a mean value of the particular vector's neighbor
    responses. In case of classification, the class is determined by voting.

    For each input vector, the neighbors are sorted by their distances to the vector.

    In case of C++ interface you can use output pointers to empty matrices and the function will
    allocate memory itself.

    If only a single input vector is passed, all output matrices are optional and the predicted
    value is returned by the method.

    The function is parallelized with the TBB library.
     */
    CV_WRAP virtual float findNearest( InputArray samples, int k,
                               OutputArray results,
                               OutputArray neighborResponses=noArray(),
                               OutputArray dist=noArray() ) const = 0;

    /** @brief Implementations of KNearest algorithm
       */
    enum Types
    {
        BRUTE_FORCE=1,
        KDTREE=2
    };

    /** @brief Creates the empty model

    The static method creates empty %KNearest classifier. It should be then trained using StatModel::train method.
     */
    CV_WRAP static Ptr<KNearest> create();
};

/****************************************************************************************\
*                                   Support Vector Machines                              *
\****************************************************************************************/

/** @brief Support Vector Machines.

@sa @ref ml_intro_svm
 */
class CV_EXPORTS_W SVM : public StatModel
{
public:

    class CV_EXPORTS Kernel : public Algorithm
    {
    public:
        virtual int getType() const = 0;
        virtual void calc( int vcount, int n, const float* vecs, const float* another, float* results ) = 0;
    };

    /** Type of a %SVM formulation.
    See SVM::Types. Default value is SVM::C_SVC. */
    /** @see setType */
    CV_WRAP virtual int getType() const = 0;
    /** @copybrief getType @see getType */
    CV_WRAP virtual void setType(int val) = 0;

    /** Parameter \f$\gamma\f$ of a kernel function.
    For SVM::POLY, SVM::RBF, SVM::SIGMOID or SVM::CHI2. Default value is 1. */
    /** @see setGamma */
    CV_WRAP virtual double getGamma() const = 0;
    /** @copybrief getGamma @see getGamma */
    CV_WRAP virtual void setGamma(double val) = 0;

    /** Parameter _coef0_ of a kernel function.
    For SVM::POLY or SVM::SIGMOID. Default value is 0.*/
    /** @see setCoef0 */
    CV_WRAP virtual double getCoef0() const = 0;
    /** @copybrief getCoef0 @see getCoef0 */
    CV_WRAP virtual void setCoef0(double val) = 0;

    /** Parameter _degree_ of a kernel function.
    For SVM::POLY. Default value is 0. */
    /** @see setDegree */
    CV_WRAP virtual double getDegree() const = 0;
    /** @copybrief getDegree @see getDegree */
    CV_WRAP virtual void setDegree(double val) = 0;

    /** Parameter _C_ of a %SVM optimization problem.
    For SVM::C_SVC, SVM::EPS_SVR or SVM::NU_SVR. Default value is 0. */
    /** @see setC */
    CV_WRAP virtual double getC() const = 0;
    /** @copybrief getC @see getC */
    CV_WRAP virtual void setC(double val) = 0;

    /** Parameter \f$\nu\f$ of a %SVM optimization problem.
    For SVM::NU_SVC, SVM::ONE_CLASS or SVM::NU_SVR. Default value is 0. */
    /** @see setNu */
    CV_WRAP virtual double getNu() const = 0;
    /** @copybrief getNu @see getNu */
    CV_WRAP virtual void setNu(double val) = 0;

    /** Parameter \f$\epsilon\f$ of a %SVM optimization problem.
    For SVM::EPS_SVR. Default value is 0. */
    /** @see setP */
    CV_WRAP virtual double getP() const = 0;
    /** @copybrief getP @see getP */
    CV_WRAP virtual void setP(double val) = 0;

    /** Optional weights in the SVM::C_SVC problem, assigned to particular classes.
    They are multiplied by _C_ so the parameter _C_ of class _i_ becomes `classWeights(i) * C`. Thus
    these weights affect the misclassification penalty for different classes. The larger weight,
    the larger penalty on misclassification of data from the corresponding class. Default value is
    empty Mat. */
    /** @see setClassWeights */
    CV_WRAP virtual cv::Mat getClassWeights() const = 0;
    /** @copybrief getClassWeights @see getClassWeights */
    CV_WRAP virtual void setClassWeights(const cv::Mat &val) = 0;

    /** Termination criteria of the iterative %SVM training procedure which solves a partial
    case of constrained quadratic optimization problem.
    You can specify tolerance and/or the maximum number of iterations. Default value is
    `TermCriteria( TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, FLT_EPSILON )`; */
    /** @see setTermCriteria */
    CV_WRAP virtual cv::TermCriteria getTermCriteria() const = 0;
    /** @copybrief getTermCriteria @see getTermCriteria */
    CV_WRAP virtual void setTermCriteria(const cv::TermCriteria &val) = 0;

    /** Type of a %SVM kernel.
    See SVM::KernelTypes. Default value is SVM::RBF. */
    CV_WRAP virtual int getKernelType() const = 0;

    /** Initialize with one of predefined kernels.
    See SVM::KernelTypes. */
    CV_WRAP virtual void setKernel(int kernelType) = 0;

    /** Initialize with custom kernel.
    See SVM::Kernel class for implementation details */
    virtual void setCustomKernel(const Ptr<Kernel> &_kernel) = 0;

    //! %SVM type
    enum Types {
        /** C-Support Vector Classification. n-class classification (n \f$\geq\f$ 2), allows
        imperfect separation of classes with penalty multiplier C for outliers. */
        C_SVC=100,
        /** \f$\nu\f$-Support Vector Classification. n-class classification with possible
        imperfect separation. Parameter \f$\nu\f$ (in the range 0..1, the larger the value, the smoother
        the decision boundary) is used instead of C. */
        NU_SVC=101,
        /** Distribution Estimation (One-class %SVM). All the training data are from
        the same class, %SVM builds a boundary that separates the class from the rest of the feature
        space. */
        ONE_CLASS=102,
        /** \f$\epsilon\f$-Support Vector Regression. The distance between feature vectors
        from the training set and the fitting hyper-plane must be less than p. For outliers the
        penalty multiplier C is used. */
        EPS_SVR=103,
        /** \f$\nu\f$-Support Vector Regression. \f$\nu\f$ is used instead of p.
        See @cite LibSVM for details. */
        NU_SVR=104
    };

    /** @brief %SVM kernel type

    A comparison of different kernels on the following 2D test case with four classes. Four
    SVM::C_SVC SVMs have been trained (one against rest) with auto_train. Evaluation on three
    different kernels (SVM::CHI2, SVM::INTER, SVM::RBF). The color depicts the class with max score.
    Bright means max-score \> 0, dark means max-score \< 0.
    ![image](pics/SVM_Comparison.png)
    */
    enum KernelTypes {
        /** Returned by SVM::getKernelType in case when custom kernel has been set */
        CUSTOM=-1,
        /** Linear kernel. No mapping is done, linear discrimination (or regression) is
        done in the original feature space. It is the fastest option. \f$K(x_i, x_j) = x_i^T x_j\f$. */
        LINEAR=0,
        /** Polynomial kernel:
        \f$K(x_i, x_j) = (\gamma x_i^T x_j + coef0)^{degree}, \gamma > 0\f$. */
        POLY=1,
        /** Radial basis function (RBF), a good choice in most cases.
        \f$K(x_i, x_j) = e^{-\gamma ||x_i - x_j||^2}, \gamma > 0\f$. */
        RBF=2,
        /** Sigmoid kernel: \f$K(x_i, x_j) = \tanh(\gamma x_i^T x_j + coef0)\f$. */
        SIGMOID=3,
        /** Exponential Chi2 kernel, similar to the RBF kernel:
        \f$K(x_i, x_j) = e^{-\gamma \chi^2(x_i,x_j)}, \chi^2(x_i,x_j) = (x_i-x_j)^2/(x_i+x_j), \gamma > 0\f$. */
        CHI2=4,
        /** Histogram intersection kernel. A fast kernel. \f$K(x_i, x_j) = min(x_i,x_j)\f$. */
        INTER=5
    };

    //! %SVM params type
    enum ParamTypes {
        C=0,
        GAMMA=1,
        P=2,
        NU=3,
        COEF=4,
        DEGREE=5
    };

    /** @brief Trains an %SVM with optimal parameters.

    @param data the training data that can be constructed using TrainData::create or
        TrainData::loadFromCSV.
    @param kFold Cross-validation parameter. The training set is divided into kFold subsets. One
        subset is used to test the model, the others form the train set. So, the %SVM algorithm is
        executed kFold times.
    @param Cgrid grid for C
    @param gammaGrid grid for gamma
    @param pGrid grid for p
    @param nuGrid grid for nu
    @param coeffGrid grid for coeff
    @param degreeGrid grid for degree
    @param balanced If true and the problem is 2-class classification then the method creates more
        balanced cross-validation subsets that is proportions between classes in subsets are close
        to such proportion in the whole train dataset.

    The method trains the %SVM model automatically by choosing the optimal parameters C, gamma, p,
    nu, coef0, degree. Parameters are considered optimal when the cross-validation
    estimate of the test set error is minimal.

    If there is no need to optimize a parameter, the corresponding grid step should be set to any
    value less than or equal to 1. For example, to avoid optimization in gamma, set `gammaGrid.step
    = 0`, `gammaGrid.minVal`, `gamma_grid.maxVal` as arbitrary numbers. In this case, the value
    `Gamma` is taken for gamma.

    And, finally, if the optimization in a parameter is required but the corresponding grid is
    unknown, you may call the function SVM::getDefaultGrid. To generate a grid, for example, for
    gamma, call `SVM::getDefaultGrid(SVM::GAMMA)`.

    This function works for the classification (SVM::C_SVC or SVM::NU_SVC) as well as for the
    regression (SVM::EPS_SVR or SVM::NU_SVR). If it is SVM::ONE_CLASS, no optimization is made and
    the usual %SVM with parameters specified in params is executed.
     */
    virtual bool trainAuto( const Ptr<TrainData>& data, int kFold = 10,
                    ParamGrid Cgrid = getDefaultGrid(C),
                    ParamGrid gammaGrid  = getDefaultGrid(GAMMA),
                    ParamGrid pGrid      = getDefaultGrid(P),
                    ParamGrid nuGrid     = getDefaultGrid(NU),
                    ParamGrid coeffGrid  = getDefaultGrid(COEF),
                    ParamGrid degreeGrid = getDefaultGrid(DEGREE),
                    bool balanced=false) = 0;

    /** @brief Trains an %SVM with optimal parameters

    @param samples training samples
    @param layout See ml::SampleTypes.
    @param responses vector of responses associated with the training samples.
    @param kFold Cross-validation parameter. The training set is divided into kFold subsets. One
        subset is used to test the model, the others form the train set. So, the %SVM algorithm is
    @param Cgrid grid for C