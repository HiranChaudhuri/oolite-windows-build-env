function remove(k, L) {
  for (var i in k) {
    if (i == L)
      k.splice(L, 1);
  }
}
function f(k) {
  var L = 0;
  for (var i in k) {
    if (L == 1)
      remove(k, L);
    L++;
    assertEq(k[i], 3);
  }
  assertEq(L, 6);
}

var a = [3, 3, 3, 3, 3, 3, 3];
f(a);
