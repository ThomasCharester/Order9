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
	// 0 - Зритель, 1 - Администратор
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

	// Учётные записи //
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

	// Участники //
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
	// Цвета 0-Белый,1-Красный,2-Синий,3-Зелёный,4-Оранжевый,5-Розовый,6-Жёлтый.
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
				printColor("&1Неправильный тип данных!");
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
				printColor("&1Неправильный тип данных!");
				cin.clear();
				cin.ignore();
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&1Введите значение в диапазоне от " + to_string(min) + " до " + to_string(max));
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
		return input<bool>("&6Вы уверены? (&10-Нет&0,&31-Да&0)");
	}

	// Наборы меню	
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6Добро пожаловать " + db->getLogin());
			printColor("1 - Редактировать список студентов");
			printColor("2 - Редактировать список аккаунтов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 2);
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
			printColor("&6Добро пожаловать " + db->getLogin());
			printColor("1 - Посмотреть на список студентов");
			printColor("2 - Сортировка списка студентов");
			printColor("3 - Поиск в списке студентов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 3);
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
			printColor("Редактировать список студентов");
			printColor("1 - Добавить студента");
			printColor("2 - Удалить студента");
			printColor("3 - Посмотреть список студентов");
			printColor("4 - Отсортировать список студентов");
			printColor("5 - Поиск в списке студентов");
			printColor("6 - Рассчитать приоритеты в конкурсе стипендий");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 6);
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
			printColor("Редактировать список аккаунтов");
			printColor("1 - Добавить аккаунт");
			printColor("2 - Удалить аккаунт");
			printColor("3 - Посмотреть список аккаунтов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 3);
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
			printColor("Порядки сортировки");
			printColor("1 - По рейтингу");
			printColor("2 - По фамилии");
			printColor("3 - По среднему баллу");
			printColor("4 - По количеству участий в научных конференциях");
			printColor("5 - По количеству студенческих работ");
			printColor("0 - Выйти");

			int choice = inputRange<int>("Выберите действие из списка", 0, 5);
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
			printColor("Признаки поиска");
			printColor("1 - По фамилии");
			printColor("2 - По количеству участий в конференциях");
			printColor("3 - По количеству научных работ");
			printColor("4 - По среднему баллу");
			printColor("0 - Выйти");

			int choice = inputRange<int>("Выберите действие из списка", 0, 4);
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



// Определения методов

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
			ui->printColor("&1Пользователь уже авторизован");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&1Нет аккаунтов для авторизации");
			ui->printColor("Создайте аккаунт администратора для начала работы с базой данных");
			addAccount(1);
			currentAccount = accounts.at(0);
			ui->printColor("&3Авторизация успешна");
			return;
		}
		bool isRegistered = ui->inputRange("Вы уже зарегистрированы в системе? (&10-Нет&0,&31-Да&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("Хотите зарегистрироваться в качестве гостя? (&10-Нет&0,&31-Да&0)", 0, 1);
			if (wantRegister) {
				addAccount(0);
				currentAccount = accounts.at(accounts.size()-1);
				ui->printColor("&3Авторизация успешна");
			}
		}
		else {
			ui->printColor("Войдите в аккаунт");
			string login;

			do {
				login = ui->input<string>("Введите логин");
				if (checkLogin(login)) {
					break;
				}
				ui->printColor("&1Неверный логин!");
			} while (true);

			int accountID = findID(login);
			int attempts = 3;
			string password;

			do {
				password = ui->input<string>("Введите пароль");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&1Неверный пароль!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&1Вы неверно ввели пароль больше 3 раз, попробуйте снова.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&3Авторизация успешна");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&1Список пуст");
		return;
	}
	for (int i = 0; i < accounts.size(); i++) {
		ui->printColor(to_string(i) + " " + accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			ui->printColor(" гость", false);
			break;
		case 1:
			ui->printColor(" администратор", false);
			break;
		}
	}
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("Введите логин");
		if (!checkLogin(login)) break;
		ui->printColor("&1Этот пользователь уже существует");
	}
	string password = ui->input<string>("Введите пароль");

	int accountType = type;
	if (type < 0 || type > 1)
		accountType = ui->inputRange<int>("Введите тип аккаунта (&10-Гость&0,&31-Администратор&0)",0,1);

	accounts.emplace_back(new Account(login, password, accountType));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&1 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемого аккаунта", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&1Вы не можете удалить аккаунт, в котором авторизованы");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&3Аккаунт успешно удалён");
}

