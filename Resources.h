#pragma once
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>

#include "HelperFunctions.h"

using std::ifstream;
using std::string;
using std::set;
using std::map;
using std::multimap;
using std::vector;




	class Set : public set<string> {
	public:
		Set(const char* path) {
			setlocale(LC_ALL, "");
			ifstream fin(path);
			if (!fin.is_open()) { throw std::fstream::failure("Open File Error!!!"); }
			else {
				for (string line; std::getline(fin, line); ) {
					toUpperString(line);
					insert(line);
				}
			}
			//fin.close();
		}
		string rand() {
			auto rand_el = this->begin();
			for (int i = 0; i < std::rand() % this->size(); ++i) { ++rand_el; }
			return *rand_el;
		}
	};
	class Map : public map<string, string> {
	public:
		Map(const char* path) {
			setlocale(LC_ALL, "");
			ifstream fin;
			fin.open(path);
			if (!fin.is_open()) { throw std::fstream::failure("Open File Error!!!"); }
			else {
				string key, value;
				ifstream fin; fin.open(path);
				string buff; char s;
				while (fin.get(s)) {
					s = toupper(s);
					if (s == '\t') { key = buff; buff.clear(); }
					else if (s == '\n') { value = buff; buff.clear(); }
					else { buff += s; }
					if (!(key.empty() || value.empty())) {
						toUpperString(key);
						toUpperString(value);
						this->emplace(key, value);
						key.clear(); value.clear();
					}
				}
			}
			fin.close();
		}
	};
	class MultiMap : public multimap<string, string> {
	private:
		// размах (размер) отрезка между итераторами 
		int scale(std::pair<std::map<std::string, std::string>::iterator, std::map<std::string, std::string>::iterator> cut) {
			int size = 0;
			for (auto i = cut.first; i != cut.second; ++i) { ++size; }
			return (size == 0) ? (1) : (size);
		}
	public:
		MultiMap(const char* path) {
			setlocale(LC_ALL, "");
			ifstream fin;
			fin.open(path);
			if (!fin.is_open()) { throw std::fstream::failure("Open File Error!!!"); }
			else {
				string key, value;
				ifstream fin; fin.open(path);
				string buff; char s;
				while (fin.get(s)) {
					s = std::toupper(s);
					if (s == '|') { value = buff; buff.clear(); }
					else if (s == '\n') { key = buff; buff.clear(); }
					else { buff += s; }
					if (!(key.empty() || value.empty())) {
						toUpperString(key);
						toUpperString(value);
						this->emplace(key, value);
						key.clear(); value.clear();
					}
				}
			}
			fin.close();
		}
		std::pair<string, bool> rand(string key) {
			if (this->find(key) == this->end()) { return std::pair<string, bool>("none", false); }
			auto cut = this->equal_range(key);
			int rand_size = std::rand() % scale(cut);
			auto input = cut.first;
			for (int i = 0; i < rand_size; ++i)
			{
				++input;
			}
			return std::pair<string, bool>((*input).second, true);
		}
	};
	class Vector : public vector<string> {
	public:
		Vector(const char* path) {
			setlocale(LC_ALL, "");
			ifstream fin;
			fin.open(path);
			if (!fin.is_open()) { throw std::fstream::failure("Open File Error!!!"); }
			else {
				for (string line; std::getline(fin, line); ) {
					toUpperString(line);
					push_back(line);
				}
			}
			fin.close();
		}
		string rand() { return operator[](std::rand() % this->size()); }
	};

	namespace MyResources {
		Set Airports("Resources/airports.txt");
		Set Airlines("Resources/airlines.txt");
		Map Code_Region("Resources/code.txt");
		MultiMap Region_City("Resources/cities.txt");
		Vector ManNames("Resources/man_names.txt");
		Vector ManSurnames("Resources/man_surnames.txt");
		Vector ManPatron("Resources/man_patronymics.txt");
		Vector WomanNames("Resources/woman_names.txt");
		Vector WomanSurnames("Resources/woman_surnames.txt");
		Vector WomanPatron("Resources/woman_patronymics.txt");
	}

