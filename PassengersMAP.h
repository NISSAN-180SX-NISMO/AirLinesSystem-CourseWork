#pragma once
#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "Checks.h"
#include "Passenger.h"
#include "HelperFunctions.h"

const int CAPACITY = 15000;		// Êîëè÷åñòâî ÿ÷ååê â òàáëèöå

int getIndex(std::string key) {
	if (KEY_IS_COOL(key)) {
		long long M = 0;
		for (int i = 0; i < key.size(); ++i)
		{
			M += ((48 - int(key[i])) * pow(9, i));
		}
		return abs(M) % CAPACITY;
	}
}

class PassengersMAP {
private:
	struct node {
		Passenger person;
		node* pNext;
		node(Passenger person, node* pNext = nullptr) {
			this->person = person;
			this->pNext = pNext;
		}
	};
	Passenger CurrentPerson;
	std::vector<node*> TABLE;
	int SIZE, FILL;
	void show_header(std::ofstream* fout) {
		*fout << " "; for (int i = 0; i < 137; ++i) { *fout << "="; } *fout << " " << std::endl;
		*fout << "|  ¹ ß×ÅÉÊÈ  | ÑÅÐÈß È ÍÎÌÅÐ ÏÀÑÏÎÐÒÀ |         ÔÀÌÈËÈß ÈÌß ÎÒ×ÑÒÂÎ         | ÄÀÒÀ ÐÎÆÄÅÍÈß |                                             |" << std::endl;
		*fout << " "; for (int i = 0; i < 137; ++i) { *fout << "="; } *fout << " " << std::endl;
	}
	void show_empty(int index, std::ofstream* fout) {
		////////////////////////////////////////////
	}
	void show_node(node* cell, int index, std::ofstream* fout, node* current = nullptr, int coll = 0) {
		// 1st row
		if (current) { cell = current; }
		if (coll == 1) {
			for (int i = 0; i < coll; ++i) { *fout << " `--"; }
		}
		else if (coll > 1) {
			for (int i = 0; i < coll - 1; ++i) { *fout << "    "; }
			*fout << " `--";
		}
		*fout << "|            |";

		*fout << "      ";
		for (int i = 0; i < cell->person.passport_number.size(); ++i)
		{
			if (i == 4) { *fout << " "; } *fout << cell->person.passport_number[i];
		}
		*fout << "       ";

		int lenght = cell->person.name.size();
		*fout << "|"; for (int i = 0; i < abs(37 - lenght) / 2; ++i) { *fout << " "; }
		*fout << cell->person.name;
		for (int i = 0; i < (abs(37 - lenght) % 2 == 0 ? abs(37 - lenght) / 2 : abs(37 - lenght) / 2 + 1); ++i)
		{
			*fout << " ";
		}

		*fout << "|  " << cell->person.birth_date << "   |";

		if (current && coll > 0) {
			*fout << " * collision " << coll; coll < 10 ? *fout << "  " : coll < 100 ? *fout << " " : *fout << "";
			string end; for (int i = 0; i < 30; ++i) { end += " "; } end.erase(0, coll * 4 + 1);
			*fout << end << "|" << std::endl;

		}
		else { *fout << "                                             |" << std::endl; }

		// 2nd row
		for (int i = 0; i < coll; ++i) { *fout << "    "; }
		index < 10 ? lenght = 1 : index < 100 ? lenght = 2 : index < 1000 ? lenght = 3 : index < 10000 ? lenght = 4 : lenght = 5;
		if (current && coll > 0) { ++lenght; }
		*fout << "|"; for (int i = 0; i < abs(12 - lenght) / 2; ++i) { *fout << " "; }
		*fout << index; if (current && coll > 0) { *fout << "*"; }
		for (int i = 0; i < (abs(12 - lenght) % 2 == 0 ? abs(12 - lenght) / 2 : abs(12 - lenght) / 2 + 1); ++i)
		{
			*fout << " ";
		} *fout << "|";
		string line;
		for (int i = 0; i < 62; i++) { line += " -"; } line.erase(0, coll * 4 + 1); *fout << line + " |" << std::endl;



		// 3rd row
		string tab;
		for (int i = 0; i < coll; ++i) { tab += "    "; }
		*fout << tab;
		*fout << "|            |";
		*fout << " ÂÛÄÀÍ " << cell->person.place_and_date;
		string end; for (int i = 0; i < 118 - cell->person.place_and_date.size(); ++i) { end += " "; } end.erase(0, coll * 4 + 1);
		*fout << end << "|" << std::endl;
		line.clear();
		*fout << " "; for (int i = 0; i < 137; i++) { line += "="; } line += " ";
		if (current) { if (current->pNext) { line.erase(0, coll * 4 + 1); line = tab + line; } }
		*fout << line << std::endl;

	}
	bool remove_coll(node*& current, std::string number) {
		if (!current) return false;
		if (number == current->person.passport_number) {
			node* tmp = current;
			current = current->pNext;
			delete tmp;
			--SIZE;
			return true;
		} else return remove_coll(current->pNext, number);
	}
public:

