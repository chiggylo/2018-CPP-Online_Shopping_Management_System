
#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <time.h>

using namespace std;

// extra functions
void swap(string tempPath, string targetPath) { // swap temp.txt with target.txt
	// setting variables
	ifstream readTempFile;
	readTempFile.open(tempPath);
	ofstream writeTargetFile;
	writeTargetFile.open(targetPath);
	string line;

	// read and write each line
	while (getline(readTempFile, line)) {
		writeTargetFile << line << endl;
	}
}
vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string token;
	vector<string> tokens;
	while (getline(ss, token, delim)) {
		tokens.push_back(token);
	}
	return tokens;
}
string randomPwGenerator() {
	srand((unsigned int)time(0));
	char characters[] = { '0','1','2','3','4','5','6','7','8','9', 'A', 'a', 'B', 'b','C','c','D','d','E','e','F','f', 'G', 'g','H','h' };
	string password = "XXXXXX";
	for (unsigned int i = 0; i < password.size(); i++) {
		password[i] = characters[rand() % sizeof(characters)];
	}
	return password;
}

// class
class Order {
private:
	string m_oid;
	string m_cid;
	int m_quantity;
public:
	bool addOrder(string id) {
		if (id.at(0) != 'c') {
			id = "c" + id;

		}
		ifstream readCartFile;
		readCartFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");

		string orderInfo;
		int index = 0;
		// check for id generation
		if (!readCartFile) {
			return false;
		}
		while (getline(readCartFile, orderInfo)) {
			vector<string> tokens = split(orderInfo, ';');
			// case-insensitive id input and data_id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 3) { // error checking
				if (tokens.at(0).at(0) == 'o') {
					vector<string> numberTag = split(tokens.at(0), 'o');
					if (stoi(numberTag.at(1)) == index) {
						index++;
					}
				}
			}
		}
		readCartFile.close();
		m_oid = "o" + to_string(index);

		// get quantity
		cout << "Enter amount for purchase..." << endl;
		cin >> m_quantity;
		while (cin.fail())
		{
			cout << "Not integer, try again\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> m_quantity;
		}
		cout << "---------------------------------------------------" << endl;

		m_cid = id;
		if (updateOrderFile()) {
			return true;

		}
		return false;
	}
	static bool deleteOrder(string id);
	bool editOrder(string oid) {
		if (oid.at(0) != 'o') {
			oid = "o" + oid;
		}
		// opening files
		ifstream readOrderFile;
		readOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");
		if (!readOrderFile) {
			return false;
		}
		ofstream writeTempFile;
		writeTempFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
		if (!writeTempFile) {
			return false;
		}
		string orderInfo;
		while (getline(readOrderFile, orderInfo)) {
			vector<string> tokens = split(orderInfo, ';');
			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 1) { // error checking
				if (tokens.at(0).compare(oid) == 0) {
					// get quantity
					cout << "Enter amount for purchase: ";
					cin >> m_quantity;
					while (cin.fail()) {
						cout << "Not integer, try again\n" << endl;
						cin.clear();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						cin >> m_quantity;
					}
					writeTempFile << tokens.at(0) << ";" << tokens.at(1) << ";" << m_quantity << endl;
				}
				else {
					writeTempFile << orderInfo << endl;
				}
			}
		}
		readOrderFile.close();
		writeTempFile.close();
		swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\order.txt");
		return true;
	}
	int viewOrder();
	bool getOrderDetails(string id) {
		if (id.at(0) != 'o') {
			id = "o" + id;
		}
		ifstream readOrderFile;
		readOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");
		if (!readOrderFile) {
			return false;
		}
		if (existOrder(id)) {
			// opening files
			
			string orderInfo;
			while (getline(readOrderFile, orderInfo)) {
				vector<string> tokens = split(orderInfo, ';');
				// case-insensitive id input
				transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
				if (tokens.size() >= 2) { // error checking
						m_oid = tokens.at(0);
						m_cid = tokens.at(1);
						m_quantity = stoi(tokens.at(2));
						readOrderFile.close();
						return true;
				}
			}
			readOrderFile.close();
		}
		return false;
	}
	void displayOInfo() {
		cout << "Order Information...\nID:\t" << m_oid << "\nName:\t" << m_cid << "\nRate:\t" << m_quantity << endl;
	}
	bool existOrder(string id);


	// return info
	string getOID() {
		return m_oid;
	}
	string getCID() {
		return m_cid;
	}
	int getQuantity() {
		return m_quantity;
	}
