try {
    try {
        throw "inner error"
    } catch (e) {
        console.log("Caught inner:", e)
        throw e
    }
} catch (e) {
    console.log("Caught outer:", e)
}
