//easylet2.cpp
#include "mlisp.h"
extern double a;
extern double b;
double a = 7.;
double b = 8.;

int main(){
//#2
//cкобки easylet отсутствубт в if: приоритет оператора , ниже условно оператора 
 display(
  (a<b ? a
       :/*(*/display("b="),display(b),newline(),b/*)*/
  )
        );newline();

 std::cin.get();
 return 0;
}
