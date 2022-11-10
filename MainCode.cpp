//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainCode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)  : TForm(Owner)
{
   Alternatives->Cells[IC][0] = "№";
   Alternatives->Cells[NC][0] = "Альтернативы";
   Alternatives->ColWidths[IC] = 24;
   Alternatives->ColWidths[NC] = Alternatives->Width - Alternatives->ColWidths[IC] - 5;
   Kriterias->Cells[IC][0] = "№";
   Kriterias->Cells[NC][0] = "Критерии";
   Kriterias->Cells[WC][0] = "Вес";
   Kriterias->Cells[LC][0] = "Длина шкалы";
   Kriterias->ColWidths[IC] = 24;
   Kriterias->ColWidths[NC] = 180;
   Kriterias->ColWidths[LC] = 115;
   nAltChange(NULL);
   nKritChange(NULL);
   ind=NULL;
   str = new TStringList;
   str->Duplicates=dupIgnore;
   str->Sorted=true;
   ves=NULL; dl=NULL;
   ind=NULL;
   tabl=NULL;
   Sogl=NULL;
   NotSogl=NULL; temp=NULL; lent2=NULL; lev2=NULL;
   str =NULL;
   lent1=NULL;len1=NULL;len2=NULL;lev1=NULL;
   OpenDialog1->InitialDir = GetCurrentDir();
   SaveDialog1->InitialDir = GetCurrentDir();
   PageChange[1]=false; PageChange[0]=false;
   //electra.Kriterias[1].
}
//---------------------------------------------------------------------------

