function testDateNow() {
    // Accessing global.Date for the first time will change the global shape,
    // so do it before the loop starts; otherwise we have to loop an extra time
    // to pick things up.
    var time = Date.now();
    for (var j = 0; j < RUNLOOP; ++j)
        time = Date.now();
    return "ok";
}
assertEq(testDateNow(), "ok");
checkStats({
    recorderStarted: 1,
    recorderAborted: 0,
    traceTriggered: 1
});
