/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor: Bob Clary
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 424954;
var summary = 'Do not crash with [].concat(null)';
var actual = 'No Crash';
var expect = 'No Crash';


//-----------------------------------------------------------------------------
test();
//-----------------------------------------------------------------------------

function test()
{
  enterFunc ('test');
  printBugNumber(BUGNUMBER);
  printStatus (summary);
 
  [].concat(null);

  reportCompare(expect, actual, summary);

  exitFunc ('test');
}