private:
	bool updateOrderFile() {
		// opening file
		ofstream writeOrderFile;
		writeOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt", ios_base::app);
		if (!writeOrderFile) {
			return false;
		}
		writeOrderFile << m_oid << ";" << m_cid << ";" << m_quantity << ";" << endl;
		writeOrderFile.close();
		return true;
	}
};
bool Order::existOrder(string id) {
	if (id.at(0) != 'o') {
		id = "o" + id;

	}
	// opening files
	ifstream readProductFile;
	readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");

	string productInfo;
	while (getline(readProductFile, productInfo)) {
		vector<string> tokens = split(productInfo, ';');
		// case-insensitive id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

		if ((tokens.at(0).compare(id) == 0)) { // does exist
			return true;
		}

	}
	readProductFile.close();
	return false;
}
bool Order::deleteOrder(string oid) {
	if (oid.at(0) != 'o') {
		oid = "o" + oid;
	}
	// opening files
	ifstream readOrderFile;
	readOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");
	if (!readOrderFile) {
		return false;
	}
	ofstream writeOrderFile;
	writeOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
	if (!writeOrderFile) {
		return false;
	}
	string orderInfo;
	while (getline(readOrderFile, orderInfo)) {
		vector<string> tokens = split(orderInfo, ';');
		// case-insensitive id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
		if (tokens.size() >= 1) { // error checking
			if (tokens.at(0).compare(oid) != 0) {
				writeOrderFile << orderInfo << endl;
			}
		}
	}
	readOrderFile.close();
	writeOrderFile.close();
	swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\order.txt");
	return true;
}
int Order::viewOrder() {
	ifstream readOrderFile;
	readOrderFile.open("C:\\Users\\chigo\\Desktop\\data\\order.txt");
	if (!readOrderFile) {
		return 0;
	}
	string productInfo;
	int result = 0;
	while (getline(readOrderFile, productInfo)) {
		vector<string> tokens = split(productInfo, ';');
		// case-insensitive id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
		if (tokens.size() >= 3) { // error checking
			cout << "Order's detail..." << endl;
			cout << "OID: " << tokens.at(0) << "\nCID: " << tokens.at(1) << "\nQuantity: " << tokens.at(2) << endl;
			cout << endl;
			result++;
			
		}
	}
	return result;
}



class Product {
private:
	string m_pid;
	string m_pname;
	float m_prate;
public:
	bool addProduct() {
		// id generation
		int index = 0;
		string productInfo;
		vector<int> indexing;
		ifstream readProductFile;
		readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");
		if (!readProductFile) {
			return false;
		}
		while (getline(readProductFile, productInfo)) {
			vector<string> tokens = split(productInfo, ';');
			// case-insensitive id input and data_id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 1) { // error checking
				vector<string> numberTag = split(tokens.at(0), 'p');
				indexing.push_back(stoi(numberTag.at(1)));
			}
		}
		readProductFile.close();
		sort(indexing.begin(), indexing.end());
		for (unsigned int i = 0; i < indexing.size(); i++)
		{
			if (indexing.at(i) == index) {
				index++;
			}
		}
		m_pid = "p" + to_string(index);

		// getting product info
		char input[50];
		cin.ignore();
		cout << "Enter name of product: ";
		cin.getline(input, sizeof(input));
		m_pname = input; // converts char[] to string
		transform(m_pname.begin(), m_pname.end(), m_pname.begin(), tolower); // to lowercase


		cout << "Enter product rate: ";
		cin >> m_prate;
		while (cin.fail() || m_prate < 0) // non-numeric input or negative value
		{
			cout << "Invalid entry, try again\nEnter product rate : ";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> m_prate;
		}
		cout << "---------------------------------------------------" << endl;

		if (addProductFile()) {
			return true;

		}
		else {
			return false;
		}
	}
	static bool deleteProduct(string id);
	bool editProduct(string id) {
		if (id.at(0) != 'p') {
			id = "p" + id;
		}
		// opening files
		ifstream readProductFile;
		readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");
		if (!readProductFile) {
			return false;
		}
		ofstream writeProductFile;
		writeProductFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
		if (!writeProductFile) {
			return false;
		}
		if (existProduct(id)) {
			// product info
			string productInfo;
			while (getline(readProductFile, productInfo)) {
				vector<string> tokens = split(productInfo, ';');

				// case-insensitive id input
				transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

				if (tokens.size() >= 1) { // error checking
					if (tokens.at(0).compare(id) == 0) {
						cout << "Changing product " << tokens.at(0) << endl;
						// getting product info
						char input[50];
						cin.ignore();
						cout << "Enter name of product: ";
						cin.getline(input, sizeof(input));
						m_pname = input; // converts char[] to string
						transform(m_pname.begin(), m_pname.end(), m_pname.begin(), tolower); // to lowercase


						cout << "Enter product rate: ";
						cin >> m_prate;
						while (cin.fail() || m_prate < 0) // non-numeric input or negative value
						{
							cout << "Invalid entry, try again\nEnter product rate : ";
							cin.clear();
							cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							cin >> m_prate;
						}
						cout << "---------------------------------------------------" << endl;

						writeProductFile << tokens.at(0) << ";" << m_pname << ";" << m_prate << endl;
					}
					else {
						writeProductFile << productInfo << endl;
					}
				}
			}
			readProductFile.close();
			writeProductFile.close();
			swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\product.txt");
			return true;
		}
		else {
			return false;
		}
	}
	int viewProduct();
	bool getProductDetails(string id) {
		if (id.at(0) != 'p') {
			id = "p" + id;

		}
		if (existProduct(id)) {
			// opening files
			ifstream readProductFile;
			readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");

			string productInfo;
			while (getline(readProductFile, productInfo)) {
				vector<string> tokens = split(productInfo, ';');

				// case-insensitive id input
				transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

				if (tokens.size() >= 2) { // error checking
					if ((tokens.at(0).compare(id) == 0)) { // does exist
						m_pid = tokens.at(0);
						m_pname = tokens.at(1);
						m_prate = stof(tokens.at(2));
						readProductFile.close();
						return true;
					}
				}
			}
			readProductFile.close();
		}
		return false;
	}
	void displayPInfo() {
		cout << "Product Information...\nID:\t" << m_pid << "\nName:\t" << m_pname << "\nRate:\t" << m_prate << endl;
	}
	static bool existProduct(string id);

	// return info
	string getPID() {
		return m_pid;
	}
	string getPName() {
		return m_pname;
	}
	float getPRate() {
		return m_prate;
	}
