function Foo() {
    return function() {
        return 99;
    };
}

console.log(new Foo()()); // NEW(Foo)()
