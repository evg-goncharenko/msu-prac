/*____________________________________________________________________

  Project:

      Dynamic arrays, set and dictionary templates
      ____________________________________________

  (C) Maxim Proskurnya,                           1999--2004, AMC, MSU
      max@axofiber.org.ru

  Component : Array
  Comments  : Defines templates of dynamic collectors for objects and
              object pointers, set, dictionary.
              All iterating operators [] check boundaries and generate
              exceptions. Use "At" method for fast element access.
  ____________________________________________________________________

  02.III.2004                                                    03.22
      Fixed and optimized "CSCDictionary::FindKey" method.
  09.II.2004                                                     03.21
      Extracted "CSCAbstractPtrArray" generic class.
  16.I.2004                                                      03.20
      Optimized "CSCDictionary" template, added "Sort",
      "ForceNotToSort". Added "CSCPtrArray::AppendAndClean", and
      "CSCPtrArray::Append" was fixed to have same semantic as
      "CSCTypeArray::Append" do.
  5.I.2004                                                       02.19
      Added "CSCDictionary::KeyOf", "CSCSet::CreateFrom",
      "CSCSet::operator >", "CSCSet::At" methods, "const" modifier to
      the "At" methods.
  20.XI.2003                                                     02.18
      Fixed "memory leaks on RemoveAll" problem.
  18.X.2003                                                      02.17
      Added "CSCPtrArray::CreateFrom" method.
  13.VI.2003                                                     02.16
      Reorganized all class templates.
  17.V.2003                                                      01.15
      Updated "Add" methods (return value type). Fixed "operator+"
      implementations and copy constructors.
  28.IV.2003                                                     01.14
      Fixed critical mistype in "CSCPtrArray::Remove" method.
  20.XII.2002                                                    01.13
      Updated "CSCPtrArray" template constructor and methods
      "Add" and "Remove" to implement doubling buffer strategy.
  09.VI.2002                                                     01.12
      Release.
  12.V.2002                                                      00.12
      Fixed "CSCTypeArray" class, added the copy constructor.
  22.IV.2002                                                     00.11
      Added "CSCDictionary" class.
  04.I.2002                                                      00.10
      Fixed "Add (,Position)" methods in "CSCPtrArray" and
      "CSCPtrArray" templates not to shift empty spaces out of
      allocated memory (critical).
  28.XI.2001                                                     00.09
      Fixed "Add (,Position)" methods in "CSCPtrArray" and
      "CSCPtrArray" templates not to forget the last element.
      Fixed "RemoveAll" methods in "CSCPtrArray" and
      "CSCPtrArray" templates issues. They didn't clear ItemsQnt
      property to 0 (critical).
  25.XI.2001                                                     00.08
      Fixed "Add" method in "CSCPtrArray" template not to cross
      over the boundaries of an old array set while copying data.
  18.XI.2001                                                     00.07
      Fixed all Add, Remove methods in "CSCPtr.." family templates
      by avoiding "memmove" and "memcpy" functions usage.
  14.XI.2001                                                     00.06
      Fixed "Add (,)" method in "CSCPtrArray" class template.
  03.XI.2001                                                     00.05
      Added "CSCPtrArray" class, based on CSCPtrArray.
  30.IX.2001                                                     00.04
      Fixed "Remove" method in "CSCTypeArray" class template.
  23.VI.2001                                                     00.03
      Fixed "void Add ( ArgType Item, unsinged long& Position)" for
      cases when Position points to the n + 1 element.
  21.VI.2001                                                     00.02
      The returning value of the "Add" methods was changed from
      "void" to "int" and means the Position of the item
      inserted into array.
  ____________________________________________________________________
*/

#ifndef SCARRAY_H_2001
#define SCARRAY_H_2001

#include "Xception.h"

#define SCModuleName __FILE__

/*____________________________________________________________________

                                                                    /
    CSCAbstractPtrArray template class definition               \  /
                                                                 \/
  ____________________________________________________________________
*/

