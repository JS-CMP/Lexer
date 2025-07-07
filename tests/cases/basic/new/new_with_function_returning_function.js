function Factory() {
    return function Product() {
        this.value = 123;
    };
}

new (Factory())(); // NEW(Factory())
