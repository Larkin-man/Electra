/*************************************************************\
*   ���������� ������                                         *
\*************************************************************/
#ifndef TwoCoherentListH
#define TwoCoherentListH
#define PATTN
#ifdef PATTN
 #define TEMPLT template <class T>
 #define IST <T>
#else
 #define TEMPLT
 #define IST
 #define T int
#endif
//---------------------------------------------------------------------------
#ifndef ClassesHPP   
enum TDuplicates { dupIgnore, dupAccept, dupError };
#endif


TEMPLT class TwoCoherentList
{
private:
   int FCount;
   int Mid;
   struct Data
   {
      T Store;
      Data *next;
      Data *prev;
   }
   *Begin, *End, *Linker, *FIterator;
   TDuplicates FDuplicates;

protected:
   Data* Get(int Index);
   Data* CreateItem(const T& Item);
   Data* Find(const T& Item);    //���� ������� ������ �� �������. NULL ���� ����.
   void Erase(Data *ExcludedItem);
   void SetCount(int NewCount)
   { FCount = NewCount; Mid = FCount/2;   }
   void Concatenate(Data *parent, Data *Item);
   void Push(Data* Position, Data* Item); //��������� ������ �� ���� Position, ������� ��� � ��� �����������. Count++
   T  GetIterator(void);
   void SetIterator(T Value);
   inline Data* GoForvard(Data* Item, int Pos);
   inline Data* GoBack(Data* Item, int Pos);
   bool FSorted;
   void QuickSort(Data *pLeft, Data *pRight);
   void SetSorted(bool Value);
   bool GetSorted(void);
   bool SortCompare(const T & Item1, const T & Item2);

public:
   typedef bool __fastcall (*TTCLSortCompare)(const T & Item1, const T & Item2); //��������� ��� ����������� �������
   TwoCoherentList(); //�����������
   __property int Count = { read=FCount, write=SetCount };
   int Add(const T& Item); //��������� ������ � ����� ������ ��� ���� Sorted �� � �������. ���������� ������. ������ ������� - 0
   int AddVector(T* Mas, int Count);   //AddVector - �������� ������ � �����. Sorted = false
   void Insert(int Index, const T& Item); //��������� ������ �� ���� Index, ������� ��� � ��� �����������
   T& operator[](int Index);    //������� ������ �� ������� Index. [0..Count-1]
   __property T Iterator = { read=GetIterator, write=SetIterator} ;
   __property bool Sorted = { read=GetSorted, write=SetSorted } ;
   void __fastcall Sort(TTCLSortCompare Compare);
   TTCLSortCompare CompareFunction;
   bool SortAccordingIncrease;
   int IndexOf(const T& Item);   //���������� ������ ������� ��������� �������. -1 ���� ���
   __property TDuplicates Duplicates = {read=FDuplicates, write=FDuplicates};
   void Exchange(int Index1, int Index2); //������ ������� �������� � ������������� Sorted � false
   T& First(void);
	T& Last(void);
   void Delete(int Index);       //������� � ����. ��������� countf �� 1 �� ���� ������ �� ������� . ������� ������ ��� ���������� Capacity
   void Remove(const T& Item);   //������� ������ �����, ���������� ������ ������ ���������, ��������� Count
   void Clear(void);
   ~TwoCoherentList(void)  {  Clear(); }
   //TwoCoherentList operator+ (TwoCoherentList & A);   //��������
   //TwoCoherentList operator+ ();               //�������   
   //Integers& operator+ (Integers & A, Integers & B);   //������
	/*TList* __fastcall Expand(void); //���� � ����� � Capacity
	//void * __fastcall Extract(void * Item);
	void __fastcall Move(int CurIndex, int NewIndex);
	void __fastcall Pack(void);
	void __fastcall Assign(TList* ListA, TListAssignOp AOperator = (TListAssignOp)(0x0), TList* ListB = (TList*)(0x0));
	__property int Capacity = {read=FCapacity, write=SetCapacity, nodefault};
	__property int Count = {read=FCount, write=SetCount, nodefault};
	__property void * Items[int Index] = {read=Get, write=Put/*, default*///};
	//__property PPointerList List = {read=FList};   

};  //TwoCoherentList
#endif

