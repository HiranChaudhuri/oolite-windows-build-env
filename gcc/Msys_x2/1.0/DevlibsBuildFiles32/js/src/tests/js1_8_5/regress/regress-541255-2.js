/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributors: Gary Kwong and Jason Orendorff
 */

function f(e) {
    eval("[function () { w.r = 0 }() for (w in [0])]")
}
f(0);
reportCompare(0, 0, "");
