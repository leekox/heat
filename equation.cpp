#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "equation.h"

double ff(double t, const double y) //функция из правой части
{
  return 0;
}

int
jac (double t, const double y[], double *dfdy, 
     double dfdt[], void *params)
{
  int i, j = 0;
  
  (void)(t); /* avoid unused parameter warning */
  double A = *(double *)params;
  gsl_matrix_view dfdy_mat 
    = gsl_matrix_view_array (dfdy, N, N);
  gsl_matrix * m = &dfdy_mat.matrix; 
  
  for(i; i < N; i++)
  {
    for(j; j < N; j++)
    {
      if(i == j)
      {
	gsl_matrix_set (m, i, j, -2 * A / (h*h));
      }
      if(i == j - 1 || i == j + 1)
      {
	gsl_matrix_set (m, i, j, A / (h*h));
      }else
      {
	gsl_matrix_set (m, i, j, 0);
      }
    }
  }
  for(int k = 0; k < N; k++)
  {
    dfdt[k] = 0.0; //вторые производные??
  }
  return GSL_SUCCESS;
}

int
func (double t, const double y[], double f[], 
      void *params)
{
  (void)(t); /* avoid unused parameter warning */
  double A = *(double *)params;
  
  f[0] = 0; //теплоизоляция
  for(int i = 1; i < N - 1; i++)
  {
    f[i] = A / (h*h) * (y[i + 1] - 2 * y[i] + y[i - 1]) + ff(t, y[i]);
  }
  f[N - 1] = 0;
  return GSL_SUCCESS;
}

void saveResult(const double x[], const double y[], int k) {
  pid_t pid;
  int status;
  //записываем скрипт для gnuplot в файл
  FILE *scrpt = fopen("/home/user/Desktop/plot.txt", "w");
  fprintf(scrpt, "set terminal png\n");
  fprintf(scrpt, "set output '/home/user/Desktop/heat_plot/%d.png'\n", k);
  fprintf(scrpt, "set yrange [0:5]\n");
  fprintf(scrpt, "plot '/home/user/Desktop/result.txt' using 1:2 with lines");
  fclose(scrpt);
  FILE *res = fopen("/home/user/Desktop/result.txt", "w");
  //пишем в файл данные
  fprintf (res, "#\tx\t\t\tu\n");
  for(int i = 0; i < N; i++)
  {
    fprintf (res, "%.5e\t%.5e\n", x[i], y[i]);
  }
  
  fclose(res);
  //запускаем gnuplot, он рисует график
  pid = fork();
  if(pid > 0) {
    wait(&status);
  }else {
    execl("/usr/bin/gnuplot", "gnuplot", 
	  "-persist", "/home/user/Desktop/plot.txt", NULL);
  }
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
