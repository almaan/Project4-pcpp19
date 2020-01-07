#include "vline.h"

Vertline::Vertline(){};
Vertline::~Vertline(){};


//x as function of parametrization variable
double Vertline::xp(double p){
	return xfunc(p);
};

//derivative of x as function of parametrization variable
double Vertline::dxp(double p){
	return xfuncd(p);
};

//y as function of parametrization variable
double Vertline::yp(double p){
	return p;
};

//derivative of y as function of parametrization variable
double Vertline::dyp(double p){
	return 1;
};