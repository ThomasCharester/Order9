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
		bool choice = ui.input<bool>("������� &1�����&0 ��� &3���������� &0������������� ���������? (&10-�����&0/&31-����������&0)");

		if (!choice) {
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6�������� ���!");
			return 0;
		}
	}

	return 0;
}