var a = {
    a: 1,
    b: 2,
    c: 3,
}
var b = {
    a: 1,
    b: null,
}

for (b in a) {
    console.log(b.a, a[b.a])
}