private:
	bool addProductFile() {
		// opening file
		ofstream writeProductFile;
		writeProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt", ios_base::app);
		if (!writeProductFile) {
			return false;
		}
		writeProductFile << m_pid << ";" << m_pname << ";" << m_prate << ";" << endl;
		writeProductFile.close();
		return true;
	}
};
bool Product:: existProduct(string id) {
	if (id.at(0) != 'p') {
		id = "p" + id;

	}
	// opening files
	ifstream readProductFile;
	readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");

	string productInfo;
	while (getline(readProductFile, productInfo)) {
		vector<string> tokens = split(productInfo, ';');
		// case-insensitive id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

		if ((tokens.at(0).compare(id) == 0)) { // does exist
			return true;
		}

	}
	readProductFile.close();
	return false;
}
bool Product:: deleteProduct(string id) {
	if (Product::existProduct(id)) {
		// opening files
		ifstream readProductFile;
		readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");
		if (!readProductFile) {
			return false;
		}
		ofstream writeTempFile;
		writeTempFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
		if (!writeTempFile) {
			return false;
		}

		// product info
		string productInfo;
		while (getline(readProductFile, productInfo)) {
			vector<string> tokens = split(productInfo, ';');
			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 1) { // error checking
				if (tokens.at(0).compare(id) != 0) {
					writeTempFile << productInfo << endl;
				}
			}
		}
		readProductFile.close();
		writeTempFile.close();
		swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\product.txt");
		return true;
	}
	else {
		return false;
	}
}
int Product:: viewProduct() {
	ifstream readProductFile;
	readProductFile.open("C:\\Users\\chigo\\Desktop\\data\\product.txt");
	if (!readProductFile) {
		return 0;
	}
	string productInfo;
	int result = 0;
	while (getline(readProductFile, productInfo)) {
		vector<string> tokens = split(productInfo, ';');
		// case-insensitive id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
		if (tokens.size() >= 3) { // error checking
			cout << "Product's detail..." << endl;
			cout << "Id: " << tokens.at(0) << "\nName: " << tokens.at(1) << "\nRate: " << tokens.at(2) << endl;
			cout << endl;
			result++;
		
		}
	}
	return result;
}



