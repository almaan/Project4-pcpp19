#include <iostream>
#include <memory>
#include <cmath>
#include <string>

#include "GFkt.h"
#include "../matrix/matrix.h"
#include "../dom/domain.h"

// *--- Constructors & Destructors ----*

// copy constructor
GFkt::GFkt(const GFkt& U) : u(U.u), grid(U.grid) {
  setDelta();
}

// construct from provided domain 
GFkt::GFkt( std::shared_ptr<Domain> grid_) :

  // function values will be initialized to zero
  u(grid_->xsize(),
    grid_->ysize()),
  grid(grid_) {

  setDelta();
}

// *-- Arithmetic Operators ---*

// gridfun-gridfun addition operator overloading
GFkt GFkt::operator+(const GFkt& U) const {
  if (grid == U.grid) { // ensure defined on same grid 
    GFkt tmp(*this);
    tmp.u += U.u; 
    return tmp;
  } else {return error();};
}

// gridfun-gridfun subtraction operator overloading
GFkt GFkt::operator-(const GFkt& U) const {
  if (grid == U.grid) { // ensure defined on same grid
    GFkt tmp(grid);
    tmp.u = u-U.u; 
    return tmp;
  }
  else return error();
}

// gridfun-gridfun multiplication operator overloading
GFkt GFkt::operator*(const GFkt& U) const {
  if (grid == U.grid) { // ensure defined on same grid
    GFkt tmp(grid);
    for (int j = 0; j < grid->ysize(); j++) {
      for (int i = 0; i < grid->xsize(); i++){
        tmp.u(i,j) = u(i,j)*U.u(i,j);
      }
    }
    return tmp;
  }
  else return error();
}

// gridfun-gridfun division operator overloading
GFkt GFkt::operator/(const GFkt& U) const {
  if (grid == U.grid) { // ensure defined on same grid
    GFkt tmp(grid);
    for (int j = 0; j < grid->ysize(); j++) {
      for (int i = 0; i < grid->xsize(); i++){
        tmp.u(i,j) = u(i,j)/U.u(i,j);
      }
    }
    return tmp;
  }
  else return error();
}

// gridfun-scalar multiplication operator overloading
GFkt GFkt::operator*(const double& x) const {
  GFkt tmp(grid);
  tmp.u = this->u * x;
  return tmp;
}

// gridfun-scalar division operator overloading
GFkt GFkt::operator/(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u * (1.0 / x);
  return tmp;
}

// gridfun-scalar addition operator overloading
GFkt GFkt::operator+(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u + x;
  return tmp;

}

// gridfun-scalar  operator overloading
GFkt GFkt::operator-(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u - x;
  return tmp;

}

// assignment operator overloading
GFkt& GFkt::operator=(const GFkt& U){
  this->grid = U.grid;
  this->u = U.u;
  return *this;
}

// *-- Additional Methods ---*

// second order (three point) partial derivative
// of grid function w.r.t. first reference coordinate
double GFkt::dudxh(int i, int j) const {
  double diff;

  // midpoint difference, for inner points
  if ((i > 0) && (i < grid->xsize()-1)){
    diff =  this->u(i+1,j) - u(i-1,j);
    return diff / (this->dxh * 2.0);
  }
  //forward one sided difference
  //for left side boundary
  else if (i == 0)
  {
    diff = 3*this->u(i,j) - 4*this->u(i+1,j) + this->u(i+2,j);
    return diff / (2.0 * this->dxh);
  }
  //backward one sided difference
  //for right side boundary
  else
  {
    diff = -this->u(i-2,j) + 4*this->u(i-1,j) - 3 * this->u(i,j);
    return diff / (2.0 * this->dxh);
  }
}

// second order (three point) partial derivative
// of grid function w.r.t. second reference coordinate
double GFkt::dudyh(int i, int j) const {
  double diff;
  // midpoint difference, for inner points
  if ((j > 0) && (j < grid->ysize()-1)){
    diff =  this->u(i,j+1) - this->u(i,j-1);
    return diff / (this->dyh * 2.0);
  }
  //forward one sided difference
  //for bottom side boundary
  else if (j == 0)
    {
      diff = 3*this->u(i,j) - 4*this->u(i,j+1) + this->u(i,j+2);
      return diff / (2.0 * this->dyh);
    }
  else
    //backward one sided difference
    //for top side boundary
    {
      diff = -this->u(i,j-2) + 4*this->u(i,j-1) - 3 * this->u(i,j);
      return diff / (2.0 * this->dyh);
    }
}

// second order (three point) partial derivative
// of map function w.r.t. first reference coordinate
Point<double> GFkt::dxd( int i, int j) const {

  Point<double> D; //initialize point

  // midpoint difference, for inner points
  if ((i > 0) && (i < grid->xsize()-1)){
    Point<double> Dp1 = (*grid)(i+1,j);
    Point<double> Dm1 = (*grid)(i-1,j);
    D = (Dp1 - Dm1);
    D *= ( 0.5 * (1.0 / this->dxh));
  }
  //forward one sided difference
  //for left side boundary
  else if (i == 0)
  {
    Point<double> D0,Dp1,Dp2;
    D0 = (*grid)(i,j); Dp1 = (*grid)(i+1,j); Dp2 = (*grid)(i+2,j);
    D = D0*3.0 - Dp1*4.0 + Dp2;
    D *= (1.0  /  (2.0 * this->dxh));
    //backward one sided difference
    //for right side boundary
  } else {
    Point<double> D0,Dm1,Dm2;
    D0 = (*grid)(i,j); Dm1 = (*grid)(i-1,j); Dm2 = (*grid)(i-2,j);
    D = D0*(-3.0) + Dm1*4.0 - Dm2;
    D *= (1.0 / (2.0 * this->dxh));
  }
  return D;
}

