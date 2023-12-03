//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit2.h"
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <iterator>
#include <numeric>
//---------------------------------------------------------------------------
#pragma package(smart_init)

c_Solver::Result c_Solver::to_solution(std::istream& in) {
	std::string entry{};
	int row{};
	using Position = std::pair<int,int>;
	using Model = std::vector<std::pair<Position,std::string>>;
    Model model{};
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
				model.push_back({Position{row,col},symbol});
				if (!number.empty()) {
					// end of number
					model.push_back({Position{row,col-1},number});
					this->logger->log("number:");
					this->logger->append(number);
					number.clear();
				}
			}
			else if (number.size()>0) {
				do_log_pos = true;
				// end of number
				model.push_back({Position{row,col-1},number});
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
	std::map<Position,int> valid_numbers{};
	for (auto const& entry : model) {
		if (entry.second.size() > 1) {
			// A number
			if (std::any_of(model.begin(),model.end(),[pos=entry.first](auto const& entry){
				return (entry.second.size()==1); // Test = find any symbol
			})) {
				valid_numbers[entry.first]=std::stoi(entry.second);
			}
		}
	}
	auto result = std::accumulate(valid_numbers.begin(),valid_numbers.end(),Result{0},[this](auto acc,auto const& entry){
		acc += entry.second;
		this->logger->log(std::to_string(entry.second));
		return acc;
	});
	return result;
}
