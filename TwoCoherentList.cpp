/*************************************************************\
* Двусвязный список                                           *
\*************************************************************/
#include <_null.h>
#include <stdio.h>
//#include <system.hpp>
//#include <Dialogs.hpp>     //для ShowMessage
//#include <dstring.h>
//#include <Classes.hpp>  //для тстринглист
//#include <conio.h>  //*/
#pragma hdrstop       
#include "TwoCoherentList.h"

//---------------------------------------------------------------------------
//Конструктор
TEMPLT TwoCoherentList IST::TwoCoherentList()
{
   FCount = 0;
   Mid = 0;
   Begin = NULL;
   End = NULL;
   FSorted = false;
   FDuplicates = dupAccept;
   CompareFunction = NULL;
   SortAccordingIncrease = true;
}
//---------------------------------------------------------------------------
//Get - Получить список по индексу
TEMPLT TwoCoherentList IST::Data* TwoCoherentList IST::Get(int Index)
{
   if (Index < 0)
      Index = 0;
   else if (Index >= FCount)
      Index = FCount-1;
   if ( Index > Mid )
   {  //C конца
      Linker = End;
      for (int i=FCount-1; i>Index; i--)
         Linker = Linker->prev;
   }
   else
   {  //C начала
      Linker = Begin;
      for (int i=0; i<Index; i++)
         Linker = Linker->next;
   }
   return Linker;
}
//---------------------------------------------------------------------------
//Find - Ищет элемент списка по объекту. NULL если нету.
TEMPLT TwoCoherentList IST::Data* TwoCoherentList IST::Find(const T& Item)
{
   for (Linker = Begin; Linker != NULL; Linker=Linker->next)
      if (Linker->Store == Item)
         return Linker;
   return NULL;
}
//---------------------------------------------------------------------------
//Push - //Вставляет объекта перед Position, сдвигая тот и все последующие. Count++
TEMPLT void TwoCoherentList IST::Push(Data* Position, Data* Item)
{
   //Вставка объекта в начало
   if (Position == Begin) //Еще сработает когда begin=NULL и Pos=NULL
   {  //В начало
      Item->next = Begin;
      Item->prev = NULL;
      if (Begin != NULL)
         Begin->prev = Item;
      Begin = Item;
      if (End == NULL)
         End = Item;
   }
   else if (Position == NULL)
   {   //В самый конец после последнего
      Item->next = NULL;
      Item->prev = End;
      if (Begin == NULL)
         Begin = Item;
      else
         End->next = Item;
      End = Item;
   }
   else
   {  //Вставляет объекта перед Position
      Item->next = Position;
      Item->prev = Position->prev;
      Position->prev->next = Item;
      Position->prev = Item;
   }
}
//---------------------------------------------------------------------------
//CreateItem - Создает элемент списка
TEMPLT inline TwoCoherentList IST::Data* TwoCoherentList IST::CreateItem(const T& Item)
{
   Data *NewItem = new Data;
   NewItem->Store = Item;
   FCount++;
   Mid = FCount / 2;
   return NewItem;
}
//---------------------------------------------------------------------------
//Add - Добавить  тут передавался не указатель
TEMPLT int TwoCoherentList IST::Add(const T& Item)
{
   if (FDuplicates == dupIgnore)
      if (Find(Item) != NULL)
         return -1;
   Data *NewItem = CreateItem(Item);
   int pos = 0;
   if (Sorted)
      for (Linker = Begin; Linker != NULL; Linker=Linker->next , pos++)
         //if (Item < Linker->Store)
         if (SortCompare(Item, Linker->Store))
         { //Вставить перед Линкером
            Push(Linker, NewItem);
            return pos;
         }
   Push(NULL, NewItem); //Вставка в конец
   return FCount-1;
}
//---------------------------------------------------------------------------
//AddVector - добавить массив
TEMPLT int TwoCoherentList IST::AddVector(T* Mas, int count)
{
   if (FDuplicates == dupIgnore)
   {
      for (int i=0; i<count; i++)
         if (Find(Mas[i]) != NULL)
         {
            T temp = Mas[i];
            count--;
            Mas[i] = Mas[count];
            Mas[count] = temp;
            i--;
            continue;
         }
      if (count == 0)
         return -1;
   }
   Data **NewMas = new Data*[count];
   for (int i=0; i<count; i++)
   {
      NewMas[i] = new Data;
      NewMas[i]->Store = Mas[i];
      if (i != 0)
      {
         NewMas[i]->prev = NewMas[i-1];
         NewMas[i-1]->next = NewMas[i];
      }    
   }
   NewMas[0]->prev = End;
   if (End != NULL)
      End->next = NewMas[0];
   if (Begin == NULL)
      Begin = NewMas[0];
   NewMas[count-1]->next = NULL;
   End = NewMas[count-1];
   FCount += count;
   Mid = FCount / 2;
   Sorted = false;
   return FCount-count;
}
//---------------------------------------------------------------------------
//Вернуть объект на позиции Index. [0..Count-1]
TEMPLT inline T& TwoCoherentList IST::operator[](int Index)
//template <class T> T& TwoCoherentList<T>::operator[](int Index)
{
   return Get(Index)->Store;
}
//---------------------------------------------------------------------------
//IndexOf - Возвращает индекс первого появление объекта
TEMPLT int TwoCoherentList IST::IndexOf(const T& Item)
{
   int Idx = 0;
   for (Linker = Begin; Linker != NULL; Linker=Linker->next , Idx++)
      if (Linker->Store == Item)
         return Idx;
   return -1;
}
//---------------------------------------------------------------------------
//Insert - //Вставляет объект на мето Index, сдвигая тот и все последующие
TEMPLT void TwoCoherentList IST::Insert(int Index, const T& Item)
{
   Push(Get(Index), CreateItem(Item));
   Sorted = false;
}
//---------------------------------------------------------------------------
//Serted = Value : SetSorted
TEMPLT void TwoCoherentList IST::SetSorted(bool Value)
{
   if (Value)
   {
      //if (CompareFunction == NULL)
      //{           FSorted = false;          return;       }
      if (FCount > 1)
      {
         QuickSort(Begin, End);
         Begin = GoBack(Begin, FCount);
      }
   }
   FSorted = Value;
}
//---------------------------------------------------------------------------
TEMPLT bool TwoCoherentList IST::GetSorted(void)
{
   //if (CompareFunction == NULL)
   //   FSorted = false;
   return FSorted;
}
//---------------------------------------------------------------------------
//Быстрая сортировка
TEMPLT bool TwoCoherentList IST::SortCompare(const T & Item1, const T & Item2)
{
   if (CompareFunction == NULL)
      if (SortAccordingIncrease)
         return (Item1 < Item2);
      else
         return (Item1 > Item2);
   return CompareFunction(Item1, Item2);
}
TEMPLT void TwoCoherentList IST::QuickSort(Data *pLeft, Data *pRight)
{
   //if (CompareFunction == NULL)
   //   return;
	Data *pStart;
	Data *pCurrent;
	int nCopyInteger;
	// сортировка окончена - выход
	if (pLeft == pRight) return;
	// установка двух рабочих указателей - Start и Current
	pStart = pLeft;
	pCurrent = pStart->next;
	// итерация по списку слева направо
	while (1)
	{
		//if (pStart->Store < pCurrent->Store)
      if (SortCompare(pStart->Store, pCurrent->Store))
		{
			nCopyInteger = pCurrent->Store;
			pCurrent->Store = pStart->Store;
			pStart->Store = nCopyInteger;
		}	
		
		if (pCurrent == pRight) break;
		pCurrent = pCurrent->next;
	}
	// переключение First и Current - максимум попадает в правый конец списка
   //ShowMessage(IntToStr(pLeft->Store) + " и "+IntToStr(pCurrent->Store));
	nCopyInteger = pLeft->Store;
	pLeft->Store = pCurrent->Store;
	pCurrent->Store = nCopyInteger;
	// сохранение Current
	Data *pOldCurrent = pCurrent;
	// рекурсия
	pCurrent = pCurrent->prev;
	if (pCurrent != NULL)
	{
		if ((pLeft->prev != pCurrent) && (pCurrent->next != pLeft))
			QuickSort(pLeft, pCurrent);
	}
	pCurrent = pOldCurrent;
	pCurrent = pCurrent->next;
	if (pCurrent != NULL)
	{
		if ((pCurrent->prev != pRight) && (pRight->next != pCurrent))
			QuickSort(pCurrent, pRight);
	}
}
//---------------------------------------------------------------------------
//Clear - Очищает всё
TEMPLT void TwoCoherentList IST::Clear(void)
{
   Data *DelItem, *Item = Begin;
	while (Item != NULL)
	{
		DelItem = Item;
		Item = Item->next;
		delete DelItem;
	}
	Begin = NULL; End = NULL;
   FCount = 0;   Mid = 0;
}
//---------------------------------------------------------------------------
//Сoncatenate - Соединяет два элемента //REMARKABLE!
TEMPLT void TwoCoherentList IST::Concatenate(Data *parent, Data *Item)
{
   if (parent != NULL)
       parent->next = Item;
   if (Item != NULL)
       Item->prev = parent;
}
//---------------------------------------------------------------------------
//Erase - Удаляет элемент списка
TEMPLT void TwoCoherentList IST::Erase(Data *ExcludedItem)
{
   if (ExcludedItem == NULL)
      return;
   if (ExcludedItem == Begin)
      Begin = Begin->next;
   if (ExcludedItem == End)
      End = End->prev;
   Concatenate(ExcludedItem->prev, ExcludedItem->next);
   delete ExcludedItem;
   Count--;
}
//---------------------------------------------------------------------------
//Delete
TEMPLT void TwoCoherentList IST::Delete(int Index)
{
   if ( FCount == 0 )
      return;
   if ( FCount == 1 )
   {
      Clear();
      return;
   }   
   Erase(Get(Index));
}
//---------------------------------------------------------------------------
//Remove - Удаляет первую копию, возвращает индекс перред удалением, уменьшает Count
TEMPLT void TwoCoherentList IST::Remove(const T& Item)
{
   Erase(Find(Item));
}
//---------------------------------------------------------------------------
//GetIterator -
TEMPLT T  TwoCoherentList IST::GetIterator(void)
{
   if (FIterator == NULL)
      return NULL;
   T Curr = FIterator->Store;
   FIterator = FIterator->next;
   return Curr;
}  
//---------------------------------------------------------------------------
//SetIterator - Установить в NULL чтобы начать
TEMPLT void TwoCoherentList IST::SetIterator(T Value)
{
   if (Value == NULL)
      FIterator = Begin;
   else
      FIterator = Find(Value);
}
//---------------------------------------------------------------------------
//Exchange - Поменять местами элементы
TEMPLT void TwoCoherentList IST::Exchange(int Index1, int Index2)
{
   if (Index1 == Index2)
      return;
   if ((Index1 >= FCount) || (Index2 >= FCount))
      return;
   Data *A = Get(Index1);
   Data *B = Get(Index2);
   T One = A->Store;
   A->Store = B->Store;
   B->Store = One;
   Sorted = false;
}
//---------------------------------------------------------------------------
//First - Указатель на первый
TEMPLT T& TwoCoherentList IST::First(void)
{
   return operator[](0);
}
//---------------------------------------------------------------------------
//First - Указатель на последний
TEMPLT T& TwoCoherentList IST::Last(void)
{
   return operator[](FCount-1);
}
//---------------------------------------------------------------------------
//GoForvard - Двигать указатель вперед
TEMPLT inline TwoCoherentList IST::Data* TwoCoherentList IST::GoForvard(Data* Item, int Pos)
{
   Data *Link = Item;
   for (int i=0 ; i<Pos; i++)
      if (Link->next != NULL)
         Link = Link->next;
      else
         break;
   return Link;
}
//---------------------------------------------------------------------------
//GoBack - Двигать указатель назад
TEMPLT inline TwoCoherentList IST::Data* TwoCoherentList IST::GoBack(Data* Item, int Pos)
{
   Data *Link = Item;
   for (int i=0 ; i<Pos; i++)
      if (Link->prev != NULL)
         Link = Link->prev;
      else
         break;
   return Link;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#pragma package(smart_init)
