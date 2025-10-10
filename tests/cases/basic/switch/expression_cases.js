var x = 10;
var y = 20;
var a = 30;
var b = 0;
function func() {
    return 40;
}

switch (x + y) {
    case a + b:
        result = 1;
        break;
    case func():
        result = 2;
        break;
    default:
        result = 0;
}

