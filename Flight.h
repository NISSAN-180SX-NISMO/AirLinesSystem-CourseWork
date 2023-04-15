#pragma once
#include <string>

using std::string;

struct Flight {
	string number;
	string name;
	string departure;
	string arrival;
	string dep_date;
	string dep_time;
	int total;
	int free;
	Flight() { number = name = departure = arrival = dep_date = dep_time = ""; total = free = -1; }
	bool is_cool() {
		if (number != "" && name != "" && departure != "" && arrival != "" && dep_date != "" && dep_time != "" && total != -1 && free != -1) { return true; }
		else { return false; }
	}
	int get_position() {
		int hash = 0;
		for (auto& el : number)
			hash += int(el);
		return hash;
	}
};