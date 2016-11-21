#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

#include "equation.h"

double R(int i){
    return 0;
}


int
func (double t, const double u[],double f[],
      void *params)
{
  (void)(t); /* avoid unused parameter warning */
  for (int i=1;i<=N-1;i++){
     f[i] = R(i)-A/(h*h)*u[i+1]+2*A/(h*h)*u[i]-A/(h*h)*u[i-1];
  }
  return GSL_SUCCESS;
}


// set y with Cauchy condition
void Cauchy(double y[]) {
    for(int i =0 ; i<N; i++) {
      if (i==N/2) {
	y[i] = 2; 
      } else {
	y[i] = 1;
      }
      //y[i] = (i==N/2)?2:1;
    }
  
}

// u_0(t)
double U0(double t) {
  return 1;
}
// u_N(t)
double UN(double t) {
  return 1;
}
