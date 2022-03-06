#include <iostream>
#include <vector>
#include <windows.h>
#include <cmath>


struct polyn {
    double* coeffs;
    int degree;
    double x;
    double value;
    polyn() {
        x = 0;
        coeffs = nullptr;
        value = 0;
        degree = 0;
    }
    polyn(double _x, double* _coeffs, int _degree) {
        x = _x;
        degree = _degree;
        coeffs = _coeffs;
        value = 0;
    }
};


DWORD WINAPI polynom(LPVOID v) {
    polyn* a = (polyn*)v;
    for (int i = 0; i < a->degree + 1; i++) {
        a->value += a->coeffs[i] * pow(a->x, i);
        Sleep(15);
    }
    std::cout << "p(" << a->x << ") = " << a->value << std::endl;
    return 0;
}



int main() {
    int deg_of_numerator  = 0;
    std::cout << "The degree_numerator of the polynom: " << std::endl;
    std::cin >> deg_of_numerator;
    std::cout << "The coefficients_numerator of the polynom: " << std::endl;
    double* coef_numer = new double[deg_of_numerator + 1];
    double n = 0;
    for (int i = 0; i < deg_of_numerator + 1; i++) {
        std::cin >> n;
        coef_numer[i] = n;
        std::cout << std::endl;
    }
    int deg_of_denomenator = 0;
    std::cout << "The degree_denominator of the polynom: " << std::endl;
    std::cin >> deg_of_denomenator;
    std::cout << "The coefficients_denominator of the polynom: " << std::endl;
    double* coef_denom = new double[deg_of_denomenator + 1];
    double k = 0;;
    for (int i = 0; i < deg_of_denomenator + 1; i++) {
        std::cin >> k;
        coef_denom[i] = k;
    }
    double x;
    std::cout << "value of x:" << std::endl;
    std::cin >> x;
    HANDLE thread[2];
    DWORD thrId[2];
    polyn* polynoms = new polyn[2];
    polynoms[0] = polyn(x, coef_numer, deg_of_numerator);
    polynoms[1] = polyn(x, coef_denom, deg_of_denomenator);
    thread[0] = CreateThread(NULL, 0, polynom, (void*)&polynoms[0], 0, &thrId[0]);
    thread[1] = CreateThread(NULL, 0, polynom, (void*)&polynoms[1], 0, &thrId[1]);
    WaitForMultipleObjects(2, thread, TRUE, INFINITE);
    std::cout << "f(" << x << ") = " << polynoms[0].value / polynoms[1].value;
}

