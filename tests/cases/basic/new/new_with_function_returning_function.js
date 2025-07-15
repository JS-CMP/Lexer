function Factory() {
    return function Product() {
        this.value = 123;
    };
}

console.log(new (Factory())()); // NEW(Factory())