// second order (three point) partial derivative
// of map function w.r.t. second reference coordinate
Point<double> GFkt::dyd( int i, int j) const {
  
  Point<double> D; //initialize point
  // midpoint difference, for inner points
  if ((j > 0) && (j < grid->ysize()-1)){

    Point<double> Dp1 = (*grid)(i,j+1);
    Point<double> Dm1 = (*grid)(i,j-1);
    D = (Dp1 - Dm1);
    D *= (0.5* (1.0 / this->dyh));
  }
  //forward one sided difference
  //for bottom side boundary
  else if (j == 0)
    {
      Point<double> D0,Dp1,Dp2;
      D0 = (*grid)(i,j); Dp1 = (*grid)(i,j+1); Dp2 = (*grid)(i,j+1);
      D = D0*3.0 - Dp1*4.0 + Dp2;
      D *= (1.0 / (2 * this->dyh));
    }
  //backward one sided difference
  //for top side boundary
  else
    {
    Point<double> D0,Dm1,Dm2;
    D0 = (*grid)(i,j); Dm1 = (*grid)(i,j-1); Dm2 = (*grid)(i,j-2);
    D = D0*(-3.0) + Dm1*4.0 - Dm2;
    D *= (1.0 / (2.0 * this->dyh));
   
  }
  return D;
}

// Differential operator d/dx
GFkt GFkt::D0x() const {
  if (grid->grid_valid()) {

    double J; // to hold Jacobian
    Point<double> dX,dY; // to hold partial derivatives
    GFkt tmp(this->grid); //return object

    // compute for all points
    for (int i = 0; i < this->grid->xsize(); i++){
      for (int j = 0; j < this->grid->ysize(); j++){

        // get partial derivatives w.r.t. cartesian coordinates
        dX = this->dxd(i,j);
        dY = this->dyd(i,j);
        // compute jacobian
        J = dX.getX()*dY.getY() -
            dY.getX()*dX.getY();
        // use Cramer's rule to obtain du/dx
        tmp.u(i,j) = (1.0 / J) *
                    (this->dudxh(i,j) *
                     dY.getY() -
                     this->dudyh(i,j) *
                     dY.getX());

      }
    }
    return tmp;
  }
  else
  {
    return error();
  }
}

// Differential operator d/dy
GFkt GFkt::D0y() const {
  if (grid->grid_valid()) {

    double J; //to hold jacobian
    Point<double> dX,dY; //to hold partial derivatives

    // compute for all points
    GFkt tmp(this->grid);
    for (int i = 0; i < this->grid->xsize(); i++){
      for (int j = 0; j < this->grid->ysize(); j++){

        // get partial derivatives w.r.t. cartesian coordinates
        dX = this->dxd(i,j);
        dY = this->dyd(i,j);
        // compute jacobian
        J = dX.getX()*dY.getY() -
            dY.getX()*dX.getY();
        // use Cramer's rule to obtain du/dx
        tmp.u(i,j) = (1.0 / J) *
          (this->dudyh(i,j) *
           dX.getX() -
           this->dudxh(i,j) *
           dX.getY());

      }
    }
    return tmp;
  }
  else
    {
      return error();
    }
}
// Differential operator d^2/dx^2 + d^2/dy^2
GFkt GFkt::del(void) const {
  // gridfuns to return
  GFkt x_part(this->grid), y_part(this->grid);
  GFkt del_part(this->grid);

  // compute first order partial derivatives
  x_part = this->D0x();
  y_part = this->D0y();

  // compute second order partial derivatives
  x_part = x_part.D0x();
  y_part = y_part.D0y();
  // summarize
  del_part = x_part + y_part;

  return del_part;
}

// print grid function values
void GFkt::printMat(void) const {
  this->u.print_matrix();
}

// fill grid function values by using
// a user-provided function
void GFkt::fillMat(double fun(double,double)) {
  Point<double> P;
  for (int i = 0; i < this->grid->xsize(); i++){
    for (int j = 0; j < this->grid->ysize(); j++){
      P = (*grid)(i,j);
      this->u(i,j) = fun(P.getX(),P.getY());
    }
  }
}

// get grid coordinates for point 
Point<double> GFkt::getGridVal(int i, int j) const{
  return (*grid)(i, j);

}

// get grid function value for point
double GFkt::getFuncVal(int i, int j) const{
  return this->u(i,j);
}

// save all data
void GFkt::saveData(std::string out_dir){
  this->u.saveData(out_dir);
  this->grid->saveCoordinates(out_dir);

}

// error handling, returns self
GFkt GFkt::error() const {
  std::cout << "[ERROR] : Cannot use grid";
  std::cout << "operations on different grids" << std::endl;
  return *this;
}

// set mesh size
void GFkt::setDelta(void){
  this->dxh = 1.0 / ((double)this->grid->xsize() -1.0);
  this->dyh = 1.0 / ((double)this->grid->ysize() -1.0);
}
