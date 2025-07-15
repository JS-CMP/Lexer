var x = "component";

var MyNS = {};
MyNS.Widget = function () {
    this.init = function (x) {
        this.x = x;
        return this;
    };
    this.render = function () {
        return "Rendered " + this.x;
    };
    return this;
};

console.log(new MyNS.Widget().init(x).render()); // NEW(MyNS.Widget).init(x).render();
