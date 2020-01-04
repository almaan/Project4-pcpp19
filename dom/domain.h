#ifndef _DOMAIN
#define _DOMAIN

#include "curvebase.h"
#include <memory>

//class to represent four sided domains
//allows user to generate grids of desired size

class Domain {

private:
    Curvebase *sides[4]; //Holder for boundary curves
		double *x_, *y_; //arrays for coordinate vectors, pointers for Dynamic Memory Allocation 
		int m_, n_; //number of intervals in vertical (m) and horizontal (n) direction
		int n_points; //total number of points

		double xmap(double r, double s);//x-coordinate mapping from unit square do domain
		double ymap(double r, double s);//y-coordinate mapping from unit square do domain

		bool lower_resolve = false;//use lower-boundary resolution if true

public:
		Domain(); //Default constructur
		Domain(Curvebase &s1, Curvebase &s2,
          Curvebase &s3, Curvebase &s4); //constructor when provided four boundary curves
		Domain(const Domain &d); //copy  constructor
		Domain& operator=(const Domain &d); //assignment operator overloading
    Point operator()(int i, int j);
		~Domain(); //default destructor

		void check_consistency(void);//assign proper side-identity to boundary curves
		void make_grid (int m, int n);//make a mxn-grid over the domain. Will remove old grid.

		Curvebase * getSide(int s);//reuturns the boundary curve of provided identity
		double sigmaT(double s);//distribution of y-coordinates when refinement is used
		void doLowerResolve(bool a);//will increase resolution of lower boundary if true

		void printCoordinates(void);//print the coordinates as comma separated tuples
		void saveCoordinates(bool user_input);//save coordinates.

    int xsize(void) const; 
    int ysize(void) const; 
    int npoints(void) const;
  
    double dxdsmap(double r, double s);
    double dxdrmap(double r, double s);

    double dydsmap(double r, double s);
    double dydrmap(double r, double s);

    bool grid_valid() const;

};

#endif
