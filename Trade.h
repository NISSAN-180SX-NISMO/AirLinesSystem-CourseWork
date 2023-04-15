#pragma once
#include <string>

using std::string;

struct Trade {
	string passport_number;	// Серия и номер пасспорта в формате "NNNN-NNNNNN"
	string air_number;		// Номер авиарейса в формате "AAANNN"
	string ticket_number;	// Номе равиабилета в формате "NNNNNNNNN"
	void clear() { passport_number = air_number = ticket_number = ""; }
	int get_position() {
		string number_only = air_number;
		number_only.erase(0, 3);
		return atoi(number_only.c_str());
	}
};