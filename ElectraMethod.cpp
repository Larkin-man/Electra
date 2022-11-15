// Copyright 2022 Larkin
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.
//---------------------------------------------------------------------------
//#include <system.hpp>
//#include <Dialogs.hpp>  //только для ShowMessage
//#include <dstring.h>
//#include <Classes.hpp>  //для тстринглист
//#include <conio.h>  //*/
#include <stdio.h>   //для fopen
#include <alloc.h>
//#include <string.h>
#include <mem.h>
//#include <system.hpp>
#pragma hdrstop
#include "ElectraMethod.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//ElectraIndexes - Базовый класс для индексов согласия и несогласия
//---------------------------------------------------------------------------
//Конструктор
template<class T> ElectraIndexes <T>::ElectraIndexes() : List(true)
{
}
//---------------------------------------------
//template<class T> T GetIndex (int i, int j)
template<class T> T& ElectraIndexes <T>::GetIndex (int i, int j)
{
   return Indexes.Cell[i][j];
}
//---------------------------------------------
template<class T> void ElectraIndexes <T>::SetIndex (int i, int j, T value)
{
   List.Add(value);
   Indexes.Cell[i][j] = value;
}
//---------------------------------------------
//Деструктор
template<class T> ElectraIndexes <T>::~ElectraIndexes()
{
}
//---------------------------------------------------------------------------
//Класс Электра
//---------------------------------------------------------------------------
//Конструктор
Electra::Electra()// : A(Alternatives.Count)//, K(Kriterias.Count) //Alternatives(A, &BaseNew) , Kriterias(K, &BaseNew)//, Concordance.List(true), Discordance.List(false)
{
   Version = ElectraI;
   Concordance.List.Invert();
   Optimal = NULL;
}
//---------------------------------------------
int Electra::GetRating (int i, int j)
{
   return Ratings.Cell[i][j];
}
//---------------------------------------------
void Electra::SetRating (int i, int j, int value)
{
   if (Ratings.RowCount < Alternatives.Count)
      Ratings.RowCount = Alternatives.Count;
   if (Ratings.ColCount < Kriterias.Count)
      Ratings.ColCount = Kriterias.Count;
   Ratings.Cell[i][j] = value;
}
//---------------------------------------------
bool Electra::VerionComparison()
{
   return true;
}
//---------------------------------------------
int Electra::CalcIndexes()
{
   //Подсчет индексов согласия
   if ((A <= 0)||(K <= 0))
      return 0;
   if (Ratings.RowCount < A)
      return -1;
   if (Ratings.ColCount < K)
      return -2;
   if (Concordance.Indexes.Size < A)
   {
      int pr = Concordance.Indexes.Size;
      Concordance.Indexes.Size = A;
      Discordance.Indexes.Size = A;
      for (; pr<A; ++pr)
      {
         Concordance.SetIndex(pr, pr, 0);
         Discordance.SetIndex(pr, pr, 0);
      }
   }
   Concordance.List.Clear();
   Discordance.List.Clear();
   //Concordance.List.Add(0);
   Discordance.List.Add(0);
   for (int Aing=0; Aing<A; Aing++)
      for (int Aed=0; Aed<A; Aed++)
      {
         if (Aing == Aed)
            continue;
         int Cidx = 0;
         double Nidx = 0;
         ///Concordance.Index[Aing][Aed] = 0;
         //Discordance.Index[Aing][Aed] = 0;
         for (int kr=0; kr<K; kr++)
         {
            if (Ratings.Cell[Aing][kr] >= Ratings.Cell[Aed][kr])    //Вот этот знак
            {
               Cidx += Kriterias[kr].weight;
               //Concordance.Index[Aing][Aed] += Kriterias[kr].weight;
            }
            else
            {
               double Lb = Ratings.Cell[Aed][kr];
               double La = Ratings.Cell[Aing][kr];
               double Li = Kriterias[kr].scale;
               if (Li <= 0)
                  Li=1;
               //double idx = (double(Ratings[Aed][kr] - Ratings[Aing][kr]))/Kriterias[kr].scale;
               double idx = (Lb - La)/Li;
               if (idx > Nidx)
                  Nidx = idx;
               //if (idx > Discordance.Index[Aing][Aed])
               //   Discordance.Index[Aing][Aed] = idx;
            }
         }//по критериям.
         //Индексы расчитаны
         //Concordance[Aing][Aed] = Cidx;
         //Concordance.Index(Aing,Aed) = Cidx;
         Concordance.SetIndex(Aing, Aed, Cidx);
         Discordance.SetIndex(Aing, Aed, Nidx);
         //Concordance.List.Add(Concordance.Index[Aing][Aed]);
         //Discordance.List.Add(Discordance.Index[Aing][Aed]);
      }
   int CMax = 0;
   for (int kr=0; kr<K; kr++)
      CMax += Kriterias[kr].weight;
   Concordance.List.Add(CMax);
   //FRunOk = true;
   return A;
}
//---------------------------------------------
void Electra::GenerateCore(int Cind, double NCind,  int &idx)
{
   for (int Aing=0; Aing<A; Aing++)
      if (Alternatives[Aing].Core < 0)
         for (int Aed=0; Aed<A; Aed++)
         {
            if (Aing == Aed)
               continue;
            if (Alternatives[Aed].Core > -1)
               continue;
            if((Cind <= Concordance.Index[Aing][Aed])
            &&(NCind >= Discordance.Index[Aing][Aed]))
            {
               Alternatives[Aed].Core = Core;
               Optimal[idx] = &Alternatives[Aed];
               idx++;
               //ListBox1->Items->Add(Alternatives->Cells[SNAME][Aing+1] +" доминирует "+Alternatives->Cells[SNAME][Aed+1]);
            }
         }
   Core++;
}
//---------------------------------------------
void Electra::Calc()
{
   if (Optimal)
      Optimal = (DataAlternatives**)realloc(Optimal, A * sizeof(DataAlternatives*));
   else
      Optimal = (DataAlternatives**)malloc(A * sizeof(DataAlternatives*));
   for (int i=0; i<A; i++)
      Alternatives[i].Core = -1;
   const int CMax = Concordance.List.First();
   const double NMax = Discordance.List.Last();
    int CurrAlt = 0;
   Core = 0;
   int CurrC = 0; int CurrN = 0;
   GenerateCore(Concordance.List.First() , Discordance.List.First(), CurrAlt);
   CurrC++; CurrN++;
   do
   {
      GenerateCore(Concordance.List[CurrC] , Discordance.List[CurrN], CurrAlt);
      double PosC = (double)Concordance.List[CurrC] / CMax;//от 1 до 0
      double PosN = 1. - Discordance.List[CurrN] / NMax;
      if (PosC > PosN)
         CurrC++;
      else
         CurrN++;
   } while (CurrAlt < A-1);
   for (int i=0; i<A; i++)
   {
      if (Alternatives[i].Core < 0)
      {
         Alternatives[i].Core = Core;
         Optimal[CurrAlt] = &Alternatives[i];
      }
   }
}
//---------------------------------------------