template <class ArgType>
class CSCAbstractPtrArray {
public:
    // Life-cylce
    CSCAbstractPtrArray() : Array(new ArgType*[16]),
                            ItemsQnt(0),
                            Size(16),
                            OwnsItems(true) {}
    ~CSCAbstractPtrArray() {
        RemoveAll();
    }
    // Interface
    CSCAbstractPtrArray<ArgType>&
    Add(ArgType* Item) {
        if (ItemsQnt < Size)
            Array[ItemsQnt] = Item;
        else {
            int newSize;
            if (Size)
                newSize = Size << 1;
            else
                newSize = 16;
            ArgType** newArray = new ArgType*[newSize];
            memcpy(newArray, Array, sizeof(ArgType*) * ItemsQnt);
            newArray[ItemsQnt] = Item;
            if (Array) delete[] Array;
            Array = newArray;
            Size = newSize;
        }
        ++ItemsQnt;
        return *this;
    }
    CSCAbstractPtrArray<ArgType>&
    Add(ArgType* Item, int Position) {
        if (Position < ItemsQnt) {
            if (ItemsQnt < Size) {
                memmove(Array + Position + 1, Array + Position, sizeof(ArgType*) * (ItemsQnt - Position));
                Array[Position] = Item;
            } else {
                int newSize = Size << 1;
                ArgType** newArray = new ArgType*[newSize];
                memcpy(newArray, Array, sizeof(ArgType*) * Position);
                newArray[Position] = Item;
                memcpy(newArray + Position + 1, Array + Position, sizeof(ArgType*) * (ItemsQnt - Position));
                if (Array) delete[] Array;
                Array = newArray;
                Size = newSize;
            }
            ++ItemsQnt;
            return *this;
        } else if (Position == 0 || Position == ItemsQnt)
            return Add(Item);
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "CSCAbstractPtrArray<ArgType>& Add ( ArgType * Item, int Position )", "CSCAbstractPtrArray", SCModuleName);
    }
    CSCAbstractPtrArray<ArgType>&
    AppendAndClean(CSCAbstractPtrArray<ArgType>& PtrArray) {
        if (0 < PtrArray.ItemsQnt) {
            if (&PtrArray != this) {
                if (ItemsQnt + PtrArray.ItemsQnt <= Size) {
                    memcpy(Array + ItemsQnt, PtrArray.Array, PtrArray.ItemsQnt * sizeof(ArgType*));
                    ItemsQnt += PtrArray.ItemsQnt;
                } else {
                    ArgType** newArray = new ArgType*[ItemsQnt + PtrArray.ItemsQnt];
                    memcpy(newArray, Array, ItemsQnt * sizeof(ArgType*));
                    memcpy(newArray + ItemsQnt, PtrArray.Array, PtrArray.ItemsQnt * sizeof(ArgType*));
                    if (Array) delete[] Array;
                    Array = newArray;
                    ItemsQnt += PtrArray.ItemsQnt;
                    Size = ItemsQnt;
                }
                delete[] PtrArray.Array;
                PtrArray.Array = NULL;
                PtrArray.Size = 0;
                PtrArray.ItemsQnt = 0;
            }
        }
        return *this;
    }
    ArgType*
    At(int Index) {
        return Array[Index];
    }
    const ArgType*
    At(int Index) const {
        return Array[Index];
    }
    int Count() const {
        return ItemsQnt;
    }
    CSCAbstractPtrArray<ArgType>&
    Exchange(int i, int j) {
        if (i != j) {
            if (i < ItemsQnt && j < ItemsQnt) {
                ArgType* p = Array[i];
                Array[i] = Array[j];
                Array[j] = p;
            } else
                throw new Xception(Xception::X_BOUNDARIES,
                                   String("Array indeces i=") + String::FromInt(i) + ",j=" + String::FromInt(j) + " are out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                                   "CSCAbstractPtrArray<ArgType> & Exchange ( int i, int j )", "CSCAbstractPtrArray", SCModuleName);
        }
        return *this;
    }
    CSCAbstractPtrArray<ArgType>&
    ForceNotToOwn() {
        OwnsItems = false;
        return *this;
    }
    ArgType*
    operator[](int Index) {
        if (Index < ItemsQnt)
            return Array[Index];
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Index) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "ArgType * operator[] ( int Index )", "CSCAbstractPtrArray", SCModuleName);
        return NULL;
    }
    const ArgType*
    operator[](int Index) const {
        if (Index < ItemsQnt)
            return Array[Index];
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Index) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "const ArgType * operator[] ( int Index ) const", "CSCAbstractPtrArray", SCModuleName);
        return NULL;
    }
    CSCAbstractPtrArray<ArgType>&
    Remove(int Position) {
        if (Position < ItemsQnt) {
            if ((Size >> 1) < ItemsQnt) {
                if (OwnsItems) delete (Array[Position]);
                memmove(Array + Position, Array + Position + 1, sizeof(ArgType*) * (ItemsQnt - Position - 1));
                --ItemsQnt;
            } else {
                // Have to release large unused buffer and acquire new buffer
                // of twicely smaller size.
                int newSize = Size >> 1;
                ArgType** newArray = new ArgType*[Size];
                memcpy(newArray, Array, sizeof(ArgType*) * Position);
                if (OwnsItems) delete (Array[Position]);
                memcpy(newArray + Position, Array + Position + 1, sizeof(ArgType*) * (ItemsQnt - Position - 1));
                if (Array) delete[] Array;
                Array = newArray;
                --ItemsQnt;
                Size = newSize;
            }
            return *this;
        } else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "CSCAbstractPtrArray<ArgType> & Remove ( int Position )", "CSCAbstractPtrArray", SCModuleName);
    }
    CSCAbstractPtrArray<ArgType>&
    RemoveAll() {
        if (0 < ItemsQnt) {
            if (OwnsItems)
                for (int Index = 0; Index < ItemsQnt; Index++)
                    delete (Array[Index]);
        }
        if (Array) {
            delete[] Array;
            Array = NULL;
        }
        Size = 0;
        ItemsQnt = 0;
        return *this;
    }
    CSCAbstractPtrArray<ArgType>&
    Replace(ArgType* Item, int Position) {
        if (Position < ItemsQnt) {
            if (OwnsItems) delete (Array[Position]);
            Array[Position] = Item;
            return *this;
        } else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "CSCAbstractPtrArray<ArgType> & Replace ( ArgType * Item, int Position )", "CSCAbstractPtrArray", SCModuleName);
    }

