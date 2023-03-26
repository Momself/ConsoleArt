/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	          Process Layer     	                                                      */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

// Header files
#include "..\..\..\MathLib\MathLib.h"

/***************************************************************************************************/
// Namespace : Neural
/// Provide Neural Network algorithm library.
namespace Neural
{
	// Define the Element datatype.
	/// Mainly using float and double.
	typedef double ElemType;

	// Process Layer Initor
	/// Used for initialization of a ProcessLayer.
	struct ProcessLayerInitor
	{
		// The size of input Matrix.
		MathLib::Size InputSize;
		// The pointer pointed to a process function.
		///This function will be applied to every element in the matrix.
		ElemType(*ProcessFunction)(ElemType x);
		// The pointer pointed to the derivative of process function.
		ElemType(*ProcessFunctionDerivative)(ElemType x);
	};

	/***************************************************************************************************/
	// Class : Process Layer
	/// Used for processign data.Such as ReLU, Normalization, Regularization.
	class ProcessLayer
	{
	public: // Constructor

		ProcessLayer(const ProcessLayerInitor _initor);

		// Set the input of the ProcessLayer.
		void SetInput(const std::vector<MathLib::Matrix<ElemType>> &  _data);

		