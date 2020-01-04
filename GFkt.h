#ifndef _GFKT
#define _GFKT

#include <iostream>

#include "matrix.h"
#include "dom/domain.h"

class GFkt {

private:
  Matrix u;
  Domain *grid;
  double dxh, dyh;

public:

  GFkt(Domain * grid_) :
    u(grid_->xsize()+1,grid_->ysize()+1),
    grid(grid_) {setDelta();}

  GFkt(const GFkt& U) : u(U.u), grid(U.grid) {setDelta();}

  void setDelta(void){
    this->dxh = 1.0 / ((double)this->grid->xsize() -1.0);
    this->dyh = 1.0 / ((double)this->grid->ysize() -1.0);
  }

  GFkt operator+(const GFkt& U) const;
  GFkt operator-(const GFkt& U) const;
  GFkt operator*(const GFkt& U) const;
  GFkt operator/(const GFkt& U) const;
  GFkt operator*(const double& x) const;
  GFkt operator+(const double& x) const;
  GFkt operator-(const double& x) const;
  GFkt operator/(const double& x) const;

  //virtual double fun(double x, double y) const;

  GFkt error() const {
    std::cout << "ERROR : Cannot use grid";
    std::cout << "operations on different grids" << std::endl;
    return *this;
  }

  double dudxh(int i, int j) const;
  double dudyh(int i, int j) const;
  GFkt D0x() const;
  void printMat(void) const;
  void fillMat(void);
  double fun (double x, double y);

};


#endif
