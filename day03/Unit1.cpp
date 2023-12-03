//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
// BEGIN class c_LoggerIfc
// public:
void TForm1::log(std::string const& s) {
	Memo2->Lines->Add(s.c_str());
}
void TForm1::append(std::string s) {
	Memo2->Text += s.c_str();
}

// END class c_LoggerIfc
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Exit1Click(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Solve1Click(TObject *Sender)
{
	auto host = std::make_unique<LoggerHost>(this);
	AnsiString s = Memo1->Text;
	std::istringstream in{s.c_str()};
	c_Solver solver{host.get()};
	auto answer = solver.to_solution(in);
	Edit1->Text = answer;
}
//---------------------------------------------------------------------------



