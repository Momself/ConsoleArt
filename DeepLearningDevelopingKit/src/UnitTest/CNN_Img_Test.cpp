/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	      ConvolutionalLayer Test   													     */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

#define ConvolutionalLayerImgDebug

#ifdef ConvolutionalLayerImgDebug

#include "..\Algorithm\NeuralNetwork\NeuralLib.h"
//#include "..\Visualizer\Visualize.h"

int main(int argc, char ** argv)
{
	cv::Mat imgRGB = cv::imread("F:\\Software\\Top Peoject\\DeepLearningProject\\DeepLearningDevelopingKit\\Dee