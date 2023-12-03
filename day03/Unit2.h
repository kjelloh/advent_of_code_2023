//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <memory>
//---------------------------------------------------------------------------

class c_LoggerIfc {
public:
	virtual void log(std::string s) = 0;
};

class c_Solver {
public:
    using Result = int;
	c_Solver(c_LoggerIfc* const logger) : logger{logger} {}
	Result to_solution(std::istream& in);
private:
	c_LoggerIfc* const logger{};
};

#endif
