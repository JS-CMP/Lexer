function Arr(x, y) {
    this.sum = x + y;
}

new Arr([1, 2][0], [3, 4][1]); // NEW(Arr, [1, 2][0], [3, 4][1])
