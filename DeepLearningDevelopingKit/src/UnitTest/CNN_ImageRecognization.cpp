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
	pool