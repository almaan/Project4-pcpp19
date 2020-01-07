#ifndef _POINT
#define _POINT

#include <iostream>
#include <iomanip>

// class for two dimensional points
// of arbitrary types
template < typename T >
class Point {

private:
  T x_; // first coordinate
  T y_; // second coordinate

public:

  // *--- Constructors and Destructors ---*
  Point() : x_(0), y_(0) {}
  Point(T x, T y) : x_(x), y_(y) {}
  ~Point() {}

  Point(const Point& P) : x_(P.x_), y_(P.y_) {}

  // *--- Arithmetic operator overloading ---*
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

  // quick call to print point
  void printPoint(){
    std::cout << "( " << this->x_ << ", " << this->y_ << " )" << std::endl;
  }

  // set first coordinate
  void setX(T x) {
    this->x_ = x;
  }

  // set second coordinate
  void setY(T y) {
    this->y_ = y;
  }

  // get first coordinate of point
  const T getX(void) const {
    return this->x_;
  }

  // get second coordinate of point
  const T getY(void) const {
    return this->y_;
  }

  // declare overloaded ostream operator as friend
  // for print compatibility
  template<typename S>
  friend std::ostream& operator<<(std::ostream& os, const Point<S>& P);
};

// osteam friend function 
template<typename S>
std::ostream& operator<<(std::ostream& os, const Point<S>& P)
{
  os << "( " << P.getX() <<  " , " << P.getY() << " )";
  return os;
}

#endif



