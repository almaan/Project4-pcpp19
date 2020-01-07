

#include <iostream>
#include <cmath>
#include <cstdio>
#include <string>
#include <memory>
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

double ufun(double x, double y){
  return(sin(x*x/100.0)*cos(x/10.0) + y);
}
// double ufun(double x, double y){
//   return(x*x +3*y);
// }


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
	std::cout << "boundary curves defined above to generate domain Omega" << std::endl;

  std::shared_ptr<Curvebase>bottom = std::make_shared<BottomBorder>(botb);
  std::shared_ptr<Curvebase>top = std::make_shared<TopBorder>(topb);
  std::shared_ptr<Curvebase>left = std::make_shared<LeftRightBorder>( leftb );
  std::shared_ptr<Curvebase>right = std::make_shared<LeftRightBorder>( rightb );

  Domain omega(bottom, top, right,left);
  // Domain omega(botb,topb,rightb,leftb);
  // Domain omega(std::shared_ptr<Curvebase> botb ,
  //              std::shared_ptr<Curvebase> topb ,
  //              std::shared_ptr<Curvebase> rightb ,
  //              std::shared_ptr<Curvebase> leftb );


  int n_rows;
	int n_cols;

  std::cout << "ncols : ";
  std::cin >> n_cols;
  std::cout << "nrows : ";
  std::cin >> n_rows;


  omega.doLowerResolve(true);
  omega.make_grid(n_rows, n_cols);

  GFkt gf(&omega);

  gf.fillMat(ufun);
  std::cout << "here0" << std::endl;
  GFkt dudx(gf);
  GFkt dudy(gf);
  GFkt ulap(gf);

  std::cout << "here0.8" << std::endl;
  dudx = gf.D0x();
  dudy = gf.D0y();
  ulap = gf.del();

  std::string ufundir = "res/ufun";
  std::string dudxdir = "res/partial_x";
  std::cout << "set oname" << std::endl;

  std::string dudydir = "res/partial_y";
  std::string ulapdir = "res/laplacian";

  gf.saveData(ufundir);
  dudx.saveData(dudxdir);
  std::cout << "save" << std::endl;
  dudy.saveData(dudydir);
  ulap.saveData(ulapdir);

	return 0;
}



