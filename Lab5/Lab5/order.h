#ifndef ORDER_H
#define ORDER_H
#include <iostream>
#include <fstream>

struct Order {
	int n;
	char name[10];
	int amount;
	double price;
};

Order* findOrder(char filename[20], int orderNumber) {
	std::ifstream in(filename, std::ios::binary);

	int orderSize = sizeof(Order);

	while (in.peek() != EOF) {
		Order* order = new Order;
		in.read((char*)order, orderSize);
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

	int pos = 0;
	int orderSize = sizeof(Order);

	while (f.peek() != EOF) {
		Order o;
		f.read((char*)&o, orderSize);
		if (order.n == o.n) {
			long new_pos = pos * orderSize;
			f.seekp(new_pos, std::ios::beg);
			f.write((char*)&order, orderSize);
			f.close();
			return;
		}
		else {
			pos++;
		}
	}
}

void printorder(Order order) {
	std::cout << "\nkey of order:" << std::endl;
	std::cout << order.n << std::endl;

	std::cout << "name of order:" << std::endl;
	std::cout << order.name << std::endl;

	std::cout << "amount of order:" << std::endl;
	std::cout << order.amount << std::endl;

	std::cout << "price of order:" << std::endl;
	std::cout << order.price << std::endl;
}

void enterOrder(Order& order) {

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
#endif ORDER_H