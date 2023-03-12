/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	       LU Decomposotion 	                                                    */
/*								        		 	                Matrix   	                                                              */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/

#include "..\..\MathLib\MathLib.h"


namespace MathLib
{
	namespace MatrixDecomposition
	{
		// LU decomposition
		/// Factors a matrix as the product of a lower triangular matrix and an upper triangular matrix.
		enum class LUDMethod {
			Doolittle,
			Crout,
			Cholesky
		};

		template<class T>
		std::pair<MathLib::Matrix<T>, MathLib::Matrix<T>> LUD(
			const MathLib::Matrix<T> & _mat, const LUDMethod & _method);

		// Doolittle Method
		template<class T>
		std::pair<MathLib::Matrix<T>, MathLib::Matrix<T>> Doolittle(const MathLib::Matrix<T> & _mat);

		// Crout Method
		te