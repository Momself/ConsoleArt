/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 			  Json Parser 	                                                              */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/
#include "JsonParser.h"

void VectorParser::Serialize(const std::string & _filePath, const std::vector<MathLib::Vector<double>>& _vectors)
{
	std::vector<MathLib::Vector<double>> tempBuffer;
	JsonHandler 