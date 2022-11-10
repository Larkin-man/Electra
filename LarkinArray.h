//������������ ������. v1.1 ��� �������� ��������� ������������ �� ����������!
//�����, �������� ��������� #define:
//��� ������� ����� (int, double etc):
//#define LRArraySimpleType
//�������������� ��� ������� �����:
//#define LRArrayAutoSort
//�������� ��������� (������������ ���������� ���������):
//#define LRArrayCollection
//---------------------------------------------------------------------------
#undef LARKINARRAY
#undef LRArrayBasic
#ifdef LRArrayAutoSort
   #ifndef LRArraySimpleType
      #error This version does not contain sorting for compound types. Define LRArraySimpleType
   #endif
   #ifdef LRArrayCollection
      #ifndef LarkinCollectionSortH
         #define LarkinCollectionSortH
         #define LARKINARRAY LRCollectionSort
      #endif
   #else
      #ifndef LarkinArraySortH
         #define LarkinArraySortH
         #define LARKINARRAY LRArraySort
      #endif
   #endif
#else
   #ifdef LRArrayCollection
      #ifndef LarkinCollectionH
         #define LarkinCollectionH
         #define LARKINARRAY LRCollection
      #endif
   #else
      #ifndef LarkinArrayH
         #define LarkinArrayH
         #define LARKINARRAY LRDynArray
         #define LRArrayBasic
      #endif
   #endif
#endif

#ifdef LARKINARRAY
   #ifndef LRArraySimpleType
      #include <new>
   #endif

//---------------------------------------------------------------------------
#define TYPED
#ifdef TYPED
 #define TEMPLT template< class T >
 #define T T
#else
 #define TEMPLT
 #define T int
#endif
#include <alloc.h>
#include <mem.h>

