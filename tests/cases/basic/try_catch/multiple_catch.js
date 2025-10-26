var x = 2
try {
    if (x === 1) {
        throw "string error"
    }
    if (x === 2) {
        throw 42
    }
    if (x === 3) {
        throw { msg: "object error" }
    }
} catch (e) {
    console.log("Caught:", e)
}
