function Foo() {
    this.value = 10
}

console.log(new (0, Foo)()) // NEW((0, Foo))
