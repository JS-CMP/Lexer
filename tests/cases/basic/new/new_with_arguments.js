function Greeter(name) {
    this.greet = function () {
        return "Hello"
    }
}

console.log(new Greeter("World").greet()) // NEW(Greeter, "World").greet()
