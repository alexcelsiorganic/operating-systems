#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include "order.h"
#include "Constants.h"

class Client {
private:
	HANDLE hClientWritePipe;
	HANDLE hClientReadPipe;
	
public:
	Client(HANDLE hWritePipe, HANDLE hReadPipe) {
		this->hClientReadPipe = hReadPipe;
		this->hClientWritePipe = hWritePipe;
	}

	HANDLE getClientWritePipe() {
		return this->hClientWritePipe;
	}

	HANDLE getClientReadPipe() {
		return this->hClientReadPipe;
	}

	int modify() {
		DWORD dwBytesWritten;
		DWORD dwBytesRead;
		std::cout << "Enter key of the order: ";
		int key_order;
		std::cin >> key_order;

		WriteFile(getClientWritePipe(), &READ_REQUEST, sizeof(READ_REQUEST), &dwBytesWritten, NULL);

		WriteFile(getClientWritePipe(), &key_order, sizeof(key_order), &dwBytesWritten, NULL);

		char serverAnswer;
		if (!ReadFile(getClientReadPipe(), &serverAnswer, sizeof(serverAnswer), &dwBytesRead, NULL)) {
			std::cout << "Read from the pipe failed" << std::endl;
			std::cout << "Press any key to finish" << std::endl;
			return GetLastError();
		}

		if (serverAnswer == FOUND) {
			Order order;
			if (!ReadFile(getClientReadPipe(), &order, sizeof(order), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}
			_cprintf("The order %d is read from the pipe.\n", order.n);

			WriteFile(getClientWritePipe(), &END_READ_REQUEST, sizeof(END_READ_REQUEST), &dwBytesWritten, NULL);

			std::cout << std::endl;

			printOrder(order);

			std::cout << "Enter the modified fields:" << std::endl;
			std::cout << "Enter new name of the product: ";
			std::cin >> order.name;
			std::cout << "Enter new amount of the product: ";
			int new_amount;
			std::cin >> new_amount;
			order.amount = new_amount;
			std::cout << "Enter new price of the product: ";
			int new_price;
			std::cin >> new_price;
			order.price = new_price;
			WriteFile(getClientWritePipe(), &MODIFY_REQUEST, sizeof(MODIFY_REQUEST), &dwBytesWritten, NULL);

			WriteFile(getClientWritePipe(), &order, sizeof(Order), &dwBytesWritten, NULL);
			printOrder(order);
		}
		else {
			std::cout << "Order not found." << std::endl;
		}

		char answer;
		WriteFile(getClientWritePipe(), &answer, sizeof(answer), &dwBytesWritten, NULL);

		WriteFile(getClientWritePipe(), &END_MODIFY_REQUEST, sizeof(END_MODIFY_REQUEST), &dwBytesWritten, NULL);
	}

	int read() {
		DWORD dwBytesWritten;
		DWORD dwBytesRead;
		std::cout << "Enter key of the order: ";
		int key_order;
		std::cin >> key_order;

		WriteFile(getClientWritePipe(), &READ_REQUEST, sizeof(READ_REQUEST), &dwBytesWritten, NULL);

		WriteFile(getClientWritePipe(), &key_order, sizeof(key_order), &dwBytesWritten, NULL);

		char serverCode;
		if (!ReadFile(getClientReadPipe(), &serverCode, sizeof(serverCode), &dwBytesRead, NULL)) {
			std::cout << "Read from the pipe failed" << std::endl;
			std::cout << "Press any key to finish" << std::endl;
			return GetLastError();
		}

		if (serverCode == FOUND) {
			Order order;
			if (!ReadFile(getClientReadPipe(), &order, sizeof(order), &dwBytesRead, NULL)) {
				std::cout << "Read from the pipe failed" << std::endl;
				std::cout << "Press any key to finish" << std::endl;
				return GetLastError();
			}
			std::cout << "\nOrder name:\n" << order.name << "\nProduct count:\n" << order.amount << "\nProduct price:" << order.price << std::endl;
		}
		else {
			std::cout << "Order not found." << std::endl;
		}

		WriteFile(getClientWritePipe(), &END_READ_REQUEST, sizeof(END_READ_REQUEST), &dwBytesWritten, NULL);
	}

	int exit() {
		DWORD dwBytesWritten;
		DWORD dwBytesRead;
		WriteFile(getClientWritePipe(), &EXIT_REQUEST, sizeof(EXIT_REQUEST), &dwBytesWritten, NULL);
	}

	int startAction() {
		while (true) {
			std::cout << std::endl;
			std::cout << "1) Modify" << std::endl;
			std::cout << "2) Read" << std::endl;
			std::cout << "3) Exit from cycle" << std::endl;
			int choice;
			std::cin >> choice;
			system("cls");

			if (choice == 2) {
				read();
			}
			else if (choice == 1) {
				modify();
			}
			else {
				
				break;
			}
		}
		return 0;
	};

	~Client() {
		CloseHandle(getClientReadPipe());
		CloseHandle(getClientWritePipe());
	}

};

int main(int argc, char* argv[]) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);
	Client client(hWritePipe, hReadPipe);
	client.startAction();
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	return 0;
}

