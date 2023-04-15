#pragma once
#include <string>
#include <vector>
#include <set>
#include <regex>

#include "InputExceptions.h"
#include "HelperFunctions.h"
#include "Resources.h"

bool KEY_IS_COOL(std::string key) {
	if (key.size() != 10) { throw UncorrectKey(key); }
	if (regex_match(key.c_str(), std::regex("[0-9]{10}"))) { return true; }
	else { throw UncorrectKey(key); }
}

bool NAME_IS_COOL(std::string name) {
	if (regex_match(name.c_str(), std::regex("^[‡-ˇ¿-ﬂ∏®a-zA-Z]{1,}'?-?[‡-ˇ¿-ﬂ∏®a-zA-Z]{2,} ?[‡-ˇ¿-ﬂ∏®a-zA-Z]{2,} ?([‡-ˇ¿-ﬂ∏®a-zA-Z]{1,})?"))) { return true; }
	else { throw UncorrectName(name); }
}

bool DATE_IS_COOL(std::string date) {
	if (regex_match(date.c_str(), std::regex("([0-9]{1,2}(\/|.)[0-9]{1,2}(\/|.)[0-9]{2,4})"))) { return true; }
	else { throw UncorrectDate(date); }
}

bool PLACE_IS_COOL(std::string place) {
	if (regex_match(place.c_str(), std::regex("((”?Ã¬ƒ)|(”‘Ã— –Œ——»»)|(Ã»À»÷»»)|((Œ“)?ƒ≈À))[a-zA-Z¿-ﬂ∏® -]+ ?((Œ¡À.?(¿—“»)?)|(Œ –”√”?)|( –¿(ﬂ|ﬁ)|(–((¿…Œ)|-)Õ(¿|≈)))|(–≈—œ”¡À» ≈|¿)){0,2} ?¬? ?((√.?(Œ–.?)?(Œƒ)?(≈)?)? [a-zA-Z¿-ﬂ∏® -]+)? ?([0-9]{1,2}(\/|.)[0-9]{1,2}(\/|.)[0-9]{2,4})"))) { return true; }
	else { throw UncorrectPlace(place); }
}

bool DATE_IS_REAL(std::string str_date, bool cut_future = false) {
	constexpr auto DAY = 0;
	constexpr auto MONTH = 1;
	constexpr auto YEAR = 2;

	std::vector<int> date;
	std::string buff = "";
	for (auto i = str_date.begin(); i <= str_date.end(); ++i) {
		if (i == str_date.end()) { date.push_back(atoi(buff.c_str())); break; }
		else if (*i == '.' || *i == '/' || i == str_date.end()) { date.push_back(atoi(buff.c_str())); buff = ""; }
		else { buff += *i; }
	}
	// —ÌÓ‚‡ ÔÓ‚ÂÍÛ ‚ÁˇÎ Ú‚Ó˛, ÚÓ˜ÌÂÂ ËÁ ÍÛÒ‡˜‡ Œœ
	if (date[DAY] < 1 || date[DAY] > 31) { throw ImpossibleDate(str_date, 0); }
	if (date[MONTH] < 1 || date[MONTH] > 12) { throw ImpossibleDate(str_date, 1); }
	if ((date[MONTH] == 4 || date[MONTH] == 6 || date[MONTH] == 9 || date[MONTH] == 11) && date[DAY] > 30)
	{ throw ImpossibleDate(str_date, 2); }
	if (date[MONTH] == 2 && date[DAY] > 29) { throw ImpossibleDate(str_date, 3); }
	if (date[MONTH] == 2 && date[DAY] == 29) {
		if (date[YEAR] % 100 == 0 && date[YEAR] % 400 != 0) { throw ImpossibleDate(str_date, 4); }
		else if (date[YEAR] % 100 != 0 && date[YEAR] % 4 != 0) { throw ImpossibleDate(str_date, 4); }
	}
	if (date[YEAR] < 1900) { throw ImpossibleDate(str_date, 5); }
	if (!cut_future) {
		if (date[YEAR] == 2023) {
			if (date[MONTH] == 5) {
				if (date[0] > 13) { throw ImpossibleDate(str_date, 6); }
			}
			else if (date[MONTH] > 5) { throw ImpossibleDate(str_date, 6); }
		}
		else if (date[MONTH] > 2023) { throw ImpossibleDate(str_date, 6); }
	}
	// ‚ÒÂ ÍÓ‰˚ Ó¯Ë·ÓÍ ÔÓÔËÒ‡Ì˚ ‚ Ù‡ÈÎÂ InputExceptions
	return true;
}

bool AIRNUMBER_IS_COOL(std::string number) {
	if (number.size() != 6) { throw UncorrectAirNumber(number); }
	if (regex_match(number.c_str(), std::regex("[A-Z¿-ﬂ®]{3}[0-9]{3}"))) { return true; }
	else { throw UncorrectAirNumber(number); }
}

bool AIRNAME_IS_COOL(std::string name) {
	return true;
}

bool DEPARTURE_IS_COOL(std::string departure) {
	if (MyResources::Airports.find(departure) == MyResources::Airports.end()) {
		throw UncorrectAirport(departure); 
	}
	return true;
}

bool ARRIVAL_IS_COOL(std::string arrival) {
	if (MyResources::Airports.find(arrival) == MyResources::Airports.end()) {
		throw UncorrectAirport(arrival); 
	}
	return true;
}

bool TIME_IS_COOL(std::string time) {
	if (std::regex_match(time.c_str(), std::regex("(((0|1)[0-9])|(2[0-3])):[0-5][0-9]"))) { return true; }
	throw UncorrectTime(time);
}

bool TOTAL_IS_COOL(int total) {
	if (total >= 80 && total <= 853) { return true; }
	throw UncorrectTotal(total);
}

bool FREE_IS_COOL(int free, int total) {
	if (free >= 0 && free <= total) { return true; }
	throw UncorrectFree(free);
}

bool TICKET_NUMBER_IS_COOL(std::string number) {
	if (number.size() != 9) { throw UncorrectTicket(number); }
	if (regex_match(number.c_str(), std::regex("[0-9]{9}"))) { return true; }
	else { throw UncorrectTicket(number); }
}

