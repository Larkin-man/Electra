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
#ifndef ElectraMethodH
#define ElectraMethodH
//---------------------------------------------------------------------------
#define LRArraySimpleType
//Автосортировка для простых типов:
#define LRArrayAutoSort
//Создание коллекции (игнорируются добавление дубликата):
#define LRArrayCollection
#include "LarkinArray.h"
//Динамический массив:
#include "LarkinArray.h"
//---------------------------------------------------------------------------
//BasicParams - Базовый класс для альтернатив и критериев
template <class T> class BasicParams
{
   friend class Electra;
private:
   T *Data;
   int *FCount;
protected:    
    int GetCount() { return *FCount; }
   //int ***pRatings;
   // int *A, *K;
   int FCapacity;
   void (__closure *ElectraNew)();
   
public:
   //BasicParams(int &sCount, void (__closure *AElectraNew)());
   ~BasicParams();
   void New(int count);
   __property int Count = {read = GetCount, write = New};
   __property int Capacity = {read = FCapacity};
   T& operator[]( int i )   {      return Data[i];   }
   const T& operator[]( int i ) const   {      return Data[i];   }
};

//---------------------------------------------------------------------------
//ElectraIndexes - Базовый класс для индексов согласия и несогласия
template <class T> class ElectraIndexes
{
   friend class Electra;
private:
   T **FIndex;
   int FCapacity;
   LRCollectionSort<T> List;
protected:
   T GetIndex ( int i,  int j) const;
   void SetIndex ( int i,  int j, T value);
   __property T Index [int i][int j] = {read=GetIndex, write=SetIndex};
   void Create(int Count);
   int GetListCount()  {      return List.Count;    }
public:
   ElectraIndexes();
   //__property T Index [ int i] [ int j] = {read=GetIndex};
   //T* operator[](int i) { return FIndex[i]; }
   //const T& operator[](int i) const { return FIndex[i]; }
   __property T Indexes [int i][int j] = {read=GetIndex};
   const T& ListIndex(int i) const { return List[i]; }
   __property int ListCount = {read=GetListCount};
   __property int Capacity = {read=FCapacity};
   const T& First() const { return List.First(); }
	const T& Last() const { return List.Last(); }
   ~ElectraIndexes();
};

//---------------------------------------------------------------------------
typedef class MemAlt
{
private:
   char *FName;
   void SetName(char* rhs)
   {
      if (FName)
      {
         if ( strlen(rhs) > strlen(FName) )
            FName = (char *) realloc(FName, strlen(rhs)+1);
         strcpy(FName, rhs);   
      }
      else
         FName = strdup(rhs);
   }
public:
   __property char* Name = {read=FName, write=SetName};
   MemAlt() {  FName = NULL; Core = -1; }
   ~MemAlt() { free ( FName ); }
   int Core; //Ядро в которое попала альтернатива. Заполняется после расчета
   //Чем выше тем качественней альтернатива. В ядре 0 находятся альтернативы доминируемые по всем параметрам.
} DataAlternatives;

typedef struct MemKrit
{
public:
   char *name;
   int weight;
   int scale;
} DataKriterias;
//------------------------------------------------------
//Класс Электра//---------------------------------------
enum ElectraVersion { ElectraI, ElectraII, ElectraIII };
//enum ElectraStatus { None=0, 

class Electra
{
private:
   int **Ratings;
   int A, K; //Не записывать
   //int ACapacity, KCapacity; //Не записывать
   //bool FRunOk;
   int Core;
   DataAlternatives **Optimal;

protected:
   int GetRating (int i, int j);
   void SetRating (int i, int j, int value);
   bool CheckIndexes(int Kidx, int Aidx);
   void GenerateCore(int Cind, double NCind, int &idx);
   bool VerionComparison();
   DataAlternatives GetOptimal(int i)   {      return *Optimal[i];   }
   void BaseNew();

public:
   Electra();
   ~Electra();
   __property  int AltCount = {read = A, write=Alternatives.New };
   __property  int KritCount = {read = K, write=Kriterias.New};
   void New(int NewAltCount, int NewKritCount); //[A][K]
   BasicParams <DataAlternatives> Alternatives;
   BasicParams <DataKriterias> Kriterias;
   __property int Rating [int i] [int j] = {read=GetRating, write=SetRating}; //Rating[A][K]
   ElectraIndexes <int> Concordance;
   ElectraIndexes <double> Discordance;
   void CreateIndexes();
   ElectraVersion Version;
   void Run();
   void Clear();
   __property DataAlternatives OptimalList [int i] = {read = GetOptimal};   
   bool SaveAsText(char *FileName);
   bool LoadFromText(char *FileName);
   //__property bool RunOk = { read=FRunOk };
   void Calc(); //Должен возвращать лучшую
};
/*Работа
1) 
*/
#endif

