#pragma once
#include <string>

using std::string;
// ���� ����� - ������ ������. ����� ������ �������� ��� ������ � ���������.
struct Passenger {
	string passport_number;	// ����� � ����� ��������� � ������� "NNNN-NNNNNN"
	string place_and_date;	// ����� � ���� ������ ��������
	string name;			// ���
	string birth_date;		// ���� ��������
	//Passenger(int zero) {}
	Passenger() { passport_number = place_and_date = name = birth_date = ""; }
	bool is_cool() {
		if (passport_number != "" && place_and_date != "" && name != "" && birth_date != "") return true;
		else { return false; }
	}
	void clear() { passport_number.clear(); place_and_date.clear(); name.clear(); birth_date.clear(); }
};