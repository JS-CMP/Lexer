for (;;) {
    console.log("loop");
    myLabel: {
        console.log("start");
        break myLabel;
        console.log("end");
    }
}

