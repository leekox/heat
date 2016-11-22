#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <math.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

double len = 1; //длина стержня
int N = 200; //число разбиений по пространству
double h = len / N; //шаг по пространству
double t1 = 20.0; //конечное время

double ff(double t, const double y) //функция из правой части
{
  return 0;
}

int //матрица системы
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

int //якобиан
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

//функция, рисующая пачку картинок
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

int
main (void)
{ 
  
  double A = 1;	//коэффициент
  gsl_odeiv2_system sys = {func, 
    jac, N, &A}; //третий аргумент - размерность системы

  gsl_odeiv2_driver * d = 
    gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
				  1e-6, 1e-6, 0.0); //стартовый шаг, абсолютная и относительная ошибки
  int i;
  double t = 0.0;
  double y[N];
  double x[N];
  //заполнение массива иксов
  for(int i = 0; i < N; i++)
  {
    x[i] = len / N * i;
  }
  
  //граничные и начальные условия
  y[0] = 0;
  for(int k = 1; k < N - 1; k++)
  {
    if(k > 90 && k < 100)
    {
      y[k] = 10;
    }else
    { 
      y[k] = 0;
    }
  }
  y[N - 1] = 0;
  //вывод в начальный момент времени
  saveResult(x, y, 0);
  //решение системы
  for (i = 1; i <= 100; i++)
    {
      double ti = i * t1 / 100000.0; 				//большую детализацию по времени?
      int status = gsl_odeiv2_driver_apply (d, &t, ti, y);

      if (status != GSL_SUCCESS)
	{
	  printf ("error, return value=%d\n", status);
	  break;
	}
	//после каждого шага сохраняем
      saveResult(x, y, i);
    }
  //освобождаем память
  gsl_odeiv2_driver_free (d);
  return 0;
}