

#include <iostream>
#include <cmath>
#include <cstdio>
#include <string>
#include <memory>

#include "dom/hline.h"
#include "dom/vline.h"
#include "dom/domain.h"
#include "matrix/matrix.h"
#include "gfkt/GFkt.h"


// *--- Classes for specific borders ---*

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

// *--- Helper Functions ---*

// Function to analyze on grid
// sin(x^2/100)*cos(x/10) + y
double ufun(double x, double y){
  return(sin(x*x/100.0)*cos(x/10.0) + y);
}

//*--- Main Program ---*

int main(){

  // *---Construct domain sides---*
  // using smart pointers 

  // Left side
  std::shared_ptr<LeftRightBorder> leftb( new LeftRightBorder(0.0,3.0,false));
	leftb->setXpos(-10.0);
	leftb->setLength();

  // Right side
	std::shared_ptr<LeftRightBorder> rightb(new LeftRightBorder(0.0,3.0, true));
	rightb->setXpos(5.0);
	rightb->setLength();

  // Bottom Side
  std::shared_ptr<BottomBorder> botb( new BottomBorder(-10.0, 5.0, true));
	botb->setLength();
	botb->printInfo();

  // Top Side
  std::shared_ptr<TopBorder> topb(new TopBorder(-10.0, 5.0, false));
	topb->setYpos(3.0);
	topb->setLength();

  // Generate Domain from given sides
  std::shared_ptr<Domain> omega(new Domain(botb,topb,rightb,leftb));

  // *--- Generate Grid on Domain ---*
  int n_rows;
	int n_cols;

  // Let user specify number of points by
  // giving rows and cols
  std::cout << "[Enter] nrows : ";
  std::cin >> n_rows;

  std::cout << "[Enter] ncols : ";
  std::cin >> n_cols;
  
  omega->doLowerResolve(true);
  omega->make_grid(n_rows, n_cols);

  // *--- Perform computations on domain ---*

  // instatiate gridfunction with domain
  GFkt gf(omega);

  // Fill grid function values based on
  // provided function
  gf.fillMat(ufun);

  // Create new objects to hold results
  // from differential operators

  // For partial_x
  GFkt dudx(gf);
  // For partial_y
  GFkt dudy(gf);
  // For laplacian
  GFkt ulap(gf);

  // perform computations

  //d/dx
  dudx = gf.D0x();
  //d/dy
  dudy = gf.D0y();
  // d^2/dx^ + d^2/dy^2
  ulap = gf.del();

  // *--- Save Results ---*

  //paths to save restults to
  std::string ufundir = "../res/ufun";
  std::string dudxdir = "../res/partial_x";
  std::string dudydir = "../res/partial_y";
  std::string ulapdir = "../res/laplacian";

  // write to files
  gf.saveData(ufundir);
  dudx.saveData(dudxdir);
  dudy.saveData(dudydir);
  ulap.saveData(ulapdir);

	return 0;
}