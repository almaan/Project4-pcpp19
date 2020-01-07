#ifndef _GFKT
#define _GFKT

#include <iostream>
#include <memory>
#include <string>

#include "matrix/matrix.h"
#include "dom/domain.h"

class GFkt {

private:
  Matrix u;
  std::shared_ptr<Domain> grid;
  // Domain * grid;
  double dxh, dyh;

public:

  ~GFkt(){};

  GFkt( std::shared_ptr<Domain> grid_) :
  // GFkt( Domain * grid_) :

    u(grid_->xsize(),
      grid_->ysize()),
    grid(grid_) {

    std::cout << "Construct from domain" << std::endl;
    setDelta();
  }

  GFkt(const GFkt& U) : u(U.u), grid(U.grid) {
    std::cout << "Construct from GFkt" << std::endl;
    setDelta();
  }

  void setDelta(void){
    this->dxh = 1.0 / ((double)this->grid->xsize() -1.0);
    this->dyh = 1.0 / ((double)this->grid->ysize() -1.0);
  }

  GFkt& operator=(const GFkt& U);
  GFkt operator+(const GFkt& U) const;
  GFkt operator-(const GFkt& U) const;
  GFkt operator*(const GFkt& U) const;
  GFkt operator/(const GFkt& U) const;
  GFkt operator*(const double& x) const;
  GFkt operator+(const double& x) const;
  GFkt operator-(const double& x) const;
  GFkt operator/(const double& x) const;

  GFkt error() const {
    std::cout << "ERROR : Cannot use grid";
    std::cout << "operations on different grids" << std::endl;
    return *this;
  }

  double dudxh(int i, int j) const;
  double dudyh(int i, int j) const;
  Point<double> dxd(int i, int j) const;
  Point<double> dyd(int i, int j) const;
  GFkt D0x() const;
  GFkt D0y() const;
  GFkt del() const;
  void printMat(void) const;
  void fillMat(double fun(double,double));

  Point<double> getGridVal(int i, int j) const;
  double getFuncVal(int i, int j) const;

  void saveData(std::string out_dir = (std::string)"/tmp");

};

#endif
