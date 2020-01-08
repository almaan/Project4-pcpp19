#ifndef _GFKT
#define _GFKT

#include <iostream>
#include <memory>
#include <string>

#include "../matrix/matrix.h"
#include "../dom/domain.h"

class GFkt {

private:

  Matrix u;  // matrix to hold grid function values
  std::shared_ptr<Domain> grid; // domain over which function is defined
  double dxh, dyh; // grid size 

public:

  //*--- Constructor and Destructors  ---*

  // Destructor
  ~GFkt(){};
  // Construct from Domain
  GFkt( std::shared_ptr<Domain> grid_);
  // Copy constructor
  GFkt(const GFkt& U);


  // *--- Arithmetic operators ---*

  // overload assignment operator
  GFkt& operator=(const GFkt& U);

  // gridfun - gridfun operations
  GFkt operator+(const GFkt& U) const;
  GFkt operator-(const GFkt& U) const;
  GFkt operator*(const GFkt& U) const;
  GFkt operator/(const GFkt& U) const;

  // gridfun - scalar operations 

  GFkt operator*(const double& x) const;
  GFkt operator+(const double& x) const;
  GFkt operator-(const double& x) const;
  GFkt operator/(const double& x) const;

  // *--- Additional Methods ---*

  // error handling
  GFkt error() const;
  // set grid size
  void setDelta(void);

  // differential operator helper functions
  double dudxh(int i, int j) const;
  double dudyh(int i, int j) const;
  Point<double> dxd(int i, int j) const;
  Point<double> dyd(int i, int j) const;

  // differential operators
  GFkt D0x() const; //partial_x
  GFkt D0y() const; //partial_y
  GFkt del() const; //laplacian

  // print function values
  void printMat(void) const;
  // fill matrix with based on provided function
  void fillMat(double fun(double,double));

  // get coordinates of grid point
  Point<double> getGridVal(int i, int j) const;
  // get function value of grid point
  double getFuncVal(int i, int j) const;

  //save data
  void saveData(std::string out_dir = (std::string)"/tmp");

};

#endif
