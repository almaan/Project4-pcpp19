#include <iostream>
#include <memory>
#include <cmath>

#include "GFkt.h"
#include "matrix/matrix.h"
#include "dom/domain.h"


GFkt GFkt::operator+(const GFkt& U) const {
  if (grid == U.grid) { // defined on the same grid?
    GFkt tmp(grid);
    tmp.u = u+U.u; 
    return tmp;
  } else {return error();};
}

GFkt GFkt::operator-(const GFkt& U) const {
  if (grid == U.grid) { 
    GFkt tmp(grid);
    tmp.u = u-U.u; 
    return tmp;
  }
  else return error();
}

GFkt GFkt::operator*(const GFkt& U) const {
  if (grid == U.grid) { 
    GFkt tmp(grid);
    for (int j = 0; j <= grid->ysize(); j++) {
      for (int i = 0; i <= grid->xsize(); i++){
        tmp.u(i,j) = u(i,j)*U.u(i,j);
      }
    }
    return tmp;
  }
  else return error();
}

GFkt GFkt::operator/(const GFkt& U) const {
  if (grid == U.grid) { 
    GFkt tmp(grid);
    for (int j = 0; j <= grid->ysize(); j++) {
      for (int i = 0; i <= grid->xsize(); i++){
        tmp.u(i,j) = u(i,j)/U.u(i,j);
      }
    }
    return tmp;
  }
  else return error();
}

GFkt GFkt::operator*(const double& x) const {
  GFkt tmp(grid);
  tmp.u = this->u * x;
  return tmp;
}

GFkt GFkt::operator/(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u * (1.0 / x);
  return tmp;
}

GFkt GFkt::operator+(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u + x;
  return tmp;

}

GFkt GFkt::operator-(const double& x) const {
  GFkt tmp(grid);
  tmp.u = tmp.u - x;
  return tmp;

}

double GFkt::dudxh(int i, int j) const {
  double diff;
  if ((i > 0) && (i < grid->xsize()-1)){
    diff =  this->u(i+1,j) - u(i-1,j);
    return diff / (this->dxh * 2.0);
  }
  else if (i == 0)
  {
    diff = 3*this->u(i,j) - 4*this->u(i+1,j) + this->u(i+2,j);
    return diff / (3.0 * this->dxh);
  }
  else
  {
    diff = this->u(i-2,j) - 4*this->u(i-1,j) + 3 * this->u(i,j);
    return diff / (3.0 * this->dxh);
  }
}

double GFkt::dudyh(int i, int j) const {
  double diff;
  if ((j > 0) && (j < grid->ysize()-1)){
    diff =  this->u(i,j+1) - this->u(i,j-1);
    return diff / (this->dyh * 2.0);
  }
  else if (j == 0)
    {
      diff = 3*this->u(i,j) - 4*this->u(i,j+1) + this->u(i,j+2);
      return diff / (3.0 * this->dyh);
    }
  else
    {
      diff = this->u(i,j-2) - 4*this->u(i,j-1) + 3 * this->u(i,j);
      return diff / (3.0 * this->dyh);
    }
}

Point<double> GFkt::dxd( int i, int j) const {
  
  //Point<double> Dp1(3.0,4.1);
  //  Point<double> Dm1(0.9,1.2);

  Point<double> D;
  if ((i > 0) && (i < grid->xsize()-1)){

    Point<double> Dp1 = (*grid)(i+1,j);
    Point<double> Dm1 = (*grid)(i-1,j);
    D = (Dp1 - Dm1);
    D *= 0.5 * (1.0 / dxh);
  }
  else if (i == 0)
  {
    Point<double> D0,Dp1,Dp2;
    D0 = (*grid)(i,j); Dp1 = (*grid)(i+1,j); Dp2 = (*grid)(i+2,j);
    D = D0*3 - Dp1*4 + Dp2;
    D *= (1.0 / 3 / this->dxh);
  } else {
    Point<double> D0,Dm1,Dm2;
    D0 = (*grid)(i,j); Dm1 = (*grid)(i-1,j); Dm2 = (*grid)(i-2,j);
    D = D0*3 - Dm1*4 + Dm2;
    D *= (1.0 / 3 / this->dxh);
  }
  return D;
}

Point<double> GFkt::dyd( int i, int j) const {
  
  //Point<double> Dp1(3.0,4.1);
  //  Point<double> Dm1(0.9,1.2);

  Point<double> D;
  if ((j > 0) && (j < grid->ysize()-1)){

    Point<double> Dp1 = (*grid)(i,j+1);
    Point<double> Dm1 = (*grid)(i,j-1);
    D = (Dp1 - Dm1);
    D *= 0.5* (1.0 / dyh);
  }
  else if (j == 0)
    {
      Point<double> D0,Dp1,Dp2;
      D0 = (*grid)(i,j); Dp1 = (*grid)(i,j+1); Dp2 = (*grid)(i,j+1);
      D = D0*3 - Dp1*4 + Dp2;
      D *= (1.0 / 3 / this->dyh);
    } else {
    Point<double> D0,Dm1,Dm2;
    D0 = (*grid)(i,j); Dm1 = (*grid)(i,j-1); Dm2 = (*grid)(i,j-2);
    D = D0*3 - Dm1*4 + Dm2;
    D *= (1.0 / 3 / this->dyh);
   
  }
  return D;
}


GFkt GFkt::D0x() const {
  if (grid->grid_valid()) {
    // generate derivative in tmp
    // according to one of the possibilities above

    double J;
    Point<double> dX,dY;

    GFkt tmp(this->grid);
    for (int i = 0; i <= this->grid->xsize(); i++){
      for (int j = 0; j <= this->grid->ysize(); j++){

        dX = this->dxd(i,j);
        dY = this->dyd(i,j);

        J = dX.getX()*dY.getY() -
            dY.getX()*dX.getY();

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

GFkt GFkt::D0y() const {
  if (grid->grid_valid()) {
    // generate derivative in tmp
    // according to one of the possibilities above

    double J;
    Point<double> dX,dY;

    GFkt tmp(this->grid);
    for (int i = 0; i <= this->grid->xsize(); i++){
      for (int j = 0; j <= this->grid->ysize(); j++){

        dX = this->dxd(i,j);
        dY = this->dyd(i,j);

        J = dX.getX()*dY.getY() -
          dY.getX()*dX.getY();

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

GFkt GFkt::del(void) const {
  GFkt x_part(this->grid), y_part(this->grid);
  x_part = this->D0x();
  x_part = x_part.D0x();
  y_part = this->D0y();
  y_part = y_part.D0y();

  return x_part + y_part;
}


void GFkt::printMat(void) const {
  this->u.print_matrix();
}

//double GFkt::fun(double x, double y){
//  return sin(pow(x/10,2))*cos(x/10)+ y;
//}

double GFkt::fun(double x, double y){
  return x*x + 3.0*y*x + x/2.0;
}


void GFkt::fillMat(void) {
  Point<double> P;
  for (int i = 0; i <= this->grid->xsize(); i++){
    for (int j = 0; j <= this->grid->ysize(); j++){

      P = (*grid)(i,j);
      this->u(i,j) = fun(P.getX(),P.getY());
    }
  }
}

Point<double> GFkt::getGridVal(int i, int j) const{
  return (*grid)(i, j);
}
double GFkt::getFuncVal(int i, int j) const{
  return this->u(i,j);
}


