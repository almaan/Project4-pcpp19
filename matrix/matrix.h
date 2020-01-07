#ifndef _MATRIX
#define _MATRIX

#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>

class Matrix {
  int m; 
  int n;
  double * A;
public:

  Matrix(int m_, int n_) : m(m_), n(n_),A(nullptr) {
    std::cout << "Construct Matrix from dims" << std::endl;
    if (m*n > 0) {
      A = new double[m*n];
      // std::fill(A,A+m*n,0.0);
      for (int i = 0; i < m*n; i++){
        A[i] = 0.0;
      }
      std::cout << "--> Filled matrix" << std::endl;
    }
  }

  Matrix(){};

  ~Matrix() { if (A != nullptr) delete [] A; A = nullptr; }

  double& operator()(int i, int j) { return A[i+j*m]; }

  const double operator()(int i, int j) const { return A[i+j*m]; }

  Matrix(const Matrix & B) : m(B.m), n(B.n), A(nullptr) {
    std::cout << "construct from Matrix" << std::endl;
    if (m*n > 0 ) {
      std::cout << "--> A : " << A <<  std::endl;
      std::cout << "-->Request New A" << std::endl;
      std::cout << "-->(m*n)" << m * n << std::endl;
      A = new double[m*n];
      std::cout << "-->Created New A" << std::endl;
      for (int i = 0; i < m; i++){
        for (int j = 0 ; j < n; j++) {
          A[i+m*j] = B.A[i+m*j];
        }
      }
    }
  }

  Matrix(Matrix&& B) noexcept : m(B.m) , n(B.n), A(B.A) {
    B.m = 0;
    B.n = 0;
    B.A = nullptr;
  }

  Matrix& operator=(const Matrix& B) {
    if (this !=&B){
      if (m*n != B.m*B.n){
        if (A!=nullptr){delete [] A;}
        if (B.A != nullptr){A = new double[B.n*B.m];}
      }
      m = B.m; n = B.n;
      // std::copy(B.A,B.A+m*n,A);
      for (int i = 0; i < m; i++){
        for (int j = 0 ; j < n; j++) {
          A[i+m*j] = B.A[i+m*j];
        }
    }
    }
    return *this;
  }

  Matrix& operator=(Matrix&& B) {
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

  Matrix operator+(const Matrix& B) const {
    Matrix C(this->m,this->n);
    int idx;
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx] + B.A[idx];
      }
    }
    return C;
  }

  const Matrix& operator+=(const Matrix& B){
    int idx;
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        this->A[idx] += B.A[idx];
      }
    }
    return *this;
  }

  Matrix operator-(const Matrix &B) const {
      int idx;
      Matrix C(this->m,this->n);
      for (int i = 0; i < this->m; i++){
        for (int j = 0; j < this->n; j++) {
          idx = i + j*this->m;
          C.A[idx] = this->A[idx] - B.A[idx];
        }
      }
      return C;
  }

  const Matrix& operator-=(const Matrix& B){
    int idx;
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        this->A[idx] -= B.A[idx];
      }
    }
    return *this;
  }

  Matrix operator*(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]*b;
      }
    }
    return C;
  }


  Matrix operator+(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]+b;
      }
    }
    return C;
  }

  Matrix operator-(const double& b) const {
    int idx;
    Matrix C = Matrix(this->m,this->n);
    for (int i = 0; i < this->m; i++){
      for (int j = 0; j < this->n; j++) {
        idx = i + j*this->m;
        C.A[idx] = this->A[idx]-b;
      }
    }
    return C;
  }


  Matrix operator*=(const double& b) const {
    for (int i = 0; i < this->m*this->n; i++){
        this->A[i]*= b;
    }
    return *this;
  }

  friend Matrix operator*(double &b, const Matrix& B);
  friend Matrix operator+(Matrix &&A, const Matrix& B);
  friend Matrix operator-(Matrix &&A, const Matrix& B);


  void print_matrix(void) const;
  void fill_matrix(void);

  void saveData(std::string out_dir = (std::string)"/tmp");

};

#endif
