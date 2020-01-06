#include <iostream>
#include <cstdio>
#include <cmath>

#include "domain.h"
#include "../point/point.h"

/*

*/

//default constructor
Domain::Domain(){};

//default destructor
Domain::~Domain(){
};

//copy constructor
//all private variables including grid will be copied
Domain::Domain(const Domain &d) {
	for (int ii = 0; ii <=3; ii++) {
		sides[ii] = d.sides[ii];
	} 
	x_ = d.x_;
	y_ = d.y_;

	m_ = d.m_;
	n_ = d.n_;
	n_points = d.n_points;
	lower_resolve = d.lower_resolve;
};

//assignment operator overloading
//all private variables including grid will be set equal
//right sid object
Domain & Domain::operator=(const Domain &d) {
	if (this == &d) {
		return *this;
	} else {

		for (int ii = 0; ii <=3; ii++) {
			sides[ii] = d.sides[ii];
		} 
		x_ = d.x_;
		y_ = d.y_;

		m_ = d.m_;	
		n_ = d.n_;
		n_points = d.n_points;
		lower_resolve = d.lower_resolve;

	}

	return *this;

}

//constructor taking four boundary curves
//representing the four sides of the domain
//boundary curves can be passed in any arbitrary order
Domain::Domain(Curvebase& s1,
               Curvebase& s2,
               Curvebase& s3,
               Curvebase& s4) : m_(0), n_(0), x_(nullptr),y_(nullptr)
{
	

	//assign curves to sides array
	//convention of curve position within sides array
	//is 0-left 1-bottom 2-right 3-top
	//this is adjusted after initial assignation

	sides[0] = std::shared_ptr<Curvebase>(&s1); //left side
	sides[1] = std::shared_ptr<Curvebase>(&s2); //bottom side
  sides[2] = std::shared_ptr<Curvebase>(&s3); //right side
	sides[3] = std::shared_ptr<Curvebase>(&s4); //top side

	//adjust boundary curve positioning in sides array
	//as to be consistent with above described convention
	check_consistency();

	//reset grid parameters
	m_ = n_ = 0;
	x_ = y_ = nullptr;

};

//returns pointer to boudary curve of sides array
//with the provided index "s"
std::shared_ptr<Curvebase>  Domain::getSide(int s) {
	return sides[s];
};

//x-coordinate mapping from unit square to domain
double Domain::xmap(double r, double s) {

	//raise warning if r or s are not in [0,1]
	if (s < 0 || s > 1 || r < 0 || r > 1) {
		std::cout << "x(r,s) : parameters out of bound" << std::endl;
	}

	//transform relative y-coordinate if adjusted
	//resolution should be used
	if (lower_resolve){
		s = sigmaT(s);
	}

	//transfinite interpolation

	//positive terms
	double pos = (1. - r)*sides[0]->x(s) +
				  r*sides[2]->x(s) + 
				  (1.-s)*sides[1]->x(r) +
			      s*sides[3]->x(r);

	//negative terms
	double neg = (1.-r)*(1.-s)*sides[0]->x(0.0) + 
				  r*(1.-s)*sides[1]->x(1.0) +
				  (1-r)*s*sides[3]->x(0.0) +
				  r*s*sides[2]->x(1.0); 

	return pos - neg;
};


double Domain::ymap(double r, double s) {

	//raise warning if r or s are not in [0,1]
	if (s < 0 || s > 1 || r < 0 || r > 1) {
		std::cout << "y(r,s): parameters out of bound" << std::endl;
	}

	//transform relative y-coordinate if adjusted
	//resolution should be used
	if (lower_resolve){
		s = sigmaT(s);
	}


	//transfinite interpolation


	//positive terms
	double pos = (1. - r)*sides[0]->y(s) +
				  r*sides[2]->y(s) + 
				  (1.-s)*sides[1]->y(r) +
			      s*sides[3]->	y(r);

	double neg = (1.-r)*(1.-s)*sides[0]->y(0.0) + 
				  r*(1.-s)*sides[1]->y(1.0) +
				  (1-r)*s*sides[3]->y(0.0) +
				  r*s*sides[2]->y(1.0);

	return pos - neg;
};



