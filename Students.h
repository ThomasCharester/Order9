#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "windows.h"


using namespace std;

class UI;

string enDecrypt(string text) {
	char key = text.size() % 10;

	for (char& ch : text)
		ch ^= key;
	return text;
}

struct Account {
	// 0 - �������, 1 - �������������
	int accountType = 0;
	string password = "";
	string login = "";

	Account() {};
	Account(string login, string password, int accountType) {
		this->login = login;
		this->password = password;
		this->accountType = accountType;
	}
	bool operator == (const Account& account) {
		return login == account.login && password == account.password;
	}
	void addToFile() const
	{
		fstream file("accounts.txt", ios::app);
		file << login << ' ' << enDecrypt(password) << ' ' << accountType << '\n';
		file.close();
	}
};
class Database {
private:
	struct Student {
		string name = "";
		string secondName = "";
		string surName = "";
		double averageMark = 0;
		int scienceConfs = 0;
		int scienceWorks = 0;

		int rate = 0;

		Student() {};
		Student(string name, string secondName, string surName, double averageMark, int scienceConfs, int scienceWorks, int rate) {
			this->name = name;
			this->surName = surName;
			this->secondName = secondName;
			this->averageMark = averageMark;
			this->scienceConfs = scienceConfs;
			this->scienceWorks = scienceWorks;
			this->rate = rate;
		}
		void addToFile() const
		{
			fstream file("participants.txt", ios::app);
			file << name << ' ' << secondName << ' ' << surName << ' ' << averageMark << ' ' << scienceConfs << ' ' << scienceWorks << ' ' << rate << '\n';

			file.close();
		}
		static bool byRate(const Student* pt1, const Student* pt2) {
			return pt1->rate > pt2->rate;
		}
		static bool byAverageMark(const Student* pt1, const Student* pt2) {
			return pt1->averageMark > pt2->averageMark;
		}
		static bool byScienceConfs(const Student* pt1, const Student* pt2) {
			return pt1->scienceConfs > pt2->scienceConfs;
		}
		static bool byScienceWorks(const Student* pt1, const Student* pt2) {
			return pt1->scienceWorks > pt2->scienceWorks;
		}
		static bool bySecondName(const Student* pt1, const Student* pt2) {
			return pt1->secondName.compare(pt2->secondName) < 0;
		}
		void calculateRating() {
			rate = averageMark + scienceConfs + scienceWorks;
		}
	};
	Account* currentAccount = nullptr;
	vector<Account*> accounts;

	vector<Student*> participants;

	UI* ui;

	int findID(string login) {
		for (int i = 0; i < accounts.size(); i++) {
			if (accounts.at(i)->login == login) return i;
		}
		return -1;
	}
	const bool checkPassword(int id, string password) {
		return accounts.at(id)->password == password;
	}

public:
	Database(UI* ui);

	~Database();

	const int whoIsNow() const {
		if (isLoggedIn()) return currentAccount->accountType;
		else return -1;
	}
	const bool isLoggedIn() const { return currentAccount; }

	const bool checkLogin(string login) {
		for (Account* ac : accounts) {
			if (ac->login == login) return true;
		}
		return false;
	}

	const string getLogin() const { return currentAccount->login; }

	// ������� ������ //
	void login();
	void deauthorize() { currentAccount = nullptr; }

	void showAccounts();
	void addAccount(int type = -1);
	void removeAccount();

