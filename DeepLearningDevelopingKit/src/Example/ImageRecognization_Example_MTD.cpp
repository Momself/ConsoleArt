/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	CNN Image Recognization   													   */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

// #define CNNImageRecognization

#ifdef CNNImageRecognization
#include "..\\Algorithm\NeuralNetwork\NeuralLib.h"

Vector<double> Matrix2Vector(const Matrix<double> & _mat);
Matrix<double> Vector2Matrix(const Vector<double> & _vec);

#include <thread>
#include <vector>
#include <mutex>

std::mutex workQueueMutex;
std::mutex plotMutex;
bool threadLifeFlag;

static void trainThreadFunc(
	Neural::ConvolutionalLayer convLayer1,
	Neural::PoolingLayer poolLayer1,
	Neural::ProcessLayer process1,
	Neural::ConvolutionalLayer convLayer2,
	Neural::PoolingLayer poolLayer2,
	Neural::ProcessLayer process2,
	Neural::SerializeLayer serial,
	Neural::InputLayer inputLayer,
	Neural::HiddenLayer hiddenLayer1,
	Neural::HiddenLayer hiddenLayer2,
	Neural::OutputLayer outputLayer,
	Data::ImageSet & trainSet,
	std::queue<size_t> & workQueue
)
{
	int ID;
	bool workFlag = false;

	while (threadLifeFlag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		workQueueMutex.lock();
		if (!workQueue.empty())
		{
			ID = workQueue.front();
			workQueue.pop();
			workFlag = true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		workQueueMutex.unlock();

		if (workFlag)
		{
			auto sample = trainSet.GetSample(ID);

			// Initialzing input
			std::vector<MathLib::Matrix<double>> input{ sample.first + Random() };

			/***************************************************************************************************/
			// Forward Propagation
			// convLayer 1
			convLayer1.SetInput(input);
			convLayer1.ForwardPropagation();
			std::vector<Neural::ConvKernel> conv1kernals = convLayer1.GetKernelAll();
			std::vector<Neural::ConvFeature> conv1features = convLayer1.GetFeatureAll();
			// poolLayer 1
			poolLayer1.SetInput(conv1features);
			poolLayer1.ForwardPropagation();
			std::vector<Neural::Feature> pool1features = poolLayer1.GetFeatureAll();
			// processLayer 1
			process1.SetInput(pool1features);
			process1.Process();
			std::vector<Neural::Feature> process1output = process1.GetOutputAll();
			// convLayer 2
			convLayer2.SetInput(process1output);
			convLayer2.ForwardPropagation();
			std::vector<Neural::ConvKernel> conv2kernals = convLayer2.GetKernelAll();
			std::vector<Neural::ConvFeature> conv2features = convLayer2.GetFeatureAll();
			// poolLayer 2
			poolLayer2.SetInput(conv2features);
			poolLayer2.ForwardPropagation();
			std::vector<Neural::Feature> pool2features = poolLayer2.GetFeatureAll();
			// processLayer 2
			process2.SetInput(pool2features);
			process2.Process();
			std::vector<Neural::Feature> process2output = process2.GetOutputAll();
			// serialLayer
			serial.SetDeserializedMat(process2output);
			MathLib::Vector<double> serializedVec = Matrix2Vector(serial.Serialize());
			// inputLayer
			inputLayer.SetInput(serializedVec);
			inputLayer.ForwardPropagation();
			MathLib::Vector<double> inputout = inputLayer.GetOutput();
			// hiddenLayer 1
			hiddenLayer1.SetInput(inputout);
			hiddenLayer1.ForwardPropagation();
			MathLib::Vector<double> hidden1output = hiddenLayer1.GetOutput();
			// hiddenLayer 2
			hiddenLayer2.SetInput(hidden1output);
			hiddenLayer2.ForwardPropagation();
			MathLib::Vector<double> hidden2output = hiddenLayer2.GetOutput();
			// outputLayer
			outputLayer.SetInput(hidden2output);
			outputLayer.ForwardPropagation();
			MathLib::Vector<double> output = outputLayer.GetOutput();
			/***************************************************************************************************/

			// Initialzing lable
			MathLib::Vector<double> lable = sample.second;

			// Calculating Error
			MathLib::Vector<double> error = output - lable;

			/***************************************************************************************************/
			// Backward Propagation
			MathLib::Vector<double> outputLayerDelta = outputLayer.BackwardPropagation(lable);
			// hiddenLayer 1
			MathLib::Vector<double> hiddenLayer2Delta = hiddenLayer2.BackwardPropagation(outputLayerDelta);
			// hiddenLayer 2
			MathLib::Vector<double> hiddenLayer1Delta = hiddenLayer1.BackwardPropagation(hiddenLayer2Delta);
			// outputLayer
			MathLib::Vector<double> inputLayerDelta = inputLayer.BackwardPropagation(hiddenLayer1Delta);
			// serialLayer
			serial.SetSerializedMat(Vector2Matrix(inputLayerDelta));
			std::vector<MathLib::Matrix<double>> deserialized = serial.Deserialize();
			// process 2
			process2.SetInput(deserialized);
			process2.Deprocess();
			std::vector<MathLib::Matrix<double>> deprocess2output = process2.GetOutputAll();
			// poolLayer 2
			poolLayer2.SetDelta(deprocess2output);
			poolLayer2.BackwardPropagation();
			std::vector<MathLib::Matrix<double>> pool2Delta = poolLayer2.GetDelta();
			// convLayer 2
			convLayer2.SetDelta(pool2Delta);
			convLayer2.BackwardPropagation();
			std::vector<MathLib::Matrix<double>> conv2Delta = convLayer2.GetDelta();
			// process 1
			process1.SetInput(conv2Delta);
			process1.Deprocess();
			std::vector<MathLib::Matrix<double>> deprocess1output = process1.GetOutputAll();
			// poolLayer 1
			poolLayer1.SetDelta(deprocess1output);
			poolLayer1.BackwardPropagation();
			std::vector<MathLib::Matrix<doubl