#include <iostream>
#include <cmath>


template < typename T >
class Point {

private:
  T x_;
  T y_;

public:

  Point() : x_(0), y_(0) {}
  Point(T x, T y) : x_(x), y_(y) {}
  ~Point() {}

  Point(const Point& P) : x_(P.x_), y_(P.y_) {
  }

  Point& operator=(const Point& P){
    if (this == &P) {
      return *this; 
    } else {
      this->x_ = P.x_;
      this->y_ = P.y_;
    }
    return *this;
  }

  const Point operator+(const Point& P) const {
    Point tmp;
    tmp.x_ = this->x_ + P.x_;
    tmp.y_ = this->y_ + P.y_;
    return tmp;
  }

  const Point operator-(const Point& P) const {
    Point tmp;
    tmp.x_ = this->x_ - P.x_;
    tmp.y_ = this->y_ - P.y_;
    return tmp;
  }

  const Point operator*(const Point& P) const{
    Point tmp;
    tmp.x_ = this->x_ * P.x_;
    tmp.y_ = this->y_ * P.y_;
    return tmp;
  }

  const Point operator+(const T& x) const {
    Point tmp;
    tmp.x_ = this->x_ + x;
    tmp.y_ = this->y_ + x;
    return tmp;
  }

  const Point operator-(const T& x) const {
    Point tmp;
    tmp.x_ = this->x_ - x;
    tmp.y_ = this->y_ - x;
    return tmp;
  }

  const Point operator*(const T& x) const{
    Point tmp;
    tmp.x_ = this->x_ * x;
    tmp.y_ = this->y_ * x;
    return tmp;
  }


  const Point& operator*=(const Point& P){
    this-> x_ *= P.x_;
    this-> y_ *= P.y_;
    return *this;
  }

  const Point& operator+=(const Point& P){
    this->x_ += P.x_;
    this->y_ += P.y_;
    return *this;
  }

  const Point& operator-=(const Point& P){
    this->x_ -= P.x_;
    this->y_ -= P.y_;
    return *this;
  }

  const Point& operator*=(const T& x){
    this-> x_ *= x;
    this-> y_ *= x;
    return *this;
  }

  const Point& operator+=(const T& x){
    this->x_ += x;
    this->y_ += x;
    return *this;
  }

  const Point& operator-=(const T& x){
    this->x_ -= x;
    this->y_ -= x;
    return *this;
  }



  void printPoint(){
    std::cout << "( " << this->x_ << ", " << this->y_ << " )" << std::endl;
  }

};


int main(){

  Point<double> P(3.0,4.0);
  Point<double> Q(1.5,1.2);

  Point<double> A = P + Q;
  A.printPoint();
  A += 3;
  A.printPoint();
  A = A * 9;
  A.printPoint();


}
