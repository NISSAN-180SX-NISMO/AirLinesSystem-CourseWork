#pragma once
#include <string>

using std::string;
// Этот класс - ячейка данных. Здесь просто хранятся все данные о пассажире.
struct Passenger {
	string passport_number;	// Серия и номер пасспорта в формате "NNNN-NNNNNN"
	string place_and_date;	// Место и дата выдачи паспорта
	string name;			// ФИО
	string birth_date;		// Дата рождения
	//Passenger(int zero) {}
	Passenger() { passport_number = place_and_date = name = birth_date = ""; }
	bool is_cool() {
		if (passport_number != "" && place_and_date != "" && name != "" && birth_date != "") return true;
		else { return false; }
	}
	void clear() { passport_number.clear(); place_and_date.clear(); name.clear(); birth_date.clear(); }
};