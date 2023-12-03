//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

c_Solver::Result c_Solver::to_solution(std::istream& in) {
	std::string entry{};
	while (std::getline(in,entry)) {
		this->logger->log(entry);
	}
	return {};
}
