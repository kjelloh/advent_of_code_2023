//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include "Unit2.h"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TButton *Button1;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Puzzle1;
	TMenuItem *Exit1;
	TMenuItem *Solve1;
	TPanel *Panel3;
	TSplitter *Splitter2;
	TMemo *Memo2;
	TEdit *Edit1;
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall Solve1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
// BEGIN class c_LoggerIfc
// public:
void log(std::string const& s);
void append(std::string s);
// END class c_LoggerIfc
};
class LoggerHost : public c_LoggerIfc  {
public:
	LoggerHost(TForm1* parent) : host{parent} {}
	// class c_LoggerIfc {
	virtual void log(std::string s) {
		host->log(s);
	}
	virtual void append(std::string s) {
		host->append(s);
    }


private:
	TForm1* host{};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
