//
// Created by jyh on 19-3-23.
//

#ifndef CODECRAFT_2019_UTILS_H
#define CODECRAFT_2019_UTILS_H

#include <iostream>
#include <numeric>

using namespace std;

template<typename T>
T max(const T *a, int n) {
    T tmp = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > tmp)
            tmp = a[i];
    }
    return tmp;
}

template<typename T>
T min(const T *a, int n) {
    T tmp = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < tmp)
            tmp = a[i];
    }
    return tmp;
}

template<typename T>
double ave(const T *a, int n) {
    T sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return double(sum) / double(n);
}


template<typename T>
T sum(const T *a, int n) {
    T su = accumulate(a, a + n, 0);
    return su;
}

template<typename T>
double mid(const T *a, int n) {
    double tmp;
    if (n % 2 == 0)
        tmp = (a[n / 2 - 1] + a[n / 2]) / 2.0;
    else
        tmp = a[(n - 1) / 2];
    return tmp;

}

template<typename T>
double variance(T *a, int n) {
    double sum = 0;
    double average = ave(a, n);
    for (int i = 0; i < n; i++)
        sum = (a[i] - average) * (a[i] - average);
    return sum / n;
}


#endif //CODECRAFT_2019_UTILS_H
