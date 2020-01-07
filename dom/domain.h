#ifndef _DOMAIN
#define _DOMAIN

#include <memory>
#include "curvebase.h"
#include "../point/point.h"

//class to represent four sided domains
//allows user to generate grids of desired size

class Domain {

private:
    std::shared_ptr<Curvebase> sides[4]; //Holder for boundary curves
		double *x_, *y_; //arrays for coordinate vectors, pointers for Dynamic Memory Allocation 
    int m_, n_ ; //number of intervals in vertical (m) and horizontal (n) direction
		int n_points; //total number of points

		double xmap(double r, double s);//x-coordinate mapping from unit square do domain
		double ymap(double r, double s);//y-coordinate mapping from unit square do domain

		bool lower_resolve = false;//use lower-boundary resolution if true

public:

  // *--- Constructor and Destructors ---*
		Domain(); //Default constructur
    Domain(std::shared_ptr<Curvebase> s1,
            std::shared_ptr<Curvebase> s2,
            std::shared_ptr<Curvebase> s3,
            std::shared_ptr<Curvebase> s4);

		Domain(const Domain &d); //copy  constructor
  
  //*-- assignment operator overloading ---*
		Domain& operator=(const Domain &d);
    const Point<double> operator()(int i, int j) const;
    ~Domain() {
          delete [] x_;
          delete [] y_;
          x_ = y_ = nullptr;
    }

  // *--- Additional Methods ---*

		void check_consistency(void);//assign proper side-identity to boundary curves
		void make_grid (int m, int n);//make a mxn-grid over the domain. Will remove old grid.

    std::shared_ptr<Curvebase> getSide(int s);//reuturns the boundary curve of provided identity
		double sigmaT(double s);//distribution of y-coordinates when refinement is used
		void doLowerResolve(bool a);//will increase resolution of lower boundary if true

		void printCoordinates(void);//print the coordinates as comma separated tuples
    void saveCoordinates(std::string out_dir = (std::string)"/tmp");//save coordinates.

    int xsize(void) const; 
    int ysize(void) const; 
    int npoints(void) const;
  
    bool grid_valid() const;

};

#endif
