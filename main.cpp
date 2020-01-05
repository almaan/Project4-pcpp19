

#include <iostream>
#include <cmath>
#include <cstdio>
#include <string>
#include "dom/hline.h"
#include "dom/vline.h"
#include "dom/domain.h"
#include "matrix/matrix.h"
#include "GFkt.h"


//class for left an right borders of Omega domain
//specific case where the boundary curve
//is a vertical straigt line segment 
class LeftRightBorder : public Vertline {
	private:
		double x_pos; //fixed x-position
		double xfunc(double p); //parametrization of x-coordinate
		double xfuncd(double p); //derivative of x-coordinate parametrixation 
		

	public:
		LeftRightBorder(double a, double b, bool dir) : Vertline(a,b, dir) {}; //use same constructor as Vertline
		~LeftRightBorder(); //default destructor
		void setXpos(double p); //set fixed x-position
		void setLength(void); //compute total curve length
};

LeftRightBorder::~LeftRightBorder(){};

//sets fixed x-position
void LeftRightBorder::setXpos(double p) {
	x_pos = p;
	return;
};

//redefines function to compute length
void LeftRightBorder::setLength(void){
	length =  (fabs(pmax-pmin));
};
//x-coordinate parametrization
double LeftRightBorder::xfunc(double p){
	return x_pos;
};
//derivative of x-coordinate parametrization
double LeftRightBorder::xfuncd(double p){
	return 0;
};

//class for top boundary curve
//specific case where boundary curve
//is a horizontal straight line segment
class TopBorder : public Horzline {
	private:
		double y_pos; //fixed y-position
		double yfunc(double p); //y-coordinate parametrization
		double yfuncd(double p);//derivative of y-coordinate parametrization

	public:
		TopBorder(double a, double b, bool dir) : Horzline(a,b,dir) {}; //use same constructor as Horzline
		~TopBorder(); //default destructor
		void setYpos(double p); //set fixed y-position
		void setLength(void); //compute length of boundary curve
};

//default destructor
TopBorder::~TopBorder(){};


//sets fixed y-position
void TopBorder::setYpos(double p){
	y_pos = p;
	return;
};

//redefines function to compute length
void TopBorder::setLength(void){
	length = fabs(pmax-pmin);
};

//y-coordinate parametrization
double TopBorder::yfunc(double p){
	return y_pos;
};

//derivative of y-coordinate parametrization
double TopBorder::yfuncd(double p){
	return 0;
};

//class for botto boundary curve
class BottomBorder : public Horzline{
	private:
		double yfunc(double p); //y-coordinate parametrization
		double yfuncd(double p);//derivative of y-coordinate parametrization
	public:
		BottomBorder(double a, double b, bool dir) : Horzline(a,b,dir) {}; //use same constructor as Horzline
		~BottomBorder(); //default destructor
};

//default destructor
BottomBorder::~BottomBorder(){};

//y-coordinate parametrization
double BottomBorder::yfunc(double p){
	if (p < (-3.0)) {
		return 0.5*(1.0/(1.0 +exp(-3.0*(p+6.0))));
	
	} else if ( p >= -3.0){
		return 0.5*(1.0/(1.0 + exp(3.0*p)));
	
	} else {
		
		std::cout << "Bottom func: Error fetching point > " << p << std::endl;
		return 0;
	}

};

//derivative of  y-function parametrization
double BottomBorder::yfuncd(double p){
	if (p < -3) {
    
		double nom = exp(-3.0*p - 18.0*p);
		double den = pow(exp(1.0 + exp(-3.0*p - 18.0*p)),2.0);
	
		return 1.5 * nom / den;
	
	} else if ( p >= -3.0){
	
		double nom = exp(3.0*p);
		double den = pow((1.0 + exp(3.0*p)),2.0);
	
		return -1.5 * nom / den;

	} else {
	
		std::cout << "Bottom funcd: Error fetching point > " << p << std::endl;
		return 0;
	}

};

//main program
int main(){

	std::cout << "Left Border" << std::endl;
	LeftRightBorder leftb(0.0,3.0,false);
	leftb.setXpos(-10.0);
	leftb.setLength();
	leftb.printInfo();


	std::cout << "\n";
	std::cout << "Right border" << std::endl;
	LeftRightBorder rightb(0.0,3.0, true);
	rightb.setXpos(5.0);
	rightb.setLength();
	rightb.printInfo();

	std::cout << "\n" ;
	std::cout << "Bottom Border " << std::endl;
	BottomBorder botb(-10.0, 5.0, true);
	botb.setLength();
	botb.printInfo();


	std::cout << "\n";
	std::cout << "Top Border" << std::endl;
	TopBorder topb(-10.0, 5.0, false);
	topb.setYpos(3.0);
	topb.setLength();
	topb.printInfo();

	std::cout << "\n" << std::endl;
	std::cout << "using above defined boundary curves to generate domain Omega" << std::endl;

	Domain omega(botb,topb,rightb,leftb);

	int n_rows = 50;
	int n_cols = 20;

	omega.make_grid(n_rows, n_cols);

  Matrix M(n_rows,n_cols);

  GFkt gf(&omega);
  gf.fillMat();
  gf.printMat();

  GFkt dgf(gf);
  GFkt dgy(gf);

  dgf = gf.D0x();
  dgy = gf.D0y();

  bool keepOn = true;
  int a,b;
  char ans;

  while (keepOn) {
    std::cout << "x >> ";
    std::cin >> a;
    std::cout << "y >> ";
    std::cin >> b;

    double val = dgf.getFuncVal(a,b);
    Point<double> P = dgf.getGridVal(a,b);

    double valy = dgy.getFuncVal(a,b);
    Point<double> Py = dgy.getGridVal(a,b);



    std::cout << "Derivative" << std::endl;
    std::cout << "x :: crd: " << P << " | e-val : " << val;
    std::cout << " | t-val : " << P.getX()*2 + 3*P.getY() + 0.5 << std::endl;

    std::cout << "y :: crd: " << Py << " | e-val : " << valy;
    std::cout << " | t-val : " << 3.0*Py.getX() << std::endl;

    std::cout << "continue (y/n) >> ";
    std::cin >> ans;

    if (ans != 'y'){
      keepOn = false;
    }
  }

	return 0;
}



