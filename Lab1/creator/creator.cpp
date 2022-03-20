#include <fstream>
#include <iostream>
#include <windows.h>

struct employee {
    int num;
    char name[10];
    double hours;
};


int main(int argc, char* argv[]) {
    char* name = argv[1];
    int num_of_notes = atoi(argv[2]);
    std::ofstream output(name, std::ios::binary);
    for (int i = 0; i < num_of_notes; i++) {
        employee e;
        std::cin >> e.num;
        std::cin >> e.name;
        std::cin >> e.hours;
        output.write((char*)&e, sizeof e);
    }
    system("pause");

    return 0;

}
