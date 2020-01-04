#include <iostream>
#include <memory>
#include <cmath>

#include "GFkt.h"
#include "matrix.h"
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
  double diff =  this->u(i+1,j) - u(i-1,j);
  return diff / (this->dxh * 2.0);
}

double GFkt::dudyh(int i, int j) const {
  double diff =  this->u(i,j+1) - this->u(i,j-1);
  return diff / (this->dyh * 2.0);
   
}

GFkt GFkt::D0x() const {
  GFkt tmp(grid);
  if (grid->grid_valid()) {
    // generate derivative in tmp
    // according to one of the possibilities above

    double xh,yh,up1,um1,val,J;

    GFkt tmp(this->grid);
    for (int i = 1; i <= this->grid->xsize() -1; i++){
      for (int j = 1; j <= this->grid->ysize()-1; j++){
        xh = this->dxh*(double)i;
        yh = this->dyh*(double)j;

        J = this->grid->dxdrmap(xh,yh) *
          this->grid->dydsmap(xh,yh) -
          this->grid->dydrmap(xh,yh) *
          this->grid->dxdsmap(xh,yh);

        tmp.u(i,j) = (1.0 / J) *
                    (this->dudxh(i,j) *
                      this->grid->dydsmap(xh,yh) -
                      this->dudyh(i,j) *
                      this->grid->dydrmap(xh,yh));
      }
    }
  }
  return tmp;
}

void GFkt::printMat(void) const {
  this->u.print_matrix();
}

double GFkt::fun(double x, double y){
  return sin(pow(x/10,2))*cos(x/10)+ y;
}

void GFkt::fillMat(void) {
  for (int i = 1; i <= this->grid->xsize() -1; i++){
    for (int j = 1; j <= this->grid->ysize()-1; j++){
      this->u(i,j) = fun(i,j);
    }
  }
}

