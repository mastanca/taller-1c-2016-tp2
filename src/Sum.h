/*
 * Sum.h
 *
 *  Created on: Apr 2, 2016
 *      Author: mastanca
 */

#ifndef SRC_LISPFUNCTIONS_SUM_H_
#define SRC_LISPFUNCTIONS_SUM_H_

#include <string>

#include "Function.h"
#include "lispFunctionConstants.h"

class Sum : public Function {
private:
	float realResult;
public:
	Sum() : Function(LISP_SUM, ""), realResult(0) {}
	void evaluate();
	virtual ~Sum();
	std::string getResult();
};



#endif /* SRC_LISPFUNCTIONS_SUM_H_ */