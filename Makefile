CC=g++
CFLAG=-O3
MFLAG=-march=native
OPT=-std=c++11
OPENMP=-fopenmp
EXECS = MMult1 val_test01_solved val_test02_solved omp_solved2 omp_solved3 omp_solved4 omp_solved5 omp_solved6 jacobi2D-omp gs2D-omp

all: ${EXECS}

MMult1: MMult1.cpp
	${CC} ${OPENMP} ${CFLAG} ${OPT} ${MFLAG} $^ -o MMult1

val_test01_solved: val_test01_solved.cpp
	${CC} ${CFLAG} $^ -o val_test01_solved

val_test02_solved: val_test02_solved.cpp
	${CC} ${CFLAG} $^ -o val_test02_solved

omp_solved2: omp_solved2.c
	${CC} ${OPENMP} ${CFLAG} $^ -o omp_solved2

omp_solved3: omp_solved3.c
	${CC} ${OPENMP} ${CFLAG} $^ -o omp_solved3

omp_solved4: omp_solved4.c
	${CC} ${OPENMP} ${CFLAG} $^ -o omp_solved4

omp_solved5: omp_solved5.c
	${CC} ${OPENMP} ${CFLAG} $^ -o omp_solved5

omp_solved6: omp_solved6.c
	${CC} ${OPENMP} ${CFLAG} $^ -o omp_solved6

jacobi2D-omp: jacobi2D-omp.cpp
	${CC} ${OPENMP} ${CFLAG} ${OPT} $^ -o jacobi2D-omp

gs2D-omp: gs2D-omp.cpp
	${CC} ${OPENMP} ${CFLAG} ${OPT} $^ -o gs2D-omp

clean:
	rm -f ${EXECS}
