#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

#include "equation.h"
//#include "equation1.h"

int
main (void)
{
 
  gsl_odeiv2_system sys = {func, jac, 2, 0};

  gsl_odeiv2_driver * d = 
    gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
				  1e-6, 1e-6, 0.0);
  int i;
  double * y = new double[2];
  Cauchy(y);

  for (i = 1; i <= N-1; i++)
    {
      double ti = i * t1 / 100.0;
      int status = gsl_odeiv2_driver_apply (d, &t, ti, y);

      if (status != GSL_SUCCESS)
	{
	  printf ("error, return value=%d\n", status);
	  break;
	}

      printf ("%.5e %.5e\n", t, y[i]);
    }

  gsl_odeiv2_driver_free (d);
  return 0;
}