//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit2.h"
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <queue>
//---------------------------------------------------------------------------
#pragma package(smart_init)

c_Solver::Result c_Solver::to_solution(std::istream& in) {
	std::string entry{};
	int row{};
	using Position = std::pair<int,int>;
	std::map<Position,std::string> possible_numbers{};
	std::map<Position,char> symbols{};
	while (std::getline(in,entry)) {
		int col{};
		this->logger->log(entry);
		std::string number{};
		for (char ch : entry) {
			bool do_log_pos{false};
			if (ch<=' ') {
				// skip white spaces (unclear why they are there though?)
			}
			else if (std::isdigit(ch)) {
				number += ch;
			}
			else if (ch != '.') {
				do_log_pos = true;
				std::string symbol{};
				symbol += ch;
				this->logger->log("symbol:'");
				this->logger->append(symbol);
				symbols[Position{row,col}] = ch;
				if (!number.empty()) {
					// end of number
					possible_numbers[Position{row,col-1}] = number;
					this->logger->log("number:'");
					this->logger->append(number);
					number.clear();
				}
			}
			else if (number.size()>0) {
				do_log_pos = true;
				// end of number
				possible_numbers[Position{row,col-1}] = number;
				this->logger->log("number:'");
				this->logger->append(number);
				number.clear();
			}
			if (do_log_pos) {
				this->logger->append("' at row:");
				this->logger->append(std::to_string(row));
				this->logger->append(" col:");
				this->logger->append(std::to_string(col-1));
			}
			++col;
		}
		++row;
	}
	std::map<Position,std::string> valid_numbers{};
	std::queue<std::pair<Position,std::string>> q{};
	for (auto const& candidate : possible_numbers) {q.push(candidate);}
	while (!q.empty()) {
		auto number = q.front();q.pop();
		auto pos = number.first;
		this->logger->log("pop "); this->logger->append(number.second);
		this->logger->append(" [");
		this->logger->append(std::to_string(number.first.first));
		this->logger->append(",");
		this->logger->append(std::to_string(number.first.second));
		this->logger->append("]");
		bool loop_again{true};
		this->logger->log("Scanning:");
		for (int row = pos.first-1;row<=pos.first+1 and loop_again;++row) {
			for (int col = pos.second-number.second.size();col<=pos.second+1 and loop_again;++col) {
				Position at{row,col};
				this->logger->append("[");
				this->logger->append(std::to_string(row));
				this->logger->append(",");
				this->logger->append(std::to_string(col));
				this->logger->append("]");
				if (symbols.count(at)>0) {
					valid_numbers[pos]=number.second;
			   		this->logger->log("valid!");
                    loop_again = false;
				}
			}
		}
	}
	auto result = std::accumulate(valid_numbers.begin(),valid_numbers.end(),Result{0},[this](auto acc,auto const& entry){
		acc += std::stoi(entry.second);
		this->logger->log(entry.second);
		return acc;
	});
	// 536655 is too low
	this->logger->log("Answer:");
    this->logger->append(std::to_string(result));
	return result;
}