protected:
    // Attributes
    ArgType** Array;
    int ItemsQnt;
    int Size;
    bool OwnsItems;
};

/*____________________________________________________________________

                                                                    /
    CSCPtrArray template class definition                       \  /
                                                                 \/
  ____________________________________________________________________
*/

template <class ArgType>
class CSCPtrArray : public CSCAbstractPtrArray<ArgType> {
    typedef CSCAbstractPtrArray<ArgType> BASE;

public:
    // Life-cylce
    CSCPtrArray() : CSCAbstractPtrArray<ArgType>() {
    }
    CSCPtrArray(const CSCPtrArray<ArgType>& PtrArray) {
        CreateFrom(PtrArray);
    }
    // Interface
    CSCPtrArray<ArgType>&
    Append(CSCPtrArray<ArgType>& PtrArray) {
        if (0 < PtrArray.ItemsQnt) {
            if (BASE::ItemsQnt + PtrArray.ItemsQnt <= BASE::Size) {
                for (int i = 0; i < PtrArray.ItemsQnt; ++i)
                    BASE::Array[i + BASE::ItemsQnt] = new ArgType(*PtrArray.Array[i]);
                BASE::ItemsQnt += PtrArray.ItemsQnt;
            } else {
                ArgType** newArray = new ArgType*[BASE::ItemsQnt + PtrArray.ItemsQnt];
                memcpy(newArray, BASE::Array, BASE::ItemsQnt * sizeof(ArgType*));
                for (int i = 0; i < PtrArray.ItemsQnt; ++i)
                    newArray[i + BASE::ItemsQnt] = new ArgType(*PtrArray.Array[i]);
                if (BASE::Array) delete[] BASE::Array;
                BASE::Array = newArray;
                BASE::ItemsQnt += PtrArray.ItemsQnt;
                BASE::Size = BASE::ItemsQnt;
            }
        }
        return *this;
    }
    CSCPtrArray<ArgType>&
    CreateFrom(const CSCPtrArray<ArgType>& PtrArray) {
        if (this != &PtrArray) {
            if (PtrArray.ItemsQnt) {
                ArgType** newArray = new ArgType*[PtrArray.Size];
                for (int i = 0; i < PtrArray.ItemsQnt; ++i)
                    newArray[i] = new ArgType(*PtrArray[i]);
                BASE::RemoveAll();
                BASE::Array = newArray;
                BASE::ItemsQnt = PtrArray.ItemsQnt;
                BASE::OwnsItems = true;
                BASE::Size = PtrArray.Size;
            } else {
                BASE::Array = new ArgType*[16];
                BASE::ItemsQnt = 0;
                BASE::Size = 16;
                BASE::OwnsItems = true;
            }
        }
        return *this;
    }
    CSCPtrArray<ArgType>&
    operator+=(CSCPtrArray<ArgType>& PtrArray) {
        return Append(PtrArray);
    }
    CSCPtrArray<ArgType>&
    operator=(const CSCPtrArray& PtrArray) {
        return CreateFrom(PtrArray);
    }
};

