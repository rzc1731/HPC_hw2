#include <stdio.h>
#include <cmath>
#include <iostream>
#include "utils.h"
#ifdef _OPENMP
#include <omp.h>
#endif

int num_t = 4;

int main(int argc, char* argv[]) {
  const int max_iter = 100;
  const double residual_factor = 0.000001;
  int N = 100;

  if (argc == 1) {
    #ifdef _OPENMP
    std::cout << "Run with default N=100, Number_of_threads=4 if openmp." << std::endl;
    #else
    std::cout << "Run with default N=100, Serial version." << std::endl;
    #endif
    std::cout << "Usage: " << argv[0] << " N Number_of_threads" << std::endl;
  } else {
    N = atoi(argv[1]);

    #ifdef _OPENMP
    num_t = atoi(argv[2]);
    std::cout << "There are " << num_t << " threads used." << std::endl;
    #else
    num_t = 1;
    std::cout << "Serial version." << std::endl;
    #endif
  }


  int Nt = N + 2; // add extra two rows and two columns to avoid edge cases

  Timer time;
	time.tic();

  double *u = (double*)calloc(sizeof(double), Nt*Nt);
  double h = 1.0 / (N + 1);
  double hsq = h * h;
  double hsq_inv = 1.0 / hsq;

  double res = 0;

  double res_start;

  bool stop_flag = 0;

  #pragma omp parallel num_threads(num_t)
  {
    for (int k = 1; k <= max_iter; k++) {
      if (stop_flag) {
        continue;
      }

      //update red points
      #pragma omp for
      for (int i = Nt+1; i <= Nt*Nt-Nt-1; i+=2) {
        if (i%Nt != 0 && i%Nt != Nt-1) {
          u[i] = 0.25 * (hsq + u[i-Nt] + u[i-1] + u[i+1] + u[i+Nt]);
        }
      }

      //update black points
      #pragma omp for
      for (int i = Nt+2; i <= Nt*Nt-Nt-1; i+=2) {
        if (i%Nt != 0 && i%Nt != Nt-1) {
          u[i] = 0.25 * (hsq + u[i-Nt] + u[i-1] + u[i+1] + u[i+Nt]);
        }
      }

      // compute residual
      #pragma omp for reduction(+:res)
      for (int i = Nt+1; i <= Nt*Nt-Nt-1; i++) {
        if (i%Nt != 0 && i%Nt != Nt-1) {
          // f(x,y) is always 1
          double temp = 1.0 + (u[i-Nt] + u[i-1] - 4.0 * u[i] + u[i+1] + u[i+Nt]) * hsq_inv;
          res += temp * temp;
        }
      }

      #pragma omp single
      {
        double res_norm = sqrt(res);
        if (k == 1) {
          res_start = res_norm * residual_factor;
          std::cout << "res_start = " << res_start << std::endl;
        }
        std::cout << "iter #: " << k << ", residual_norm = " << res_norm << std::endl;
        if (res_norm <= res_start) {
          std::cout << "Goal reached after " << k << " iterations. Final Residual: " << res_norm << std::endl;
          stop_flag = true;
        }
        res = 0;
      }
    }
  }

  free(u);

  std::cout << "Computation time: " << time.toc() << std::endl;

  return 0;
}
