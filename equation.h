#pragma once
// right part U
int
func (double t, const double y[], double f[],
      void *params);

// jacobian dUi/dUj
int
jac (double t, const double y[], double *dfdy, 
     double dfdt[], void *params);


// set y with Cauchy condition
void Cauchy(double y[]);

// u_0(t)
double U0(double t);

// u_N(t)
double UN(double t);

const int N = 100;
const int A = 1;
const double t=0.0;
const double t1=100.0;
const double h=0.1;

