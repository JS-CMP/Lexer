outer: for (var i = 0; i < 3; i++) {
    inner: for (var j = 0; j < 3; j++) {
        if (i === 1 && j === 1) {
            break outer;
        }
        console.log(i, j);
    }
}