class OrderItem : virtual public Order, virtual public Product {
public:
	bool newOrderItem(string cid, string pid) {
		if (Product::getProductDetails(pid)) {
			Order::addOrder(cid); // no need to check for existence cid
			// opening files
			ofstream writeCartFile;
			writeCartFile.open("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt", ios_base::app);
			// add to cart file
			writeCartFile << Order::getOID() << ";" << Order::getCID() << ";" << Product::getPID() << ";" << Product::getPRate() << ";" << Order::getQuantity() << endl;
			writeCartFile.close();
			return true;
		}
		else {
			return false;
		}
	}
	static bool deleteOrderItem(string oid);
	/*
	bool editOrderItem(string oid) {
		if (editOrder(oid)) {
			// opening files

			ifstream readOrderItemFile;
			readOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");
			ofstream writeTempFile;
			writeTempFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");

			string orderInfo;
			if (oid.at(0) != 'o') {
				oid = "o" + oid;
			}
			if (!readOrderItemFile) {
				cerr << "Unable to edit order";
				return false;
			}
			while (getline(readOrderItemFile, orderInfo)) {
				vector<string> tokens = split(orderInfo, ';');
				//cout << tokens.at(0) << " versus " << itemInfo << endl;

				// case-insensitive id input
				transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

				if (tokens.size() >= 1) { // error checking
					if (tokens.at(0).compare(oid) == 0) {
						writeTempFile << tokens.at(0) << ";" << tokens.at(1) << ";" << tokens.at(2) << ";" << tokens.at(3) << ";" << getQuantity() << endl;
					}
					else {
						writeTempFile << orderInfo << endl;
					}
				}
			}
			readOrderItemFile.close();
			writeTempFile.close();
			//rename("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt", "C:\\Users\\chigo\\Desktop\\data\\temp2.txt");
			swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");
			//rename("C:\\Users\\chigo\\Desktop\\data\\temp2.txt", "C:\\Users\\chigo\\Desktop\\data\\temp.txt");

			cout << "OrderItem editted\n" << endl;

		}
		else {
			return false;
		}
	}
	bool viewOrderItem() {
		ifstream readOrderItemFile;
		readOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");
		string line;
		if (!readOrderItemFile) {
			return false;
		}
		while (getline(readOrderItemFile, line)) {
			vector<string> tokens = split(line, ';');

			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 2) { // error checking
				if (tokens.at(0).at(0) == 'o') {
					cout << "Order's detail..." << endl;
					cout << "OID: " << tokens.at(0) << "\nCID: " << tokens.at(1) << "\nPID: " << tokens.at(2) << "\nRate: " << tokens.at(3) << "\nQuantity: " << tokens.at(4) << endl;
					cout << endl;
				}
			}
		}
		return true;
	}
	
	bool searchOrderItem(string oid) {

		ifstream readOrderItemFile;
		readOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");

		string line;

		if (oid.at(0) != 'o') {
			oid = "o" + oid;
		}
		while (getline(readOrderItemFile, line)) {
			vector<string> tokens = split(line, ';');

			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 2) { // error checking
				if (tokens.at(0).compare(oid) == 0) {
					cout << "Order's detail..." << endl;
					cout << "OID: " << tokens.at(0) << "\nCID: " << tokens.at(1) << "\nPID: " << tokens.at(2) << "\nQuantity: " << tokens.at(3) << endl;
					cout << endl;
					return true;
				}
			}
		}
		return false;
	}
	*/
};
bool OrderItem:: deleteOrderItem(string oid) {
	if (oid.at(0) != 'o') {
		oid = "o" + oid;
	}
	ifstream readOrderItemFile;
	readOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\orderItem.txt");
	if (!readOrderItemFile) {
		return false;
	}
	ofstream writeOrderItemFile;
	writeOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
	if (!writeOrderItemFile) {
		return false;
	}
	if (deleteOrder(oid)) {
		string orderItemInfo;
		while (getline(readOrderItemFile, orderItemInfo)) {
			vector<string> tokens = split(orderItemInfo, ';');
			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 1) { // error checking
				if (tokens.at(0).compare(oid) != 0) {
					writeOrderItemFile << orderItemInfo << endl;
				}
			}
		}
		readOrderItemFile.close();
		writeOrderItemFile.close();
		swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");
		return true;
	}
	else {
		return false;
	}
}



