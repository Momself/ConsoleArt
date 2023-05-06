
/*
int main(int argc, char ** argv)
{
	srand((unsigned)time(NULL));
	std::cout << "Unit Test : CNN-PaddingLayer" << std::endl;
	std::cout << "Project : DeepLearningDevelopingKit\n";
	std::cout << "Branch  : Master\n";
	std::cout << "Version : Windows-x64-1.0.0-CPU\n";
	std::cout << "Copyright 2015-2018 Celestial Tech Inc.\nFor more check www.tianshicangxie.com\n";

	MathLib::Matrix<double> input1(10, 10, MathLib::MatrixType::Identity);
	std::vector<MathLib::Matrix<double>> input;
	input.push_back(input1);

	// Convolutional Layer Init
	Neural::ConvLayerInitor convInitor;
	convInitor.InputSize = MathLib::Size(10, 10);
	convInitor.KernelSize = MathLib::Size(3, 3);
	convInitor.Stride = 1;
	convInitor.KernelNum = 5;
	convInitor.ActivationFunction = ActivationFunction::Linear;
	convInitor.PaddingMethod = Neural::PaddingMethod::Surround;
	convInitor.PaddingNum = Neural::PaddingNum::ZeroPadding;
	Neural::ConvolutionalLayer convLaye