/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	       Activation Function   	                                                       */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/
#pragma once

#include <iostream>
#include <vector>
#include <cmath>

/************************************************************************************************************/
// Activation functions 
/// Activation Function Enums
enum class ActivationFunction {
	Linear,
	Sigmoid,
	ReLU,
	LeakyReLU,
	ELU,
	Sinh,
	Cosh,
	Tanh,
	Softplus,
	Custom
}; 

enum class ActivationFunctionDerivative {
	LinearDerivative,
	SigmoidDerivative,
	ReLUDerivative,
	LeakyReLUDerivative,
	ELUDerivative,
	SinhDerivative,
	CoshDeriv