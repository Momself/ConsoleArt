/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	         OpenCV Test   															  */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

// #define OpenCVDebug

#ifdef OpenCVDebug

#include "..\Visualizer\Visualize.h"

int main(int argc, char ** argv)
{
	srand((unsigned)time(NULL));
	/*
	cv::Mat imageTest = cv::imread("F:\\Software\\Top Peoject\\DeepLearningProject\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\DeepLearningDevelopingKit\\data\\XO\\Data\\O_0.png");
	if (!imageTest.data)
		return -1;
	cv::resize(image