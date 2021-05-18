var a = 11;
var e = 3;
var b = 2;
var s = "abc";
var d = 6;
var i = 0;
a = b = 25;

write("a: ");
write(a);
write("b: ");
write(b);

1000-7;

var p = true;
for (a = 0; a < 10; a = a + 1) {
    for (i = 0; i < 3; i = i + 1) {
        write("i");
    }
    a = a + 1;
    write("loop iteration");
    if (a == 5) {
        break;
    }
}

if (b == 3) {
    b = 2;
} else {
    b++;
}

write("b: ");
write(b);
write("s: ");
write(s);
b++;
write("b: ");
write(b);

s = s + "def";
write("s: ");
write(s);

// write(a, b);