	void writeAccountsToFile()
	{
		fstream file("accounts.txt", ios::out);
		file << accounts.size() << '\n';
		file.close();

		for (int i = 0; i < accounts.size(); i++)
			accounts.at(i)->addToFile();
	}
	void readAccountsFromFile()
	{
		accounts.clear();
		fstream file("accounts.txt", ios::in);

		if (file.eof() || !file.is_open())
		{
			file.close();
			return;
		}

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; i++)
		{
			string login, password;
			getline(file, login, ' ');
			getline(file, password, ' ');

			getline(file, temp, '\n');
			int accountType = stoi(temp);

			accounts.emplace_back(new Account(login, enDecrypt(password), accountType));
		}
		file.close();
	}

	// ��������� //
	void showStudentInfo();

	void addStudent();
	void removeStudent();
	void calculateStudentsRatings();

	void sortStudentRate() { sort(participants.begin(), participants.end(), Student::byRate); }
	void sortStudentAverageMark() { sort(participants.begin(), participants.end(), Student::byAverageMark); }
	void sortStudentScienceConfs() { sort(participants.begin(), participants.end(), Student::byScienceConfs); }
	void sortStudentScienceWorks() { sort(participants.begin(), participants.end(), Student::byScienceWorks); }
	void sortStudentSecondName() { sort(participants.begin(), participants.end(), Student::bySecondName); }

	void findStudentSecondName();
	void findStudentScienceConfs();
	void findStudentScienceWorks();
	void findStudentAverageMark();

	void writeStudentsToFile()
	{
		fstream file("participants.txt", ios::out);
		file << participants.size() << '\n';
		file.close();

		for (int i = 0; i < participants.size(); i++)
			participants.at(i)->addToFile();
	}
	void readStudentsFromFile()
	{
		participants.clear();
		fstream file("participants.txt", ios::in);

		if (file.eof() || !file.is_open())
		{
			file.close();
			return;
		}

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; )
		{
			i++;
			string name, surName, secondName;
			int scienceConfs, scienceWorks, rate;
			double averageMark;
			getline(file, name, ' ');
			getline(file, secondName, ' ');
			getline(file, surName, ' ');
			getline(file, temp, ' ');
			averageMark = stod(temp);
			getline(file, temp, ' ');
			scienceConfs = stoi(temp);
			getline(file, temp, ' ');
			scienceWorks = stoi(temp);
			getline(file, temp, '\n');
			rate = stoi(temp);
			participants.emplace_back(new Student(name, secondName, surName, averageMark, scienceConfs, scienceWorks, rate));

		}
		file.close();
	}
};
class UI {
private:
	Database* db = nullptr;
public:
	UI() {
		setlocale(LC_ALL, "Russian");
	}
	void setDatabase(Database* db) { this->db = db; }
	// ����� 0-�����,1-�������,2-�����,3-������,4-���������,5-�������,6-Ƹ����.
	enum colors : int {
		White,
		Red,
		Blue,
		Green,
		Orange,
		Pink,
		Yellow
	};
	void setColor(UI::colors color) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (color) {
		case UI::colors::Red:
			SetConsoleTextAttribute(handle, 12);
			break;
		case UI::colors::Green:
			SetConsoleTextAttribute(handle, 10);
			break;
		case UI::colors::Blue:
			SetConsoleTextAttribute(handle, 9);
			break;
		case UI::colors::Yellow:
			SetConsoleTextAttribute(handle, 14);
			break;
		case UI::colors::Orange:
			SetConsoleTextAttribute(handle, 6);
			break;
		case UI::colors::Pink:
			SetConsoleTextAttribute(handle, 13);
			break;
		default:
			SetConsoleTextAttribute(handle, 7);
			break;
		}
	}
	template<typename T>
	T input(string text, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&1������������ ��� ������!");
				cin.clear();
				cin.ignore();
			}
			else {
				return input;
			}
		}
	}
	template<typename T>
	T inputRange(string text, T min, T max, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&1������������ ��� ������!");
				cin.clear();
				cin.ignore();
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&1������� �������� � ��������� �� " + to_string(min) + " �� " + to_string(max));
			}
			else {
				return input;
			}
		}
	}
	template <typename T>
	void print(T data, bool newLine = true) {
		if (newLine)  cout << '\n';
		cout << data;
	}
	void printColor(string str, bool newLine = true) {
		if (newLine)  cout << '\n';
		bool flag = false;
		for (char ch : str) {
			if (!flag)
				if (ch != '&')
					cout << ch;
				else flag = true;
			else {
				setColor(static_cast<UI::colors>((ch - 48)));
				flag = false;
			}
		}
		setColor(White);
	}
	void pressAnyButton() {
		cout << '\n';
		system("pause");
	}
	const bool confirm() {
		return input<bool>("&6�� �������? (&10-���&0,&31-��&0)");
	}

	// ������ ����	
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6����� ���������� " + db->getLogin());
			printColor("1 - ������������� ������ ���������");
			printColor("2 - ������������� ������ ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 2);
			system("cls");
			switch (choice) {
			case 0:
				db->deauthorize();
				return;
				break;
			case 1:
				participantsEditing();
				break;
			case 2:
				accountsEditing();
				break;
			}
		}
	}

	void spectatorMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6����� ���������� " + db->getLogin());
			printColor("1 - ���������� �� ������ ���������");
			printColor("2 - ���������� ������ ���������");
			printColor("3 - ����� � ������ ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 3);
			system("cls");
			switch (choice) {
			case 0:
				db->deauthorize();
				return;
				break;
			case 1:
				db->showStudentInfo();
				pressAnyButton();
				break;
			case 2:
				sortMenu();
				break;
			case 3:
				searchMenu();
				break;
			}
		}
	}
	
	void participantsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������������� ������ ���������");
			printColor("1 - �������� ��������");
			printColor("2 - ������� ��������");
			printColor("3 - ���������� ������ ���������");
			printColor("4 - ������������� ������ ���������");
			printColor("5 - ����� � ������ ���������");
			printColor("6 - ���������� ���������� � �������� ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 6);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->addStudent();
				break;
			case 2:
				db->removeStudent();
				break;
			case 3:
				system("cls");
				db->showStudentInfo();
				pressAnyButton();
				break;
			case 4:
				sortMenu();
				break;
			case 5:
				searchMenu();
				break;
			case 6:
				db->calculateStudentsRatings();
				break;
			}
		}
	}

	void accountsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������������� ������ ���������");
			printColor("1 - �������� �������");
			printColor("2 - ������� �������");
			printColor("3 - ���������� ������ ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 3);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->addAccount();
				break;
			case 2:
				db->removeAccount();
				break;
			case 3:
				system("cls");
				db->showAccounts();
				pressAnyButton();
				break;
			}
		}
	}
	void sortMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������� ����������");
			printColor("1 - �� ��������");
			printColor("2 - �� �������");
			printColor("3 - �� �������� �����");
			printColor("4 - �� ���������� ������� � ������� ������������");
			printColor("5 - �� ���������� ������������ �����");
			printColor("0 - �����");

			int choice = inputRange<int>("�������� �������� �� ������", 0, 5);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->sortStudentRate();
				break;
			case 2:
				db->sortStudentSecondName();
				break;
			case 3:
				db->sortStudentAverageMark();
				break;
			case 4:
				db->sortStudentScienceConfs();
				break;
			case 5:
				db->sortStudentScienceWorks();
				break;
			}
			return;
		}
	}
	void searchMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("�������� ������");
			printColor("1 - �� �������");
			printColor("2 - �� ���������� ������� � ������������");
			printColor("3 - �� ���������� ������� �����");
			printColor("4 - �� �������� �����");
			printColor("0 - �����");

			int choice = inputRange<int>("�������� �������� �� ������", 0, 4);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				system("cls");
				db->findStudentSecondName();
				pressAnyButton();
				break;
			case 2:
				system("cls");
				db->findStudentScienceConfs();
				pressAnyButton();
				break;
			case 3:
				system("cls");
				db->findStudentScienceWorks();
				pressAnyButton();
				break;
			case 4:
				system("cls");
				db->findStudentAverageMark();
				pressAnyButton();
				break;
			}
		}
	}
};



