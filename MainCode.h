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
//Константы идентификаторы столбцов
const int SNM = 0;
const int SNAME = 1;
const int SWE = 2;
const int SSC = 3;
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
   TTabSheet *TabSheet1;
   TStringGrid *Alternatives;
   TStringGrid *Kriterias;
   TButton *SetNames;
   TCSpinEdit *nAlt;
   TCSpinEdit *nKrit;
   TTabSheet *TabSheet2;
   TStringGrid *Ratings;
   TBitBtn *BitBtn1;
   TPanel *Panel1;
   TEdit *EditK;
   TStaticText *StaticText1;
   TStaticText *StaticText2;
   TEdit *EditA;
   TButton *Button5;
   TButton *Button6;
   TButton *NewA;
   TLabeledEdit *AltNew;
   TLabeledEdit *KritNew;
   TButton *NewK;
   TButton *Button4;
   TButton *GetRatBtn;
   TButton *SetRatBtn;
   TTabSheet *TabSheet3;
   TLabel *Label5;
   TLabel *Label6;
   TStringGrid *Soglas;
   TStringGrid *NotSoglas;
   TTabSheet *TabSheet4;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TTrackBar *TrackBar1;
   TTrackBar *TrackBar2;
   TListBox *ListBox1;
   TScrollBar *ScrollBar1;
   TScrollBar *ScrollBar2;
   TMemo *Memo1;
   TLabel *Changed0;
   TStaticText *Newed;
   TLabel *Changed1;
   TButton *Run;
   void __fastcall nAltChange(TObject *Sender);
   void __fastcall nKritChange(TObject *Sender);
   void __fastcall BitBtn1Click(TObject *Sender);
   void __fastcall TrackBar1Change(TObject *Sender);
   void __fastcall TrackBar2Change(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TabSheet2Show(TObject *Sender);
   void __fastcall NewAClick(TObject *Sender);
   void __fastcall NewKClick(TObject *Sender);
   void __fastcall Button4Click(TObject *Sender);
   void __fastcall Button5Click(TObject *Sender);
   void __fastcall Button6Click(TObject *Sender);
   void __fastcall SaveBtnClick(TObject *Sender);
   void __fastcall OpenBtnClick(TObject *Sender);
   void __fastcall PageControl1Change(TObject *Sender);
   void __fastcall PageControl1Changing(TObject *Sender,
          bool &AllowChange);
   void __fastcall RatingsSetEditText(TObject *Sender, int ACol, int ARow,
          const AnsiString Value);
   void __fastcall SetNamesClick(TObject *Sender);
   void __fastcall KriteriasSetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
   void __fastcall GetRatBtnClick(TObject *Sender);
   void __fastcall SetRatBtnClick(TObject *Sender);
   void __fastcall RunClick(TObject *Sender);
private:	// User declarations
   bool FPageChange[5];
public:		// User declarations
   Electra electra;
   int A, K;
   int   *ves, *dl;
   int ***ind;
   int **tabl;
   int **Sogl;
   float **NotSogl, temp, *lent2,lev2;
   TStringList *str;
   int max1,max2;
   int *lent1,len1,len2,lev1;
   bool GetPageChange(int i)
   {
      return FPageChange[i];
   }
   void SetPageChange(int i, bool value)
   {
      FPageChange[i] = value;
      if (i == 0)
         if (value)  Changed0->Caption = "true";
         else        Changed0->Caption = "false";
      if (i == 1)
         if (value)  Changed1->Caption = "true";
         else        Changed1->Caption = "false";

   }
   __property bool PageChange [int i] = {read=GetPageChange, write=SetPageChange};
   bool IndexesFromOne;

template <class TRE>
void SelectionSort(TRE *M, int const count)
{
  TRE  t;
  for (int i=0; i <= count-1; i++)
    for (int j=count; j >= i+1; j--)
      if (M[i] > M[j])
      {
        t = M[i];
        M[i] = M[j];
        M[j] = t;
      }
}

void DestroidThemAll()
{
   delete[] ves; delete[] dl;
   ves=NULL; dl=NULL;
   delete[] ind;/* TODO : Разобраться с удалением */
   ind=NULL;
   delete[] tabl; tabl = NULL;
   delete[] Sogl; Sogl =NULL;
   delete[] NotSogl; NotSogl = NULL;
   delete[] lent1; lent1=NULL;
}

void fluht()
{
   ListBox1->Items->Clear();
   for (int a1=0;a1<A;a1++)
      for (int a2=0;a2<A;a2++)
      {
         if (a1 == a2)
            continue;
         if (Sogl[a1][a2] >= lev1)
            if (NotSogl[a1][a2] <= lev2)
               ListBox1->Items->Add(Alternatives->Cells[0][a1+1]+" доминирует "+Alternatives->Cells[0][a2+1]+" "+IntToStr(Sogl[a1][a2])+" "+FloatToStr(NotSogl[a1][a2]));
      }
}
   __fastcall TForm1(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
