/*
 * main.cpp
 *
 *  Created on: Mar 31, 2016
 *      Author: mastanca
 */

#include <iostream>
#include <string>
#include <vector>

#include "LispLineValidator.h"
#include "LispParser.h"
#include "Tokenizer.h"

#define INPUT_ARGUMENTS_ERROR 1
#define INVALID_LISP_LINE_ERROR 2

int main(int argc, char *argv[]){
//	std::string testing("(+ (+ 2 (- 10 7)) (* 2 2))"); // 9	// Is ok
//	std::string testing = "(- 25 (* 5 4) (/ 50 10))"; // 20	// Is ok
//	std::string testing  = "(+ 5 5 (- 2 (+ 0 1)))"; // 11	// Is ok
//	std::string testing = "(- 25 (* 5 4))"; // 5 // Is ok
//	std::string testing = "(+ 5 5 5)";	// 15// Is ok
//	std::string testing = "(+ (* 2 2) (/ 50 (+ 2 (+ 2 1))))"; // 14 // Is ok
//	std::string testing = "(print 1 2 3 4 5)"; // Print seems to work fine
//	std::string testing = "(list 1 2 3 4)"; // Seems ok
	std::string testing = "(list)"; // Returns "", seems fine
	if (argc > 1){
		argv++; // Using argv just to satisfy compiler
		std::cerr << "ERROR: argumentos" << std::endl;
		return INPUT_ARGUMENTS_ERROR;
	}
	std::cout << "Original string: " << testing << std::endl;
	LispParser aLispParser;
	std::string parsedLine(aLispParser.prepareLineForParsing(&testing));
	std::vector<std::string>* tokensVector;
	Tokenizer tokenizer;
	tokensVector = tokenizer.tokenize(parsedLine);
	LispLineValidator validator = LispLineValidator(tokensVector);
	if (validator.isValidLine() == false){
		std::cerr << "ERROR: <linea invalida>" << std::endl;
		return INVALID_LISP_LINE_ERROR;
	} else {
		aLispParser.parseLispLine(tokensVector);
	}
	return 0;
}


