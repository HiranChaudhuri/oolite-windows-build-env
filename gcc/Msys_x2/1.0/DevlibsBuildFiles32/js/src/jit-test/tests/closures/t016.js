actual = '';
expected = '0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,';

function g(a) {
  for (var i = 0; i < 3; ++i) {
    a();
  }
}

function f(y) {
  var q;
  for (var i = 0; i < 7; ++i) {
    q = function() { appendToActual(y); };
  }
  g(q);
}

for (var i = 0; i < 5; ++i) {
  f(i);
 }


assertEq(actual, expected)
