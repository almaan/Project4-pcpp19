#ifndef _VLINE
#define _VLINE

#include "curvebase.h"

//abstract class for vertical lines where x-coordinate is 
//a function of the y-coordinate

class Vertline : public Curvebase {
	protected:

		double xp(double p);//x as function of parametrization variable
		double yp(double p);//y as function of parametrization variable
		double dxp(double p);//derivative of x as function of parametrization variable
		double dyp(double p);//derivative of x as function of parametrization variable

		virtual double xfunc(double p) = 0;//derivative of x-coordinate as function of y-coordinate
		virtual double xfuncd(double p) = 0;//derivative of x-coordinate as function of y-coordinate

	public:
		Vertline(double a, double b, bool dir) : Curvebase(a,b, dir) {}; //use same constructor as for curvebase
		Vertline(); //default constructor
		~Vertline(); //default destructor
};

#endif