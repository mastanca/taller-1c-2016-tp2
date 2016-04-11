/*
 * SubVectorService.h
 *
 *  Created on: Apr 10, 2016
 *      Author: mastanca
 */

#ifndef SRC_SUBVECTORSERVICE_H_
#define SRC_SUBVECTORSERVICE_H_

#include <sys/types.h>
#include <vector>
#include <string>

class SubVectorService {
private:
	uint counter;
	uint openingParenthesisCount;
	uint closingOnePosition;
	uint closingParenthesisCount;
public:
	SubVectorService() : counter(0), openingParenthesisCount(0), closingOnePosition(0), closingParenthesisCount(0) {}
	std::vector<std::string> run(std::vector<std::string>* vector, uint offset);
	virtual ~SubVectorService();
};

#endif /* SRC_SUBVECTORSERVICE_H_ */
