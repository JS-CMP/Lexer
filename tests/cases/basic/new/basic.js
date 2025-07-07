function Foo(a, b, c) {
    this.a = a;
    this.b = b;
    this.c = c;
}
var a = 1, b = 2, c = 3;

new Foo; // NEW(Foo)
new Foo(); //NEW(Foo)
new Foo(a, b, c); // NEW(Foo, a, b, c)