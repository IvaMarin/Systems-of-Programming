/*  IAM   */
#include "mlisp.h"
extern double a/*2*/;
	 extern double b/*2*/;
	 double fun/*3*/ (double);
	 double golden__section__search/*7*/ (double, double);
	 double golden__start/*15*/ (double, double);
	 extern double mphi/*24*/;
	 double __IAM__try/*25*/ (double, double
	 , double, double
	 , double, double);
	 bool close__enough_Q/*45*/ (double x, double y);
	 extern double tolerance/*48*/;
	 extern double total__iterations/*49*/;
	 extern double xmin/*50*/;
	 //________________ 
double a/*2*/ = -4.;
	 
double b/*2*/ = -2.5;
	 
double fun/*3*/ (double x){
 x = (x - (20. / 21.));
	 return
 (((- x) + (5. * expt(log(expt(atan((x + 3.)), 2.)
	 ), 4.)
	 )) - 8.);
	 }

double golden__section__search/*7*/ (double a, double b){
 {
 double xmin(((b > a)
	? golden__start(a, b)
	 
	: (golden__start(b, a)
	 )));
	 newline();
	 return
 xmin;
	 }
}

double golden__start/*15*/ (double a, double b){
 total__iterations = 0.;
	 {
 double xa((a + (mphi * (b - a)))),
	 xb((b + (- (mphi * (b - a)))));
	 return
 __IAM__try(a, b
	 , xa, fun(xa)
	 , xb, fun(xb))
	 ;
	 }
}

double mphi/*24*/ = ((3. - sqrt(5.)) * (1. / 2.0));
	 
double __IAM__try/*25*/ (double a, double b
	 , double xa, double ya
	 , double xb, double yb){
 return
 (close__enough_Q(a, b)
	? ((a + b) * 0.5)
	: (display("+"),
	 total__iterations = (total__iterations + 1.),
	 ((yb > ya)
	? b = xb,
	 xb = xa,
	 yb = ya,
	 xa = (a + (mphi * (b - a))),
	 __IAM__try(a, b
	 , xa, fun(xa)
	 , xb, yb)
	 
	: (a = xa,
	 xa = xb,
	 ya = yb,
	 xb = (b - (mphi * (b - a))),
	 __IAM__try(a, b
	 , xa, ya
	 , xb, fun(xb))
	 ))));
	 }

bool close__enough_Q/*45*/ (double x, double y){
 return (tolerance > abs((x - y)));
	 }

double tolerance/*48*/ = 0.001;
	 
double total__iterations/*49*/ = 0.;
	 
double xmin/*50*/ = 0.;
	 int main(){
 xmin = golden__section__search(a, b)
	 ;
	 display("Interval=\t[");
	 display(a);
	 display(" , ");
	 display(b);
	 display("]\n");
	 display("Total number of iteranions=");
	 display(total__iterations);
	 newline();
	 display("xmin=\t\t");
	 display(xmin);
	 newline();
	 display("f(xmin)=\t");
	 display(fun(xmin));
	 newline();
	  std::cin.get();
	 return 0;
	 }