	PassengersMAP() {
		SIZE = FILL = 0;
		for (int i = 0; i < CAPACITY; ++i)
			TABLE.push_back(nullptr);
	}
	node* operator[](const int index) { return TABLE[index]; }
	void insert() {
		if (!CurrentPerson.is_cool()) { throw IncompleteData("Çàïîëíåíû íå âñå ïîëÿ Passenger!!!"); }
		int index = getIndex(CurrentPerson.passport_number);
		if (!TABLE[index]) {
			TABLE[index] = new node(CurrentPerson);
			++SIZE; ++FILL;
		}
		else {
			node* current = TABLE[index];
			while (current->pNext) {
				if (current->person.passport_number == CurrentPerson.passport_number)
					throw TheSamePassenger(CurrentPerson.passport_number);
				current = current->pNext;
			}
			current->pNext = new node(CurrentPerson); ++SIZE;
		}
	}
	bool remove(std::string number) {
		int index = getIndex(number);
		node*& current = TABLE[index];

		if (!TABLE[index]) return false;
		if (TABLE[index]->person.passport_number == number) {
			node *tmp = TABLE[index];
			TABLE[index] = tmp->pNext;
			delete tmp;
			--SIZE; 
			if(!TABLE[index])
				--FILL;
			return true;
		}
		return remove_coll(current, number);
	}
	void print() {
		std::ofstream fout;
		fout.open("PassengersTable.txt");
		show_header(&fout);
		if (!SIZE) { fout << "EMPTY!!!" << std::endl; }
		else {
			for (int index = 0; index < CAPACITY; ++index) {
				if (TABLE[index]) {
					if (!TABLE[index]->pNext)
						show_node(TABLE[index], index, &fout);
					else {
						node* current = TABLE[index];
						int i = 0;
						while (current != nullptr) {
							show_node(TABLE[index], index, &fout, current, i);
							current = current->pNext; ++i;
						}
					}
				}
			}
		}
		fout.close();
		system("start PassengersTable.txt");
	}
	Passenger* find(std::string number) {
		int index = getIndex(number);
		if (!TABLE[index]) return nullptr;
		node* current = TABLE[index];
		while (current) {
			if (current->person.passport_number == number)
				return &current->person;
			current = current->pNext;
		}
		return nullptr;
	}
	std::vector<Passenger*> findAll(std::string name) {
		std::vector<Passenger*> all;
		for (int i = 0; i < CAPACITY; ++i) {
			node* current = TABLE[i];
			while (current) {
				if (name == current->person.name)
					all.push_back(&current->person);
				current = current->pNext;
			}
		}
		return all;
	}
	int size() { return SIZE; }
	int fill() { return FILL; }
#pragma region Setters
	bool set_number(std::string number, bool check = true) {
		if (!check) { CurrentPerson.passport_number = number; return true; }
		if (KEY_IS_COOL(number)) { CurrentPerson.passport_number = number; return true; }
		else { return false; }
	}
	bool set_place(std::string place, std::string date, bool check = true) {
		if (!check) { CurrentPerson.place_and_date = place + " " + date; return true; }
		toUpperString(place);
		if (PLACE_IS_COOL(place + " " + date) && DATE_IS_COOL(date) && DATE_IS_REAL(date)) { CurrentPerson.place_and_date = place + " " + date; return true; }
		else { return false; }
	}
	bool set_name(std::string name, bool check = true) {
		if (!check) { CurrentPerson.name = name; return true; }
		toUpperString(name);
		if (NAME_IS_COOL(name)) {
			CurrentPerson.name = name;
			return true;
		}
		else { return false; }

	}
	bool set_date(std::string date) {
		if (DATE_IS_COOL(date) && DATE_IS_REAL(date)) { CurrentPerson.birth_date = date; return true; }
		else { return false; }
	}
#pragma endregion
};