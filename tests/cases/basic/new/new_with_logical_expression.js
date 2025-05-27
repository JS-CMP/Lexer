function A() { this.name = "A"; }
function B() { this.name = "B"; }

var useA = false;
new (useA && A || B)(); // NEW((useA && A || B));