// ����������� �������

void Database::calculateStudentsRatings()
{
	for (Student* p : participants)
		p->calculateRating();

	writeStudentsToFile();
}

Database::Database(UI* ui) {
	readStudentsFromFile();
	calculateStudentsRatings();

	this->ui = ui;
}
Database::~Database() {
	writeAccountsToFile();
	calculateStudentsRatings();
	writeStudentsToFile();
}

void Database::login() {
	while (true) {
		if (currentAccount) {
			ui->printColor("&1������������ ��� �����������");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&1��� ��������� ��� �����������");
			ui->printColor("�������� ������� �������������� ��� ������ ������ � ����� ������");
			addAccount(1);
			currentAccount = accounts.at(0);
			ui->printColor("&3����������� �������");
			return;
		}
		bool isRegistered = ui->inputRange("�� ��� ���������������� � �������? (&10-���&0,&31-��&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("������ ������������������ � �������� �����? (&10-���&0,&31-��&0)", 0, 1);
			if (wantRegister) {
				addAccount(0);
				currentAccount = accounts.at(accounts.size()-1);
				ui->printColor("&3����������� �������");
			}
		}
		else {
			ui->printColor("������� � �������");
			string login;

			do {
				login = ui->input<string>("������� �����");
				if (checkLogin(login)) {
					break;
				}
				ui->printColor("&1�������� �����!");
			} while (true);

			int accountID = findID(login);
			int attempts = 3;
			string password;

			do {
				password = ui->input<string>("������� ������");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&1�������� ������!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&1�� ������� ����� ������ ������ 3 ���, ���������� �����.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&3����������� �������");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&1������ ����");
		return;
	}
	for (int i = 0; i < accounts.size(); i++) {
		ui->printColor(to_string(i) + " " + accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			ui->printColor(" �����", false);
			break;
		case 1:
			ui->printColor(" �������������", false);
			break;
		}
	}
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("������� �����");
		if (!checkLogin(login)) break;
		ui->printColor("&1���� ������������ ��� ����������");
	}
	string password = ui->input<string>("������� ������");

	int accountType = type;
	if (type < 0 || type > 1)
		accountType = ui->inputRange<int>("������� ��� �������� (&10-�����&0,&31-�������������&0)",0,1);

	accounts.emplace_back(new Account(login, password, accountType));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ���������� ��������", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&1�� �� ������ ������� �������, � ������� ������������");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&3������� ������� �����");
}

