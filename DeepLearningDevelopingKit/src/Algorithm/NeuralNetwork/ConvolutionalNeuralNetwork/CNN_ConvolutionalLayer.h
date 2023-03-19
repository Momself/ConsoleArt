/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 Convolutional Neural Network     	                                          */
/*								        		 	    Convolutional Layer     	                                                   */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/
#pragma once

// Header files
#include "..\..\..\MathLib\MathLib.h"
#include "..\ActivationFunction.h"
#include "..\LossFunction.h"
#include "CNN_PaddingLayer.h"

/***************************************************************************************************/
// Namespace : Neural 
/// Provide Neural Network algorithm library.
namespace Neural
{
	// Define the Element datatype.
	/// Mainly using float and double.
	typedef double ElemType;

	// Convolutional Layer Initor
	/// Used for initialization of a ConvLayer.
	struct ConvLayerInitor
	{
		// Stride
		size_t Stride;
		// The numner of kernels.
		size_t KernelNum;
		// Size of input matrix. 
		MathLib::Size InputSize;
		// Size of convolutional kernel.
		MathLib::Size KernelSize;
		// Method of padding.
		PaddingMethod PaddingMethod;
		// Value uesd in Padding
		PaddingNum PaddingNum;
		// Activation Function
		ActivationFunction ActivationFunction;
	};

	// Define Kernel and Feature.
	typedef MathLib::Matrix<ElemType> ConvKernel;
	typedef MathLib::Matrix<ElemType> ConvFeature;

	/***************************************************************************************************/
	// Struct : ConvNode
	/// Node in convolutional layer.
	struct ConvNode
	{
		ConvNode() = default;

		ConvNode(const size_t _kernelM, const size_t _kernelN, const size_t _featureM, const size_t _featureN) {
			kernel.Init(_kernelM, _kernelN, MathLib::MatrixType::Random);
			kernelDelta.Init(_kernelM, _kernelN, MathLib::MatrixType::Zero);
			kernelDeltaSum.Init(_kernelM, _kernelN, MathLib::MatrixType::Zero);
			feature.Init(_featureM, _featureN, MathLib::MatrixType::Zero);
			bias = MathLib::Random();
			biasDelta = 0;
			biasDeltaSum = 0;
		}

		ConvNode(const MathLib::Size _kernelSize, const MathLib::Size _featureSize) {
			kernel.Init(_kernelSize.m, _kernelSize.n, MathLib::MatrixType::Random);
			kernelDelta.Init(_kernelSize.m, _kernelSize.n, MathLib::MatrixType::Zero);
			kernelDeltaSum.Init(_kernelSize.m, _kernelSize.n, MathLib::MatrixType::Zero);
			feature.Init(_featureSize.m, _featureSize.n, MathLib::MatrixType::Zero);
			bias = MathLib::Random();
			biasDelta = 0;
			biasDeltaSum = 0;
		}

		ConvKernel kernel;
		ElemType bias;
		ConvFeature feature;

		ConvKernel kernelDelta;
		ElemType biasDelta;

		ConvKernel kernelDeltaSum;
		ElemType biasDeltaSum;
	};

	/***************************************************************************************************/
	// Class : Convolutional Layer
	/// Used for extracting features out of input.
	class ConvolutionalLayer
	{
	public: // Constructors

		// Invoke constructor
		ConvolutionalLayer(const ConvLayerInitor & _initor);

	public: // Getter

		inline const ConvFeature GetFeature(const size_t _index) const { return _convNodes.at(_index).kernel; }
		inline const std::vector<ConvFeature> GetFeatureAll(void) const {
			std::vector<ConvFeature> features;
			for (const ConvNode & node : _convNodes)
				features.push_back(node.feature);
			return features;
		}

		inline const ConvKernel GetKernel(c