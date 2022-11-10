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
#pragma hdrstop
#include "ElectraMethod.h"
//---------------------------------------------------------------------------
//BasicParams - Базовый класс для альтернатив и критериев
//---------------------------------------------------------------------------
//Конструктор с параметрами
template<class T> BasicParams<T>::BasicParams(int &sA, int &sK, int** &sRatings, bool CountIsAorK)
{
   //ShowMessage("BasicParams Constr");
   //FCount = 0;
   if (CountIsAorK)
      FCount = &sA;
   else
      FCount = &sK; 
   Data = NULL;
   A = &sA;
   K = &sK;
   Ratings = &sRatings;
}
//---------------------------------------------
//New - Изменение кол-ва
template<class T> void BasicParams<T>::New(int count)
{
   if (*FCount == count)
      return;
   if ( Data )
      Data = (T*)realloc( Data, count * sizeof(T));
   else
      Data = (T*)malloc(count * sizeof(T));
   *FCount = count;
   if ((*A != 0)&&(*K != 0))
      NewRatings(*A, *K);
}
//---------------------------------------------
//Деструктор
template<class T> BasicParams<T>::~BasicParams()
{
   if ( Data )
      free ( Data );
   Data = NULL;
}
//---------------------------------------------
//Создание новой таблицы оценок или изменение размеров старой
template<class T> void BasicParams<T>::NewRatings(const int &nAlt, const int &nKrit)
{
   if (*Ratings)
   {
      (*Ratings) = (int**)realloc((*Ratings), nAlt * sizeof(int*));
      for (int i=0; i<nAlt; i++)
         (*Ratings)[i] = (int*)realloc((*Ratings)[i], nKrit * sizeof(int));
   }
   else
   {
      (*Ratings) = (int**)malloc(nAlt * sizeof(int*));
      for (int i=0; i<nAlt; i++)
         (*Ratings)[i] = (int*)malloc(nKrit * sizeof(int));
      //Инициализация
      for (int i=0; i<nAlt; i++)
         for (int j=0; j<nKrit; j++)
            (*Ratings)[i][j] = 0;
   }
}
//---------------------------------------------------------------------------
//Класс Электра
//---------------------------------------------------------------------------
//Конструктор
Electra::Electra()
   : Alternatives(AltCount, KritCount, Ratings, true) , Kriterias(AltCount, KritCount, Ratings, false)
{
   //ShowMessage("electra const");
   AltCount = 0;
   KritCount = 0;
   Ratings = NULL;
   FConsentIndex = NULL;
   AIndexes = 0;
   //Alternatives.Set();
   //Kriterias.Set();
}
//---------------------------------------------
//Деструктор
Electra::~Electra()
{
   if (Ratings)
   {
      for (int i=0; i<A; i++)
         free ( Ratings[i] );
      free ( Ratings );
      Ratings = NULL;
   }
   if (FConsentIndex)
   {
      for (int i=0; i<AIndexes; i++)
         free ( FConsentIndex[i] );
      free ( FConsentIndex );
      FConsentIndex = NULL;
   }
}
//---------------------------------------------
int Electra::CheckIndexes(int Aidx, int Kidx)
{
   if ((A < 0)||(K < 0))
   {
      //ShowMessage("Error, Alt="+IntToStr(A)+" Krit="+IntToStr(K));
      return 0;
   }
   if ((Kidx >= K )|| (Aidx >= A))
   {
      //ShowMessage("2Error, Alt="+IntToStr(A)+" Krit="+IntToStr(K));
      //ShowMessage("Error, Aidx="+IntToStr(Aidx)+" Kidx="+IntToStr(Kidx));
      return -1;
   }
   return 1;
}
//---------------------------------------------
int Electra::GetRating (int i, int j)
{
   if (int check = CheckIndexes(i, j) == 1)
      return Ratings[i][j];
   else
      return check;

}
//---------------------------------------------
void Electra::SetRating (int i, int j, int value)
{
   if (int check = CheckIndexes(i, j) == 1)
      Ratings[i][j] = value;
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
   fprintf(file, "K=%d ", K);
   for (int i=0; i<K; i++)
   {
      //fprintf(file, "%s ", Kriterias[i].name);
      fprintf(file, "%d ", Kriterias[i].weight);
      fprintf(file, "%d ", Kriterias[i].scale);
   }
	for (int i = 0; i < A; i++)
   {
      //fprintf(file, "%s ", Alternatives[i].name);
      for (int j = 0; j < K; j++)
         fprintf(file, "%d ", Ratings[i][j]);
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

	//if (Record) 	{  		delete [] Record; 		Record = NULL; 	}
   int Ain, Kin;
	fscanf(file, "A=%d ", &Ain);
   fscanf(file, "K=%d ", &Kin);
   Alternatives.New(Ain);
   Kriterias.New(Kin);
   for (int i=0; i<K; i++)
   {
      //char Buff[80];      fscanf(file, "%s ", &Buff);       //ShowMessage(Buff);
      //Kriterias[i].name = ("Krit"+IntToStr(i)).c_str();
      fscanf(file, "%d ", &Kriterias[i].weight);
      fscanf(file, "%d ", &Kriterias[i].scale);
   }
   for (int i = 0; i < A; i++)
   {
      //char Buff[80];      fscanf(file, "%s ", &Buff);
      //fscanf(file, "%s ", &Alternatives[i].name);
      //Alternatives[i].name = Buff;
      //Alternatives[i].name = ("Alt"+IntToStr(i)).c_str();
      for (int j = 0; j < K; j++)
         fscanf(file, "%d ", &Ratings[i][j]);
   }

	fclose(file);
	return true;
 }
//---------------------------------------------
int Electra::GetConsentIndex (int i, int j)
{
   if ((i < 0)||(j < 0))
      return -1;
   if ((i >= AIndexes)||(j >= AIndexes))
      return -2;
   return FConsentIndex[i][j];
}
//---------------------------------------------
void Electra::SetConsentIndex (int i, int j, int value)
{
   if ((i < 0)||(j < 0))
      return ;
   if ((i >= AIndexes)||(j >= AIndexes))
      return ;
   FConsentIndex[i][j] = value;
}
//---------------------------------------------

void Electra::CreateIndexes()
{    
   if (AIndexes == A)
      return;
   AIndexes = A;
   //Создание таблицы
   if (FConsentIndex)
   {
      FConsentIndex = (int**)realloc(FConsentIndex, A * sizeof(int*));
      for (int i=0; i<A; i++)
         FConsentIndex[i] = (int*)realloc(FConsentIndex[i], A * sizeof(int));
   }
   else
   {
      FConsentIndex = (int**)malloc(A * sizeof(int*));
      for (int i=0; i<A; i++)
         FConsentIndex[i] = (int*)malloc(A * sizeof(int));
      //Инициализация
      for (int i=0; i<A; i++)
         for (int j=0; j<A; j++)
            FConsentIndex[i][j] = 0;
   }
}

void Electra::Run()
{
   //Подсчет индексов согласия
   if (AIndexes <= 0)
      return;
   if (K <= 0)
      return;
   for (int Aing=0; Aing<Aindexes; Aing++)
      for (int Aed=0; Aed<Aindexes; Aed++)
      {
         if (Aing == Aed)
            continue;
         FConsentIndex[Aing][Aed] = 0;
         for (int kr=0; kr<K; kr++)
         {           
            if (Ratings[Aind][kr] >= Ratings[Aed][kr])    //Вот этот знак
            {
               FConsentIndex += Kriterias[kr].weight'
            }
            //else               ind[a1][a2][kr]=0;
         }
         //if (Sogl[a1][a2] > max1)            max1=Sogl[a1][a2];
         //str->Add(Sogl[a1][a2]);
      }
   //len1 = str->Count;
   //lent1 = new int [str->Count];
   //for (int i=0;i<str->Count;i++)
   //   lent1[i]=StrToInt(str->Strings[i]);
   //SelectionSort(lent1, len1);
   //str->Clear();
   //str->Add(0);


 /////////////
   for (int a1=0;a1<A;a1++)
      for (int a2=0;a2<A;a2++)
      {
         NotSogl[a1][a2]=100;
         for (int kr=0;kr<K;kr++)
         {
            if (a1 == a2 )
            {
               NotSogl[a1][a2]=0;
               continue;
            }
            temp=(tabl[a1][kr]-tabl[a2][kr]);///float(dl[kr]);
            if (temp < NotSogl[a1][a2])
               NotSogl[a1][a2]=temp;

         }
         if (NotSogl[a1][a2]>max2)
            max2=NotSogl[a1][a2];
         NotSogl[a1][a2]*=(-1);
         if ((NotSogl[a1][a2]>0)&&(NotSogl[a1][a2]<1))
            {
               ShowMessage(IntToStr(a1)+" "+IntToStr(a2));
            }
         str->Add(NotSogl[a1][a2]);
      }
   len2=str->Count;
   lent2 = new float [str->Count];
   for (int i=0;i<str->Count;i++)
      lent2[i]=StrToFloat(str->Strings[i]);
   str->Clear();
   SelectionSort(lent2, len2);
   NotSoglas->ColCount=A+1;
   NotSoglas->RowCount=A+1;
   for (int i=1;i<A+1;i++)
   {
      NotSoglas->Cells[0][i]=Alternatives->Cells[0][i];
      NotSoglas->Cells[i][0]=Alternatives->Cells[0][i];
   }
   for (int a1=0;a1<A;a1++)
      for (int a2=0;a2<A;a2++)
      {
         NotSoglas->Cells[a1+1][a2+1]=FloatToStr(NotSogl[a1][a2]);
      }
   ScrollBar1->Max=max1;
   ScrollBar2->Max=max2;
   TrackBar1->Max=len1-1;
   TrackBar2->Max=len2-1;    */
}
//---------------------------------------------
//---------------------------------------------
#pragma package(smart_init)
