function Outer() {
    this.Foo = function () {
        return 123;
    };
}

console.log(new new Outer().Foo()); // NEW(NEW(Outer).Foo)
