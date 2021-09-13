#include <iostream>
#include <omp.h>

int main()
{
    std::cout << omp_get_num_procs();
    int a[100], b[100], c[100], d[100];

#pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        a[i] = i;
        b[i] = i;
        c[i] = 2 * i;
        std::cout << omp_get_thread_num();
        std::cout << omp_get_num_threads();
    }

#pragma opm parallel for
    for (int i = 0; i < 100; i++) {
        c[i] = a[i] + b[i] + c[i];
    }
    std::cout << a[50] << "/n";
}