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
			a