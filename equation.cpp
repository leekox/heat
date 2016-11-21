#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

#include "equation.h"

int
func (double t, const double u[],double f[],
      void *params)
{
  (void)(t); /* avoid unused parameter warning */
  double R= *(double *)params;
  for (int i=1;i<=N-1;i++){
     f[i] = 0;
    
  }
  return GSL_SUCCESS;
}

int
jac (double t, const double y[], double *dfdy, 
     double dfdt[], void *params
  int A;
  for (int i = 1; i <=N-1; i++)
      {
      for (int j = 1; j <=N-1; j++)
        {
          if (i==j)
          {
          otvet= (2*A)/(h*h));
          }
          else if (i==j+1)
          {
          otvet = (-A/(h*h));
          }
=          else if (i==j-1)
          {
          otvet = (-A/(h*h));
          }
          else
          {
          otvet = 0;
          }
          gsl_matrix_set(m,i,j,otvet);
        }
      }

{
  (void)(t); /* avoid unused parameter warning */
  double mu = *(double *)params;
  gsl_matrix_view dfdy_mat 
    = gsl_matrix_view_array (dfdy, N, N);
  gsl_matrix * m = &dfdy_mat.matrix;
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
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
