#include <iostream>
#include <string>
#include "matrix.h"

// initialize by dimensions
Matrix::Matrix(int m_, int n_) : m(m_), n(n_),A(nullptr) {
  if (m*n > 0) {
    A = new double[m*n];
    std::fill(A,A+m*n,0.0);
  }
}

// destructor, remove dynamically allocated
//object
Matrix::~Matrix() {
  if (A != nullptr) {delete [] A;}
  A = nullptr;
}

// Copy constructor
Matrix::Matrix(const Matrix & B) : m(B.m), n(B.n), A(nullptr) {
  if (m*n > 0 ) {
    A = new double[m*n];
    std::copy(B.A,B.A+m*n,A); 
  }
}

// for move operation
Matrix::Matrix(Matrix&& B) noexcept : m(B.m) , n(B.n), A(B.A) {
  B.m = 0;
  B.n = 0;
  B.A = nullptr;
}

// assignment operator
Matrix& Matrix::operator=(const Matrix& B) {
    if (this !=&B){
      if (m*n != B.m*B.n){
        if (A!=nullptr){delete [] A;}
        if (B.A != nullptr){A = new double[B.n*B.m];}
      }
      m = B.m; n = B.n;
      std::copy(B.A,B.A+m*n,A);
    }
    return *this;
  }

// move assignment operator
// for chaining
Matrix& Matrix::operator=(Matrix&& B) {
    m = B.m; n = B.n;
    if (A != nullptr) {
      delete [] A; 
      A = nullptr;
    }

    A = B.A;
    B.m = B.n = 0;
    delete [] B.A;
    B.A = nullptr;
    return *this;
  }

// addition operator, Matrix-Matrix
Matrix Matrix::operator+(const Matrix& B) const {
    Matrix C(this->m,this->n);
    int idx;
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx] + B.A[idx];
      }
    }
    return C;
  }

// plus equal operator Matrix-Matrix
const Matrix& Matrix::operator+=(const Matrix& B){
    int idx;
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        this->A[idx] += B.A[idx];
      }
    }
    return *this;
  }

// subtraction operator Matrix-Matrix
Matrix Matrix::operator-(const Matrix &B) const {
      int idx;
      Matrix C(this->m,this->n);
      for (int j = 0; j < this->n; j++){
        for (int i = 0; i < this->m; i++) {
          idx = i + j*this->m;
          C.A[idx] = this->A[idx] - B.A[idx];
        }
      }
      return C;
  }

// minus equal to operator Matrix-Matrix
const Matrix& Matrix::operator-=(const Matrix& B){
    int idx;
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        this->A[idx] -= B.A[idx];
      }
    }
    return *this;
  }

// Multiplication operator Matrix-scalar
Matrix Matrix::operator*(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]*b;
      }
    }
    return C;
  }

// Addition operator Matrix-scalar
Matrix Matrix::operator+(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]+b;
      }
    }
    return C;
  }
// Subtraction operator Matrix-scalar
Matrix Matrix::operator-(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int j = 0; j < this->n; j++){
      for (int i = 0; i < this->m; i++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]-b;
      }
    }
    return C;
  }

// times equal operator Matrix-scalar
Matrix Matrix::operator*=(const double& b) const {
    for (int i = 0; i < this->m*this->n; i++){
        this->A[i]*= b;
    }
    return *this;
  }

// Multiplication operator scalar-Matrix
Matrix operator*(double &b, const Matrix& B){
  Matrix C = Matrix(B.m,B.n);
  int idx;
  for (int j = 0; j < B.n; j++){
    for (int i = 0; i < B.m; i++) {
      idx = i + j*B.m;
      C.A[idx] = B.A[idx]*b;
    }
  }
  return C;
}

// operator addition with move (for chaining)
Matrix operator+(Matrix &&A, const Matrix& B ) {
  A += B;
  return std::move(A);
}
// operator subtraction with move (for chaining)
Matrix operator-(Matrix &&A, const Matrix& B ) {
    A -= B;
    return std::move(A);
}

// print matrix in structured manner
void Matrix::print_matrix(void) const {
  for (int i = 0; i < this->m; i++){
    std::cout << "[ ";
    for (int j = 0; j <this->n; j++){
      std:: cout <<  this->A[i + j*this->m] << " ";
    }
    std::cout << "]\n";
  }
}

// write matrix to file
void Matrix::saveData(std::string out_dir) {
	std::string vals_outname = out_dir + "/values.bin";
	FILE *fp_vals;
	fp_vals =fopen(vals_outname.c_str(),"wb");
	fwrite(this->A,sizeof(double),this->m*this->n,fp_vals);
	fclose(fp_vals);
}
