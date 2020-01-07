#ifndef _CURVEBASE
#define _CURVEBASE




class Curvebase {

protected:
	double pmin; //lower boundary of curve
	double pmax; //upper boundary of curve
	double mid; //mid point of curve
	bool rev = true; // orientation of the curve, default positive
	double tol; //tolerance

	double length; //length of curve

	virtual double xp(double p) = 0; //returns original x curve coordinates
	virtual double yp(double p) = 0; //returns original ycurve coordinates
	virtual double dxp(double p) = 0; //returns original x curve derivative
	virtual double dyp(double p) = 0; //returns original y curve derivative

	double function(double p); //function to be integrated
	double integrate(double p, double lower); //arc length integral
	double solve(double s); //newton rhapson fixed point iteration solver
	
	double I1(double a, double b); //help-function for ASI
	double I2(double a, double b); //help-function for ASI


public:

	Curvebase(double p_min,
			  double p_max,
			  bool dir = true,
	 		  double tolerance = 1e-6);

	Curvebase(); //constructor
	~Curvebase(); //destructor
	Curvebase(const Curvebase &cb); //copy constructor
	double x(double s); //arc length parametrization
	double y(double s); //arc length parametrization
	virtual void setLength(void); //compute and set length of curve
	void printInfo(void); //print information about curve
	void reverse_orientation(void); //reverse curve orientation
	bool ori(void);


};

#endif
