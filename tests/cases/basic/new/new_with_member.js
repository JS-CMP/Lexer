function Bar() {
    this.x = 42;
    this.get = function () { return this.x; };
}


console.log(new Bar().get()); // NEW(Bar).get();
