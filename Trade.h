#pragma once
#include <string>

using std::string;

struct Trade {
	string passport_number;	// ����� � ����� ��������� � ������� "NNNN-NNNNNN"
	string air_number;		// ����� ��������� � ������� "AAANNN"
	string ticket_number;	// ���� ����������� � ������� "NNNNNNNNN"
	void clear() { passport_number = air_number = ticket_number = ""; }
	int get_position() {
		string number_only = air_number;
		number_only.erase(0, 3);
		return atoi(number_only.c_str());
	}
};