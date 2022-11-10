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
#include <string.h>
#include <mem.h>
#pragma hdrstop   
#include "ElectraMethod.h"
//---------------------------------------------------------------------------
//BasicParams - Базовый класс для альтернатив и критериев
//---------------------------------------------------------------------------
//Конструктор с параметрами   
template<class T> BasicParams<T>::BasicParams(int &sCount, void (__closure *AElectraNew)())
{
   Data = NULL;
   FCapacity = 0;
   FCount = &sCount;
   if (sCount > 0)
      New(sCount);
   ElectraNew = AElectraNew;     
}
//---------------------------------------------
//New - Изменение кол-ва
template<class T> void BasicParams<T>::New(int count)
{
   *FCount = count;
   if (FCapacity >= count)
      return;
   if ( Data )
      Data = (T*)realloc( Data, count * sizeof(T));
   else
      Data = (T*)malloc(count * sizeof(T));      
   FCapacity = count;
   //if ((*A != 0)&&(*K != 0))
   //ElectraNew();
}
//---------------------------------------------
//Деструктор
template<class T> BasicParams<T>::~BasicParams()
{
   FCapacity = 0;
   *FCount = 0;
   if ( Data )
      free ( Data );
   Data = NULL;   
}

//---------------------------------------------------------------------------
//ElectraIndexes - Базовый класс для индексов согласия и несогласия
//---------------------------------------------------------------------------
//Конструктор
template<class T> ElectraIndexes <T>::ElectraIndexes() : List(true)
{
   FIndex = NULL;
   FCapacity = 0;
   //List.Duplicates = dupIgnore;
   //List.Sorted = true;
}
//---------------------------------------------
//template<class T> T GetIndex (int i, int j)
template<class T> T ElectraIndexes <T>::GetIndex (int i, int j) const
{
   //if ((i < 0)||(j < 0))
   //   return -1;
   if ((i >= FCapacity)||(j >= FCapacity))
      return -2;
   return FIndex[i][j];
}
//---------------------------------------------
template<class T> void ElectraIndexes <T>::SetIndex (int i, int j, T value)
{
   //if ((i < 0)||(j < 0))
   //   return ;
   if ((i >= FCapacity)||(j >= FCapacity))
      return ;
   List.Add(value);
   FIndex[i][j] = value;
}
//---------------------------------------------
template<class T> void ElectraIndexes <T>::Create(int Count)
{
   if (Count <= FCapacity)
      return; //нулями фигануть
   if ( FIndex )
   {
      for (int i=0; i<FCapacity; ++i)
         FIndex[i] = (T*)realloc(FIndex[i], Count * sizeof(T));
      FIndex = (T**)realloc(FIndex, Count * sizeof(T*));
      for (int i=FCapacity; i<Count; ++i)
         FIndex[i] = (T*)malloc(Count * sizeof(T));
   }
   else //Первое
   {
      FIndex = (T**)malloc(Count * sizeof(T*));
      for (int i=0; i<Count; i++)
         FIndex[i] = (T*)malloc(Count * sizeof(T));
   }
   //Инициализация диагонали нулями
   for (int i=FCapacity; i<Count; i++)
      FIndex[i][i] = 0;/*
      for (int j=FCapacity; j<Count; j++)
         FIndex[i][j] = 0;  //*/
   FCapacity = Count;
   List.Capacity = Count * 2;
}
//---------------------------------------------
//Деструктор
template<class T> ElectraIndexes <T>::~ElectraIndexes()
{
   if (FIndex)
   {
      for (int i=0; i<FCapacity; i++)
         free ( FIndex[i] );
      free ( FIndex );
      FIndex = NULL;
      FCapacity = 0;
   }
}
//---------------------------------------------------------------------------
//Класс Электра
//---------------------------------------------------------------------------
//Конструктор
Electra::Electra() : Alternatives(A, &BaseNew) , Kriterias(K, &BaseNew)//, Concordance.List(true), Discordance.List(false)
{
   A = 0;
   K = 0;
   Ratings = NULL;
   //FRunOk = false;
   Version = ElectraI;
   Concordance.List.Invert();
   //Concordance.List.SortAccordingIncrease = false;
   //Discordance.List.SortAccordingIncrease = true;
   Optimal = NULL;
}
//---------------------------------------------
//Обнулить
void Electra::Clear()
{
   A = 0;
   K = 0;
   Concordance.List.Clear();
   Discordance.List.Clear();
}
//Деструктор
Electra::~Electra()
{
   if (Ratings)
   {
      for (int i=0; i<Alternatives.Capacity; i++)
         free ( Ratings[i] );
      free ( Ratings );
      Ratings = NULL;
   }
   if (Optimal)
      free (Optimal);
   Optimal = NULL;
}
//---------------------------------------------
void Electra::BaseNew()
{
   if ((A != 0)&&(K != 0))
      New(A, K);
}
//---------------------------------------------
//Создание новой таблицы оценок или изменение размеров старой Rating[A][K]
void Electra::New(int NewAltCount, int NewKritCount)
{
   int KC = Kriterias.Capacity;
   int AC = Alternatives.Capacity;
   if( KC < NewKritCount )
      Kriterias.New(NewKritCount);
   if ( AC < NewAltCount )
      Alternatives.New(NewAltCount);
   if ( Ratings )
   {
      if( KC < NewKritCount )
         for (int i=0; i<AC; ++i)
            Ratings[i] = (int*)realloc(Ratings[i], NewKritCount * sizeof(int));

      if ( AC < NewAltCount )
      {
         Ratings = (int**)realloc(Ratings, NewAltCount * sizeof(int*));
         int MaxK = ( KC > NewKritCount ) ? KC : NewKritCount ;
         for (int i=AC; i<NewAltCount; ++i)
            Ratings[i] = (int*)malloc(MaxK * sizeof(int));  ///
      }
   }
   else //Во - первых
   {
      Ratings = (int**)malloc(NewAltCount * sizeof(int*));
      for (int i=0; i<NewAltCount; i++)
         Ratings[i] = (int*)malloc(NewKritCount * sizeof(int));
   }
   A = NewAltCount;
   K = NewKritCount;
   if ( KC < NewKritCount )
   {
         for (int i=0; i<AC; ++i)
            for (int j=KC; j<NewKritCount; ++j)
               Ratings[i][j] = 0;
        //Kriterias.FCapacity = NewKritCount;
   }
   if ( AC < NewAltCount )
   {
         for (int i=AC; i<NewAltCount; ++i)
            for (int j=0; j<KC; ++j)
               Ratings[i][j] = 0;
      //Alternatives.FCapacity = NewAltCount;
   }
}           
//---------------------------------------------
bool Electra::CheckIndexes(int Aidx, int Kidx)
{
   //if ((A < 0)||(K < 0))
   //{        return false;     }
   if ((Kidx >= K )|| (Aidx >= A))
   {        return false;     }
   return true;
}
//---------------------------------------------
int Electra::GetRating (int i, int j)
{
   if (CheckIndexes(i, j))
      return Ratings[i][j];
   else
      return -1;     
}
//---------------------------------------------
void Electra::SetRating (int i, int j, int value)
{
   if (CheckIndexes(i, j))
      Ratings[i][j] = value;
}
//---------------------------------------------
void Electra::CreateIndexes()
{
   if (Concordance.Capacity != A)
      Concordance.Create(A);
   if (Discordance.Capacity != A)
      Discordance.Create(A);  
}
//---------------------------------------------
bool Electra::VerionComparison()
{
   return true;
}
//---------------------------------------------
void Electra::Run()
{   
   //Подсчет индексов согласия
   if ((Concordance.Capacity <= 0)||(Discordance.Capacity <= 0))
      return; 
   if (K <= 0)
      return;
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
            if (Ratings[Aing][kr] >= Ratings[Aed][kr])    //Вот этот знак
            {
               Cidx += Kriterias[kr].weight;
               //Concordance.Index[Aing][Aed] += Kriterias[kr].weight;
            }
            else
            {
               double Lb = Ratings[Aed][kr];
               double La = Ratings[Aing][kr];
               double Li = Kriterias[kr].scale;
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
         Concordance.Index[Aing][Aed] = Cidx;
         Discordance.Index[Aing][Aed] = Nidx;
         //Concordance.List.Add(Concordance.Index[Aing][Aed]);
         //Discordance.List.Add(Discordance.Index[Aing][Aed]);
      }
   int CMax = 0;
   for (int kr=0; kr<K; kr++)
      CMax += Kriterias[kr].weight;
   Concordance.List.Add(CMax);
   //FRunOk = true;
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
   FILE *file = fopen(FileName, "w");
	if (!file)
	{
		//ShowMessage ( "Cannot create text file");
		return false;
	}
        //Node *Item = Record;

	fprintf(file, "A=%d ", A);
   fprintf(file, "K=%d\n", K);

   for (int i=0; i<K; ++i)
      //fprintf(file, "%s ", Kriterias[i].name);
      fprintf(file, "%d ", Kriterias[i].weight);
   fprintf(file, "\n");
   for (int i=0; i<K; ++i)
      fprintf(file, "%d ", Kriterias[i].scale);   
   fprintf(file, "\n+\n");
	for (int i = 0; i < A; ++i)
   {
      //fprintf(file, "%s ", Alternatives[i].name);
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
   FILE *file = fopen(FileName, "r");
	if (!file)
	{
		printf ( "Cannot open text file");
		return false;
	}
   int Ain, Kin;
   int NL;
	fscanf(file, "A=%d ", &Ain);
   fscanf(file, "K=%d ", &Kin);
   New(Ain, Kin);
   for (int i=0; i<Kin-1; ++i)
      //char Buff[80];      fscanf(file, "%s ", &Buff);       //ShowMessage(Buff);
      //Kriterias[i].name = ("Krit"+IntToStr(i)).c_str();
      fscanf(file, "%d ", &Kriterias[i].weight);
   fscanf(file, "%d\n", &Kriterias[Kin-1].weight);
   for (int i=0; i<Kin-1; ++i)
      fscanf(file, "%d ", &Kriterias[i].scale);
   fscanf(file, "%d\n+\n", &Kriterias[Kin-1].scale);
   //fscanf(file, "%*\n+\n", &NL);
   int sc = 0;
   for (int i = 0; i < Ain; ++i)
   {
      //char Buff[80];      fscanf(file, "%s ", &Buff);
      //fscanf(file, "%s ", &Alternatives[i].name);
      //Alternatives[i].name = Buff;
      //Alternatives[i].name = ("Alt"+IntToStr(i)).c_str();
      Alternatives[i].Core = -1;
      for (int j = 0; j < Kin-1; ++j)
      {
         //fscanf(file, "%d ", &Ratings[i][j]);
         fscanf(file, "%d ", &sc);
         Ratings[i][j] = sc;
      }
      fscanf(file, "%d ", &sc);
      Ratings[i][Kin-1] = sc;
      //fscanf(file, "\n%*");
   }

	fclose(file);
	return true;
 }
//---------------------------------------------
#pragma package(smart_init)


