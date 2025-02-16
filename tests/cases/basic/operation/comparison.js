var a = 5 == "5";
var b = 5 != "5";
var c = 5 > 3;
var d = 5 < 3;
"var e = 5 === \"5\"; // Not supported by the lib and the lexer";
"var f = 5 !== \"5\"; // Not supported by the lib and the lexer";
console.log(a, b, c, d);