/*____________________________________________________________________

                                                                    /
    CSCTypeArray template class definition                      \  /
                                                                 \/
  ____________________________________________________________________
*/

template <class ArgType>
class CSCTypeArray {
public:
    // Life-cycle
    CSCTypeArray() : Array(new ArgType[8]),
                     ItemsQnt(0),
                     Size(8) {}
    CSCTypeArray(const CSCTypeArray& TypeArray) : Array(NULL),
                                                  ItemsQnt(0) {
        if (0 < TypeArray.ItemsQnt) {
            Array = new ArgType[TypeArray.ItemsQnt];
            ItemsQnt = TypeArray.ItemsQnt;
            Size = ItemsQnt;
            for (int i = 0; i < ItemsQnt; ++i)
                Array[i] = TypeArray[i];
        }
    }
    ~CSCTypeArray(void) {
        RemoveAll();
    }
    // Operations
    CSCTypeArray<ArgType>&
    Add(const ArgType& Item) {
        if (ItemsQnt < Size)
            Array[ItemsQnt] = Item;
        else {
            int newSize;
            if (Size)
                newSize = Size << 1;
            else
                newSize = 8;
            ArgType* newArray = new ArgType[newSize];
            for (int i = 0; i < ItemsQnt; ++i)
                newArray[i] = Array[i];
            newArray[ItemsQnt] = Item;
            if (Array) delete[] Array;
            Array = newArray;
            Size = newSize;
        }
        ++ItemsQnt;
        return *this;
    }
    CSCTypeArray<ArgType>&
    Add(const ArgType& Item, int Position) {
        if (Position < ItemsQnt) {
            if (ItemsQnt < Size) {
                for (int Index = ItemsQnt; Position < Index; --Index)
                    Array[Index] = Array[Index - 1];
                Array[Position] = Item;
            } else

            {
                int i;
                int newSize = Size << 1;
                ArgType* newArray = new ArgType[newSize];
                for (i = 0; i < Position; ++i)
                    newArray[i] = Array[i];
                newArray[Position] = Item;
                for (i = Position + 1; i <= ItemsQnt; ++i)
                    newArray[i] = Array[i - 1];
                if (Array) delete[] Array;
                Array = newArray;
                Size = newSize;
            }
            ++ItemsQnt;
            return *this;
        } else if (Position == 0 || Position == ItemsQnt)
            return Add(Item);
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "CSCTypeArray<ArgType>&  Add ( const ArgType & Item, int Position )", "CSCTypeArray", SCModuleName);
    }
    CSCTypeArray<ArgType>&
    Append(const CSCTypeArray<ArgType>& TypeArray) {
        if (0 < TypeArray.ItemsQnt) {
            if (ItemsQnt + TypeArray.ItemsQnt <= Size) {
                for (int i = 0; i < TypeArray.ItemsQnt; ++i)
                    Array[ItemsQnt + i] = TypeArray.Array[i];
                ItemsQnt += TypeArray.ItemsQnt;
            } else {
                int i;
                ArgType* newArray = new ArgType[ItemsQnt + TypeArray.ItemsQnt];
                for (i = 0; i < ItemsQnt; ++i)
                    newArray[i] = Array[i];
                for (i = 0; i < TypeArray.ItemsQnt; ++i)
                    newArray[i + ItemsQnt] = TypeArray.Array[i];
                if (Array) delete[] Array;
                Array = newArray;
                ItemsQnt += TypeArray.ItemsQnt;
                Size = ItemsQnt;
            }
        }
        return *this;
    }
    ArgType& At(int Index) {
        return Array[Index];
    }
    const ArgType& At(int Index) const {
        return Array[Index];
    }
    CSCTypeArray<ArgType>&
    CreateFrom(const CSCTypeArray<ArgType>& TypeArray) {
        if (this != &TypeArray) {
            if (TypeArray.ItemsQnt) {
                ArgType* newArray = new ArgType[TypeArray.ItemsQnt];
                for (int i = 0; i < TypeArray.ItemsQnt; ++i)
                    newArray[i] = TypeArray.Array[i];
                if (Array) delete[] Array;
                Array = newArray;
                ItemsQnt = TypeArray.ItemsQnt;
                Size = TypeArray.Size;
            } else
                RemoveAll();
        }
        return *this;
    }
    int Count() const {
        return ItemsQnt;
    }
    ArgType& operator[](int Index) {
        if (Index < ItemsQnt)
            return Array[Index];
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Index) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "ArgType & operator[] ( int Index )", "CSCTypeArray", SCModuleName);
    }
    const ArgType& operator[](int Index) const {
        if (Index < ItemsQnt)
            return Array[Index];
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Index) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "const ArgType & operator[] ( int Index ) const", "CSCTypeArray", SCModuleName);
    }
    CSCTypeArray<ArgType>&
    operator=(const CSCTypeArray<ArgType>& TypeArray) {
        return CreateFrom(TypeArray);
    }
    CSCTypeArray<ArgType>&
    operator+=(const CSCTypeArray<ArgType>& TypeArray) {
        return Append(TypeArray);
    }
    CSCTypeArray<ArgType>&
    Remove(int Position) {
        if (Position < ItemsQnt) {
            if ((Size >> 1) < ItemsQnt) {
                --ItemsQnt;
                for (int Index = Position; Index < ItemsQnt; ++Index)
                    Array[Index] = Array[Index + 1];
            } else {
                // Have to release large unused buffer and acquire new buffer
                // of twicely smaller size.
                int i;
                int newSize = Size >> 1;
                ArgType* newArray = new ArgType[newSize];
                for (i = 0; i < Position; ++i)
                    newArray[i] = Array[i];
                for (i = Position; i < ItemsQnt - 1; ++i)
                    newArray[i] = Array[i + 1];
                if (Array) delete[] Array;
                Array = newArray;
                --ItemsQnt;
                Size = newSize;
            }
            return *this;
        } else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(ItemsQnt - 1) + ")",
                               "CSCTypeArray<ArgType> & Remove ( int Position )", "CSCTypeArray", SCModuleName);
    }
    CSCTypeArray<ArgType>&
    RemoveAll(void) {
        if (Array) {
            if (Array) delete[] Array;
            Array = NULL;
        }
        ItemsQnt = 0;
        Size = 0;
        return *this;
    }

