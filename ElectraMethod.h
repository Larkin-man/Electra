// Copyright 2022 Larkin
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef ElectraMethodH
#define ElectraMethodH
#include <dstring.h>

//int **Ratings;
//int A, K;

class BasicParams
{
private:
         
protected:

public:
   //__property unsigned int Count = {read=Fcount, w
   AnsiString Name; 
};


class EAlternatives
{
private:
   BasicParams *Alt;
   unsigned int count;
   int ***Ratings;
   int *A, *K;
   
protected:

public:
   EAlternatives()//int** &sRatings, int &sA, int &sK)
   {
      ShowMessage("yes");
      //A = &sA;
      //K = &sK;
      //Ratings = &sRatings;
      Alt = NULL;
      count = 0;  
   }
   ~EAlternatives()
   {
      if ( Alt )
         free ( Alt );
      count = 0;
   }
   
   void New(unsigned int Count)
   {
      if ( Alt )
         Alt = (BasicParams*)realloc( Alt, Count * sizeof(BasicParams));
      else
         Alt = (BasicParams*)malloc(Count * sizeof(BasicParams));
      count = Count;
      /*if (K != 0) 
         if (A)
            Ratings = (int*)realloc( Ratings, Count * sizeof(int));
         else
            Ratings = (int*)malloc( Count * sizeof(int));      */

   }

   void AddI()
   {
      unsigned int g = GetCount();
   }
   unsigned int GetCount()
   {
      return count;
   }
   BasicParams& operator[]( int i )
   {  return Alt[i];   }

   const BasicParams& operator[]( int i ) const
   {  return Alt[i];   }

};


class Electra
{
private:
   int A,K;
   int **Ratings;

protected:

public:
   Electra()
   {
      ShowMessage("electra const");
      A = 0;
      K = 0;
      Ratings = 0;
   }
   EAlternatives Alternatives;
   void Run()
   {
   }

};

if ( Data )
   {
      Data = (int**)realloc(Data, n * sizeof(int*));
         for (int i=0;i<n;i++)
            Data[i] = (int*)realloc(Data[i], m * sizeof(int));
   }
         //Data = (int**)realloc( Data, Count * sizeof(int));
      else
      {
         Data = (int**)malloc(n * sizeof(int*));
         for (int i=0;i<n;i++)
            Data[i] = (int*)malloc(m * sizeof(int));
      }      

#endif
 