#include "Students.h"

int main() {

	UI ui;
	Database database(&ui);

	ui.setDatabase(&database);

	while (true) {
		system("cls");

		database.login();

		switch (database.whoIsNow()) {
		case 0:
			ui.spectatorMenu();
			break;
		case 1:
			ui.adminMenu();
			break;
		}
		bool choice = ui.input<bool>("Желаете &1выйти&0 или &3продолжить &0использование программы? (&10-Выйти&0/&31-Продолжить&0)");

		if (!choice) {
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6Хорошего дня!");
			return 0;
		}
	}

	return 0;
}