function Outer() {
    this.Foo = function () {
        return 123;
    };
}

new new Outer().Foo(); // NEW(NEW(Outer).Foo)
