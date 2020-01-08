#ifndef _VLINE
#define _VLINE

#include "curvebase.h"

//abstract class for vertical lines where x-coordinate is 
//a function of the y-coordinate

class Vertline : public Curvebase {
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
		virtual double xfunc(double p) = 0;
    //derivative of x-coordinate as function of y-coordinate
		virtual double xfuncd(double p) = 0;

	public:
		Vertline(double a, double b, bool dir) : Curvebase(a,b, dir) {}; 
		Vertline(); //default constructor
		~Vertline(); //default destructor
};

#endif
