#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <gsl/gsl_blas.h>
#include "my_timer.h"

#define N 5

int cal_index_offset(int ndim, int *darr, int *position) {
    int size = 1;
    int offset = 0;
    for(int i = ndim-1; i >= 0; i--) {
        offset += position[i] * size;
        size *= darr[i];
    }
    return offset;
}

void print_mat_recursive(double *mat, int ndim, int *darr, int pdim, int *position) {
    if(pdim >= ndim) {
        int index_offset = cal_index_offset(ndim, darr, position);
        printf("%lf ", mat[index_offset]);
        return;
    }

    printf("[ ");
    for(int i = 0; i < darr[pdim]; i++) {
        position[pdim] = i;
        print_mat_recursive(mat, ndim, darr, pdim+1, position);
    }
    printf("]");
}

void print_mat(double *mat, int ndim, ...) {
    int position[ndim];
    int darr[ndim];

    va_list args;
    va_start(args, ndim);
    for(int i = 0; i < ndim; i++) {
        darr[i] = va_arg(args, int);
    }
    va_end(args);

    print_mat_recursive(mat, ndim, darr, 0, position);
    printf("\n");
}

void mm_brute_force(double *a, double *b, double *c, int p, int q, int r) {
    for(int i = 0; i < p; i++) {
        for(int k = 0; k < q; k++) {
            for(int j = 0; j < r; j++) {
                c[i * r + j] += a[i * q + k] * b[k * r + j];
            }
        }
    }
}

void mm_dgemm(double *a, double *b, double *c, int p, int q, int r) {
    int l = p;
    int m = q;
    int n = r;

    int lda = m;
    int ldb = n;
    int ldc = n;

    double alpha = 1.0;
    double beta = 0.0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
            l, n, m, alpha, a, lda, b, ldb, beta, c, ldc);
}


void example_dgemm() {
    int l, m, n;
    l = m = n = N;

    double *mat_a = (double*)malloc(sizeof(double) * l * m);
    double *mat_b = (double*)malloc(sizeof(double) * m * n);
    double *mat_c = (double*)malloc(sizeof(double) * l * n);

    for(int i = 0; i < (l*m); i++) {
        mat_a[i] = i + 1;
    }

    for(int i = 0; i < (m*n); i++) {
        mat_b[i] = i + 1;
    }

    for(int i = 0; i < (l*n); i++) {
        mat_c[i] = 0;
    }

    my_timer_t mt;
    my_timer_start(&mt);
    mm_dgemm(mat_a, mat_b, mat_c, l, m, n);
    my_timer_end(&mt);
    my_timer_print(&mt);
    printf("\n");

    print_mat(mat_a, 2, l, m);
    printf("\n");
    print_mat(mat_b, 2, m, n);
    printf("\n");
    print_mat(mat_c, 2, l, n);
    printf("\n");

    free(mat_a);
    free(mat_b);
    free(mat_c);
}

int main(void) {
    example_dgemm();
    return 0;
}