bool Electra::SaveAsText(char *FileName)   //Функция сохраняет список в текстовый файл
{
	FILE *file = fopen(FileName, "wt");
	if (!file)
		return false;
	fprintf(file, "A=%d ", A);
	fprintf(file, "K=%d\n", K);
	//TODO:a че с пробелами????
	for (int i=0; i<K; ++i)
	{
		for (int s=1; s <= Kriterias[i].Name.Length(); ++s)
			if (Kriterias[i].Name[s] == ' ')
				Kriterias[i].Name[s] = '_';
		fprintf(file, "%s ", Kriterias[i].Name);//.c_str()); можно и так
		fprintf(file, "%d ", Kriterias[i].weight);
		fprintf(file, "%d ", Kriterias[i].scale);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
	for (int i = 0; i < A; ++i)
	{
		for (int s=1; s <= Alternatives[i].Name.Length(); ++s)
			if (Alternatives[i].Name[s] == ' ')
				Alternatives[i].Name[s] = '_';
		fprintf(file, "%s ", Alternatives[i].Name);
		for (int j = 0; j < K; ++j)
			fprintf(file, "%d ", Ratings[i][j]);
      fprintf(file, "\n");
	}
	fclose(file);
	return true;
}
//---------------------------------------------
bool Electra::LoadFromText(char *FileName)     //Функция загружает список из текстового файла
{
   FILE *file = fopen(FileName, "rt");
	if (!file)
	{
		printf ( "Cannot open text file");
		return false;
	}
	int Ain, Kin;
	fscanf(file, "A=%d ", &Ain);
   fscanf(file, "K=%d ", &Kin);
//////////////   New(Ain, Kin);
   Kriterias.Count = Kin;
	Alternatives.Count = Ain;
	char name[80];
	for (int i=0; i<Kin; ++i)
	{

		fscanf(file, "%s ", &name);
		Kriterias[i].Name = name;
		fscanf(file, "%d ", &Kriterias[i].weight);
		fscanf(file, "%d ", &Kriterias[i].scale);
		fscanf(file, "\n");
	}
	//fscanf(file, "%d\n", &Kriterias[Kin-1].weight);
	//for (int i=0; i<Kin-1; ++i)
	//fscanf(file, "%d\n+\n", &Kriterias[Kin-1].scale);
	//fscanf(file, "%*\n+\n", &NL);
	fscanf(file, "\n");
	int sc;
   for (int i = 0; i < Ain; ++i)
   {
		fscanf(file, "%s ", &name);
		Alternatives[i].Name = name;
		//fscanf(file, "%s ", &Alternatives[i].name);
      //Alternatives[i].name = Buff;
      //Alternatives[i].name = ("Alt"+IntToStr(i)).c_str();
      Alternatives[i].Core = -1;
		for (int j = 0; j < Kin; ++j)
      {
         //fscanf(file, "%d ", &Ratings[i][j]);
			fscanf(file, "%d ", &sc);
			Ratings.Cell[i][j] = sc;
		}
		fscanf(file, "\n");
		//fscanf(file, "%d ", &sc);
		//Ratings.Cell[i][Kin-1] = sc;
      //fscanf(file, "\n%*");
   }
	fclose(file);
	return true;
 }
//---------------------------------------------
//Обнулить
void Electra::Clear()
{
   Alternatives.Clear();
   Kriterias.Clear();
   Ratings.Clear();
   Concordance.Indexes.Clear();
   Discordance.Indexes.Clear();
   Concordance.List.Clear();
   Discordance.List.Clear();
}
//---------------------------------------------
//Деструктор
Electra::~Electra()
{
   if (Optimal)
      free (Optimal);
   Optimal = NULL;
}
//---------------------------------------------*/
#pragma package(smart_init)
