//set.cpp
#include "mlisp.h"
extern double a;
double g(double x, double a);

double a = 7.;
double g(double x, double a){
  a = 5.;
  return x + a;
}
int main(){
  display(g(1., 0.));newline();
  display(a);newline();
  a = 8.; //не обернут в display(), т.к. lisp не печатает set!
  display(a);newline();
std::cin.get();
 return 0;
}
