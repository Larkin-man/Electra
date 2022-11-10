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
//Включай только для ShowMessage
//#include <system.hpp>
//#include <Dialogs.hpp>
//#include <dstring.h>
//#include <conio.h>  //*/
#include <stdio.h>
#include <alloc.h>

template <class T> class BasicParams
{
private:

protected:
   T *Data;
   int *FCount;
   int ***Ratings;
   int *A, *K;
   void NewRatings(const int &nAlt, const int &nKrit); //[A][K]

public:
   BasicParams(int &sA, int &sK, int** &sRatings, bool CountIsAorK);
   ~BasicParams();
   int GetCount() { return *FCount; }
   __property int Count = {read = GetCount, write = New};
   void New(int count);

   T& operator[]( int i )   {      return Data[i];   }
   const T& operator[]( int i ) const   {      return Data[i];   }
};

typedef struct
{
   char *name;
} DataAlternatives;

typedef class DataKrit
{
public:
   char *name;
   int weight;
   int scale;
   DataKrit()
   {
      ShowMessage("constr DataKrit");
   }

} MemKrit;

class Electra
{
private:
   int **Ratings;
   int AltCount, KritCount; //Не записывать
   int AIndexes;
   int **FConsentIndex;

protected:
   int GetRating (int i, int j);
   void SetRating (int i, int j, int value);
   int CheckIndexes(int Kidx, int Aidx);
   int GetConsentIndex (int i, int j);
   void SetConsentIndex (int i, int j, int value);

public: 
   Electra();
   ~Electra();
   __property int A = {read = AltCount };
   __property int K = {read = KritCount };
   BasicParams <DataAlternatives> Alternatives;
   BasicParams <MemKrit> Kriterias;
   __property int Rating [int i] [int j] = {read=GetRating, write=SetRating}; //Rating[A][K]
   __property int ConsentIndex [int i] [int j] = {read=GetConsentIndex, write=SetConsentIndex}; //Rating[Ai][Aj] i domine nad j
   void CreateIndexes();
   void Run();
   bool SaveAsText(char *FileName);
   bool LoadFromText(char *FileName);

};  
#endif