// ������������ ������
TEMPLT class LARKINARRAY
{
private:
	T *Data;    //���������� ������
	int FCapacity; //������ ���������� ������
	int FCount; //����� ��������� � ������� (����� ���� ������ ������� ���������� ������)
   #ifdef LRArrayAutoSort
      #ifdef LRArraySimpleType
      bool FSortingAccordIncrease;
      #endif
   #endif
                          
protected:
   //�������� ������
	void SetCapacity(int newCapacity)
	{
      if (newCapacity < 0)
         return;           
		if ( Data )
			Data = (T*)realloc( Data, newCapacity * sizeof(T) );
		else
			Data = (T*)malloc( newCapacity * sizeof(T) );
      //Data = new T[newCapacity];
		FCapacity = newCapacity;
	}

   void SetCount(int newCount)
   {
      #ifdef LRArraySimpleType
      if (newCount > FCount)
         EnsureCapacity(newCount);
      else if (newCount < 0)
         newCount = 0;
      FCount = newCount;
      #else
      if (newCount < FCount)
      {
         if (newCount < 0)
         newCount = 0;
         while (FCount > newCount)
			   PopBack();
      }
      else if (newCount > FCount)
      {
         EnsureCapacity(newCount);
         T temp;
         for (int i=FCount; i<newCount; ++i)
            ConstructCopy(i, temp);
         FCount = newCount;
      }
      #endif
   }

	//����������� ����� ������� ����� idx �� num : �������+ ������-
	void MoveElements(int idx, int num)
	{
		int numBytesToCopy = (FCount-idx) * sizeof(T);
		if (numBytesToCopy > 0)
			memmove(&Data[idx+num], &Data[idx], numBytesToCopy);
	}

public:
   //����������� ��� LRDynArray � LRCollection � ����� ����������
   //����������� ��� LRDynArraySort � LRCollectionSort � ����� �����������. ���������� �������� � ����� ������
   #ifdef LRArrayAutoSort
   LARKINARRAY(bool sortingAccordingIncrease, int capacity = 0)
   {
      FSortingAccordIncrease = sortingAccordingIncrease;
   #else
	LARKINARRAY(int capacity = 0)
	{
   #endif
		Data = 0;
      FCount = 0;
		if (capacity > 0)
		   SetCapacity(capacity);   
      else
         FCapacity = 0;
	}
   LARKINARRAY(const LARKINARRAY& Array)
   {
      #ifdef LRArrayAutoSort
      FSortingAccordIncrease = Array.SortingAccordIncrease;
      #endif
      Data = 0;
      Capacity = Array.Capacity;
      FCount = Array.Count;
      for (int i=0; i<FCount; ++i)
         ConstructCopy(i, Array[i]);
   }
   LARKINARRAY&operator=(const LARKINARRAY& rhs)
   {
      SetCount(0);
      #ifdef LRArrayAutoSort
      FSortingAccordIncrease = rhs.SortingAccordIncrease;
      #endif
      Data = 0;
      Capacity = rhs.Capacity;
      FCount = rhs.Count;
      for (int i=0; i<FCount; ++i)
         ConstructCopy(i, rhs[i]);
   }
   ~LARKINARRAY()
	{
      Free();
	}
   #ifdef LRArrayAutoSort
   //�������� ������� � ������
   void Add(const T& el)
	{
      int Pos = 0;
      if (FSortingAccordIncrease)
      {
         for (; Pos < FCount; Pos++)
         {
            #ifdef LRArrayCollection
            if (Data[Pos] == el)
               return ;
            #endif
            if (Data[Pos] > el)
               break;
         }
         if (Pos == FCount-1)
         {
            if (el > Data[Pos])
               Pos++;
         }
      }
      else
      {
         for (; Pos < FCount; Pos++)
         {
            #ifdef LRArrayCollection
            if (Data[Pos] == el)
               return ;
            #endif
            if (Data[Pos] < el)
               break;
         }
         if (Pos == FCount-1)
         {
            if (el < Data[Pos])
               Pos++;
         }         
      }
		EnsureCapacity(FCount+1);
      if (Pos < FCount) 
		   MoveElements(Pos, 1);
      //Data[idx] = el;
		ConstructCopy(Pos, el);
		FCount++;
	}
   #else
   //�������� ������� � ����� �������
	void PushBack(const T& el)
	{
      #ifdef LRArrayCollection
      if (Find(el) != -1)
         return;
      #endif
		EnsureCapacity(FCount+1);
		ConstructCopy(FCount, el);
		FCount++;
	}   
   #endif

   __property int Count = { read=FCount, write=SetCount };
   __property int Capacity = { read=FCapacity, write=SetCapacity };

   //��������, ��� ������ ������ ��� �������� �� ������� ���� ����� ����� ���������.
	void EnsureCapacity(int capacity)
	{
		if ( capacity > FCapacity )
		{
			//int newCapacity = FCapacity;
			if ( FCapacity == 0 )
				FCapacity = 4;
			while ( FCapacity < capacity )
				FCapacity *= 2;
			SetCapacity( FCapacity );
		}
	}
	// ������ � ���������
   #ifdef LRArrayBasic
	T& operator[]( int i ) { return Data[i]; }
   #endif
	const T& operator[]( int i ) const { return Data[i]; }
	// ������� ���������
	//T *Base() { return Data; }
	const T *Base() const { return Data; }
   #ifdef LRArrayBasic
	// ������ � ��������� ��������
	T& First() { return Data[0]; }
	T& Last() { return Data[FCount-1]; }
   #endif
	const T& First() const { return Data[0]; }
	const T& Last() const { return Data[FCount-1]; }
	bool Empty() const { return FCount == 0; } // ������?
   void Invert()
   {
      T temp;// = //(T*)malloc( sizeof(T) );
      for (int i=0; i<FCount/2; ++i)
      {
         temp = Data[i];
         Data[i] = Data[FCount - i - 1];
         Data[FCount - i - 1] = temp;
      }
      #ifdef LRArrayAutoSort
      FSortingAccordIncrease = !FSortingAccordIncrease;
      #endif   
   }

   #ifdef LRArrayAutoSort
   __property bool SortingAccordIncrease = { read=FSortingAccordIncrease };
   protected:
   #endif
   void ConstructCopy(int i, const T& el)
	{
      #ifdef LRArraySimpleType
      Data[i] = el;
      #else
      new  (&Data[i]) T( el );  //
      #endif
	}
   #ifdef LRArrayAutoSort
   public:
   #endif
   #ifndef LRArraySimpleType
   void Destruct(int i)	{		Data[i].~T();	}
   #endif
   //�������� ������� � ������������ �����
   #ifndef LRArrayAutoSort
	void Insert(int idx, const T& el)
	{
      #ifdef LRArrayCollection
      if (Find(el) != -1)
         return;
      #endif
      if (idx < 0)
         idx = 0;
      else if (idx > FCount)
         idx = FCount;
		EnsureCapacity(FCount+1);
		MoveElements(idx, 1);
      //Data[idx] = el;
		ConstructCopy(idx, el);
		FCount++;
	}
   #endif

   //����� ������ ��������
	int Find(const T& el) const
	{
		for (int i=0; i < FCount; i++)
		{
			if (Data[i] == el)
				return i;
		}
		return -1;
	}
   
   // ������� ��������� �������
   void PopBack()
	{
      if (FCount == 0)
         return;
      #ifndef LRArraySimpleType
      Destruct(FCount-1);
      #endif
		FCount--;
	}
	//������� ������������ �������
	void Delete(int idx)
	{
      if (FCount <= 0)
      {
         FCount = 0;
         return;
      }
      if (idx < 0)
         return;
      else if (idx >= FCount)
         return;
      #ifndef LRArraySimpleType
		Destruct(idx);
      #endif
		MoveElements(idx+1, -1);
		FCount--;
	}

   //����� � ����������!
	void Remove(const T& el)
	{
		Delete( Find(el) );
	}
   
   //�������� ������ ��� ������������ ������
   void Clear()
   {
      SetCount(0);
   }
	//�������� ������
	void Free()
	{
      #ifdef LRArraySimpleType
      FCount = 0;
      #else
      while (FCount > 0)
			PopBack();
      #endif
      if ( Data )
		{
			free( Data );
         //delete [] Data;
		}
      Data = 0;
      FCapacity = 0;
	}
};
#endif
#undef LRArraySimpleType
#undef LRArrayAutoSort
#undef LRArrayCollection
#undef LRArrayBasic
