function Bar() {
    this.x = 42;
}
Bar.prototype.get = function () { return this.x; };

new Bar().get(); // NEW(Bar).get();
