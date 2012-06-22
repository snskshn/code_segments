#include <stdio.h>
/* $begin func */
int f();

int func1() {
    return f() + f() + f() + f();
}

int func2() {
    return 4*f();
}
/* $end func */

/* $begin f */
int counter = 0;

int f() {
    return counter++;
}
/* $end f */

/* $begin func1in */
/* Result of inlining f in func1 */
int func1in() {
    int t = counter++;  /* +0 */
    t += counter++;     /* +1 */
    t += counter++;     /* +2 */
    t += counter++;     /* +3 */
    return t;
}
/* $end func1in */

/* $begin func1opt */
/* Optimization of inlined code */
int func1opt() {
    int t = 4 * counter + 6;
    counter = t + 4;
    return t;
}
/* $end func1opt */

int main(int argc, char *argv[]) {
  counter = 0; printf("func1 -> %d\n", func1());
  counter = 0; printf("func2 -> %d\n", func2());
  counter = 0; printf("func1in -> %d\n", func1in());
  counter = 0; printf("func1opt -> %d\n", func1opt());
  return 0;
}