protected:
    // Attributes
    ArgType* Array;
    int ItemsQnt;
    int Size;
};

/*____________________________________________________________________

                                                                    /
    CSCDictionary template class definition                     \  /
                                                                 \/
  ____________________________________________________________________
*/

template <class ArgType>
class CSCDictionary {
public:
    // Life-cycle
    CSCDictionary() : OwnsItems(true),
                      Sorted(true) {}
    ~CSCDictionary() {
        RemoveAll();
    }

    // Interface
    CSCDictionary<ArgType>&
    Add(const String& Key, ArgType* Item) {
        int Position;

        if (Item) {
            IndexItem* pIndex = new IndexItem;
            pIndex->Key = Key;
            pIndex->Item = Item;
            if (Sorted) {
                if (!FindKey(Key, Position))
                    Index.Add(pIndex, Position);
            } else
                Index.Add(pIndex);
        }
        return *this;
    }
    CSCDictionary<ArgType>&
    Add(const String& Key, ArgType* Item, int Position) {
        if (Position <= Index.Count()) {
            IndexItem* pIndex = new IndexItem;
            pIndex->Key = Key;
            pIndex->Item = Item;
            Index.Add(pIndex, Position);
        } else
            throw new Xception(Xception::X_BOUNDARIES,
                               String("Array index ") + String::FromInt(Position) + " is out of bounds (0-" + String::FromInt(Index.Count() - 1) + ")",
                               "CSCDictionary<ArgType> & Add ( const String & Key, ArgType * Item, int Position )", "CSCDictionary", SCModuleName);
        return *this;
    }
    int Count() const { return Index.Count(); };
    ArgType* At(int Position) const {
        return Index.At(Position)->Item;
    }
    ArgType* At(const String& Key) const {
        int Position;

        if (FindKey(Key, Position))
            return Index[Position]->Item;
        return NULL;
    }
    bool FindKey(const String& Key, int& Position) const {
        int RightBound, LeftBound, Compare;
        bool Found = false;

        if (0 < Index.Count()) {
            Position = LeftBound = 0;
            RightBound = Index.Count() - 1;
            while (0 <= RightBound - LeftBound) {
                Position = (LeftBound + RightBound) >> 1;
                if ((Found = (Compare = Key.Compare(Index.At(Position)->Key)) == 0)) break;
                if (0 < Compare)
                    LeftBound = Position + 1;
                else
                    RightBound = Position - 1;
            }
            if (0 < Compare) ++Position;
        } else
            Position = 0;
        return Found;
    }
    void ForceNotToOwn() { OwnsItems = false; }
    void ForceNotToSort() { Sorted = false; }
    String KeyOf(int Position) const { return Index[Position]->Key; }
    ArgType* operator[](const String& Key) const { return At(Key); }
    ArgType* operator[](int Position) const { return Index[Position]->Item; }
    void Remove(const String& Key) {
        int Position;

        if (FindKey(Key, Position))
            Remove(Position);
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               "Key " + Key + " is not found in the dictionary",
                               "void Remove ( const String & Key )", "CSCDictionary", SCModuleName);
    }
    void Remove(int Position) {
        if (OwnsItems) delete (Index[Position]->Item);
        Index.Remove(Position);
    }
    void RemoveAll() {
        if (OwnsItems)
            for (int i = 0; i < Index.Count(); ++i)
                delete (Index.At(i)->Item);
        Index.RemoveAll();
    }
    void Replace(ArgType* Item, const String& Key) {
        int Position;
        if (FindKey(Key, Position)) {
            if (OwnsItems) delete (Index[Position]->Item);
            Index.At(Position)->Item = Item;
        } else
            throw new Xception(Xception::X_BOUNDARIES,
                               "Key " + Key + " is not found in the dictionary",
                               "void Replace ( ArgType * Item, const String & Key )", "CSCDictionary", SCModuleName);
    }
    void Replace(ArgType* Item, int Position) {
        if (OwnsItems) delete (Index[Position]->Item);
        Index[Position]->Item = Item;
    }
    void Sort() {
        int Count(Index.Count());
        if (1 < Count)
            QSort(0, Count - 1);
        Sorted = true;
    }