void Database::showStudentInfo() {
	if (participants.empty())
	{
		ui->printColor("&1Список участников пуст");
		return;
	}
	for (int i = 0; i < participants.size(); i++) {
		ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
			+ "\n\tСредний балл: " + to_string(participants.at(i)->averageMark)
			+ "\n\tКоличество участий в научных конференциях: " + to_string(participants.at(i)->scienceConfs)
			+ "\n\tКоличество научных работ: " + to_string(participants.at(i)->scienceWorks)
			+ "\n\tПриоритет: " + to_string(participants.at(i)->rate));
	}
}

void Database::addStudent() {
	string name = ui->input<string>("Введите имя");
	string secondName = ui->input<string>("Введите фамилию");
	string surName = ui->input<string>("Введите отчество");
	double averageMark = ui->input<double>("Введите средний балл");
	int scienceConfs = ui->input<int>("Введите количество участий в конференциях");
	int scienceWorks = ui->input<int>("Введите количество научных работ");

	participants.emplace_back(new Student(name, secondName, surName, averageMark, scienceConfs, scienceWorks, 0));

	writeStudentsToFile();
}
void Database::removeStudent() {
	showStudentInfo();

	if (participants.empty()) {
		ui->printColor("&1 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемого студента", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeStudentsToFile();

	ui->printColor("&3Студент успешно удален");
}

void Database::findStudentSecondName() {
	if (participants.empty())
	{
		ui->printColor("&1Список участниц пуст");
		return;
	}
	string secondName = ui->input<string>("Введите фамилию для поиска: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->secondName == secondName)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\tСредний балл: " + to_string(participants.at(i)->averageMark)
				+ "\n\tКоличество участий в научных конференциях: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\tКоличество научных работ: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\tПриоритет: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentAverageMark() {
	if (participants.empty())
	{
		ui->printColor("&1Список участников пуст");
		return;
	}
	int averageMark = ui->input<int>("Введите средний балл для поиска: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->averageMark == averageMark)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\tСредний балл: " + to_string(participants.at(i)->averageMark)
				+ "\n\tКоличество участий в научных конференциях: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\tКоличество научных работ: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\tПриоритет: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentScienceConfs() {
	if (participants.empty())
	{
		ui->printColor("&1Список участников пуст");
		return;
	}
	int scienceConfs = ui->input<int>("Введите количество участий в научных конференциях для поиска: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->scienceConfs == scienceConfs)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\tСредний балл: " + to_string(participants.at(i)->averageMark)
				+ "\n\tКоличество участий в научных конференциях: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\tКоличество научных работ: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\tПриоритет: " + to_string(participants.at(i)->rate));
	}
}
void Database::findStudentScienceWorks() {
	if (participants.empty())
	{
		ui->printColor("&1Список участников пуст");
		return;
	}
	int scienceWorks = ui->input<int>("Введите количество участий в научных конференциях для поиска: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->scienceWorks == scienceWorks)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->secondName + " " + participants.at(i)->surName
				+ "\n\tСредний балл: " + to_string(participants.at(i)->averageMark)
				+ "\n\tКоличество участий в научных конференциях: " + to_string(participants.at(i)->scienceConfs)
				+ "\n\tКоличество научных работ: " + to_string(participants.at(i)->scienceWorks)
				+ "\n\tПриоритет: " + to_string(participants.at(i)->rate));
	}
}

