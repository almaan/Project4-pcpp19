#include "hline.h"

Horzline::Horzline(){};
Horzline::~Horzline(){};

//x as function of parametrization variable
double Horzline::xp(double p){
	return p;

};

//derivative of x as function of parametrization variable
double Horzline::dxp(double p){
	return 1;
};

//y as function of parametrization variable
double Horzline::yp(double p){
	return yfunc(p);
};

//derivative of y as function of parametrization variable
double Horzline::dyp(double p) {
	return yfuncd(p);
};