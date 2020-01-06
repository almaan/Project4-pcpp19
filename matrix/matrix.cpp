#include <iostream>
#include <string>
#include "matrix.h"


Matrix operator*(double &b, const Matrix& B){
  Matrix C = Matrix(B.m,B.n);
  int idx;
  for (int i = 0; i < B.m; i++){
    for (int j = 0; j < B.n; j++) {
      idx = i + j*B.m;
      C.A[idx] = B.A[idx]*b;
    }
  }
  return C;
}

Matrix operator+(Matrix &&A, const Matrix& B ) {
  A += B;
  return std::move(A);
}

Matrix operator-(Matrix &&A, const Matrix& B ) {
    A -= B;
    return std::move(A);
}

void Matrix::print_matrix(void) const {
  for (int i = 0; i < this->m; i++){
    std::cout << "[ ";
    for (int j = 0; j <this->n; j++){
      std:: cout <<  this->A[i + j*this->m] << " ";
    }
    std::cout << "]\n";
  }
}

void Matrix::saveData(std::string out_dir) {

	//names of files to be saved
	std::string vals_outname = out_dir + "/values.bin";
	//save x-coordinate file
	FILE *fp_vals;
	fp_vals =fopen(vals_outname.c_str(),"wb");
  std::cout << this->m << " " << this->n << std::endl;
	fwrite(this->A,sizeof(double),this->m*this->n,fp_vals);
	fclose(fp_vals);

}
