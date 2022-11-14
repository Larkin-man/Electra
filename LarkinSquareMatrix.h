//Матрица. v1.0
//Опции, задаются командами #define:
//Для простых типов (int, double etc):
//#define LRArraySimpleType
//---------------------------------------------------------------------------
#ifndef LARKINSQUAREMATRIXH
#define LARKINSQUAREMATRIXH
#include <new>
//---------------------------------------------------------------------------
#define TYPED
#ifdef TYPED
 #define TEMPLT template< class T >
 #define T T
#else
 #define TEMPLT
 #define T int
#endif

//Матрица
TEMPLT class LRSquareMatrix
{
private:
	T **Data;    //Матрица [R][C]
   int FCapacity; //Размер выделенной памяти
	int FSize; //Число элементов в матрице (может быть меньше размера выделенной памяти)
   //Защита от копирования
   LRSquareMatrix& operator=(const LRSquareMatrix&) ;
   LRSquareMatrix(const LRSquareMatrix& CopyBase)
   {
      Data = 0;
      EnsureCapacity(CopyBase.Size); 
   }

protected:
   //vВыделить память
	void SetCapacity(int newCapacity)
	{
      if( newCapacity < FCapacity )
         return;
      if ( Data )
      {
         for (int i=0; i<FCapacity; ++i)
            Data[i] = (T*)realloc(Data[i], newCapacity * sizeof(T));
         Data = (T**)realloc(Data, newCapacity * sizeof(T*));
         for (int i=FCapacity; i<newCapacity; ++i)
            Data[i] = (T*)malloc(newCapacity * sizeof(T));
      }
      else //Первое
      {
         Data = (T**)malloc(newCapacity * sizeof(T*));
         for (int i=0; i<newCapacity; i++)
            Data[i] = (T*)malloc(newCapacity * sizeof(T));
      }
      FCapacity = newCapacity;
	}

   void SetSize(int newSize)
   {
      if (newSize < 0)
         newSize = 0;
      #ifdef LRArraySimpleType
      if (newSize > FSize)
         EnsureCapacity(newSize);
      FSize = newSize;
      #else
      if (newSize < FSize)
      {  Compress(FSize-newSize);      }
      else if (newSize > FSize)
      {
         EnsureCapacity(newSize);
         T temp;
         Expand(newSize, temp);
      }
      #endif
   }

   //   
   void ConstructCopy(int Row, int Col, const T& Item)
	{
      #ifdef LRArraySimpleType
      Data[Row][Col] = Item;
      #else
      new  (&Data[Row][Col]) T( Item );  //Размещение, вызывается конструктор копирования, или побитовое.
      #endif
	}
   T& GetCell(int R, int C) { return Data[R][C]; }
   void SetCell(int Row, int Col, const T& rhs)
   {
      if ( (Row < FSize)&&(Col < FSize) )
      {
         #ifndef LRArraySimpleType
         Destruct(Row, Col);
         #endif
         ConstructCopy(Row, Col, rhs);
      }
      else
      {
         if (Col > Row)
            Row = Col;
         Expand(Row+1, rhs);
      }
   }   

public:

	LRSquareMatrix(int capacity = 0)
	{
		Data = 0;
      FSize = 0;
		if ( capacity > 0 )
      {
		   Data = (T**)malloc(capacity * sizeof(T*));
         for (int i=0; i<capacity; i++)
            Data[i] = (T*)malloc(capacity * sizeof(T));
         FCapacity = capacity;
      }
      else
         FCapacity = 0;
	}

   ~LRSquareMatrix()
	{
      #ifndef LRArraySimpleType
      for (int i=0; i<FSize; ++i)
         for (int j=0; j<FSize; ++j)
            Data[i][j].~T();
      #endif
      FSize = 0;
      if ( Data )
		{
			for (int i=0; i<FCapacity; i++)
            free ( Data[i] );
         free ( Data );
		}
      Data = 0;
      FCapacity = 0;
   }

   __property int Size = { read=FSize, write=SetSize };
   __property int Capacity = { read=FCapacity, write=SetCapacity };

   //Убедимся, что памяти хватит для хранения по крайней мере этого числа элементов.
	void EnsureCapacity(int size)
	{
		if (size <= FCapacity)
		   return;
      int newCapacity = FCapacity;
      if ( FCapacity == 0 )
         newCapacity = 4;
      while ( newCapacity < size )
         newCapacity *= 2;
      SetCapacity(newCapacity);
	}
   //Добавить строку в матрицу (принимает массив из Size+1 элементов)
	void SetRow(int Row, const T* elements)
	{
      if (Row >= FSize)
         return;
      for (int i=0; i<FSize; ++i)
      {
         #ifndef LRArraySimpleType
         Destruct(Row, i);
         #endif
         ConstructCopy(Row, i, elements[i]);
      }
	}
   //Добавить столбец в матрицу (принимает массив из Size+1 элементов)
	void SetCol(int Col, const T* elements)
	{
      if (Col >= FSize)
         return;
      for (int i=0; i<FSize; ++i)
      {
         #ifndef LRArraySimpleType
         Destruct(i, Col);
         #endif
         ConstructCopy(i, Col, elements[i]);
      }
	}
	// Доступ к элементам
   __property T Cell[int Row][int Col] = {read=GetCell, write=SetCell};
   //T* operator[]( int r ) { return Data[r]; }      */
	const T* operator[]( int Row ) const { return Data[Row]; }
	// Базовый указатель
	//T *Base() { return Data; }
	T **Base() const { return Data; }
   //vРасширяет матрицу до Size строк, Size столбцов, заполняя новые ячейки элементом Item
   void Expand(int size, const T& Item)
   {
      if ( size > FCapacity )
         EnsureCapacity(size);
      if (size > FSize)
      {
         for (int j=0; j<FSize; ++j)
            for (int i=FSize; i<size; ++i)
               ConstructCopy(i, j, Item);
         for (int i=0; i<size; ++i)
            for (int j=FSize; j<size; ++j)
               ConstructCopy(i, j, Item);  
         FSize = size;
      }
   }
   void Compress(int size)
   {
      if (size > FSize)
         size = FSize;
      for (int i=FSize-size; i<FSize; ++i)
      {
         for (int j=0; j<i; ++j)
         {
            Data[i][j].~T();
            Data[j][i].~T();
         }
         Data[i][i].~T();
      }
      if (size > 0)
         FSize -= size;
   }
   void ExchangeRows(int Row1, int Row2)
   {
      if ( (Row1 >= FSize)||(Row2 >= FSize)||(Row1 == Row2) )
         return;
      T* temp = Data[Row1];
      Data[Row1] = Data[Row2];
      Data[Row2] = temp;
   }
   //v
   #ifndef LRArraySimpleType
   void Destruct(int Row, int Col)	{	Data[Row][Col].~T();	}
   void DestructRow(int Row)
   {
      for (int Ci=0; Ci<FSize; ++Ci)
         Data[Row][Ci].~T();
   }
   void DestructCol(int Col)
   {
      for (int R=0; R<FSize; ++R)
         Data[R][Col].~T();
   }
   #endif
   //Очистить список без освобождения памяти
   void Clear()
   {
      #ifndef LRArraySimpleType
      for (int i=0; i<FSize; ++i)
         for (int j=0; j<FSize; ++j)
            Data[i][j].~T();
      #endif
      FSize = 0;
   }
};
#endif
#undef LRArraySimpleType

