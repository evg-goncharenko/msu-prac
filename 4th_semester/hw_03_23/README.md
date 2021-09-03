## Homework by 03.23

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [special_integers](./special_integers.cpp): </b><br>
    
`SpecialInts` class, represents the concept of "ordered set of integers", implemented via an array with a variable length (the length isn't known in advance, it can change and is unlimited from above). <br>
The `,` (`operator , `) operation is defined for this class, which adds another number to the set and returns a reference to the object for which it was called, so that can call several such operations in a row. <br>
An indexing operation (`operator []`) is also defined for this class to extract the specified element of the set and the `len()` method that outputs the current length of the set. When accessing the elements of the set through the indexing operation, the correctness of the specified index is checked. If it is less than zero, or, conversely, too large, that is, there is no in the set an element with the specified number, then an exception of the `BadRange` class is thrown. <br>
The `get()` method of this class returns an invalid index value that caused the error. <br>
The main program has been written, which demonstrates the correctness of the written class. <br>
All allocated dynamic memory is correctly released.
