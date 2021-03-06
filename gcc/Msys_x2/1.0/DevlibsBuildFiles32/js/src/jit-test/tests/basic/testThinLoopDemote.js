function testThinLoopDemote() {
    function f()
    {
        var k = 1;
        for (var n = 0; n < 4; n++) {
            k = (k * 10);
        }
        return k;
    }
    f();
    return f();
}
assertEq(testThinLoopDemote(), 10000);
checkStats({
    recorderStarted: 1,
    recorderAborted: 0,
    traceCompleted: 1,
    traceTriggered: 2,
    unstableLoopVariable: 0
});
