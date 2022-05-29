#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include "order.h"
#include "Constants.h"
#include "BinaryFile.h"

volatile int readerCount = 0;
HANDLE semaphore;
CRITICAL_SECTION cs;
char filename[20];

class Process_Server {
private:
	int numberOfClients = 0;
public:
	Process_Server() {
		createBinaryFile(filename);

		PrintBinaryFile(filename);

		std::cout << "Enter the number of clients:" << std::endl;
		std::cin >> this->numberOfClients;
	}
	
	class workWithClient {
	private:
		int clientsRequest;
		HANDLE hWritePipe;
		HANDLE hReadPipe;
		HANDLE hClientWritePipe;
		HANDLE hClientReadPipe;
		SECURITY_ATTRIBUTES secAttr;
		char dataForCommandLine[256] = "Client ";
		bool work;

	public:
		workWithClient() {
			this->work = true;
			secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			secAttr.lpSecurityDescriptor = NULL;
			secAttr.bInheritHandle = TRUE;

			if (!CreatePipe(&this->hReadPipe, &this->hClientWritePipe, &secAttr, 0)) {
				std::cerr << "Create pipe failed." << std::endl;
			}

			if (!CreatePipe(&this->hClientReadPipe, &this->hWritePipe, &secAttr, 0)) {
				std::cerr << "Create pipe failed." << std::endl;
			}

			std::string num1 = std::to_string((int)this->hClientWritePipe);
			strcat_s(this->dataForCommandLine, num1.c_str());
			strcat_s(this->dataForCommandLine, " ");
			std::string num2 = std::to_string((int)this->hClientReadPipe);
			strcat_s(this->dataForCommandLine, num2.c_str());

			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(STARTUPINFOA));
			si.cb = sizeof(STARTUPINFOA);

			if (!CreateProcessA(NULL, this->dataForCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
				std::cerr << "The new process is not created." << std::endl;
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		int clientWantsToModify() {
			DWORD dwBytesWritten;
			DWORD dwBytesRead;
			WaitForSingleObject(semaphore, INFINITE);

			Order order;
			if (!ReadFile(this->hReadPipe, &order, sizeof(Order), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}

			modify(filename, order);

			WriteFile(this->hWritePipe, &END_MODIFY_REQUEST, sizeof(END_MODIFY_REQUEST), &dwBytesWritten, NULL);

			char end;
			if (!ReadFile(this->hReadPipe, &end, sizeof(end), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}
			ReleaseSemaphore(semaphore, 1, NULL);
			return 0;
		}

		int clientWantsToRead() {
			DWORD dwBytesWritten;
			DWORD dwBytesRead;
			EnterCriticalSection(&cs);
				
			++readerCount;
			if (readerCount == 1) {
				WaitForSingleObject(semaphore, INFINITE);
			}
			LeaveCriticalSection(&cs);

			int orderNumber;

			if (!ReadFile(hReadPipe, &orderNumber, sizeof(int), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}
			Order* order = findOrder(filename, orderNumber);

			if (order != nullptr) {
				WriteFile(this->hWritePipe, &FOUND, sizeof(FOUND), &dwBytesWritten, NULL);
				WriteFile(this->hWritePipe, order, sizeof(Order), &dwBytesWritten, NULL);
			}
			else {
				WriteFile(this->hWritePipe, &NOT_FOUND, sizeof(NOT_FOUND), &dwBytesWritten, NULL);
			}

			char end;
			if (!ReadFile(this->hReadPipe, &end, sizeof(end), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}

			EnterCriticalSection(&cs);
			--readerCount;

			if (readerCount == 0) {
				ReleaseSemaphore(semaphore, 1, NULL);
			}

			LeaveCriticalSection(&cs);
			return 0;
		}

		int clientWantsToExit() {
			this->work = false;
			return 0;

		}

		int startAction() {
			while (this->work) {
				char client_request;
				DWORD dwBytesRead;

				if (!ReadFile(this->hReadPipe, &client_request, sizeof(client_request), &dwBytesRead, NULL)) {
					std::cout << "Read from the pipe failed" << std::endl;
					std::cout << "Press any key to finish" << std::endl;
					return GetLastError();
				}

				if (client_request == MODIFY_REQUEST) {
					clientWantsToModify();
				}

				else if (client_request == READ_REQUEST) {
					clientWantsToRead();
				}

				else if (client_request == EXIT_REQUEST) {
					clientWantsToExit();
				}
			}
			return 0;
		}
	};

	static DWORD WINAPI client(LPVOID data) {
		Process_Server::workWithClient work = Process_Server::workWithClient();
		work.startAction();
		return 0;
	}

	int createClients() {
		HANDLE* handles = new HANDLE[this->numberOfClients];
		DWORD* ID = new DWORD[this->numberOfClients];

		for (int i = 0; i < this->numberOfClients; i++) {
			handles[i] = CreateThread(NULL, NULL, client, (void*)i, 0, &ID[i]);
		}

		PrintBinaryFile(filename);
		WaitForMultipleObjects(this->numberOfClients, handles, TRUE, INFINITE);
		return 0;
	}
};

int main() {

	std::cout << "Enter name of the file: ";
	std::cin >> filename;

	InitializeCriticalSection(&cs);

	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	Process_Server process = Process_Server();
	process.createClients();

	system("cls");
}
