//Матрица. v1.0
//Опции, задаются командами #define:
//Для простых типов (int, double etc):
//#define LRArraySimpleType
//---------------------------------------------------------------------------
#ifndef LARKINMATRIXH
#define LARKINMATRIXH
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
TEMPLT class LRMatrix
{
private:
	T **Data;    //Матрица [R][C]
   #ifndef SquareMatrix
	int FColCapacity; //Размер выделенной памяти
	int FColCount; //Число элементов в матрице (может быть меньше размера выделенной памяти)
   int FRowCapacity; //Размер выделенной памяти
	int FRowCount; //Число элементов в матрице (может быть меньше размера выделенной памяти)
   #else
   int FCapacity; //Размер выделенной памяти
	int FSize; //Число элементов в матрице (может быть меньше размера выделенной памяти)
   #endif
   //Защита от копирования
   //LARKINMATRRIX(const LARKINMATRRIX&) ;
   LRMatrix& operator=(const LRMatrix&) ;
           
protected:
   //vВыделить память
	void SetCapacity(int Row, int Col)
	{
      if ( Data )
      {
         if( Col > FColCapacity )
         {
            for (int i=0; i<FRowCapacity; ++i)
               Data[i] = (T*)realloc(Data[i], Col * sizeof(T));
            FColCapacity = Col;
         }
         if ( Row > FRowCapacity ) // Row ++
         {
            Data = (T**)realloc(Data, Row * sizeof(T*));
            for (int i=FRowCapacity; i<Row; ++i)
               Data[i] = (T*)malloc(FColCapacity * sizeof(T));
            FRowCapacity = Row;
         }                                         
      }
      else //Первое
      {
         Data = (T**)malloc(Row * sizeof(T*));
         for (int i=0; i<Row; i++)
            Data[i] = (T*)malloc(Col * sizeof(T));
         FRowCapacity = Row;
         FColCapacity = Col;
      }
	}
   //
   void SetColCapacity(int Col)
	{
      if ( Data )
      {
         if( Col > FColCapacity )
         {
            for (int i=0; i<FRowCapacity; ++i)
               Data[i] = (T*)realloc(Data[i], Col * sizeof(T));
            FColCapacity = Col;
         }
      }
      else //Первое NENRSVITSA
      {
         Data = (T**)malloc(FRowCapacity * sizeof(T*));
         for (int i=0; i<FRowCapacity; i++)
            Data[i] = (T*)malloc(Col * sizeof(T));
         FColCapacity = Col;
      }
	}

   void SetRowCapacity(int Row)
	{
      if ( Data )
      {
         if ( Row > FRowCapacity ) // Row ++
         {
            Data = (T**)realloc(Data, Row * sizeof(T*));
            for (int i=FRowCapacity; i<Row; ++i)
               Data[i] = (T*)malloc(FColCapacity * sizeof(T));
            FRowCapacity = Row;
         }                                         
      }
      else //Первое
      {
         Data = (T**)malloc(Row * sizeof(T*));
         for (int i=0; i<Row; i++)
            Data[i] = (T*)malloc(FColCapacity * sizeof(T));
         FRowCapacity = Row;
      }
	}

   void SetColCount(int newColCount)
   {
      if (FColCount < 0)
         FColCount = 0;
      #ifdef LRArraySimpleType
      if (newColCount > FColCount)
         EnsureCapacity(FRowCount, newColCount);
      FColCount = newColCount;
      #else
      if (newColCount < FColCount)
      {
         while (FColCount > newColCount)
			   PopBackCol();
      }
      else if (newColCount > FColCount)
      {
         EnsureCapacity(FRowCount, newColCount);
         if (FRowCount > 0)
         {
            T temp;
            for (int i=FColCount; i<newColCount; ++i)
               for (int j=0; j<FRowCount; ++j)
                  ConstructCopy(j, i, temp);
         }
         FColCount = newColCount;
      }
      #endif
   }
   void SetRowCount(int newRowCount)
   {
      if (FRowCount < 0)
         FRowCount = 0;
      #ifdef LRArraySimpleType
      if (newRowCount > FRowCount)
         EnsureCapacity(newRowCount, FColCount);
      FRowCount = newRowCount;
      #else
      if (newRowCount < FRowCount)
      {
         while (FRowCount > newRowCount)
			   PopBackRow();
      }
      else if (newRowCount > FRowCount)
      {
         EnsureCapacity(newRowCount, FColCount);
         if (FColCount > 0)
         {
            T temp;
            for (int i=FRowCount; i<newRowCount; ++i)
               for (int j=0; j<FColCount; ++j)
                  ConstructCopy(i, j, temp);
         }
         FRowCount = newRowCount;
      }

      #endif
   }
   //v     
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
      if ( (Row < FRowCount)&&(Col < FColCount) )
      {
         #ifndef LRArraySimpleType
         Destruct(Row, Col);
         #endif
         ConstructCopy(Row, Col, rhs);
      }
      else
      { Expand(Row+1, Col+1, rhs);      }
   }


