
﻿/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	      Linear Regression     	                                                       */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

#include "LinearRegression.h"

/***************************************************************************************************/
// Class : LinearRegression
/// One variable linear resgression
Regression::LinearRegression::LinearRegression(void)
{
	this->_weight = 0.f;
	this->_bias = 0.f;
}

void Regression::LinearRegression::Train(void)
{
	OrdinaryLeastSquares();
}

void Regression::LinearRegression::Test(void) const
{
	std::cout << "y = " << _weight << " x + " << _bias << std::endl;
	std::cout << "Predict value for 5.3698 is " << Predict(5.3698) << "\n\n" << std::endl;
}

const double Regression::LinearRegression::Predict(const double _x) const 
{
	double predict = _weight * _x + _bias;
	return predict;
}

void Regression::LinearRegression::SetTrainSet(Data::NumericSet * _trainset)
{
	this->_trainset = _trainset;
}

void Regression::LinearRegression::SetTestSet(Data::NumericSet * _testset)
{
	this->_testset = _testset;
}

void Regression::LinearRegression::SetValidationSet(Data::NumericSet * _validationset)
{
	this->_validationset = _validationset;
}

void Regression::LinearRegression::OrdinaryLeastSquares(void)
{
	double sumX{ 0.f };
	double sumY{ 0.f };
	double 	sumXSquared{ 0.f };
	double 	sumXY{ 0.f };

	size_t tarinsetSize = _trainset->GetSize();
	for (int i = 0; i < tarinsetSize; i++)
	{
		Data::NumericSet::Sample sample = _trainset->GetSample(i);
		double x = sample.first(0);
		double y = sample.second(0);
		sumX += x;
		sumY += y;
		sumXSquared += x * x;
		sumXY += x * y;
	}

	if (fabs(tarinsetSize * sumXSquared - sumX * sumX) > DBL_EPSILON) 
	{
		this->_weight = (tarinsetSize * sumXY - sumY * sumX) / (tarinsetSize * sumXSquared - sumX * sumX);
		this->_bias = (sumXSquared * sumY - sumX * sumXY) / (tarinsetSize * sumXSquared - sumX * sumX);
	}
	else 
	{
		this->_weight = 0;
		this->_bias = 0;
	}
}

/***************************************************************************************************/
// Class : MultivariateLinearRegression (MLR)
/// Multi variable linear resgression
Regression::MultivariateLinearRegression::MultivariateLinearRegression(const size_t _inputNum)
{
	this->_theta.Init(_inputNum + 1, 1, MathLib::MatrixType::Random);
}

void Regression::MultivariateLinearRegression::Train(void)
{
	static const double learn_rate{ 0.00005 };
	unsigned int iterCount{ 0 };
	MathLib::Matrix<double> X(_trainset->GetSize(), _theta.ColumeSize());
	MathLib::Matrix<double> y_lable(_trainset->GetSize(), 1);

	for (size_t i = 0; i < _trainset->GetSize(); i++)
	{
		Data::NumericSet::Sample sample = _trainset->GetSample(i);
		for (size_t j = 0; j < sample.first.Size(); j++)
			X(i, j) = sample.first(j);
		X(i, sample.first.Size()) = 1;
		y_lable(i, 0) = sample.second(0);
	}
	MathLib::Matrix<double> x_hat(X.ColumeSize(), _theta.RowSize());

	MathLib::Matrix<double> sum(_theta.ColumeSize(), 1);
	do
	{
		for (size_t i = 0; i < _theta.ColumeSize(); i++)
		{
			sum(i, 0) = 0;
			for (size_t j = 0; j < X.ColumeSize(); j++)
				sum(i, 0) += (x_hat(j, 0) - y_lable(j, 0)) * X(j, i) / X.ColumeSize();
		}

		_theta -= sum * learn_rate;
		x_hat = X * _theta;
		iterCount++;

		if (iterCount % 100 == 0)
		{
			system("cls");
			std::cout << "/***********************************************************/" << std::endl;
			std::cout << "Multivariate Linear Regression Test : " << std::endl;
			std::cout << std::fixed << std::setprecision(3) << "Iteration : " << iterCount << " | "
				<< std::fixed << std::setprecision(3) << "Cost : " << CostFunction(x_hat, y_lable) << " | "
				<< "Percentage : " << 1 / CostFunction(x_hat, y_lable) << " % " << " | "
				// << std::fixed << std::setprecision(3) << "Time used : " << trainTimer.GetTime() << " ms" << " | "
				<< std::endl;
		}
	} while (CostFunction(x_hat, y_lable) > 0.5);
}

void Regression::MultivariateLinearRegression::Test(void) const
{
}

void Regression::MultivariateLinearRegression::SetTrainSet(Data::NumericSet * const _trainset)
{
	this->_trainset = _trainset;
}

void Regression::MultivariateLinearRegression::SetTestSet(Data::NumericSet * const _testset)
{
	this->_testset = _testset;
}

void Regression::MultivariateLinearRegression::SetValidationSet(Data::NumericSet * const _validationset)
{
	this->_validationset = _validationset;
}

const double Regression::MultivariateLinearRegression::CostFunction(const MathLib::Matrix<double>& _predict, const MathLib::Matrix<double>& _lable) const
{
	double sum{ 0.f };
	for (size_t i = 0; i < _predict.ColumeSize(); i++)
	{
		sum += (_predict(i, 0) - _lable(i, 0)) * (_predict(i, 0) - _lable(i, 0)) / _predict.ColumeSize();
	}
	return sum;
}