class User {
private:
	string m_id;
	string m_pw;
	string m_name;
	string m_address;
	string m_contact;
public:
	// methods
	void setLogin() {
		cout << "Enter ID: ";
		cin >> m_id;
		transform(m_id.begin(), m_id.end(), m_id.begin(), tolower);
		cout << "Enter Password: ";
		cin >> m_pw;
	}
	char verifyLogin() {
		ifstream readAccountFile;
		readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");

		string accountInfo;

		if (!readAccountFile) {
			return '1';
		}
		while (getline(readAccountFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');

			// case-insensitive id input and data_id input just in case
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			transform(m_id.begin(), m_id.end(), m_id.begin(), tolower);

			//cout << tokens.size() << endl;
			if (tokens.size() >= 2) { // error checking
				if (tokens.at(0) == m_id) {
					if (tokens.at(1) == m_pw) {
						m_name = tokens.at(2);
						m_address = tokens.at(3);
						m_contact = tokens.at(4);
						readAccountFile.close();
						return m_id[0];
					}
				}
			}
		}
		readAccountFile.close();
		return '2';
	}

	// return info
	string getID() {
		return m_id;
	}
	string getPW() {
		return m_pw;
	}
	string getName() {
		return m_name;
	}
	string getAddress() {
		return m_address;
	}
	string getContact() {
		return m_contact;
	}
};



class Admin :virtual public User {
public:
	bool addUser() {
		// user info
		string accountInfo, id, name, address, contactno;
		string password = randomPwGenerator();

		int optionAns;
		cout << "Creating user...\nIs the user a \"manager\" or \"customer\"?\n1: Manager\n2: Customer\n" << endl;
		cin >> optionAns;
		while (cin.fail() || optionAns < 1 || optionAns > 2) // non-numeric input
		{
			cout << "Invalid entry...\n1: Manager\n2: Customer\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;

		// id generation
		int index = 0;
		vector<int> indexing;
		ifstream readAccountFile;
		readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");
		if (!readAccountFile) {
			return false;
		}
		while (getline(readAccountFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');
			// case-insensitive id input and data_id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 1) { // error checking
				if (optionAns == 1 && tokens.at(0).at(0) == 'm') {
					vector<string> numberTag = split(tokens.at(0), 'm');
					indexing.push_back(stoi(numberTag.at(1)));
				}
				else if (optionAns == 2 && tokens.at(0).at(0) == 'c') {
					vector<string> numberTag = split(tokens.at(0), 'c');
					indexing.push_back(stoi(numberTag.at(1)));
				}
			}
		}
		readAccountFile.close();
		sort(indexing.begin(), indexing.end());
		for (unsigned int i = 0; i < indexing.size(); i++)
		{
			if (indexing.at(i) == index) {
				index++;
			}
		}
		if (optionAns == 1) {
			id = "m" + to_string(index);
		}
		else if (optionAns == 2) {
			id = "c" + to_string(index);
		}

		// getting details
		char input[50];
		cin.ignore();
		cout << "Enter name of user: ";
		cin.getline(input, sizeof(input));
		name = input; // converts char[] to string
		transform(name.begin(), name.end(), name.begin(), tolower); // to lowercase
		cout << "Enter address of user: ";
		cin.getline(input, sizeof(input));
		address = input;
		transform(address.begin(), address.end(), address.begin(), tolower);
		cout << "Enter contact number of user: ";
		cin.getline(input, sizeof(input));
		contactno = input;
		transform(contactno.begin(), contactno.end(), contactno.begin(), tolower);
		cout << "---------------------------------------------------" << endl;

		// write to file
		ofstream writeAccountFile;
		writeAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt", ios_base::app);
		writeAccountFile << id << ";" << password << ";" << name << ";" << address << ";" << contactno << endl;
		writeAccountFile.close();
		return true;
	}
	bool deleteUser() {
		// opening files
		ifstream readAccountFile;
		readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");
		if (!readAccountFile) {
			return false;
		}
		ofstream writeTempFile;
		writeTempFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
		if (!writeTempFile) {
			return false;
		}

		int optionAns;
		cout << "Deleting user...\nIs the user a \"manager\" or \"customer\"?\n1: Manager\n2: Customer\n" << endl;
		cin >> optionAns;
		while (cin.fail() || optionAns < 1 || optionAns > 2) // non-numeric input
		{
			cout << "Invalid entry...\n1: Manager\n2: Customer\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;
		
		string id;
		char input[50];
		cin.ignore();
		cout << "Enter user's ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'm' && optionAns == 1) {
			id = "m" + id;
		}
		else if (id.at(0) != 'c' && optionAns == 2) {
			id = "c" + id;
		}
		cout << "---------------------------------------------------" << endl;

		// user info
		string accountInfo;
		while (getline(readAccountFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');

			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 1) { // error checking
				if (tokens.at(0).compare(id) != 0) {
					writeTempFile << accountInfo << endl;
				}
			}
		}
		readAccountFile.close();
		writeTempFile.close();
		swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");

		//also need to delete order and orderitem if customer is gone
		return true;
	}
	bool editUser() {
		// opening files
		ifstream readAccountFile;
		readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");
		if (!readAccountFile) {
			return false;
		}
		ofstream writeTempFile;
		writeTempFile.open("C:\\Users\\chigo\\Desktop\\data\\temp.txt");
		if (!writeTempFile) {
			return false;
		}

		int optionAns;
		cout << "Editing user...\nIs the user a \"manager\" or \"customer\"?\n1: Manager\n2: Customer\n" << endl;
		cin >> optionAns;
		while (cin.fail() || optionAns < 1 || optionAns > 2) // non-numeric input
		{
			cout << "Invalid entry...\n1: Manager\n2: Customer\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;

		string id;
		char input[50];
		cin.ignore();
		cout << "Enter user's ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'm' && optionAns == 1) {
			id = "m" + id;
		}
		else if (id.at(0) != 'c' && optionAns == 2) {
			id = "c" + id;
		}
		cout << "---------------------------------------------------" << endl;

		// user info
		string accountInfo, name, address, contactno;
		while (getline(readAccountFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');

			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 1) { // error checking
				if (tokens.at(0).compare(id) == 0) {
					cout << "Changing User " << tokens.at(0) << "..." << endl;
					
					// getting details
					char input[50];
					//cin.ignore();
					cout << "Enter name of user: ";
					cin.getline(input, sizeof(input));
					name = input; // converts char[] to string
					transform(name.begin(), name.end(), name.begin(), tolower); // to lowercase
					cout << "Enter address of user: ";
					cin.getline(input, sizeof(input));
					address = input;
					transform(address.begin(), address.end(), address.begin(), tolower);
					cout << "Enter contact number of user: ";
					cin.getline(input, sizeof(input));
					contactno = input;
					transform(contactno.begin(), contactno.end(), contactno.begin(), tolower);
					cout << "---------------------------------------------------" << endl;
					writeTempFile << id << ";" << tokens.at(1) << ";" << name << ";" << address << ";" << contactno << endl;
				}
				else {
					writeTempFile << accountInfo << endl;
				}
			}
		}
		readAccountFile.close();
		writeTempFile.close();
		swap("C:\\Users\\chigo\\Desktop\\data\\temp.txt", "C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");

		return true;
	}
	int viewUser() {
		// opening files
		ifstream readAccountFile;
		readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");
		if (!readAccountFile) {
			return -1;
		}

		int optionAns;
		cout << "Viewing users...\nViewing \"manager users\" or \"customer users\"?\n1: Manager\n2: Customer\n" << endl;
		cin >> optionAns;
		while (cin.fail() || optionAns < 1 || optionAns > 2) // non-numeric input
		{
			cout << "Invalid entry...\n1: Manager\n2: Customer\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;
		
		int result = 0;
		string accountInfo;
		while (getline(readAccountFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');
			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 5 && tokens.at(0).at(0) == 'm' && optionAns == 1) { // error checking
					cout << "User's detail..." << endl;
					cout << "Id: " << tokens.at(0) << "\nPassword: " << tokens.at(1) << "\nName: " << tokens.at(2) << "\nAddress: " << tokens.at(3) << "\nContact No.: " << tokens.at(4) << endl;
					cout << endl;
					result++;
			}
			if (tokens.size() >= 5 && tokens.at(0).at(0) == 'c' && optionAns == 2) { // error checking
				cout << "User's detail..." << endl;
				cout << "Id: " << tokens.at(0) << "\nPassword: " << tokens.at(1) << "\nName: " << tokens.at(2) << "\nAddress: " << tokens.at(3) << "\nContact No.: " << tokens.at(4) << endl;
				cout << endl;
				result++;
			}
		}
		cout << "---------------------------------------------------" << endl;
		return result;
	}
	bool searchUser() {
		// opening files
		ifstream readFile;
		readFile.open("C:\\Users\\chigo\\Desktop\\data\\accountinfo.txt");
		if (!readFile) {
			return false;
		}

		int optionAns;
		cout << "Searching users...\nSearching \"manager users\" or \"customer users\"?\n1: Manager\n2: Customer\n" << endl;
		cin >> optionAns;
		while (cin.fail() || optionAns < 1 || optionAns > 2) // non-numeric input
		{
			cout << "Invalid entry...\n1: Manager\n2: Customer\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;
	
		string id;
		char input[50];
		cin.ignore();
		cout << "Enter user's ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'm' && optionAns == 1) {
			id = "m" + id;
		}
		else if (id.at(0) != 'c' && optionAns == 2) {
			id = "c" + id;
		}
		cout << "---------------------------------------------------" << endl;

		string accountInfo;
		while (getline(readFile, accountInfo)) {
			vector<string> tokens = split(accountInfo, ';');

			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

			if (tokens.size() >= 4) { // error checking
				if (tokens.at(0).compare(id) == 0) {
					cout << "User's detail..." << endl;
					cout << "Id: " << tokens.at(0) << "\nPassword: " << tokens.at(1) << "\nName: " << tokens.at(2) << "\nAddress: " << tokens.at(3) << "\nContact No.: " << tokens.at(4) << endl;
					cout << "---------------------------------------------------" << endl;
					return true;
				}
			}
		}
		return false;
	}
};



class Customer : virtual public User {
public:
	static bool existCustomer(string cid);
	bool searchProduct() {
		string id;
		char input[50];
		cin.ignore();
		cout << "Enter ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'p') {
			id = "p" + id;
		}
		Product p;
		if (p.getProductDetails(id)) { // check existence
			p.displayPInfo();
			return true;
		}
		return false;
	}
	bool addProductToCart() {
		// get pid
		string id;
		char input[50];
		cin.ignore();
		cout << "Enter Product's ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'p') {
			id = "p" + id;
		}
		OrderItem ot;
		if (ot.newOrderItem(getID(), id)) {
			return true;
		}
		else {
			return false;
		}
	}
	bool displayBill() {
		float bill = 0.0;
		string orderItemInfo;
		ifstream readOrderItemFile;
		readOrderItemFile.open("C:\\Users\\chigo\\Desktop\\data\\orderitem.txt");
		if (!readOrderItemFile) {
			return false;
		}
		while (getline(readOrderItemFile, orderItemInfo)) {
			vector<string> tokens = split(orderItemInfo, ';');
			// case-insensitive id input
			transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);
			if (tokens.size() >= 5) { // error checking
				if (tokens.at(0).compare(getID()) == 1) {
					bill = bill + stof(tokens.at(3))*stof(tokens.at(4));
				}
			}
		}
		if (bill == 0.0) {
			cout << "No bill" << endl;
			return true;
		}
		else {
			cout << "Total bill is RM" << bill << endl;
			return true;
		}
	}
};
bool Customer::existCustomer(string cid) {
	if (cid.at(0) != 'c') {
		cid = "c" + cid;
	}
	ifstream readAccountFile;
	readAccountFile.open("C:\\Users\\chigo\\Desktop\\data\\accountInfo.txt");
	string userInfo;
	// check for cid existence
	if (!readAccountFile) {
		return false;
	}
	while (getline(readAccountFile, userInfo)) {
		vector<string> tokens = split(userInfo, ';');

		// case-insensitive id input and data_id input
		transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), tolower);

		if (tokens.size() >= 1) { // error checking
			if (tokens.at(0).compare(cid) == 0) {
				readAccountFile.close();
				return true;
			}
		}
	}
	readAccountFile.close();
	return false;
}



