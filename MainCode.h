//---------------------------------------------------------------------------

#ifndef MainCodeH
#define MainCodeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <math.h>
#include "ElectraMethod.h"
#include "ConstElec.h"

#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Mask.hpp>
#include "CSPIN.h"
#include "WinXP.hpp"
//Константы идентификаторы столбцов
const int SNM = 0;
const int SNAME = 1;
const int SWE = 2;
const int SSC = 3;
//Sheet Листы PageIndex
const int PAGEBEGIN = 0;
const int PAGERAT = 1;
const int PAGEINDEX = 2;
const int PAGERUN = 3;
//Данные изменены
const int ALTCOUNT = 0;
const int KRITCOUNT = 1;
const int DATAA = 2;
const int DATAK = 3;
const int DATARAT = 4;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TSpeedButton *OpenBtn;
   TSpeedButton *SaveBtn;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
   TPageControl *PageControl1;
   TTabSheet *SheetBegins;
   TStringGrid *Alternatives;
   TStringGrid *Kriterias;
   TTabSheet *SheetRatings;
   TStringGrid *Ratings;
   TButton *GetRatBtn;
   TButton *SetRatBtn;
   TTabSheet *SheetIndexes;
   TTabSheet *SheetRun;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *LabelS;
   TLabel *LabelNS;
   TTrackBar *TrackBar1;
   TTrackBar *TrackBar2;
   TListBox *ListBox1;
   TPanel *Panel1;
   TScrollBar *ScrollBar1;
   TPanel *Panel2;
   TScrollBar *ScrollBar2;
   TLabel *LabelSC;
   TLabel *LabelNC;
   TCheckBox *Scrolls;
   TButton *Calc;
   TStaticText *ModRat;
   TPanel *Panel3;
   TLabel *LA;
   TCSpinEdit *nAlt;
   TLabel *LK;
   TCSpinEdit *nKrit;
   TSplitter *Splitter1;
   TPanel *Panel4;
   TLabel *Label5;
   TSplitter *Splitter2;
   TPanel *Panel5;
   TStringGrid *NotSoglas;
   TLabel *Label3;
   TStringGrid *Soglas;
   TButton *RunBtn;
   TWinXP *WinXP1;
   void __fastcall nAltChange(TObject *Sender);
   void __fastcall nKritChange(TObject *Sender);
   void __fastcall SaveBtnClick(TObject *Sender);
   void __fastcall OpenBtnClick(TObject *Sender);
   void __fastcall PageControl1Changing(TObject *Sender,
          bool &AllowChange);
   void __fastcall GetRatBtnClick(TObject *Sender);
   void __fastcall SetRatBtnClick(TObject *Sender);
   void __fastcall RunBtnClick(TObject *Sender);
   void __fastcall TrackBar1Change(TObject *Sender);
   void __fastcall TrackBar2Change(TObject *Sender);
   void __fastcall ScrollsClick(TObject *Sender);
   void __fastcall ScrollBar1Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
   void __fastcall ScrollBar2Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
   void __fastcall CalcClick(TObject *Sender);
   void __fastcall AlternativesKeyPress(TObject *Sender, char &Key);
   void __fastcall KriteriasKeyPress(TObject *Sender, char &Key);
   void __fastcall RatingsKeyPress(TObject *Sender, char &Key);
private:	// User declarations
   //bool FPageChange[5];
   bool FDataChange[5];
public:		// User declarations
   Electra electra;
   int FA, FK;
   void SetA(int rhs)
   {
      FA = rhs;
      A1 = rhs + 1;
      Alternatives->RowCount = A1;
      Ratings->ColCount = A1;
      ///electra.Alternatives.Count = A;   точно не надо
      if (A1 < 1)
      {
         Ratings->ColCount = A1;
      }
   }
   void SetK(int rhs)
   {
      FK = rhs;
      K1 = rhs + 1;
      Kriterias->RowCount = K1;
      Ratings->RowCount = K1;
      ////electra.Kriterias.Count = K;
      if (K1 < 1)
      {
         Ratings->RowCount = K1;
      }
   }

   __property int A = {read=FA, write=SetA};
   __property int K = {read=FK, write=SetK};
   int A1, K1;
   int FCurrC; double FCurrN;
   void SetCurrC(int value)
   {
      FCurrC = value;
      LabelS->Caption = CurrC;
      CalcDominating();
   }
   void SetCurrN(double value)
   {
      FCurrN = value;
      LabelNS->Caption = CurrN;
      CalcDominating();
   }
   __property int CurrC = {read=FCurrC, write=SetCurrC};
   __property double CurrN = {read=FCurrN, write=SetCurrN};
   bool GetDataChange(int i)   {      return FDataChange[i];   }  
   void SetDataChange(int i, bool value)
   {    
      FDataChange[i] = value;
      switch (i)
      {
      case DATAA :
         if (value)  Alternatives->Cells[NC][0] = "Альтернативы  Modified"; //ModA->Caption = "Modified";
         else        Alternatives->Cells[NC][0] = "Альтернативы";//ModA->Caption = "";
         break;
      case DATAK :
         if (value)  Kriterias->Cells[NC][0] = "Критерии  Modified";//ModK->Caption = "Modified";
         else        Kriterias->Cells[NC][0] = "Критерии";//ModK->Caption = "";
         break;

      case DATARAT :
         if (value)  ModRat->Caption = "Modified";
         else        ModRat->Caption = "";
         break;
      }
   }
   //__property bool PageChange [int i] = {read=GetPageChange, write=SetPageChange};
   __property bool DataChange [int i] = {read=GetDataChange, write=SetDataChange};
   void __fastcall CalcDominating();
   __fastcall TForm1(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