protected:
    // Index definition
    struct IndexItem {
        String Key;
        ArgType* Item;
    };

    // Operations
    void QSort(int Left, int Right);

    // Attributes
    bool OwnsItems;
    bool Sorted;
    CSCAbstractPtrArray<IndexItem> Index;
};

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    QSort                                                     |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

template <class ArgType>
void CSCDictionary<ArgType>::QSort(int Left, int Right) {
    int i, j;
    IndexItem* p;

    do {
        i = Left;
        j = Right;
        p = Index[(i + j) >> 1];
        do {
            while (Index.At(i)->Key < p->Key) ++i;
            while (p->Key < Index.At(j)->Key) --j;
            if (i <= j) {
                Index.Exchange(i, j);
                ++i;
                --j;
            }
        } while (i <= j);
        if (Left < j) QSort(Left, j);
        Left = i;
    } while (i < Right);
}

/*____________________________________________________________________

                                                                    /
    CSCSet template class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

template <class ArgType, class CollectorArray>
class CSCSet {
public:
    // Life-cycle
    CSCSet() {}
    CSCSet(const ArgType& Item) {
        Add(Item);
    }
    ~CSCSet() {}
    // Interface
    int Add(const ArgType& Item) {
        int Position;

        if (!Find(Item, Position)) {
            Set.Add(Item, Position);
            return Position;
        }
        return -1;
    }
    ArgType& At(int Position) const {
        return Set.At[Position];
    }
    CSCSet<ArgType, CollectorArray>&
    CreateFrom(const CSCSet<ArgType, CollectorArray>& aSet) {
        Set.CreateFrom(aSet.Set);
        return *this;
    }
    int Count() const {
        return Set.Count();
    }
    bool Find(const ArgType& Item, int& Position) const {
        int RightBound, LeftBound;
        bool Found = false;

        if (0 < Set.Count()) {
            Position = LeftBound = 0;
            RightBound = Set.Count() - 1;
            if (Item < Set[LeftBound])
                Position = LeftBound;
            else if (Set[RightBound] < Item)
                Position = RightBound + 1;
            else if (Set[LeftBound] == Item) {
                Position = LeftBound;
                Found = true;
            } else if (Set[RightBound] == Item) {
                Position = RightBound;
                Found = true;
            }
            // Fall in by dividing the interval on two halfs
            else if (!Found && RightBound - LeftBound == 1) {
                Position = RightBound;
            } else
                while (!Found && RightBound - LeftBound > 1) {
                    Position = (LeftBound + RightBound) >> 1;
                    if (Set[Position] == Item)
                        Found = true;
                    else {
                        // Define what the subinterval to fall in
                        if (Set[Position] < Item) {
                            LeftBound = Position;
                            if (RightBound - Position <= 1)
                                // Store the position where to insert a new element
                                Position = RightBound;
                        } else
                            RightBound = Position;
                    }
                }
        } else
            Position = 0;
        return Found;
    }
    void Remove(const ArgType& Item) {
        int Position;

        if (Find(Item, Position))
            Remove(Position);
        else
            throw new Xception(Xception::X_BOUNDARIES,
                               "Item is not found in the set",
                               "void Remove ( const ArgType & Item )", "CSCSet", SCModuleName);
    }
    void Remove(int Position) {
        Set.Remove(Position);
    }
    void RemoveAll() {
        Set.RemoveAll();
    }
    CSCSet<ArgType, CollectorArray>&
    operator,
        (const ArgType& Item) {
        Add(Item);
        return *this;
    }
    CSCSet<ArgType, CollectorArray>&
    operator=(const CSCSet<ArgType, CollectorArray>& aSet) {
        return CreateFrom(aSet);
    }
    bool operator>(const ArgType& Item) {
        int pos;
        return Find(Item, pos);
    }
    bool operator>(const CSCSet<ArgType, CollectorArray>& aSet) const {
        if (this != &aSet) {
            int i(0), pos;
            bool Result(Set.Count() >= aSet.Count());

            while (Result && i < aSet.Count())
                if (!Find(aSet[i], pos))
                    Result = false;
                else
                    ++i;
            return Result;
        } else
            return true;
    }
    bool operator==(const CSCSet<ArgType, CollectorArray>& aSet) const {
        if (this != &aSet) {
            int i(0);
            bool Result(Set.Count() != aSet.Count());

            while (Result && i < aSet.Count())
                if (aSet.Set[i] != Set[i])
                    Result = false;
                else
                    ++i;
            return Result;
        } else
            return true;
    }
    ArgType operator[](int Position) const {
        return Set[Position];
    }

protected:
    // Attributes
    CollectorArray Set;
};

typedef CSCTypeArray<String> Strings;
typedef CSCTypeArray<int> NumberList;

#endif