class Manager :virtual public User {
public:
	bool addItem() {
		int optionAns;
		cout << "Creating \"product\" or \"order\"?\n1: Product\n2: Order\n" << endl;
		cin >> optionAns;
		while (cin.fail() || (optionAns > 2 || optionAns < 1)) // non-numeric input
		{
			cout << "Invalid entry\n1: Product\n2: Order\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;

		if (optionAns == 1) {
			Product p;
			p.addProduct();
			return true;
		}
		else if (optionAns == 2) {
			// get cid
			string cid;
			cout << "Enter customer's ID: ";
			cin >> cid;
			if (Customer::existCustomer(cid)) {
				// get pid
				string pid;
				cout << "Enter product's ID: ";
				cin >> pid;
				OrderItem ot;
				if (ot.newOrderItem(cid, pid)) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		return false;
	}
	bool deleteItem() {
		int optionAns;
		cout << "Deleting \"product\" or \"order\"?\n1: Product\n2: Order\n" << endl;
		cin >> optionAns;
		while (cin.fail() || (optionAns > 2 || optionAns < 1)) // non-numeric input
		{
			cout << "Invalid entry\n1: Product\n2: Order\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;
		string id;
		char input[50];
		cin.ignore();
		cout << "Enter ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'p' && optionAns == 1) {
			id = "p" + id;
		}
		else if (id.at(0) != 'o' && optionAns == 2) {
			id = "o" + id;
		}
		if (optionAns == 1) {
			if (Product::deleteProduct(id)) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (optionAns == 2) {
			/*
			if (OrderItem::deleteOrderItem(id)) {
				return true;
			}
			*/
			if (Order::deleteOrder(id)) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
	bool editItem() {
		int optionAns;
		cout << "Editing \"product\" or \"order\"?\n1: Product\n2: Order\n" << endl;
		cin >> optionAns;
		while (cin.fail() || (optionAns > 2 || optionAns < 1)) // non-numeric input
		{
			cout << "Invalid entry\n1: Product\n2: Order\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;

		string id;
		char input[50];
		cin.ignore();
		cout << "Enter ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'p' && optionAns == 1) {
			id = "p" + id;
		}
		else if (id.at(0) != 'o' && optionAns == 2) {
			id = "o" + id;
		}
		if (optionAns == 1) {
			Product p;
			if (p.editProduct(id)) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (optionAns == 2) {
			Order o;
			if (o.editOrder(id)) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
	int viewItem() {
		int optionAns;
		cout << "Editing \"product\" or \"order\"?\n1: Product\n2: Order\n" << endl;
		cin >> optionAns;
		while (cin.fail() || (optionAns > 2 || optionAns < 1)) // non-numeric input
		{
			cout << "Invalid entry\n1: Product\n2: Order\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;

		if (optionAns == 1) {
			Product p;
			return p.viewProduct();
		}
		else if (optionAns == 2) {
			Order o;
			return o.viewOrder();
		}
		return 0;
	}
	bool searchItem() {
		int optionAns;
		cout << "Searching \"product\" or \"order\"?\n1: Product\n2: Order\n" << endl;
		cin >> optionAns;
		while (cin.fail() || (optionAns > 2 || optionAns < 1)) // non-numeric input
		{
			cout << "Invalid entry\n1: Product\n2: Order\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;


		string id;
		char input[50];
		cin.ignore();
		cout << "Enter ID: ";
		cin.getline(input, sizeof(input));
		id = input; // converts char[] to string
		transform(id.begin(), id.end(), id.begin(), tolower); // to lowercase
		if (id.at(0) != 'p' && optionAns == 1) {
			id = "p" + id;
		}
		else if (id.at(0) != 'o' && optionAns == 2) {
			id = "o" + id;
		}
		if (optionAns == 1) {
			Product p;
			if (p.getProductDetails(id))
			{
				p.displayPInfo();
				return true;
			}
			else {
				return false;
			}
		}
		else if (optionAns == 2) {
			Order o;
			if (o.getOrderDetails(id)) {
				o.displayOInfo();
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
};

int main()
{
	// menu
	cout << "---------------------------------------------------" << endl;
	cout << "Welcome to Chiggy Online Shopping Management System" << endl;
	cout << "---------------------------------------------------" << endl;


	while (true) {
		// controls
		bool notLogged = true;
		int optionAns;

		cout << "Please choose your login..." << endl;
		cout << "1: Admin\n2: Manager\n3: Customer\n4: Exit System\n" << endl;
		cin >> optionAns;
		while(cin.fail() || optionAns < 1 || optionAns > 4) {
			cout << "\nInvalid entry...\n1: Admin\n2: Manager\n3: Customer\n4: Exit System\n" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cin >> optionAns;
		}
		cout << "---------------------------------------------------" << endl;


		// controller
		if (optionAns == 1) { // admin
			Admin a;
			a.setLogin();
			cout << "---------------------------------------------------" << endl;
			char returnValue = a.verifyLogin();
			if (returnValue == 'a') {
				notLogged = false;
				while (!notLogged) {
					cout << "Please choose your action..." << endl;
					cout << "1: Add User\n2: Delete User\n3: Edit User\n4: View User\n5: Search User\n6: Logoff\n7: Exit System\n" << endl;
					cin >> optionAns;
					while (cin.fail() || optionAns < 1 || optionAns > 7) {
						cout << "\nInvalid entry...\n1: Add User\n2: Delete User\n3: Edit User\n4: View User\n5: Search User\n6: Logoff\n7: Exit System\n" << endl;
						cin.clear();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						cin >> optionAns;
					}
					cout << "---------------------------------------------------" << endl;
					switch (optionAns) {
					case 1:
						if (a.addUser()) {
							cout << "New user created" << endl;
						}
						else {
							cout << "Failed to create new user" << endl;
						}
						cout << "---------------------------------------------------" << endl;
						break;
					case 2:
						if (a.deleteUser()) {
							cout << "User deleted" << endl;
						}
						else {
							cout << "Failed to delete user" << endl;
						}
						cout << "---------------------------------------------------" << endl;
						break;
					case 3:
						if (a.editUser()) {
							cout << "User edited" << endl;
						}
						else {
							cout << "Failed to edit user" << endl;
						}
						cout << "---------------------------------------------------" << endl;
						break;
					case 4:
						cout << "Number of result: " << a.viewUser() << endl;
						cout << "---------------------------------------------------" << endl;
						break;
					case 5:
						if (a.searchUser()) {
							cout << "User searched" << endl;
						}
						else {
							cout << "Failed to search user" << endl;
						}
						cout << "---------------------------------------------------" << endl;
						break;
					case 6:
						notLogged = true;
						break;
					case 7:
						cout << "Exiting System" << endl;
						return 0;
					}
				}
			}
			else if (returnValue == '1') {
				cout << "Unable to verify account\n" << endl;
				return 0;
			}
			else {
				cout << "Account does not exist" << endl;
				cout << "---------------------------------------------------" << endl;
			}
		}
		else if (optionAns == 2) {
			Manager m;
			m.setLogin();
			cout << "---------------------------------------------------" << endl;
			char returnValue = m.verifyLogin();
			if (returnValue == 'm') {
				notLogged = false;
				while (!notLogged) {
					cout << "Please choose your action...\n" << endl;
					cout << "1: Add Item\n2: Delete Item\n3: Edit Item\n4: View Item\n5: Search Item\n6: Logoff\n7: Exit System\n" << endl;
					cin >> optionAns;
					while (cin.fail() || optionAns < 1 || optionAns > 7) {
						cout << "\nInvalid entry...\n1: Add User\n2: Delete User\n3: Edit User\n4: View User\n5: Search User\n6: Logoff\n7: Exit System\n" << endl;
						cin.clear();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						cin >> optionAns;
					}
					cout << "---------------------------------------------------" << endl;
					switch (optionAns) {
					case 1:
						if (m.addItem()) {
							cout << "New item created" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to create new item" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 2:
						if (m.deleteItem()){
							cout << "Item deleted" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to delete item" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 3:
						if (m.editItem()) {
							cout << "Item edited" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to edit item" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 4:
						cout << "Number of result: " << m.viewItem() << endl;
						cout << "---------------------------------------------------" << endl;
						break;
					case 5:
						if (m.searchItem()) {
							cout << "Item searched" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to search item" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 6:
						notLogged = true;
						break;
					case 7:
						cout << "Exiting System" << endl;
						return 0;
					}
				}
			}
			else if (returnValue == '1') {
				cout << "Unable to verify account\n" << endl;
				return 0;
			}
			else {
				cout << "Account does not exist" << endl;
				cout << "---------------------------------------------------" << endl;
			}
		}
		else if (optionAns == 3) {
			Customer c;
			c.setLogin();
			cout << "---------------------------------------------------" << endl;
			char returnValue = c.verifyLogin();
			if (c.verifyLogin() == 'c') {
				notLogged = false;
				while (!notLogged) {
					cout << "Please choose your action...\n" << endl;
					cout << "1: Search Product\n2: Add to Cart\n3: Total Bill\n4: Logoff\n5: Exit\n" << endl;
					cin >> optionAns;
					while (cin.fail() || optionAns < 1 || optionAns > 7) {
						cout << "\nInvalid entry...\n1: Search Product\n2: Add to Cart\n3: Total Bill\n4: Logoff\n5: Exit\n" << endl;
						cin.clear();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						cin >> optionAns;
					}
					cout << "---------------------------------------------------" << endl;
					switch (optionAns) {
					case 1:
						if (c.searchProduct()) {
							cout << "Item searched" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to search item" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 2:
						if (c.addProductToCart()) {
							cout << "Product added to cart" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Fail to add product to cart" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 3:
						if (c.displayBill()) {
							cout << "End of calculation" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						else {
							cout << "Failed to calculate" << endl;
							cout << "---------------------------------------------------" << endl;
						}
						break;
					case 4:
						notLogged = true;
						break;
					case 5:
						cout << "Exiting System" << endl;
						return 0;
					default:
						cout << "Invalid Entry\n" << endl;
					}
				}
			}
			else {
				cout << "Invalid Account\n" << endl;
			}
		}
		else if (optionAns == 4) {
			cout << "Exiting System" << endl;
			return 0;
		}
	}
	return 0;
}