void Database::showStudentInfo() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	for (int i = 0; i < participants.size(); i++) {
		ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
			+ "\n\t������� ����: " + to_string(participants.at(i)->averageMark)
			+ "\n\t���������� ������� � ������� ������������: " + to_string(participants.at(i)->scienceConfs)
			+ "\n\t���������� ������� �����: " + to_string(participants.at(i)->scienceWorks)
			+ "\n\t���������: " + to_string(participants.at(i)->rate));
	}
}

void Database::addStudent() {
	string name = ui->input<string>("������� ���");
	string secondName = ui->input<string>("������� �������");
	string surName = ui->input<string>("������� ��������");
	double averageMark = ui->input<double>("������� ������� ����");
	int scienceConfs = ui->input<int>("������� ���������� ������� � ������������");
	int scienceWorks = ui->input<int>("������� ���������� ������� �����");

	participants.emplace_back(new Student(name, secondName, surName, averageMark, scienceConfs, scienceWorks, 0));

	writeStudentsToFile();
}
void Database::removeStudent() {
	showStudentInfo();

	if (participants.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ���������� ��������", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeStudentsToFile();

	ui->printColor("&3������� ������� ������");
}

void Database::findStudentSecondName() {
	if (participants.empty())
	{
		ui->printColor("&1������ �������� ����");
		return;
	}
	string secondName = ui->input<string>("������� ������� ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->secondName == secondName)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\t������� ����: " + to_string(participants.at(i)->averageMark)
				+ "\n\t���������� ������� � ������� ������������: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\t���������� ������� �����: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\t���������: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentAverageMark() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	int averageMark = ui->input<int>("������� ������� ���� ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->averageMark == averageMark)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\t������� ����: " + to_string(participants.at(i)->averageMark)
				+ "\n\t���������� ������� � ������� ������������: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\t���������� ������� �����: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\t���������: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentScienceConfs() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	int scienceConfs = ui->input<int>("������� ���������� ������� � ������� ������������ ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->scienceConfs == scienceConfs)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\t������� ����: " + to_string(participants.at(i)->averageMark)
				+ "\n\t���������� ������� � ������� ������������: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\t���������� ������� �����: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\t���������: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentScienceWorks() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	int scienceWorks = ui->input<int>("������� ���������� ������� � ������� ������������ ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->scienceWorks == scienceWorks)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\t������� ����: " + to_string(participants.at(i)->averageMark)
				+ "\n\t���������� ������� � ������� ������������: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\t���������� ������� �����: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\t���������: " + to_string(participants.at(i)->rate));
	}
}

