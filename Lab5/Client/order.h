#pragma once
#include <iostream>
#include <fstream>

struct Order {
	int n; // номер заказа
	char name[10]; // имя товара
	int amount; // количество единиц товара
	double price; // стоимость единицы товара
};


Order* findOrder(char* filename, int orderNumber) {
	std::ifstream in(filename, std::ios::binary);
	while (in.peek() != EOF) {
		Order* order = new Order;
		std::cout << " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << orderNumber;
		in.read((char*)order, sizeof(Order));
		if (order->n == orderNumber) {
			in.close();
			return order;
		}
	}

	in.close();
	return nullptr;
}

void modify(char* filename, Order order) {
	std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);

	long counter = 0;
	long orderSize = sizeof(Order);

	while (f.peek() != EOF) {
		Order tempOrder;
		f.read((char*)&tempOrder, sizeof(Order));

		if (order.n == tempOrder.n) {
			f.seekp(counter * orderSize, std::ios::beg);
			f.write((char*)&order, sizeof(Order));
			f.close();
			return;
		}
		else {
			counter += 1;
		}
	}
}

void printOrder(Order order) {
	std::cout << "\nkey of order:" << std::endl;
	std::cout << order.n << std::endl;

	std::cout << "name of order:" << std::endl;
	std::cout << order.name << std::endl;

	std::cout << "amount of order:" << std::endl;
	std::cout << order.amount << std::endl;

	std::cout << "price of order:" << std::endl;
	std::cout << order.price << std::endl;
}

void EnterOrder(Order& order) {

	std::cout << "Enter order number: ";
	std::cin >> order.n;
	std::cout << std::endl;

	std::cout << "Enter order name: ";
	std::cin >> order.name;
	std::cout << std::endl;

	std::cout << "Enter product count: ";
	std::cin >> order.amount;
	std::cout << std::endl;

	std::cout << "Enter product price: ";
	std::cin >> order.price;
	std::cout << std::endl;

}