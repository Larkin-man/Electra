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
//�������������� ��� ������� �����:
#define LRArrayAutoSort
//�������� ��������� (������������ ���������� ���������):
#define LRArrayCollection
#include "LarkinArray.h"
//������������ ������:
#include "LarkinArray.h"
//���������� ������� (������� ��������, ����������)
#define LRArraySimpleType
#include "LarkinSquareMatrix.h"
//������� (������� ������)
#define LRArraySimpleType
#include "LarkinMatrix.h"
//---------------------------------------------------------------------------
//ElectraIndexes - ������� ����� ��� �������� �������� � ����������
template <class T> class ElectraIndexes
{
   friend class Electra;
private:
   LRSquareMatrix <T> Indexes; //   T **FIndex;
   LRCollectionSort<T> List;
protected:
   void SetIndex (int i,  int j, T value); //����������� �� �������
   //������� ��������:
   int GetSize(void) const { return Indexes.Size; }
   int GetCapacity(void) const { return Indexes.Capacity; }
   T& GetIndex (int i,  int j);
   //��������������� ��������� ��������:
   //__property T Index [int i][int j] = {read=GetIndex, write=SetIndex};
   int GetListCount() const  {      return List.Count;   }
   int GetListCapacity() const  {      return List.Capacity;   }

public:
   ElectraIndexes();
   //������� ��������:
   __property T Index [int i][int j] = {read=GetIndex};
   __property int Size = { read=GetSize };
   __property int Capacity = { read=GetCapacity };
   //T **Base() const { return Indexes.Base; }
   //��������������� ��������� ��������:
   const T& ListIndex(int i) const { return List[i]; }
   const operator[] (int i) { return List[i]; }
   __property int ListCount = {read=GetListCount};
   __property int ListCapacity = {read=GetListCapacity};
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
      //if (rhs == FName)
      //   return;
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
   //__property char* Name = {read=FName, write=SetName};
   AnsiString Name;
   MemAlt() {  Core = -1; }
   ~MemAlt() {  }
   int Core; //���� � ������� ������ ������������. ����������� ����� �������
   //��� ���� ��� ������������ ������������. � ���� 0 ��������� ������������ ������������ �� ���� ����������.
} DataAlternatives;

typedef struct MemKrit
{
public:
	AnsiString Name; //char *name;
   int weight;
   int scale;
} DataKriterias;
//------------------------------------------------------
//����� �������//---------------------------------------
enum ElectraVersion { ElectraI, ElectraIM, ElectraII, ElectraIII };
//enum ElectraStatus { None=0,

class Electra
{
private:
   LRMatrix <int> Ratings;
   int Core;
   DataAlternatives **Optimal;
   Electra(const Electra& copy);
   operator =(const Electra& copy);

protected:
   int  GetRating (int i, int j);
   void SetRating (int i, int j, int value);
   void GenerateCore(int Cind, double NCind, int &idx);
   bool VerionComparison();
   DataAlternatives GetOptimal(int i)   {      return *Optimal[i];   }
   int GetA(void) { return Alternatives.Count; }
   int GetK(void) { return Kriterias.Count; }

public:
   Electra();
   __property int A = {read=GetA};
   __property int K = {read=GetK};
   LRDynArray <DataAlternatives> Alternatives;
   LRDynArray <DataKriterias> Kriterias;
   __property int Rating [int i][int j] = {read=GetRating, write=SetRating}; //Rating[A][K]
   ElectraIndexes <int> Concordance;
   ElectraIndexes <double> Discordance;
   ElectraVersion Version;
   int CalcIndexes(); //���������� ���-�� ����������� � ������ ������.
   __property DataAlternatives OptimalList [int i] = {read = GetOptimal};
   bool SaveAsText(char *FileName);
   bool LoadFromText(char *FileName);
   //__property bool RunOk = { read=FRunOk };
   void Calc(); //������ ���������� ������
   void Clear();
   ~Electra();
};
/*������
1) ������ �������� � ������������
Alternatives.Count ��� []
*/
#endif
