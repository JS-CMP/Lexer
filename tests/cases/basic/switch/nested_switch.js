
var x = 1;
var y = 10;

switch (x) {
    case 1:
        switch (y) {
            case 10:
                console.log("nested");
                break;
        }
        break;
    default:
        console.log("outer default");
}

