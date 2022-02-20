#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};


int main(int argc, char* argv[]) {
    char* binary_name = argv[1];
    char* report_name = argv[2];
    int salary_per_hour = atoi(argv[3]);
    std::ifstream input_b(binary_name, std::ios::binary);
    std::ofstream output_txt(report_name);
    output_txt << std::string("Отчет по файлу") + " " + std::string(binary_name) + "\n";
    employee* empls = new employee[100];
    while (!input_b.eof()) {
        int size = 0;
        while (input_b.peek() != EOF) {
            employee e;
            input_b.read((char*)&e, sizeof(employee));
            double finalCost = salary_per_hour * e.hours;
            output_txt << std::to_string(e.num) + " " + std::string(e.name) +
                  " " + std::to_string(e.hours) + " " + std::to_string(finalCost) + '\n';
        }
    }
    system("pause");
    return 1;

}