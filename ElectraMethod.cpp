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
//Базовый класс для альтернатив и критериев
BasicParams::BasicParams(int &sA, int &sK, int** &sRatings)
{
   //ShowMessage("BasicParams Constr");
   A = &sA;
   K = &sK;
   Ratings = &sRatings;
}

//Создание новой таблицы оценок или изменение размеров старой
void BasicParams::NewRatings(const int &Krit, const int &Alt)
{
   if (*Ratings)
   {
      (*Ratings) = (int**)realloc((*Ratings), Krit * sizeof(int*));
      for (int i=0;i< Krit;i++)
         (*Ratings)[i] = (int*)realloc((*Ratings)[i], Alt * sizeof(int));
   }
   else
   {
      (*Ratings) = (int**)malloc(Krit * sizeof(int*));
      for (int i=0;i<Krit;i++)
         (*Ratings)[i] = (int*)malloc(Alt * sizeof(int));
      //Инициализация
      for (int i=0;i<Krit;i++)
         for (int j=0;j<Alt;j++)
            (*Ratings)[i][j] = 0;
   }
}
//--------------------------------------------------------------
//Класс Альернатив
EAlternatives::EAlternatives(int &sA, int &sK, int** &sRatings)
   :  BasicParams(sA, sK, sRatings)
{
   //ShowMessage("ealternative Constr"+IntToStr(sA));
   Alt = NULL;
   count = 0;
}

EAlternatives::~EAlternatives()
{
   if ( Alt )
      free ( Alt );
   count = 0;
}
//-------------------------------------------------
//Класс Критериев
EKriterias::EKriterias(int &sA, int &sK, int** &sRatings)
   :  BasicParams(sA, sK, sRatings)
{
   //ShowMessage("EKriterias Constr"+IntToStr(sA));
   Krit = NULL;
   count = 0;
}

EKriterias::~EKriterias()
{
   if ( Krit )
      free ( Krit );
   count = 0;
}
//---------------------------------------------------------------------------
//Класс Электра
//Конструктор
Electra::Electra()
   : Alternatives(A, K, Ratings) , Kriterias(A, K, Ratings)
{
      IndexesFromOne = true;   //true - 1; false - 0;
      //ShowMessage("electra const");
      A = 0;
      K = 0;
      Ratings = 0;
      //Alternatives = new EAlternatives(A, K, Ratings);
     // Alternatives->
}
//Деструктор
Electra::~Electra()
{
  // delete Alternatives;
   if (Ratings)
   {
      for (int i=0;i<K;i++)
         free ( Ratings[i] );
      free ( Ratings );
      Ratings = 0;
   }
}
int Electra::CheckIndexes(int Kidx, int Aidx)
{
   if ((A == 0)||(K == 0))
   {
      ShowMessage("Error, Alt="+IntToStr(A)+" Krit="+IntToStr(K));
      return 0;
   }
   if ((Kidx >= K )|| (Aidx >= A))
   {
      ShowMessage("2Error, Alt="+IntToStr(A)+" Krit="+IntToStr(K));
      ShowMessage("Error, Aidx="+IntToStr(Aidx)+" Kidx="+IntToStr(Kidx));
      return -1;
   }
   return 1;
}

int Electra::GetRating (int i, int j)
{
   if (FIndexesFromOne)
   {
      i--;
      j--;
   }
   if (int check = CheckIndexes(i, j) == 1)
      return Ratings[i][j];
   else
      return check;

}
void Electra::SetRating (int i, int j, int value)
{
   if (FIndexesFromOne)
   {
      i--;
      j--;
   }
   if (int check = CheckIndexes(i, j) == 1)
      Ratings[i][j] = value;
}

void Electra::SetIndexesFromOne(bool set)
{
   FIndexesFromOne = set;
   Alternatives.IndexesFromOne = set;
   Kriterias.IndexesFromOne = set;
}

bool Electra::SaveAsText(char *FileName)   //Функция сохраняет список в текстовый файл
{
   FILE *file = fopen(FileName, "w");
	if (!file)
	{
		ShowMessage ( "Cannot create text file");
		return false;
	}
        //Node *Item = Record;

	fprintf(file, "%d ", A);
   fprintf(file, "%d ", K);
	for (int i = 0; i < K; i++)
      for (int j = 0; j < A; j++)
         fprintf(file, "%d ", Ratings[i][j]);
	fclose(file);
   for (int i = 0; i < K; i++)
   {
      //fprintf(file, "%d ", Kriterias);
   }

	return true;
}

bool Electra::LoadFromText(char *FileName)     //Функция загружает список из текстового файла
{
   FILE *file = fopen(FileName, "r");
	if (!file)
	{
		printf ( "Cannot open text file");
		return false;
	}

	//if (Record) 	{  		delete [] Record; 		Record = NULL; 	}

	fscanf(file, "%d", &A);
   fscanf(file, "%d", &K);
   Alternatives.New(A);
   Kriterias.New(K);
   for (int i = 0; i < K; i++)
      for (int j = 0; j < A; j++)
         fscanf(file, "%d", &Ratings[i][j]);

	fclose(file);
	return true;
 }
//---------------------------------------------
#pragma package(smart_init)
