function Foo() {
    return function() {
        return 99;
    };
}

new Foo()(); // NEW(Foo)()