//function to generate grid over domain
//boundary curves need to be passed before grid
//generation.
void Domain::make_grid(int m, int n){

	//check if grid has already been generated over domain
	//if such. remove old grid
	if (!(m <= 0) && (n <= 0)) {
		std::cout << "Erasing old grid" << std::endl;
		delete [] x_;
		delete [] y_;
	}	

	m_ = m; //number of horizontal intervals in grid
	n_ = n; //number of vertical intervals in grid


	double dx = 1.0/(double(m_) -1.0);//horizontal sub-interval length
	double dy = 1.0/(double(n_) -1.0);//vertical sub-interval length

	n_points = m_*n_; //total number of grid-points

	x_ = new double [n_points]; //generate array to store x-coordinates in (dynamic)
	y_ = new double [n_points]; //generate array to store y-coordinates in (dynamic)

	//algebraic grid formation
	//for (int ii = 0; ii < m_; ii++){
	//	for (int jj = 0; jj < n_; jj++){

	//		x_[jj*m_ + ii] = xmap(dx*(double)ii, dy*(double)jj);
	//		y_[jj*m_ + ii] = ymap(dx*(double)ii, dy*(double)jj);

	//	}
	//}

  for (int ii = 0; ii < m_; ii++){
		for (int jj = 0; jj < n_; jj++){

			x_[jj*m_ + ii] = xmap(dx*(double)ii, dy*(double)jj);
			y_[jj*m_ + ii] = ymap(dx*(double)ii, dy*(double)jj);

		}
	}

}

//function to check that boundary curves
//are properly sorted
//allows for boundary curves to be
//arbitrarily passed to domain upon initialization
void Domain::check_consistency(void){

	int neg[2], pos[2]; //arrays to store index of curve orientations
	int p = 0, n = 0; 

	//find which sides are negatively
	//respectively positively oriented
	for(int ii = 0; ii < 4; ii++) {
		if(sides[ii]->ori()){
			pos[p] = ii;
			p++;
		} else {
			neg[n] = ii;
			n++;
		}
	}

	Domain tmp(*this); //temporary domain to access sides from

	//get index of left and top boundary curve
	//assign to position compatible with grid mapping
	int p_neg = (int)(tmp.getSide(neg[0])->y(0.0) < tmp.getSide(neg[1])->y(0.0));
	sides[0] = tmp.getSide(neg[1-p_neg]);
	sides[3] = tmp.getSide(neg[p_neg]);

	//get index of lower and right boundary curve
	//assign to position compatible with grid mapping
	int p_pos =  (int)(tmp.getSide(pos[0])->x(0.0) < tmp.getSide(pos[1])->x(0.0));
	sides[1] = tmp.getSide(pos[1-p_pos]);
	sides[2] = tmp.getSide(pos[p_pos]);

};

//function used to increase resolution of
//lower boundary curve
double Domain::sigmaT(double sigma) {
	return 1 + tanh(3.0 * (sigma-1.0))/tanh(3.0);
};

//call to activate lower boundary increased resolution
//upon grid generation
void Domain::doLowerResolve(bool a) {
	lower_resolve = a;
}

//function to print coordinates in format "x,y"
void Domain::printCoordinates(void) {
	for (int ii = 0; ii < n_points; ii++)
		{
   		 std::cout << x_[ii] << "," << y_[ii] << std::endl;
		} 
};

int Domain::xsize(void) const{
  return this->m_;
}

int Domain::ysize(void) const{
  return this->n_;
}

int Domain::npoints(void) const{
  return this->n_points;
}

// function that generating two .bin files in a directory res
// x-coordinate vector and y-coordinates vector of grid
// if user_input is set to true the user will be asked
// to provide a suffix to be used for the file name

void Domain::saveCoordinates(std::string out_dir) {

	//names of files to be saved
	std::string x_outname = out_dir + "/x_vec.bin";
	std::string y_outname = out_dir + "/y_vec.bin";

	//save x-coordinate file
	FILE *fp_x;
	fp_x =fopen(x_outname.c_str(),"wb");
	fwrite(x_,sizeof(double),m_*n_,fp_x);
	fclose(fp_x);

	//save y-coordinate file
	FILE *fp_y;
	fp_y =fopen(y_outname.c_str(),"wb");
	fwrite(y_,sizeof(double),m_*n_,fp_y);
	fclose(fp_y);

}

bool Domain::grid_valid(void) const {
  return this->m_*this->n_ > 0;
}

const Point<double> Domain::operator()(int i, int j) const{
  Point <double>P;
  if (this->grid_valid()){
    P.setX(x_[i + this->m_*j]);
    P.setY(y_[i + this->m_*j]);
  } else {
    std::cout << "ERROR : Can't access elements when";
    std::cout << "no grid is defined." << std::endl;
  }
  return P;
}
