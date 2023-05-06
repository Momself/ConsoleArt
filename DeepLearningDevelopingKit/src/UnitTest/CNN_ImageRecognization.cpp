/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	CNN Image Recognization   													   */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

// #define CNNImageRecognization

#ifdef CNNImageRecognization
#include "..\NeuralLib.h"

int main(int argc, char ** argv)
{
	srand((unsigned)time(NULL));
	Data::ImageSet XOImageTrainSet;
	XOImageTrainSet.LoadFromJson("F:\\Software\\Top Peoject\\DeepLearningProject\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\data\\XO\\TrainSet");

	Data::ImageSet XOImageTestSet;
	XOImageTestSet.LoadFromJson("F:\\Software\\Top Peoject\\DeepLearningProject\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\data\\XO\\TestSet");

	/***************************************************************************************************/
	// Initializing Convolutional Layer 1
	Neural::ConvLayerInitor convInitor1;
	convInitor1.InputSize = MathLib::Size(16, 16);
	convInitor1.KernelSize = MathLib::Size(3, 3);
	convInitor1.Stride = 1;
	convInitor1.KernelNum = 5;
	convInitor1.ActivationFunction = ActivationFunction::ReLU;
	convInitor1.PaddingMethod = Neural::PaddingMethod::Surround;
	convInitor1.PaddingNum = Neural::PaddingNum::ZeroPadding;
	Neural::ConvolutionalLayer convLayer1(convInitor1);

	/***************************************************************************************************/
	// Initializing Pooling Layer 1
	Neural::PoolLayerInitor poolInitor1;
	poolInitor1.InputSize = MathLib::Size(16, 16);
	poolInitor1.Stride = 4;
	poolInitor1.PoolSize = MathLib::Size(4, 4);
	poolInitor1.PoolingMethod = Neural::PoolingMethod::MaxPooling;
	poolInitor1.PaddingMethod = Neural::PaddingMethod::Surround;
	poolInitor1.PaddingNum = Neural::PaddingNum::ZeroPadding;
	Neural::PoolingLayer poolLayer1(poolInitor1);

	/***************************************************************************************************/
	// Initializing Convolutional Layer 2
	Neural::ConvLayerInitor convInitor2;
	convInitor2.InputSize = MathLib::Size(4, 4);
	convInitor2.KernelSize = MathLib::Size(3, 3);
	convInitor2.Stride = 1;
	convInitor2.KernelNum = 5;
	convInitor2.ActivationFunction = ActivationFunction::ReLU;
	convInitor2.PaddingMethod = Neural::PaddingMethod::Surround;
	convInitor2.PaddingNum = Neural::PaddingNum::ZeroPadding;
	Neural::ConvolutionalLayer convLayer2(convInitor2);

	/***************************************************************************************************/
	// Initializing Pooling Layer 2
	Neural::PoolLayerInitor poolInitor2;
	poolInitor2.InputSize = MathLib::Size(4, 4);
	poolInitor2.Stride = 2;
	poolInitor2.PoolSize = MathLib::Size(2, 2);
	poolInitor2.PoolingMethod = Neural::PoolingMethod::MaxPooling;
	poolInitor2.PaddingMethod = Neural::PaddingMethod::Surround;
	poolInitor2.PaddingNum = Neural::PaddingNum::ZeroPadding;
	Neural::PoolingLayer poolLayer2(poolInitor2);

	/***************************************************************************************************/
	// Initializing Process Layer
	Neural::ProcessLayerInitor processInitor;
	processInitor.InputSize = MathLib::Size(2, 2);
	processInitor.ProcessFunction = ReLU;
	processInitor.ProcessFunctionDerivative = ReLUDerivative;
	Neural::ProcessLayer process(processInitor);

	/***************************************************************************************************/
	// Initializing Serialize Layer
	Neural::SerializeLayerInitor serialInitor;
	serialInitor.SerializeSize = MathLib::Size(2 * 2 * 10, 1);
	serialInitor.DeserializeSize = MathLib::Size(2, 2);
	Neural::SerializeLayer serial(serialInitor);

	/***************************************************************************************************/
	// Initializing FullConnect Layer
	Neural::InputLayer inputLayer(2 * 2 * 10, 2 * 2 * 10);
	inputLayer.SetActivationFunction(ActivationFunction::Sigmoid);
	inputLayer.SetLossFunction(LossFunction::MES);

	Neural::HiddenLayer hiddenLayer(2 * 2 * 10, 2 * 2 * 10);
	hiddenLayer.SetActivationFunction(ActivationFunction::ReLU);
	hiddenLayer.SetLossFunction(LossFunction::MES);

	Neural::OutputLayer outputLayer(2 * 2 * 10, 2);
	outputLayer.SetActivationFunction(ActivationFunction::Sigmoid);
	outputLayer.SetLossFunction(LossFunction::MES);

	int totalIteration = 100;

	for (size_t iteration = 0; iteration < totalIteration; iteration++)
	{
		for (size_t ID = 0; ID < XOImageTrainSet.GetSampleSize(); ID++)
		{
			auto sample = XOImageTrainSet.GetRandomSample();

			std::vector<MathLib::Matrix<double>> input;
			input.clear();
			input.push_back(sample.first + Random());

			/***************************************************************************************************/
			// Forward Propagation
			convLayer1.SetInput(input);
			convLayer1.ForwardPropagation();
			std::vector<Neural::ConvKernel> conv1kernals = convLayer1.GetKernelAll();
			std::vector<Neural::ConvFeature> conv1features = convLayer1.GetFeatureAll();

			poolLayer1.SetInput(conv1features);
			poolLayer1.ForwardPropagation();
			std::vector<Neural::Feature> pool1features = poolLayer1.GetFeatureAll();

			convLayer2.SetInput(pool1features);
			convLayer2.ForwardPropagation();
			std::vector<Neural::ConvKernel> conv2kernals = convLayer2.GetKernelAll();
			std::vector<Neural::ConvFeature> conv2features = convLayer2.GetFeatureAll();

			poolLayer2.SetInput(conv2features);
			poolLayer2.ForwardProp