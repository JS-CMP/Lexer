function Foo() {
    return function () {
        console.log("Inside Foo")
        return "Hello, World!"
    }
}
var x = 123

new new Foo() // NEW(NEW(Foo))
new (new Foo)(x) // NEW(NEW(Foo), x)
console.log(new new Foo(x)(x)) // NEW(NEW(Foo, x), x)
