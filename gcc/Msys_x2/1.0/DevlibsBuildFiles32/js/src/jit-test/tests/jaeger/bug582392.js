function cmp(x, y) {
  if (x < y)
    return -1;
  if (x > y)
    return 1;
  return 0;
}
assertEq(cmp('true', 'false'), 1);
