#include <iostream>
#include <vector>
#include <windows.h>
#include <cmath>

class Polynom {
private:
    double* coeffs;
    int degree;
    double x;
    double value;
public:
    Polynom() {
        coeffs = nullptr;
        degree = 0;
        x = 0;
        value = 0;
    }
    Polynom(double _x, double* _coeffs, int _degree) {
        x = _x;
        degree = _degree;
        coeffs = _coeffs;
        value = 0;
    }
    int getDegree() {
        return this->degree;
    }
    double getValue() {
        return this->value;
    }
    void setValue(int new_value) {
        this->value = new_value;
    }
    double getX() {
        return this->x;
    }
    int getDegree() {
        return this->degree;
    }
    double* getCoeffs() {
        return this->coeffs;
    }
};

class Calculate_Polynom {
private:
    Polynom numerator;
    Polynom denomenator;
    double x;
    double answer;
    HANDLE thread[2];
    DWORD threadId[2];

public:

    Calculate_Polynom() {
        std::cout << "value of x:" << std::endl;
        std::cin >> x;

        int deg_of_numerator;
        std::cout << "The degree_numerator of the polynom: " << std::endl;
        std::cin >> deg_of_numerator;

        std::cout << "The coefficients_numerator of the polynom: " << std::endl;
        double* coeffs_of_numerator = new double[deg_of_numerator + 1];
        double n = 0;
        for (int i = 0; i < deg_of_numerator + 1; i++) {
            std::cin >> n;
            coeffs_of_numerator[i] = n;
            std::cout << std::endl;
        }

        this->numerator = Polynom(x, coeffs_of_numerator, deg_of_numerator);

        int deg_of_denomenator;
        std::cout << "The degree_denominator of the polynom: " << std::endl;
        std::cin >> deg_of_denomenator;

        std::cout << "The coefficients_denominator of the polynom: " << std::endl;
        double* coeffs_of_denomenator = new double[deg_of_denomenator + 1];
        double k = 0;;
        for (int i = 0; i < deg_of_denomenator + 1; i++) {
            std::cin >> coeffs_of_denomenator[i];
        }

        this->denomenator = Polynom(x, coeffs_of_denomenator, deg_of_denomenator);

        answer = 0;
    }

    DWORD WINAPI polynom(LPVOID v) {
        Polynom* a = (Polynom*)v;
        for (int i = 0; i < a.getDegree() + 1; i++) {
            a->setValue(a->getValue() + a->getCoeffs()[i] * pow(a->getX(), i));
            Sleep(15);
        }
        std::cout << "p(" << a->getValue() << ") = " << a->getValue() << std::endl;
        return 0;
    }

    void startCalculating() {
        this->thread[0] = CreateThread(NULL, 0, polynom, (void*)&numerator, 0, &this->threadId[0]);
        if (this->thread[0] == NULL) {
            cout << "Error!";
            return GetLastError();
        }

        this->thread[1] = CreateThread(NULL, 0, polynom, (void*)&denomenator, 0, &this->threadId[1]);
        if (this->thread[1] == NULL) {
            cout << "Error!";
            return GetLastError();
        }
        WaitForMultipleObjects(2, thread, TRUE, INFINITE);
    }

    void setAnswer() {
        if (denomenator.getValue() != 0) {
            this->answer = numerator.getValue() / denomenator.getValue();
        }
        else {
            std::cerr << "Denomenator can't be 0";
        }
    }

    void printAnswer() {
        std::cout << "f(" << this->x << ") = " << this->answer;
    }
};

int main() {
    Calculate_Polynom calculating = Calculate_Polynom();
    calculating.printAnswer();
}