public:

	LRMatrix(int Row = 0, int Col = 0)
	{
		Data = 0;
      FRowCount = 0;
      FColCount = 0;  
		if ( (Row > 0)||(Col > 0) )
      {
		   Data = (T**)malloc(Row * sizeof(T*));
         for (int i=0; i<Row; i++)
            Data[i] = (T*)malloc(Col * sizeof(T));
         FRowCapacity = Row;
         FColCapacity = Col;
      }
      else
      {
         FRowCapacity = 0;
         FColCapacity = 0;
      }
	}

   LRMatrix(const LRMatrix& CopyBase)
   {
      Data = 0;
      EnsureCapacity(CopyBase.FRowCount, CopyBase.FColCount); 
   }

   ~LRMatrix()
	{
      #ifndef LRArraySimpleType
      for (int i=0; i<FRowCount; ++i)
         for (int j=0; j<FColCount; ++j)
            Data[i][j].~T();
      #endif
      FRowCount = 0;
      FColCount = 0;
      if ( Data )
		{
			for (int i=0; i<FRowCapacity; i++)
            free ( Data[i] );
         free ( Data );
		}
      Data = 0;
      FRowCapacity = 0;
      FColCapacity = 0;
   }
   
   __property int RowCount = { read=FRowCount, write=SetRowCount };
   __property int ColCount = { read=FColCount, write=SetColCount };
   __property int RowCapacity = { read=FRowCapacity, write=SetRowCapacity };
   __property int ColCapacity = { read=FColCapacity, write=SetColCapacity };

   //vУбедимся, что памяти хватит для хранения по крайней мере этого числа элементов.
	void EnsureCapacity(int Row, int Col)
	{
		if (Row > FRowCapacity)
		{
         int newRowCapacity = FRowCapacity;
			if ( FRowCapacity == 0 )
				newRowCapacity = 4;
			while ( newRowCapacity < Row )
				newRowCapacity *= 2;
         Row = newRowCapacity;
		}
      if (Col > FColCapacity)
		{
         int newColCapacity = FColCapacity;
			if ( FColCapacity == 0 )
				newColCapacity = 4;
			while ( newColCapacity < Col )
				newColCapacity *= 2;
         Col = newColCapacity;
		}
      if ( (Row > FRowCapacity) || (Col > FColCapacity) )
         SetCapacity(Row, Col);
	}
   //Добавить строку в матрицу (принимает массив из ColCount элементов)
	void AppendRow(const T* elements)
	{
      if (FRowCount+1 > FRowCapacity)
		   EnsureCapacity(FRowCount+1, FColCount);
      for (int i=0; i<FColCount; ++i)
         ConstructCopy(FRowCount, i, elements[i]);
		FRowCount++;
	}
	// Доступ к элементам
   __property T Cell[int Row][int Col] = {read=GetCell, write=SetCell};
   //T* operator[]( int r ) { return Data[r]; }      */
	const T* operator[]( int Row ) const { return Data[Row]; }
	// Базовый указатель
	//T *Base() { return Data; }
	const T **Base() const { return Data; }
   //vРасширяет матрицу до Row строк, Col столбцов, заполняя новые ячейки элементом Item
   void Expand(int Row, int Col, const T& Item)
   {
      if ( (Row > FRowCapacity)||(Col > FColCapacity) )
         EnsureCapacity(Row, Col);
      if (FRowCount < Row)
      {
         for (int j=0; j<FColCount; ++j)
            for (int i=FRowCount; i<Row; ++i)   
               ConstructCopy(i, j, Item);
         FRowCount = Row;
      }
      if (FColCount < Col)
      {
         for (int i=0; i<FRowCount; ++i)
            for (int j=FColCount; j<Col; ++j) 
               ConstructCopy(i, j, Item);  
         FColCount = Col;
      }
   }
   void ExchangeRows(int Row1, int Row2)
   {
      if ( (Row1 >= FRowCount)||(Row2 >= FRowCount)||(Row1 == Row2) )
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
      for (int Ci=0; Ci<FColCount; ++Ci)
         Data[Row][Ci].~T();
   }
   void DestructCol(int Col)
   {
      for (int R=0; R<FRowCount; ++R)
         Data[R][Col].~T();
   }
   #endif
   //vУдалить последний элемент
   void PopBackRow()
	{
      if (FRowCount == 0)
         return;
      #ifndef LRArraySimpleType
      for (int C=0; C<FColCount; ++C)
         Data[FRowCount-1][C].~T();
      #endif
		FRowCount--;
	}
   void PopBackCol()
	{
      if (FColCount == 0)
         return;
      #ifndef LRArraySimpleType
      for (int R=0; R<FRowCount; ++R)
         Data[R][FColCount-1].~T();
      #endif
		FColCount--;
	}

   //Очистить список без освобождения памяти
   void Clear()
   {
      #ifndef LRArraySimpleType
      for (int i=0; i<FRowCount; ++i)
         for (int j=0; j<FColCount; ++j)
            Data[i][j].~T();
      #endif
      FRowCount = 0;
      FColCount = 0;
   }
};
#endif

