## Preparing for the first test:

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [v1_1](./v1_1.cpp): </b><br>
```
Describe the set_min() function, which takes two parameters-variables of type int, determines
which of the two variables has the value less, and puts this value in both variables.
The function call will be executed with the variable names specified as parameters without any
additional operations (taking the address, etc.).
```

<br> <b> [v1_2](./v1_2.cpp): </b><br>
```
Describe the Flag class, the object of which can be in one of two states - "cocked" (on) and
"reset" (off). Provide the SetOn and SetOff methods that set the flag to the appropriate
position, as well as the IsOn method that returns a boolean value (true if the object is
in the "cocked" state, false otherwise), which should allow operation for constant objects.
```

<br> <b> [v1_3](./v1_3.cpp): </b><br>
```
Modify the Flag class from the previous task so that its objects (without explicitly applying
any additional operations to them) can be used as a boolean value (conditional expression),
for example, in the if, while, etc. statements. This should also work for constant objects.
```

<br> <b> [v1_4](./v1_4.cpp): </b><br>
```
Describe the Watched class, which allows you to create an object without specifying parameters.
Implement the Amount method in this class so that you can call it without an object by specifying
the class name. The Amount method must return an integer corresponding to the number of currently
existing objects of the Counted class. Don't forget about the case of creating an object as
a copy of another object!
```

<br> <b> [v1_5](./v1_5.cpp): </b><br>
```
Describe classes A and B so that (assuming that a and b are objects of Class A and Class B):
• Expressions of the form a[b] and b[a] were equal to the number 75
(hereafter, int numbers are meant);
• Expression a() was equal to the number 50, expression a (b) was equal to the number 100,
expression a (b, b) was equal to the number 150, expression b (a) was equal to the number 200;
• The expression a (a,a,a) was successfully compiled as an integer, but when trying to calculate
threw an exception of type A;
• Expression --a was equal to the number 500, expression a-- was equal to the number 501.
```

<br> <b> [v1_6](./v1_6.cpp): </b><br>
```
Describe the class D and (if necessary) additional classes or structures so that
(provided that d is an object of class D) the expression "d->1" is an integer variable that
can be assigned values, which stores the assigned value and from which this value can be
extracted as from a normal variable; if the program describes more than one object of class D,
each such object must correspond to its own integer variable, accessible via the "->f" operation.
that is, such variables should not conflict. An object of class D must be created without
specifying parameters. Any dynamic memory must be released correctly.
```
    
<br> <b> [v1_7](./v1_7.cpp): </b><br>
```  
Describe the IntQueue class, which represents the concept of a "queue of integers", implemented
through a singly linked list with pointers to the beginning and end (do not use the features of
the C++ standard library!). Provide the operations "<<=" (the left operand is an object of
your class, the right is an integer expression, the operation adds an element to the end
of the queue) and ">>=" (the right operand is an int variable, the operation extracts an element
from the beginning of the queue; if there is nothing to extract, throw an exception of
the QueueEmpty class, which you describe yourself). Provide an Amount method that returns
the current number of items in the queue. All allocated dynamic memory must be properly freed -
the memory from the extracted elements is freed when they are extracted, the memory from
everything else is freed when the queue object is destroyed.
```

<br> <b> [v1_8](./v1_8.cpp): </b><br>
```
Modify the IntQueue class from the previous task by providing it with a virtual Check() function
that accepts an int parameter and returns a Boolean value; the function must allow operation for
constant objects. In the IntQueue class, the Check function should always return "true", but
it is assumed that the inheriting classes can replace this function with their own version of it.
When adding a new number to the queue, this number should be checked by calling the Check()
function, and if the function returned true, the number should be placed in the queue, otherwise
it should be ignored. Inherit from the IntQueue class the EvenFilter class, which accepts only
even numbers for queuing. This class should differ from the base class only by the Check()
function. Note that during testing, other descendants will also inherit from the class.
```

<br> <b> [v2_1](./v2_1.cpp): </b><br>
```
Describe class A, and in it an open function f with an empty list of parameters that returns
an integer and allows calling without an object, those with the class name specified.
The function must return zero the first time it is called, and each time it is called,
it must return a number one greater than the previous one.
```

<br> <b> [v2_2](./v2_2.cpp): </b><br>
```
Describe the Functor class in such a way that (provided that f is an object of this class)
the expression "f(x,y)" was equal to the sum of the numbers x and y. The expression should also
work for constant objects.
```
    
<br> <b> [v2_3](./v2_3.cpp): </b><br>
```
Describe the PseudoArray class, which is a "pseudo-array" of integers. An indexing operation
must be applied to objects of the class, the argument of which, having the type int, must be
ignored, that is, it must give the impression that there are as many elements in the array as
there are different int values, and all elements are equal to the same number. The initial value
of all "elements" is zero. The value is changed by assignment, as for regular arrays. Extracting
the value of elements should also work for constant objects.
```
    
<br> <b> [v2_4](./v2_4.cpp): </b><br>
```
Describe the Smart class and, if necessary, additional classes in such a way that (if s is
an object of the Smart class, possibly of constant type, the expression "t = s[x]", where t
is an integer variable and x is an integer expression, puts in t a value greater than x by one,
and an expression of the form "t = s[x][y]" puts in the product of the numbers x and y.
```
    
<br> <b> [v3_1](./v3_1.cpp): </b><br>
```
Create the Assign class (and, if necessary, additional classes) so that an expression of
the form "a->f(x,y)" (where a is an object of the class assigns the integer variable x
the value y, while the assigned value is stored in the object and can be obtained by
the Get() method.
```

<br> <b> [v3_2](./v3_2.cpp): </b><br>
```
Modify the Assign class from the previous task so that you can assign a new value to a variable
of any type in this way, as long as there is an assignment operation for it in principle
(including for variables whose type prohibits copying, but allows assignment). In this version,
you do not need to implement value storage and the Get() method.
```

<br> <b> [v3_3](./v3_3.cpp): </b><br>
```  
Describe the class M and (if necessary) additional classes or structures so that (provided that
m is an object of class M) the expression "m(m)" is an integer variable that can be assigned
values, which stores the assigned value and from which the value can be extracted as from
a normal variable; if the program describes more than one object of class M, each such object
must correspond to its own integer variable, accessible via an operation of the form "x(x)". 
In this case, an expression of the form "m(x)", where x is an object of the same class other
than m, should always return zero, and assigning any integer value to such an expression should
not change anything. An object of the class must be created without specifying parameters.
Remember that compiling with the "-Wall" flag should not cause warnings!
```

<br> <b> [v3_4](./v3_4.cpp): </b><br>
```
Describe the SumHold class for storing the sum of integers. Adding a number to the sum must be
performed by the "<<" operation, and this operation must be described in such a way that it is
possible to write "chains" like "sh << 20 << 30 << 40 << 50", where sh is an object of the SumHold
class; such a "chain" must sequentially add numbers to the sum from left to right. Provide a
Get() method that returns the current sum value for this object. You can assume that the number
of int type will be sufficient.
```
    
<br> <b> [v3_5](./v3_5.cpp): </b><br>
```
Add the GetAll() method to the SumHold class from the previous task, which returns the sum of
all the numbers added to all the objects of the SumHold class since the start of the program.
If necessary, enter additional fields and / or methods.
```
