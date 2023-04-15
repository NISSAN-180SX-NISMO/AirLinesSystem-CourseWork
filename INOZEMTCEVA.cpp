#include <Windows.h>
#include <iostream>
#include <string>

#include "InputExceptions.h"
#include "PassengersMAP.h"
#include "FlightsTree.h"
#include "TradesList.h"
#include "Resources.h"
#include "Checks.h"

#include "MENU.h"
using namespace std;

PassengersMAP tbl;
FlightsTree tree;
TradesList lst;
string choice;

#pragma region Fillers
string genticket() {
	string number;
	for (int i = 0; i < 9; i++) { number += to_string(rand() % 10); }
	return number;
}
string gendate(string IntPoint = "", int flag = 0) {
	while (true) {
		string day, month, year;
		day = to_string(1 + rand() % 30);
		month = to_string(1 + rand() % 12);
		if (flag == 0) { year = to_string(1950 + rand() % 75); }
		else if (flag == 1) { year = "20" + IntPoint; }
		else {
			switch (rand() % 3) {
			case 0: year = to_string(2000 + atoi(IntPoint.c_str()) - 15); break;
			case 1: year = to_string(2000 + atoi(IntPoint.c_str()) - 21); break;
			case 2: year = to_string(2000 + atoi(IntPoint.c_str()) - 46); break;
			}
		}
		if (atoi(day.c_str()) < 10) { day = "0" + day; }
		if (atoi(month.c_str()) < 10) { month = "0" + month; }
		string date = day + "/" + month + "/" + year;
		try { DATE_IS_REAL(date); return date; }
		catch (UncorrectDate& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
	}
}

namespace FillTable {
	string gennumber() {
		string code_region, year, number, output;
		code_region = to_string(rand() % 10) + to_string(rand() % 10);
		year = "1" + to_string(rand() % 10); number.clear();
		for (int i = 0; i < 6; ++i) { number += to_string(rand() % 10); }
		output = code_region + year + number;
		return output;
	}
	string genplace(string& NUM) {
		string code_region, year, number, department, region, city;
		do {
			do {
				code_region = to_string(rand() % 10) + to_string(rand() % 10);
				year = "1" + to_string(rand() % 10); number.clear();
				for (int i = 0; i < 6; ++i) { number += to_string(rand() % 10); }
			} while (MyResources::Code_Region.find(code_region) == MyResources::Code_Region.end());
			region = (*MyResources::Code_Region.find(code_region)).second;
		} while (!MyResources::Region_City.rand(region).second);
		city = MyResources::Region_City.rand(region).first;

		switch (rand() % 4) {
		case 0: department = "ОТДЕЛОМ УФМС РОССИИ"; break;
		case 1: department = "ОТДЕЛОМ УФМС"; break;
		case 2: department = "МВД РОССИИ"; break;
		case 3: department = "ОТДЕЛОМ МИЛИЦИИ"; break;
		}

		NUM = code_region + NUM[2] + NUM[3] + number;
		return department + " ПО " + region + " В Г. " + city;
	}
	string genname() {
		string name, surname, patron;
		switch (rand() % 2) {
		case 0: name = MyResources::ManNames.rand(); surname = MyResources::ManSurnames.rand(); patron = MyResources::ManPatron.rand(); break;
		case 1: name = MyResources::WomanNames.rand(); surname = MyResources::WomanSurnames.rand(); patron = MyResources::WomanPatron.rand(); break;
		}

		return surname + " " + name + " " + patron;
	}
}

void Fill(PassengersMAP& tbl, int size) {
	for (int i = 0; i < size; ++i) {
		try {
			string year, number = FillTable::gennumber();
			year = number; year.erase(0, 2).erase(2, 8);
			tbl.set_place(FillTable::genplace(number), gendate(year, 1), false);
			tbl.set_number(number, false);
			tbl.set_name(FillTable::genname(), false);
			tbl.set_date(gendate(year, 2));
			auto flight = tree.get_rand();
			if (!flight) { cout << "Свободные места кончились!!!" << endl; system("pause"); return; }
			if (!flight->airplane.free) continue;
			flight->airplane.free--; tree.take_a_seat();
			lst.set_ticket(genticket());
			lst.set_airplane(flight->airplane.number);
			lst.set_passport(number);
			lst.push_back(TYPE::SALE);
			tbl.insert();
		}
		catch (InputExceptions& ex) { cout << "\t" << ex.what() << " " << ex.getmore() << endl; --i; }
		catch (exception& ex) { cout << ex.what() << endl; }
	}
	//cout << "Сортировка логов..." << endl;
	//lst.sort();
}

namespace FillTree {
	string gennumber() {
		string chars, nums;
		for (int i = 0; i < 3; i++) { chars += char(65 + rand() % 26); }
		nums = to_string(100 + rand() % 900);
		return chars + nums;
	}
	string genname() { return MyResources::Airlines.rand(); }
	string gendeparture() { return MyResources::Airports.rand(); }
	string genarrival() { return MyResources::Airports.rand(); }
	string gendep_time() {
		string hours, minutes;
		hours = to_string(rand() % 25);
		minutes = to_string(rand() % 61);
		if (atoi(hours.c_str()) < 10) { hours = "0" + hours; }
		if (atoi(minutes.c_str()) < 10) { minutes = "0" + minutes; }
		return hours + ":" + minutes;
	}
	int genotal() { return 150 * (1 + rand() % 6); }

}

void Fill(FlightsTree& tree, int size) {
	for (int i = 0; i < size; i++) {
		try {
			int total = FillTree::genotal();
			tree.set_number(FillTree::gennumber());
			tree.set_name(FillTree::genname());
			tree.set_departure(FillTree::gendeparture());
			tree.set_arrival(FillTree::genarrival());
			tree.set_dep_date(gendate());
			tree.set_dep_time(FillTree::gendep_time());
			tree.set_total(total);
			tree.set_free(total);
			tree.insert();
		}
		catch (InputExceptions& ex) { cout << ex.what() << " " << ex.getmore() << endl; --i; }
		catch (exception& ex) { cout << ex.what() << endl; }
	}
}

#pragma endregion

#pragma region MENUS_CASES


void Add_new_Passenger() {
	cout << endl;
	if (!tree.GetAllFree()) {
		cout << "Свободные места отстутствуют!!! Зарегистрируйте новые авиарейсы!!!" << endl;
		system("pause");
		return;
	}
	string series, number, place, date1, name, date2;
	bool WAS_SETTED = false;
	try {
		do {
			try {
				cout << "Введите серию паспорта: "; getline(cin, series);
				cout << "Введите номер паспорта: "; getline(cin, number); 
				WAS_SETTED = tbl.set_number(series + number);
			}
			catch (UncorrectKey& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try {
				cout << "Введите место выдачи (как в паспорте):\n"; getline(cin, place);
				cout << "Введите дату выдачи: "; getline(cin, date1); WAS_SETTED = tbl.set_place(place, date1);
			}
			catch (UncorrectPlace& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "Введите ФИО: "; getline(cin, name); WAS_SETTED = tbl.set_name(name); }
			catch (UncorrectName& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "Введите дату рождения: "; getline(cin, date2); WAS_SETTED = tbl.set_date(date2); }
			catch (UncorrectDate& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
			catch (ImpossibleDate& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;
		string airnumber;
		while (true) {
			do {
				try { cout << "Введите номер желаемого авиарейса: "; getline(cin, airnumber); WAS_SETTED = AIRNAME_IS_COOL(airnumber); }
				catch (UncorrectAirNumber& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
			} while (!WAS_SETTED);
			WAS_SETTED = false;

		
			auto flight = tree.find(airnumber);
			if (!flight) cout << "Самолет не найден!!! Повторите ввод!!!" << endl;
			else if (!flight->airplane.free) cout << "Все места заняты!!! Выберите другой самолет!!!" << endl;
			else { flight->airplane.free--; tree.take_a_seat(); break; }
		}
		string ticket;
		do {
			try { cout << "Введите номер посадочного билета: "; getline(cin, ticket); WAS_SETTED = lst.set_ticket(ticket); }
			catch (UncorrectTicket& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		lst.set_airplane(airnumber);
		lst.set_passport(series + number);
		lst.push_back(TYPE::SALE);
		tbl.insert();
		cout << "Пассажир успешно зарегистрирован!" << endl; system("pause");
	}
	catch (InputExceptions& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
	catch (exception& ex) { cout << ex.what() << endl; }
}
void Delete_Passenger() {
	string passport;
	while (true) {
		try {
			cout << "Введите номер номер паспорта ненужного пассажира: "; getline(cin, passport);
			KEY_IS_COOL(passport);
			break;
		}
		catch (UncorrectKey & ex) { cout << "Повторите ввод!!!" << endl; }
	}
	if (tbl.remove(passport)) {
		auto trade = lst.find(passport);
		if (!trade) { cout << "Такой пассажир не покупал билет!!!" << endl; system("pause"); return; }
		tree.find(trade->trade.air_number)->airplane.free++;
		tree.free_up_space();
		lst.set_airplane(trade->trade.air_number);
		lst.set_passport(trade->trade.passport_number);
		lst.set_ticket(trade->trade.ticket_number);
		lst.push_back(TYPE::REFUND);
		//lst.sort();
		cout << "Возврат успешно оформлен!" << endl; 
		cout << "Пассажир успешно удалён!" << endl; 
	}
	else { cout << "Такого пассажира нет!!!" << endl; }
	system("pause");
}
void Clear_Table() { 
	for (int i = 0; i < CAPACITY; ++i) {
		while (tbl[i]) {
			auto trade = lst.find(tbl[i]->person.passport_number);
			tree.find(trade->trade.air_number)->airplane.free++;
			tree.free_up_space();
			lst.set_airplane(trade->trade.air_number);
			lst.set_passport(trade->trade.passport_number);
			lst.set_ticket(trade->trade.ticket_number);
			lst.push_back(TYPE::REFUND);
			tbl.remove(tbl[i]->person.passport_number);
		}
	}
	//cout << "Сортировква логов..." << endl;
	//lst.sort();
	cout << "Список авиарейсов очищен!" << endl; system("pause");
}
void Rand_Fill_Table() {
	string size;
	cout << "\tВведите желаемый размер таблицы: "; getline(cin, size);
	if (atoi(size.c_str()) == 0) { cout << "\tЭлементы не будут добавлены!" << endl; }
	else { Fill(tbl, atoi(size.c_str())); cout << "\tТаблица заполнена!" << endl; }
	cout << "\t"; system("pause");
}	
void PassengersMenu() {
	cout << " [ 4 ]=[ Действия со списком пассажиров ]:" << endl;
	cout << "   `-=[ 1 ]=[ Добавить пассажира ] [...]" << endl;
	cout << "   `-=[ 2 ]=[ Удалить пассажирa ] [...]" << endl;
	cout << "   `-=[ 3 ]=[ Очистить список ]" << endl;
	cout << "   `-=[ 4 ]=[ Заполнить случайно ] [...]" << endl;
	cout << "   `-=[ 0 ]=[ Назад ]" << endl << endl;
	cout << "\tВведите номер пункта: ";
	getline(cin, choice);
	switch (atoi(choice.c_str()))
	{
	case 1: Add_new_Passenger(); break;
	case 2: Delete_Passenger(); break;
	case 3: Clear_Table(); break;
	case 4: Rand_Fill_Table(); break;
	case 0: return;
	default:
		cout << "\tТакого пункта меню нет!!!" << endl;
		system("pause");
		break;
	}

}
////////////////////////////
void Add_new_Airline() {
	string number, name, departure, arrival, date, time, total, free;
	bool WAS_SETTED = false;

	try {
		do {
			try { cout << "\tВведите номер авиарейса: "; getline(cin, number); WAS_SETTED = tree.set_number(number); }
			catch (UncorrectAirNumber& ex) { cout << ex.what() << " " << ex.getmore() << endl;}
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			cout << "\tВведите имя авиакомпании: "; getline(cin, name); WAS_SETTED = tree.set_name(name);
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите аэропорт отправления: "; getline(cin, departure); WAS_SETTED = tree.set_departure(departure); }
			catch (UncorrectAirport& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите аэропорт прибытия: "; getline(cin, arrival); WAS_SETTED = tree.set_arrival(arrival); }
			catch (UncorrectAirport& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите дату вылета: "; getline(cin, date); WAS_SETTED = tree.set_dep_date(date); }
			catch (UncorrectDate& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
			catch (ImpossibleDate& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите время вылета: "; getline(cin, time); WAS_SETTED = tree.set_dep_time(time); }
			catch (UncorrectTime& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите общее количество мест: "; getline(cin, total); WAS_SETTED = tree.set_total(atoi(total.c_str())); }
			catch (UncorrectTotal& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		WAS_SETTED = false;

		do {
			try { cout << "\tВведите количество свободных мест: "; getline(cin, free); WAS_SETTED = tree.set_free(atoi(free.c_str())); }
			catch (UncorrectFree& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
		} while (!WAS_SETTED);
		tree.insert();
		cout << "\tЭлемент успешно добавлен!" << endl; system("pause");
	}
	catch (exception& ex) { cout << ex.what() << endl; system("pause"); }
}
void Delete_Airline() {
	string key;
	while (true) {
		try {
			cout << "Введите номер авиарейса: "; getline(cin, key);
			AIRNUMBER_IS_COOL(key);
			break;
		}
		catch (UncorrectAirNumber& ex) { cout << ex.what() << " " << ex.getmore() << endl; }
	}
	auto flight = tree.find(key);
	if (!flight) { cout << "Авиарейс не найден!!!" << endl; system("pause"); return; }
	if (flight->airplane.free != flight->airplane.total) {
		cout << "На данный авиарейс зарегестрированы пассажиры!!!" << endl;
		cout << "   [ 1 ]=[ Оформить возврат всем пассажирам ]" << endl;
		cout << "   [ 2 ]=[ Отменить удаление авиарейса ]" << endl << endl;
		cout << "\tВведите номер пункта: ";
		getline(cin, choice);
		switch (atoi(choice.c_str())) {
		case 1: {
			auto all = lst.findAll(key);
			for (auto& passenger : all) {
				flight->airplane.free++;
				tree.free_up_space();
				lst.set_airplane(flight->airplane.number);
				lst.set_passport(passenger->passport_number);
				lst.set_ticket(lst.find(passenger->passport_number)->trade.ticket_number);
				lst.push_back(TYPE::REFUND);
				tbl.remove(passenger->passport_number);
				cout << "Возврат успешно оформлен!" << endl;
				cout << "Пассажир успешно удалён!" << endl;
				
			}
			break;
		}
		case 2: return;
		default:
			cout << "\tТакого пункта меню нет!!!" << endl;
			system("pause");
			return;
		}
	}
	int hash = 0;
	for (auto& el : key)
		hash += int(el);
	tree.remove(hash);
	//lst.sort();
	//cout << "Сортировка логов..." << endl;
	cout << "Элемент удалён!" << endl;
	system("pause");
}
void Clear_Tree() { 
	while (tree.size()) {
		auto flight = tree.GetHead();
		auto all = lst.findAll(flight->number);
		for (auto& passenger : all) {
			flight->free++;
			tree.free_up_space();
			lst.set_airplane(flight->number);
			lst.set_passport(passenger->passport_number);
			lst.set_ticket(lst.find(passenger->passport_number)->trade.ticket_number);
			lst.push_back(TYPE::REFUND);
			tbl.remove(passenger->passport_number);
		}
		tree.remove(flight->get_position());
	}
	cout << "Сортировка логов..." << endl;
	//lst.sort();
	//cout << "Список авиарейсов очищен!" << endl; 
	system("pause"); 
}
void Rand_Fill_Tree() {

	string size;
	cout << "Введите желаемый размер древа: "; getline(cin, size);
	if (atoi(size.c_str()) == 0) { cout << "Элементы не будут добавлены!" << endl; }
	else { Fill(tree, atoi(size.c_str())); cout << "Древо заполнено!" << endl; }
	system("pause");
}
void FlightMenu() {
	cout << " [ 5 ]=[ Действия со списком авиарейсов ] [...]" << endl;
	cout << "   `-=[ 1 ]=[ Добавить авиарейс ] [...]" << endl;
	cout << "   `-=[ 2 ]=[ Удалить aвиарейс ] [...]" << endl;
	cout << "   `-=[ 3 ]=[ Очистить список ]" << endl;
	cout << "   `-=[ 4 ]=[ Заполнить случайно ] [...]" << endl;
	cout << "   `-=[ 0 ]=[ Назад ]" << endl << endl;
	cout << "\tВведите номер пункта: ";
	getline(cin, choice);
	switch (atoi(choice.c_str()))
	{
	case 1: Add_new_Airline(); break;
	case 2: Delete_Airline(); break;
	case 3: Clear_Tree(); break;
	case 4: Rand_Fill_Tree(); break;
	case 0: return;
	default:
		cout << "\tТакого пункта меню нет!!!" << endl;
		system("pause");
		break;
	}
}
///////////////////////////////
void PassFind() {
	string number;
	while (true) {
		try {
			cout << "Введите серию и номер паспорта (без пробелов): ";
			getline(cin, number);
			if (KEY_IS_COOL(number)) break;
		}
		catch (UncorrectKey) {
			cout << "Повторите ввод!!!" << endl;
		}
	}
	auto person = tbl.find(number);
	if (!person) {
		cout << "Такой пассажир не найден!!!" << endl;
		system("pause");
		return;
	}
	cout << "ФИО пассажира: " << person->name << endl <<
		"Дата рождения: " << person->birth_date << endl <<
		"Серия и номер пасспрта: " << person->passport_number << endl <<
		"Место и дата выдачи: " << person->place_and_date << endl;
	system("pause");
}
void NameFind() {
	string name;
	while (true) {
		try {
			cout << "Введите ФИО: ";
			getline(cin, name);
			toUpperString(name);
			if (NAME_IS_COOL(name)) break;
		}
		catch (UncorrectName) {
			cout << "Повторите ввод!!!" << endl;
		}
	}
	auto persons = tbl.findAll(name);
	if (!persons.size()) {
		cout << "Такой пассажир не найден!!!" << endl;
		system("pause");
		return;
	}
	for (auto& person : persons) {
		cout << "ФИО: " << person->name <<  ", пасспорт: " << person->passport_number << endl;
	}
	system("pause");
}
void AirNumFind() {
	string number;
	while (true) {
		try {
			cout << "Введите номер авиабилета: ";
			getline(cin, number);
			toUpperString(number);
			if (AIRNUMBER_IS_COOL(number)) break;
		}
		catch (UncorrectAirNumber) {
			cout << "Повторите ввод!!!" << endl;
		}
	}
	auto flight = tree.find(number);
	if (!flight) {
		cout << "Такой самолет не найден!!!" << endl;
		system("pause");
		return;
	}
	tree.show_header();
	tree.show_node(flight);
	auto trade = lst.findAll(flight->airplane.number);
	cout << "Список пассажиров: " << endl;
	lst.show_header();
	for (auto& passenger : trade) {
		lst.show_node(passenger);
	}
	system("pause");
}
void FragmentFind() {
	string fragment;
	cout << "Введите фрагмент названия аэропорта: ";
	getline(cin, fragment);
	toUpperString(fragment);
	auto flights = tree.findAll_fragment(fragment);
	if (!flights.size()) {
		cout << "Таких самолетов не найдено!!!" << endl;
		system("pause"); return;
	}
	tree.show_header();
	for (auto& flight : flights) {
		tree.show_node(flight);
	}
	system("pause");
}
void FindMenu() {
	cout << " [ 6 ]=[ Поиск пассажира или авиарейса ] [...]" << endl;
	cout << "   `-=[ 1 ]=[ Поиск пассажира по номеру паспорта ] [...]" << endl;
	cout << "   `-=[ 2 ]=[ Поиск пассажиров по ФИО ] [...]" << endl;
	cout << "   `-=[ 3 ]=[ Поиск авиарейса по номеру авиарейса ] [...]" << endl;
	cout << "   `-=[ 4 ]=[ Поиск авиарейса по фрагменту аэропорта прибытия ] [...]" << endl;
	cout << "   `-=[ 0 ]=[ Назад ]" << endl << endl;
	cout << "\tВведите номер пункта: ";
	getline(cin, choice);
	switch (atoi(choice.c_str()))
	{
	case 1: PassFind(); break;
	case 2: NameFind(); break;
	case 3: AirNumFind(); break;
	case 4: FragmentFind(); break;
	case 0: return;
	default:
		cout << "\tТакого пункта меню нет!!!" << endl;
		system("pause");
		break;
	}
}
#pragma endregion

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "");
	srand(time(NULL));

	while (true) {
		system("cls");
		cout << endl;
		//cout << "Free: " << tree.GetAllFree() << endl;
		cout << " [ 1 ]=[ Открыть список пассажиров ]" << " [size = " << tbl.size() << "]" << endl;
		cout << " [ 2 ]=[ Открыть список авиарейсов ]" << " [size = " << tree.size() << "]" << endl;
		cout << " [ 3 ]=[ Открыть список регистрации ]" << " [size = " << lst.size() << "]" << endl;
		cout << " [ 4 ]=[ Действия со списком пассажиров ] [...]" << endl;
		cout << " [ 5 ]=[ Действия со списком авиарейсов ] [...]" << endl;
		cout << " [ 6 ]=[ Поиск пассажира или авиарейса ] [...]" << endl;
		cout << " [ 7 ]=[ Сортировать логи ]" << endl;
		cout << " [ 0 ]=[ Выход ]" << endl << endl;
		cout << "\tВведите номер пункта: ";
		getline(cin, choice);
		cout << endl;
		switch (atoi(choice.c_str()))
		{
		case 1: tbl.print(); break;
		case 2: tree.print(); break;
		case 3: lst.print(); break;
		case 4: PassengersMenu(); break;
		case 5: FlightMenu(); break;
		case 6: FindMenu(); break;
		case 7: cout << "Сортировка логов..." << endl; lst.sort(); break;
		case 0: return 0;
		default:
			cout << "\tТакого пункта меню нет!!!" << endl;
			system("pause");
			break;
		}
	}
}

