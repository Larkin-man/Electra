//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MainCode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
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
   FA = 0; A1 = 1;
   FK = 0; K1 = 1;
   SheetBegins->Show();
   //electra.New(nAlt->Position, nKrit->Position);
	eAltChange(NULL);
   eKritChange(NULL);
   DataChange[DATARAT] = false;
   OpenDialog1->InitialDir = GetCurrentDir();
   SaveDialog1->InitialDir = GetCurrentDir();  
	//electra.Kriterias[1].
	ScrollsClick(NULL);
	Panel1->Top = TrackBar1->Top + 4;
	Panel2->Top = TrackBar2->Top + 4;
	ScrollBar2->Width = 661;
	//electra.Concordance[0][3] = 2;
}
//---------------------------------------------------------------------------
//Изменилось количество альтернатив
void __fastcall TForm1::eAltChange(TObject *Sender)
{
	if (A == nAlt->Position)
		return;
	DataChange[DATAA] = true;
	A = nAlt->Position;
	for (int i=1; i<A1; i++)
	{
      Alternatives->Cells[IC][i] = i;
      if (Alternatives->Cells[NC][i] == "")
      {
         Alternatives->Cells[NC][i]="А "+IntToStr(i);
         for (int j=1; j<K1; ++j)
            Ratings->Cells[i][j] = "0";
      }
	}
}
//---------------------------------------------------------------------------
//Изменилось количество критериев
void __fastcall TForm1::eKritChange(TObject *Sender)
{
	if (K == nKrit->Position)
		return;
   DataChange[DATAK] = true;
   K = nKrit->Position;
   for (int i=1; i<K1; i++)
   {
      Kriterias->Cells[IC][i] = i;
      if (Kriterias->Cells[NC][i] == "")
      {
         Kriterias->Cells[NC][i]="К " + IntToStr(i);
         Kriterias->Cells[WC][i]="0";
         Kriterias->Cells[LC][i]="0";
         for (int j=1; j<A1; ++j)
            Ratings->Cells[j][i] = "0";
      }
	}
}
//---------------------------------------------------------------------------
//Переход на новую страницу (срабатывает прежде чем Show)
void __fastcall TForm1::PageControl1Changing(TObject *Sender, bool &AllowChange)
{
//Было изменено название альтернатив
if ( DataChange[DATAA] )
   {
      electra.Alternatives.Count = A;
      for (int i=0; i<A; ++i)
      {
			electra.Alternatives[i].Name = Alternatives->Cells[SNAME][i+1];
         Ratings->Cells[i+1][0] = Alternatives->Cells[SNAME][i+1];
      }   
      DataChange[DATAA] = false;
	}
//Было изменено параметры критерия
if ( DataChange[DATAK] )
   {
      electra.Kriterias.Count = K;
      for (int i=0; i<K; ++i)
      {
			try
			{
				electra.Kriterias[i].Name = Kriterias->Cells[SNAME][i+1];
            electra.Kriterias[i].weight = StrToInt(Kriterias->Cells[SWE][i+1]);
            electra.Kriterias[i].scale = StrToInt(Kriterias->Cells[SSC][i+1]);
         }
         catch (EConvertError&)
         {
            if ( DataChange[DATAK] )
            {
               ShowMessage("Не введены веса критериев и длины шкал");
               DataChange[DATAK] = false;
            }
            Kriterias->Cells[SWE][i+1] = 0;
            Kriterias->Cells[SSC][i+1] = 0;
            electra.Kriterias[i].weight = 0;
            electra.Kriterias[i].scale = 0;
            Kriterias->Cells[1][i] = "0";
            Kriterias->Cells[2][i] = "0";     
         }
         Ratings->Cells[0][i+1] = Kriterias->Cells[SNAME][i+1];
      }
      DataChange[DATAK] = false;
   }  //*/

if ( DataChange[DATARAT] )
   {    
      for (int i = 0; i < A; i++)
         for (int j = 0; j < K; j++)
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
      DataChange[DATARAT] = false;
   }       //*/
}
//---------------------------------------------------------------------------
//СОХРАНИТЬ
void __fastcall TForm1::SaveBtnClick(TObject *Sender)
{
   if ( SaveDialog1->Execute() == ID_OK )
      electra.SaveAsText(SaveDialog1->FileName.c_str());
}
//---------------------------------------------------------------------------
//ОТКРЫТЬ
void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
   if ( OpenDialog1->Execute() == ID_OK )
   {
      //SetNames->SetFocus();
      electra.LoadFromText(OpenDialog1->FileName.c_str());
      //Newed->Caption = "A="+IntToStr(electra.Alternatives.Count)+" K="+IntToStr(electra.Kriterias.Count);
      A = electra.A;
      K = electra.K;
      SheetBegins->Show();
      //SetNames->SetFocus();
		nAlt->Position = A;
      nKrit->Position = K;
      //SheetRatingsShow(Sender);
      for (int i=0; i<electra.A; i++)
      {
         //ShowMessage(electra.Alternatives[i].name);
			Alternatives->Cells[SNAME][i+1] = electra.Alternatives[i].Name;
      }
      for (int j = 0; j < electra.Kriterias.Count; j++)
      {
         Kriterias->Cells[SNAME][j+1] = electra.Kriterias[j].Name;
         Kriterias->Cells[SWE][j+1] = electra.Kriterias[j].weight;
         Kriterias->Cells[SSC][j+1] = electra.Kriterias[j].scale;
         for (int i = 0; i < electra.Alternatives.Count; i++)
            Ratings->Cells[i+1][j+1] = electra.Rating[i][j];
      }
      Alternatives->TopRow = 1;
      Kriterias->TopRow = 1;
      Kriterias->Invalidate();
      Alternatives->Invalidate();
   }
}
//---------------------------------------------------------------------------
//ВВод символов в таблице оценок           
void __fastcall TForm1::GetRatBtnClick(TObject *Sender)
{
   for (int i=0; i<electra.Alternatives.Count; i++)
      for (int j=0; j<electra.Kriterias.Count; j++)
         Ratings->Cells[i+1][j+1] = electra.Rating[i][j];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetRatBtnClick(TObject *Sender)
{
   for (int i=0; i<A; i++)
      for (int j=0; j<K; j++)
         electra.Rating[i][j] = StrToInt(Ratings->Cells[i+1][j+1]);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RunBtnClick(TObject *Sender)
{
   //electra.CreateIndexes();
   //ShowMessage("Create Succes");
   electra.CalcIndexes();
   Soglas->ColCount=A1;
   Soglas->RowCount=A1;
   NotSoglas->ColCount=A1;
   NotSoglas->RowCount=A1;
   for (int i=1;i<A1;i++)
   {
      Soglas->Cells[0][i] = Alternatives->Cells[SNAME][i];
      Soglas->Cells[i][0] = Alternatives->Cells[SNAME][i];
      NotSoglas->Cells[0][i] = Alternatives->Cells[SNAME][i];
      NotSoglas->Cells[i][0] = Alternatives->Cells[SNAME][i];
   }

   //electra.Concordance.Index[0][0] = 6;
   for (int a1=0; a1<A; a1++)
      for (int a2=0; a2<A; a2++)
      {
         Soglas->Cells[a1+1][a2+1] = electra.Concordance.Index[a1][a2];
         NotSoglas->Cells[a1+1][a2+1] = electra.Discordance.Index[a1][a2];
      }
   
   //if (electra.RunOk)
   {
      TrackBar1->Max = electra.Concordance.ListCount-1;
      TrackBar2->Max = electra.Discordance.ListCount-1;
      TrackBar1->Position = 0;
      TrackBar2->Position = 0;
      ScrollBar1->Max = electra.Concordance.First();
      ScrollBar2->Max = (electra.Discordance.Last()*100);
      LabelSC->Caption = "Уровней: "+IntToStr(electra.Concordance.ListCount);
      LabelNC->Caption = "Уровней: "+IntToStr(electra.Discordance.ListCount);
   }        //*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
   //if (electra.RunOk)
      CurrC = electra.Concordance.ListIndex(TrackBar1->Position);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
   //if (electra.RunOk)
      CurrN = electra.Discordance.ListIndex(TrackBar2->Position);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CalcDominating()
{
   ListBox1->Clear();
   for (int Aing=0; Aing<A; Aing++)
      for (int Aed=0; Aed<A; Aed++)
      {
         if (Aing == Aed)
            continue;
         if((CurrC <= electra.Concordance.Index[Aing][Aed])
         &&(CurrN >= electra.Discordance.Index[Aing][Aed]))
            ListBox1->Items->Add(Alternatives->Cells[SNAME][Aing+1] +" доминирует "+Alternatives->Cells[SNAME][Aed+1]);
      }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ScrollsClick(TObject *Sender)
{
   Panel1->Visible = Scrolls->Checked;
   Panel2->Visible = Scrolls->Checked;
   TrackBar1->Visible = !Scrolls->Checked;
   TrackBar2->Visible = !Scrolls->Checked;
   if (Scrolls->Checked)
   {
      //LabelS->Caption = (int)(ScrollBar1->Max - ScrollPos);
      ScrollBar1->Position = CurrC;
      ScrollBar2->Position = CurrN;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBar1Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
   //if (electra.RunOk)
      if (ScrollCode == scEndScroll)
         CurrC = ScrollBar1->Max - ScrollPos;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBar2Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
   //if (electra.RunOk)
      if (ScrollCode == scEndScroll)
         CurrN = (double)ScrollPos/100;//electra.Discordance.List[TrackBar2->Position];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CalcClick(TObject *Sender)
{                                   
   electra.Calc();
   int Core = 0;
   for (int i=0; i<electra.A; i++)
   {
      if (electra.OptimalList[i].Core >= Core)
      {
         ListBox1->Items->Add("Ядро "+IntToStr(electra.OptimalList[i].Core));
         Core = electra.OptimalList[i].Core + 1;
      }
      ListBox1->Items->Add(electra.OptimalList[i].Name);
   }
   ListBox1->Items->Add("Лучшая альтернатива: " + AnsiString(electra.OptimalList[electra.A-1].Name));
}
//---------------------------------------------------------------------------
           
void __fastcall TForm1::AlternativesKeyPress(TObject *Sender, char &Key)
{
   DataChange[DATAA] = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::KriteriasKeyPress(TObject *Sender, char &Key)
{
   DataChange[DATAK] = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RatingsKeyPress(TObject *Sender, char &Key)
{
   DataChange[DATARAT] = true;
}
//---------------------------------------------------------------------------

