function b(depth) {
    a(depth);
}

function a(depth) {
    if (depth == 0) {
        console.log('depth is 0');
        return 0;
    }
    return b(depth - 1);
}

a(3);

