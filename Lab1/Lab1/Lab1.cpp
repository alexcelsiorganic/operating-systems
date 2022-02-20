#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

#pragma warning(disable : 4996)

char* Creator(char* file_employees) {
	int num_of_notes = 0;
	std::cin >> num_of_notes;
	std::string s = std::to_string(num_of_notes);
	char file[200] = "creator ";
	char* a = strcat(file_employees, " ");
	char* b = strcat(file, a);
	char* creator = new char[100];
	strcpy(creator, strcat(b, s.c_str()));
	return creator;
}



void checkDataInBinaryFile(char* fileName)
{
	ifstream in(fileName, ios::binary);

	cout << "Empls:\n";
	while (in.peek() != EOF)
	{
		employee e;
		in.read((char*)&e, sizeof(employee));
		cout << "Empl number:\n" << e.num << "\nEmpl name:\n" << e.name << "\nEmpl hours:\n" << e.hours << endl;
	}

	in.close();
}


void runReporterProcess(char *&data)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);
	if (!CreateProcessA(NULL, data, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "The new process is not created.\n";
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void checkDataInFile(char* fileName)
{
	ifstream fin(fileName);

	char line[200];
	while (!fin.eof())
	{
		fin.getline(line, 200);

		cout << line << endl;
	}

	fin.close();
}


void runCreatorProcess(char *&dataForProcess)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);

	if (!CreateProcessA(NULL, dataForProcess, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "The new process is not created.\n";
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

int main() {
	std::cout << "Binary file name:" << std::endl;
	char* file_employees = new char[100];
	std::cin >> file_employees;
	std::cout << std::endl;
	char* creator = Creator(file_employees);
	runCreatorProcess(creator);
	char* file_report = new char[100];	
	std::cout << "File report name:" << std::endl;
	std::cin >> file_report;
	std::cout << std::endl;
	int salary_per_hour;
	std::cin >> salary_per_hour;
	std::string sal = std::to_string(salary_per_hour);
	char file[200] = "reporter ";
	char* a = strcat(file_employees, " ");
	char* b_1 = strcat(file, a);
	char* d = strcat(b_1, file_report);
	char* e = strcat(d, " ");
	char* reporter = strcat(e, sal.c_str());
	runReporterProcess(reporter);
	system("pause");
		return 0;
}