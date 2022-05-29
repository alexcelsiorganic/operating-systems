#ifndef BINARYFILE_H
#define BINARYFILE_H
#include <iostream>
#include <fstream>
#include "order.h"

void createBinaryFile(char filename[20]) {
	std::ofstream out(filename, std::ios::binary);

	std::cout << "Number of orders: ";
	int num;
	std::cin >> num;

	for (int i = 0; i < num; i++) {
		system("cls");
		Order order;
		enterOrder(order);
		out.write((char*)&order, sizeof(Order));
	}
	system("cls");
	out.close();
}

void PrintBinaryFile(char filename[20]) {
	std::ifstream in(filename, std::ios::binary);
	system("cls");

	std::cout << "The list of orders:" << std::endl;

	while (in.peek() != EOF) {
		Order order;
		in.read((char*)&order, sizeof(Order));
		std::cout << "Order number: " << order.n << std::endl << "Order name: " << order.name << std::endl
			<< "Product count: " << order.amount << std::endl << "Product price: " << order.price << std::endl;
	}

	in.close();
}
#endif BINARYFILE_H