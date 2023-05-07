
#include "..\Algorithm\NeuralNetwork\NeuralLib.h"

/*
int main(int argc, char ** argv)
{
	srand((unsigned)time(NULL));
	std::cout << "Unit Test : CNN-PaddingLayer" << std::endl;
	std::cout << "Project : DeepLearningDevelopingKit\n";
	std::cout << "Branch  : Master\n";
	std::cout << "Version : Windows-x64-1.0.0-CPU\n";
	std::cout << "Copyright 2015-2018 Celestial Tech Inc.\nFor more check www.tianshicangxie.com\n";


	MathLib::Matrix<double> testMat(5, 5, MathLib::MatrixType::Ones);
	std::cout << "Original input: " << testMat << std::endl;
	MathLib::Matrix<double> result;

	std::cout << "--PaddingMethod--" << std::endl;
	result = Neural::Pad::Padding(testMat, Neural::PaddingMethod::LeftDown, Neural::PaddingNum::ZeroPadding, 1, 1);
	std::cout << "PaddingMethod::LeftDown: \n" << result << std::endl;
	result = Neural::Pad::Padding(testMat, Neural::PaddingMethod::LeftUp, Neural::PaddingNum::ZeroPadding, 1, 1);
	std::cout << "PaddingMethod::LeftUp: \n" << result << std::endl;
	result = Neural::Pad::Padding(testMat, Neural::PaddingMethod::RightDown, Neural::PaddingNum::ZeroPadding, 1, 1);
	std::cout << "PaddingMethod::RightDown: \n" << result << std::endl;
	result = Neural::Pad::P