void __fastcall TForm1::nAltChange(TObject *Sender)
{
   A = nAlt->Value;
   Alternatives->RowCount=A+1;
   for (int i=1;i<A+1;i++)
   {
      Alternatives->Cells[IC][i] = i;
      if (Alternatives->Cells[NC][i] == "")
         Alternatives->Cells[NC][i]="Альтернатива"+IntToStr(i);
   }
   PageChange[0] = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::nKritChange(TObject *Sender)
{
   K = nKrit->Value;;
   Kriterias->RowCount=K+1;
   for (int i=1;i<K+1;i++)
   {
      Kriterias->Cells[IC][i] = i;
      if (Kriterias->Cells[NC][i] == "")
      {
         Kriterias->Cells[NC][i]="Критерий" + IntToStr(i);
         Kriterias->Cells[WC][i]="0";
         Kriterias->Cells[LC][i]="0";
      }
   }
   PageChange[0] = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
   Ratings->Cells[1][1]=7;
   Ratings->Cells[1][2]=9;
   Ratings->Cells[1][3]=6;
   Ratings->Cells[1][4]=9;
   Ratings->Cells[1][5]=7;
   Ratings->Cells[1][6]=2;
   Ratings->Cells[1][7]=5;

   Ratings->Cells[2][1]=8;
   Ratings->Cells[2][2]=9;
   Ratings->Cells[2][3]=6;
   Ratings->Cells[2][4]=5;
   Ratings->Cells[2][5]=6;
   Ratings->Cells[2][6]=9;
   Ratings->Cells[2][7]=7;

   Ratings->Cells[3][1]=7;
   Ratings->Cells[3][2]=8;
   Ratings->Cells[3][3]=7;
   Ratings->Cells[3][4]=4;
   Ratings->Cells[3][5]=6;
   Ratings->Cells[3][6]=8;
   Ratings->Cells[3][7]=6;

   Ratings->Cells[4][1]=5;
   Ratings->Cells[4][2]=5;
   Ratings->Cells[4][3]=8;
   Ratings->Cells[4][4]=8;
   Ratings->Cells[4][5]=6;
   Ratings->Cells[4][6]=4;
   Ratings->Cells[4][7]=7;

   Ratings->Cells[5][1]=6;
   Ratings->Cells[5][2]=5;
   Ratings->Cells[5][3]=2;
   Ratings->Cells[5][4]=1;
   Ratings->Cells[5][5]=2;
   Ratings->Cells[5][6]=4;
   Ratings->Cells[5][7]=5;
   Kriterias->Cells[WC][1]=6;
   Kriterias->Cells[WC][2]=4;
   Kriterias->Cells[WC][3]=6;
   Kriterias->Cells[WC][4]=5;
   Kriterias->Cells[WC][5]=3;
   Kriterias->Cells[WC][6]=9;
   Kriterias->Cells[WC][7]=9;
   for (int j = 1 ;j<K+1;j++)
      Kriterias->Cells[LC][j]=9; 
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
   lev1=lent1[TrackBar1->Position];
   Label1->Caption=lev1;
   fluht();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
   lev2=lent2[TrackBar2->Position];
   Label2->Caption=lev2;
   fluht();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   DestroidThemAll();
}
//---------------------------------------------------------------------------
//Переход на новую страницу (срабатывает прежде чем Show)
void __fastcall TForm1::PageControl1Changing(TObject *Sender, bool &AllowChange)
{
   if (PageChange[1])
   {
      //ShowMessage("Перезапись A="+IntToStr(electra.Alternatives.Count)+" K="+IntToStr(electra.Kriterias.Count));

      for (int i = 0; i <= electra.A; i++)
         for (int j = 0; j <= electra.K; j++)
            try
            {
               if (Ratings->Cells[i+1][j+1] == "")
               {
                  Ratings->Cells[i+1][j+1] = "0";
                  electra.Rating[i][j] = 0;
               }
               else
                  electra.Rating[i][j] = StrToInt(Ratings->Cells[i+1][j+1]);
            }
            catch(...)
            {
               electra.Rating[i][j] = 0;
               Ratings->Cells[i+1][j+1] = 0;
            }      
      PageChange[1] = false;
   }       //*/
   if (PageChange[0])
   {
      electra.Alternatives.New(nAlt->Value);
      electra.Kriterias.New(nKrit->Value);
      Newed->Caption = "A="+IntToStr(electra.Alternatives.Count)+" K="+IntToStr(electra.Kriterias.Count);
      for (int i=0; i<nKrit->Value; i++)
      {
         try
         {
            electra.Kriterias[i].weight = StrToInt(Kriterias->Cells[SWE][i+1]);
            electra.Kriterias[i].scale = StrToInt(Kriterias->Cells[SSC][i+1]);
         }
         catch (EConvertError&)
         {
            if ( PageChange[0] )
            {
               ShowMessage("Не введены веса критериев и длины шкал");
               PageChange[0] = false;
            }
            Kriterias->Cells[SWE][i+1] = 0;
            Kriterias->Cells[SSC][i+1] = 0;
            electra.Kriterias[i].weight = 0;
            electra.Kriterias[i].scale = 0;
            Kriterias->Cells[1][i] = "0";
            Kriterias->Cells[2][i] = "0";     
         }
         PageChange[0] = false;
      }
   }  //*/
}
//---------------------------------------------------------------------------
//Переход на страницу оценок
void __fastcall TForm1::TabSheet2Show(TObject *Sender)
{
   Ratings->RowCount = electra.K+1;
   Ratings->ColCount = electra.A+1;
   for (int i=1; i<electra.A+1; i++)
      Ratings->Cells[i][0] = Alternatives->Cells[SNAME][i];
   for (int i=1; i<electra.K+1; i++)
      Ratings->Cells[0][i] = Kriterias->Cells[SNAME][i];      
}
//---------------------------------------------------------------------------
 
void __fastcall TForm1::NewAClick(TObject *Sender)
{
   electra.Alternatives.New(StrToInt(AltNew->Text));
   Newed->Caption = "A="+IntToStr(electra.Alternatives.Count)+" K="+IntToStr(electra.Kriterias.Count);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewKClick(TObject *Sender)
{
   electra.Kriterias.New(StrToInt(KritNew->Text));
   Newed->Caption = ("A="+IntToStr(electra.A)+" K="+IntToStr(electra.K));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
   ShowMessage("Alt="+IntToStr(electra.Alternatives.Count )+" Krit="+IntToStr(electra.Kriterias.Count));
   electra.test();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
   int I = StrToInt(EditK->Text);
   int J = StrToInt(EditA->Text);
   electra.Rating[I][J] = StrToInt(Ratings->Cells[J][I]);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
   int I = StrToInt(EditK->Text);
   int J = StrToInt(EditA->Text);
   Ratings->Cells[J][I]=electra.Rating[I][J];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveBtnClick(TObject *Sender)
{
   if ( SaveDialog1->Execute() == ID_OK )
   {
      electra.SaveAsText(SaveDialog1->FileName.c_str());
   }
}
//---------------------------------------------------------------------------
 
void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
   if ( OpenDialog1->Execute() == ID_OK )
   {
      electra.LoadFromText(OpenDialog1->FileName.c_str());
   }
   Newed->Caption = "A="+IntToStr(electra.Alternatives.Count)+" K="+IntToStr(electra.Kriterias.Count);
   nAlt->Value = electra.A;
   nKrit->Value = electra.K;
   for (int i=0; i<electra.A; i++)
   {
      //ShowMessage(electra.Alternatives[i].name);
      //Alternatives->Cells[SNAME][i+1] = electra.Alternatives[i].name;
   }
   for (int j = 0; j <= electra.Kriterias.Count; j++)
   {
      //Kriterias->Cells[SNAME][j+1] = electra.Kriterias[j].name;
      Kriterias->Cells[SWE][j+1] = electra.Kriterias[j].weight;
      Kriterias->Cells[SSC][j+1] = electra.Kriterias[j].scale;
      for (int i = 0; i <= electra.Alternatives.Count; i++)
         Ratings->Cells[j][i] = electra.Rating[i][j];
   }
}
//---------------------------------------------------------------------------
//Переход на новую страницу (срабатывает на новой)
void __fastcall TForm1::PageControl1Change(TObject *Sender)
{
   //ShowMessage("OnChange");
}
//---------------------------------------------------------------------------

//ВВод символов в талице оценок
void __fastcall TForm1::RatingsSetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{       
   PageChange[1] = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetNamesClick(TObject *Sender)
{
   for (int i=0; i<nAlt->Value; i++)
      electra.Alternatives[i].name = Alternatives->Cells[SNAME][i+1].c_str();

   for (int i=0; i<nKrit->Value; i++)
   {
      electra.Kriterias[i].name = Kriterias->Cells[SNAME][i+1].c_str();
      electra.Kriterias[i].weight = StrToInt(Kriterias->Cells[SWE][i+1]);
      electra.Kriterias[i].scale = StrToInt(Kriterias->Cells[SSC][i+1]);
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::KriteriasSetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
   PageChange[0] = true; 
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GetRatBtnClick(TObject *Sender)
{
   for (int i=0; i<electra.Alternatives.Count; i++)
      for (int j=0; j<electra.Kriterias.Count; j++)
      {
         Ratings->Cells[i+1][j+1] = electra.Rating[i][j];
      }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetRatBtnClick(TObject *Sender)
{
   for (int i=0; i<A; i++)
      for (int j=0; j<K; j++)
      {
         electra.Rating[i][j] = StrToInt(Ratings->Cells[i+1][j+1]);
      }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RunClick(TObject *Sender)
{
   Soglas->ColCount=A+1;
   Soglas->RowCount=A+1;
   for (int i=1;i<A+1;i++)
   {
      Soglas->Cells[0][i] = Alternatives->Cells[0][i];
      Soglas->Cells[i][0] = Alternatives->Cells[0][i];
   }
   for (int a1=0;a1<A;a1++)
      for (int a2=0;a2<A;a2++)
      {
         Soglas->Cells[a1+1][a2+1]=Sogl[a1][a2];
      }
}
//---------------------------------------------------------------------------

