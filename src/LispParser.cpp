/*
 * lispParser.cpp
 *
 *  Created on: Apr 1, 2016
 *      Author: mastanca
 */

#include "LispParser.h"

#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Constant.h"
#include "lispFunctionConstants.h"
#include "lispFunctions/Divide.h"
#include "lispFunctions/Minus.h"
#include "lispFunctions/Multiply.h"
#include "lispFunctions/Sum.h"

int LispParser::parseLispLine(std::vector<std::string>* lispLine) {
//	Expression* expression = getExpression(lispLine);
//	expression->evaluate();
	getExpression(lispLine);

	return EXIT_SUCCESS;
}

std::string LispParser::prepareLineForParsing(std::string* lispLine) {
	char tempChar;
	std::stringstream newString;
	for (uint i = 0; i < lispLine->size(); ++i) {
		tempChar = lispLine->at(i);
		if (tempChar == OPENING_PARENTHESIS || tempChar == CLOSING_PARENTHESIS) {
			std::string withSpaces = (" " + std::string(sizeof(char), tempChar)
					+ " ");
			newString << withSpaces;
		} else {
			newString << tempChar;
		}
	}
	return newString.str();
}

LispParser::~LispParser() {
	for (std::vector<Expression*>::iterator it = expressionPointers.begin() ; it != expressionPointers.end(); ++it){
		if (*it != NULL)
			delete *it;
	}
}

Expression* LispParser::getExpression(std::vector<std::string>* lispLine) {
	Expression* expressionToReturn = NULL;
	if (lastPos == 0)
		lastPos = lispLine->size();
	std::stringstream returningExpression;
	int i = 0;
	while (currentPos < lastPos && i < (int)lispLine->size()) {
		++currentPos;
		std::string element = lispLine->at(i);

		if (element == ")") {

			// If it's a closing one append and return
			std::cout << "Closing expression" << std::endl;

		} else if (isNumeric(element, 10)) {

			// If it's a number then append and return
			expressionToReturn = getConstant(element);
			expressionToReturn->evaluate();

		} else if (getFunction(element) != NULL) {

			// If it's a function then append symbol and get expression of following element
			expressionToReturn = parseFunction(lispLine, &i, &element);

		} else if (element == "(") {

			// if it's an opening one get recursive
			std::cout << "Opening expression" << std::endl;
			std::vector<std::string> subvec = getSubVector(lispLine, i + 1);
			expressionToReturn = getExpression(&subvec);
			i += subvec.size() -1;
		}
		++i;
	}

	return expressionToReturn;
}

Function* LispParser::getFunction(std::string &string) {
	if (string == LISP_SUM){
		Sum* aSum = new Sum();
		expressionPointers.push_back(aSum);
		return aSum;
	}
	if (string == LISP_MINUS){
		Minus* aMinus = new Minus();
		expressionPointers.push_back(aMinus);
		return aMinus;
	}
	if (string == LISP_MULTIPLY){
		Multiply* aMultiply = new Multiply();
		expressionPointers.push_back(aMultiply);
		return aMultiply;
	}
	if (string == LISP_DIVIDE){
		Divide* aDivide = new Divide();
		expressionPointers.push_back(aDivide);
		return aDivide;
	}
//	if (string == "=")
//		return "I am an equals";
//	if (string == ">")
//		return "I am a bigger than";
//	if (string == "<")
//		return "I am a less than";
//	if (string == "car")
//		return "I am a car";
//	if (string == "cdr")
//		return "I am a cdr";
//	if (string == "append")
//		return "I am an append";
//	if (string == "if")
//		return "I am an if";
//	if (string == "defun")
//		return "I am a defun";
//	if (string == "print")
//		return "I am a print";
//	if (string == "setq")
//		return "I am a setq";
//	if (string == "list")
//		return "I am a list";
//	if (string == "sync")
//		return "I am a sync";
	return NULL;
}

Expression* LispParser::getConstant(std::string &string) {
	Constant* aConstant = new Constant(atoi(string.c_str()));
	expressionPointers.push_back(aConstant);
	return aConstant;
}

bool LispParser::isNumeric(std::string pszInput, int nNumberBase) {
	std::string base = "0123456789ABCDEF";
	std::string input = pszInput;

	return (input.find_first_not_of(base.substr(0, nNumberBase))
			== std::string::npos);
}

std::vector<std::string> LispParser::getSubVector(
		std::vector<std::string>* vector, unsigned int offset) {
	// Will return a sub vector until the end of the current level
	uint counter = 0;
	uint openingParenthesisCount = 0;
	uint closingParenthesisCount = 0;
	uint closingOnePosition = 0;
	bool quit = false;
	for (std::vector<std::string>::iterator it = vector->begin() + offset ; it != vector->end() && !quit; ++it){
		if (*it == "("){
			++openingParenthesisCount;
		} else if (*it == ")"){
			++closingParenthesisCount;
		}
		if ((closingParenthesisCount - openingParenthesisCount) == 1){
			closingOnePosition = counter;
			quit = true;
		}
		++counter;
	}
	// Up to here we have the position of the levels closing parenthesis
	std::vector<std::string> newVec(vector->begin() + offset, vector->begin() + offset + closingOnePosition + 1);
	return newVec;
}

Expression* LispParser::parseFunction(std::vector<std::string>* lispLine, int* position, std::string* element){
	Function* function = getFunction(*element);
	std::stringstream returningExpression;
	returningExpression << function->getIdentifier();
	std::cout << returningExpression.str() << std::endl;
	std::vector<std::string> subvec = getSubVector(lispLine, *position + 1);
	for (std::vector<std::string>::iterator it = subvec.begin() ; it != subvec.end(); ++it){
		// Its failing here when reaching the end!
		std::cout << *it << std::endl;
		if (*it == "("){
			std::vector<std::string> anotherVec = getSubVector(&subvec, *position + 1);
			Expression* tempExpression = getExpression(&anotherVec);
			function->appendArgument(tempExpression);
			it += anotherVec.size();
			// We add size + 1 to maintain the offset
			*position += anotherVec.size() + 1;
		} else {
			// We need a vector argument so...
			std::vector<std::string> tempVec;
			tempVec.push_back(*it);
			Expression* tempExpression = getExpression(&tempVec);
			if (tempExpression != NULL)
				// Its NULL if it was a ")"
				function->appendArgument(tempExpression);
			// Keep moving original lisp line cursor
			++*position;
		}

	}
	std::cout << "I am " << function->getIdentifier() << " , and have " <<
			function->getArguments().size() << " arguments" <<
			". My result is ";
	function->evaluate();
	std::cout << function->getResult() << std::endl;
	return function;
}
