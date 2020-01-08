#include <iostream>
#include <cmath>
#include "curvebase.h"


//constructor 
Curvebase::Curvebase(double p_min,
					 double p_max,
					 bool dir,
					 double tolerance) {

	pmin = p_min;
	pmax = p_max;
	mid = (p_min + p_max) / 2.0;
	rev = dir;
	tol = tolerance;;
};

void Curvebase::setLength(void){
	length = fabs(integrate(pmax,pmin));
}

//default constructor
Curvebase::Curvebase(){};

//default destructor
Curvebase::~Curvebase(){};

//copy constructor
Curvebase::Curvebase(const Curvebase &cb) {
	pmin = cb.pmin;
	pmax = cb.pmax;
	mid = cb.mid;
	rev = cb.rev;
	tol = cb.tol;
};


//Simpson Adaptive Integration
//Uses Recursion 
double Curvebase::integrate(double p, double lower){

	double errest;
	double integral1, integral2;

	
	integral1 = I1(lower,p);
	integral2 = I2(lower,p);


	//estimate error in approximation
	errest = fabs(integral1 - integral2);  

	//refine interval until error below threshold
	if (errest < 15.0 * tol) {
		return integral2;
	} else {
    return (integrate((lower+p)*0.5, lower) + integrate(p,(lower+p)*0.5));
	}
};

//Helper functions for integrate-function

//helper function for integrate function
double Curvebase::I1(double a_in, double b_in){
		return ((b_in -a_in)/6.0)*(function(a_in)+4.0*function(0.5*(a_in + b_in)) + function(b_in));
}

//helper function for integrate function
double Curvebase::I2(double a_in, double b_in) {
	double c = (a_in + b_in) * 0.5;
	return I1(a_in,c) + I1(c, b_in);
};

//integrand of integral in curve length computation
//used by integrate function in the adaptive integration
double Curvebase::function(double p) {
	return sqrt( dxp(p)*dxp(p) + dyp(p)*dyp(p));
};

//End of Helper functions for integrate-function

//returns x-coordinate of relative position along curve
//relative position is given in interval [0,1]
double Curvebase::x(double s){
	if ( fabs(s) > 1.0) {
		std::cout << "ERROR: bad relative position of x-coordinate" << std::endl;
	}
	//get parametrized poisiton from relative position
	double p = solve(s);
	//get actual y-coordinate for parametrized position 
	return xp(p);
};

//returns y-coordinate of relative position along curve
//relative position is given in interval [0,1]
double Curvebase::y(double s){
	if ( fabs(s) > 1.0) {
		std::cout << "ERROR: bad relative position of x-coordinate" << std::endl;
	}
	//get parametrized poisiton from relative position
	double p = solve(s);
	//get actual y-coordinate for parametrized position 
	return yp(p);

};

//Newton Rhapson's method to parametrized value
//corresponding to relative position
double Curvebase::solve(double s) {
	
	//set initial guess to length of
	//line segment between endpoints
	double p;
	double p_new  = s*(pmax - pmin);
	double eps = 1e-8;

	int n_iter = 0;
	const int maxiter = 1e7;

	//iterate untill error is below tolerance or
	//100 iterations have been performed
	do {
		p = p_new;
		if (function(p) == 0.0) {
			std::cout << "Zero Division. Bad Value." << std::endl;
			p_new = p + eps;
		} else {
			p_new = p - (integrate(p,pmin) - s*length) / function(p);
			
		}
		++n_iter;
	} while ((fabs(p-p_new) > tol) && n_iter <= maxiter);

	if (n_iter >= maxiter) {
		std::cout << "ERROR: Convergence not reached." << std::endl;
	}
	return p_new;
};

//reverse orientation of curve
void Curvebase::reverse_orientation(void){
	rev = !rev;
};

//returns orientation of curve
bool Curvebase::ori(void){
	return rev;
}

//prints information about curve
//for inquiry of configuration
void Curvebase::printInfo(void){
	std::cout << "lower boundary : > " << pmin << std::endl;
	std::cout << "upper boundary : > " << pmax << std::endl;
	std::cout << "total length : > " << length << std::endl;
	std::cout << "orientation : > " << rev << std::endl;
	std::cout << "x(0.5) : > " << x(0.5) << std::endl;
	std::cout << "y(0.5) : > " << y(0.5) << std::endl;
	return;
};
