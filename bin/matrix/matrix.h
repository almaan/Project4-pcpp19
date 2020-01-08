#ifndef _MATRIX
#define _MATRIX

#include <iostream>
#include <string>
#include <memory>

class Matrix {
  int m; 
  int n;
  double * A;
public:

  Matrix(){} // default constructur
  ~Matrix();// default 

  // construct from dimension specifications
  Matrix(int m_, int n_); 

  // overload () operator for accession of elements
  double& operator()(int i, int j) { return A[i+j*m]; }
  const double operator()(int i, int j) const { return A[i+j*m]; }

  // copy and move constructors
  Matrix(const Matrix & B); 
  Matrix(Matrix&& B) noexcept;
  // overload assignment operator
  Matrix& operator=(const Matrix& B);
  Matrix& operator=(Matrix&& B);
  // overload arithmetic operators

  // matrix-matrix operations
  Matrix operator+(const Matrix& B) const;
  Matrix operator-(const Matrix &B) const;

  const Matrix& operator+=(const Matrix& B);
  const Matrix& operator-=(const Matrix& B);

  //matrix-scala operations
  Matrix operator+(const double& b) const ;
  Matrix operator-(const double& b) const ;
  Matrix operator*(const double& b) const;
  Matrix operator*=(const double& b) const ;

  // scalar-matrix operations
  friend Matrix operator*(double &b, const Matrix& B);
  friend Matrix operator+(Matrix &&A, const Matrix& B);
  friend Matrix operator-(Matrix &&A, const Matrix& B);


  // *--- Addintional Functions ---*

  // print elements
  void print_matrix(void) const;
  // write matrix to file
  void saveData(std::string out_dir = (std::string)"/tmp");

};

#endif
