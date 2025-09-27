try {
    console.log("Outer try");
    try {
        throw "Inner error";
    } catch (e) {
        console.log("Caught inner:", e);
    }
} catch (e) {
    console.log("Caught outer:", e);
}
