//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

c_Solver::Result c_Solver::to_solution(std::istream& in) {
	std::string entry{};
	int row{};
	while (std::getline(in,entry)) {
		int col{};
		this->logger->log(entry);
		std::string number{};
		for (char ch : entry) {
			bool do_log_pos{false};
			if (std::isdigit(ch)) {
				number += ch;
			}
			else if (ch != '.') {
				do_log_pos = true;
				std::string symbol{};
				symbol += ch;
				this->logger->log("symbol:");
				this->logger->append(symbol);
				if (!number.empty()) {
					// end of number
					this->logger->log("number:");
					this->logger->append(number);
					number.clear();
				}
			}
			else if (number.size()>0) {
				do_log_pos = true;
				// end of number
				this->logger->log("number:");
				this->logger->append(number);
				number.clear();
			}
			if (do_log_pos) {
				this->logger->append(" at row:");
				this->logger->append(std::to_string(row));
				this->logger->append(" col:");
				this->logger->append(std::to_string(col-1));
			}
			++col;
		}
        ++row;
	}
	return {};
}
