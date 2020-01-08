#ifndef _HLINE
#define _HLINE

#include "curvebase.h"

//abstract class for hertical lines where x-coordinate is 
//a function of the y-coordinate

class Horzline : public Curvebase {
	protected:

    //x as function of parametrization variable
    double xp(double p);
    //y as function of parametrization variable
    double yp(double p);
    //derivative of x as function of parametrization variable
    double dxp(double p);
    //derivative of x as function of parametrization variable
    double dyp(double p);
    //derivative of x-coordinate as function of y-coordinate
		virtual double yfunc(double p) = 0;
    //derivative of x-coordinate as function of y-coordinate
		virtual double yfuncd(double p) = 0;
	public:
  //use same constructor as for Curvebase
		Horzline(double a, double b, bool dir) : Curvebase(a,b,dir) {}; 
		Horzline(); //default constructor
		~Horzline();//default destructor
};
#endif
