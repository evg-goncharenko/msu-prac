## Homework by 03.30

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [float_pair](./float_pair.cpp): </b> <br>

The abstract class `FloatPair` is described, which represents a pair of float numbers and has a pure virtual function for calculating a certain measure `virtual float Measure() const;`
<br> <br> 

The `Segment` class is inherited from `FloatPair`, which defines the concept of a segment of a numeric line (that is, the numbers specified by the base class are considered as the start and end points of the segment; the `Segment` class doesn't have its own data fields). When trying to construct a segment whose starting point is to the right of the end, an exception of the `BadSegment` type is thrown (the class is empty). The `Measure()` function is defined as the length of the segment.
<br> <br> 

An addition operation is introduced in the class, which builds a new segment containing all the points of the original segments (union) using two segments that completely or partially overlap. If the segments do not have common points, their union will not be a segment; in this case, an exception of the `BadAddition` class is thrown, the object of which contains the boundaries of both segments that couldn't be folded.
