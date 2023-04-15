#pragma once
#include <fstream>

#include "Checks.h"
#include "Flight.h"
#include "HelperFunctions.h"

class FlightsTree {
private:
	struct node {
		Flight airplane;
		node* pLeft, * pRight;
		int index;
		int height;
		node() { pLeft = pRight = nullptr; height = 1; }
		node(Flight airplane, int index, node* pLeft = nullptr, node* pRight = nullptr) {
			this->airplane = airplane;
			this->index = index;
			this->pLeft = pLeft;
			this->pRight = pRight;
			height = 1;
		}
	};
	node* HEAD;
	int SIZE;
	int AllFreeSeats;
	node* pRand;
	Flight CurrentAirplane;
	int height(node* current) { return current ? current->height : 0; }
	int bfactor(node* current) { return height(current->pRight) - height(current->pLeft); }
	void fixheight(node* current)
	{
		unsigned char hLeft = height(current->pLeft);
		unsigned char hRight = height(current->pRight);
		current->height = (hLeft > hRight ? hLeft : hRight) + 1;
	}
	node* rotateright(node* current) {
		node* temp = current->pLeft;
		current->pLeft = temp->pRight;
		temp->pRight = current;
		fixheight(current);
		fixheight(temp);
		return temp;
	}
	node* rotateleft(node* current) {
		node* temp = current->pRight;
		current->pRight = temp->pLeft;
		temp->pLeft = current;
		fixheight(current);
		fixheight(temp);
		return temp;
	}
	node* balance(node* current) // áàëàíñèðîâêà óçëà p
	{
		fixheight(current);
		if (bfactor(current) == 2)
		{
			if (bfactor(current->pRight) < 0)
				current->pRight = rotateright(current->pRight);
			return rotateleft(current);
		}
		if (bfactor(current) == -2)
		{
			if (bfactor(current->pLeft) > 0)
				current->pLeft = rotateleft(current->pLeft);
			return rotateright(current);
		}
		return current; // áàëàíñèðîâêà íå íóæíà
	}
	node* findmin(node* current) // ïîèñê óçëà ñ ìèíèìàëüíûì êëþ÷îì â äåðåâå p 
	{ return current->pLeft ? findmin(current->pLeft) : current; }
	node* removemin(node* current) // óäàëåíèå óçëà ñ ìèíèìàëüíûì êëþ÷îì èç äåðåâà p
	{
		if (current->pLeft == nullptr) return current->pRight;
		current->pLeft = removemin(current->pLeft);
		return balance(current);
	}
	bool is_removed = false;
	node* remove(node* current, int key) // óäàëåíèå êëþ÷à k èç äåðåâà p
	{
		if (current == nullptr) return 0;
		if (key < current->airplane.get_position()) current->pLeft = remove(current->pLeft, key);
		else if (key > current->airplane.get_position()) current->pRight = remove(current->pRight, key);
		else //  k == p->key 
		{
			node* left_copy = current->pLeft; 
			node*  right_copy = current->pRight; 
			AllFreeSeats -= current->airplane.free;
			delete current;
			if (right_copy == nullptr) {
				is_removed = true;
				return left_copy;
			}
			node* min = findmin(right_copy);
			min->pRight = removemin(right_copy);
			min->pLeft = left_copy;
			is_removed = true;
			return balance(min);
			
		}
		return balance(current);
	}
	node* insert(node* current) {
		if (!current) { ++SIZE; AllFreeSeats += CurrentAirplane.free; return new node(CurrentAirplane, SIZE); }
		if (current->airplane.number == CurrentAirplane.number)
			throw TheSameFlight(CurrentAirplane.number);
		if (CurrentAirplane.get_position() < current->airplane.get_position()) { current->pLeft = insert(current->pLeft); }
		else { current->pRight = insert(current->pRight); }
		return balance(current);
	}
	void show_header(std::ofstream* fout) {
		*fout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl
			<< "| ÍÎÌÅÐ ÐÅÉÑÀ | ÄÀÒÀ ÂÛËÅÒÀ | ÂÐÅÌß ÂÛËÅÒÀ |     ÍÀÇÂÀÍÈÅ ÀÂÈÀÊÎÌÏÀÍÈÈ     |   ÀÝÐÎÏÎÐÒ ÎÒÁÛÒÈß   |   ÀÝÐÎÏÎÐÒ ÏÐÈÁÛÒÈß   |  ÌÅÑÒÀ  |" << std::endl;
		*fout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
	}
	void show_node(node* current, std::ofstream* fout) {
			*fout << "|   " << current->airplane.number << "    "
			<< "| " << current->airplane.dep_date << "  "
			<< "|    " << current->airplane.dep_time << "     ";

		int lenght = current->airplane.name.size();
		*fout << "|"; for (int i = 0; i < abs(31 - lenght) / 2; ++i) { *fout << " "; }
		*fout << current->airplane.name;
		for (int i = 0; i < (abs(31 - lenght) % 2 == 0 ? abs(31 - lenght) / 2 : abs(31 - lenght) / 2 + 1); ++i) { *fout << " "; }

		lenght = current->airplane.departure.size();
		*fout << "|"; for (int i = 0; i < abs(22 - lenght) / 2; ++i) { *fout << " "; }
		*fout << current->airplane.departure;
		for (int i = 0; i < (abs(22 - lenght) % 2 == 0 ? abs(22 - lenght) / 2 : abs(22 - lenght) / 2 + 1); ++i) { *fout << " "; }

		lenght = current->airplane.arrival.size();
		*fout << "|"; for (int i = 0; i < abs(23 - lenght) / 2; ++i) { *fout << " "; }
		*fout << current->airplane.arrival;
		for (int i = 0; i < (abs(23 - lenght) % 2 == 0 ? abs(23 - lenght) / 2 : abs(23 - lenght) / 2 + 1); ++i) { *fout << " "; }

		*fout << "|"; (current->airplane.free < 10) ? (*fout << "   ") : ((current->airplane.free < 100) ? (*fout << "  ") : (*fout << " "));
		*fout << current->airplane.free << "/" << current->airplane.total;
		(current->airplane.total < 100) ? (*fout << "  |") : (*fout << " |");
		*fout << std::endl;
		*fout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl;

	}
	void print(node* current, std::ofstream* fout) {
		if (!HEAD) { *fout << "Ñïèñîê ïóñò!" << std::endl; }
		else {
			if (current->pLeft) { print(current->pLeft, fout); }
			if (current->pRight) { print(current->pRight, fout); }
			show_node(current, fout);
		}
	}
	void find_index(int index, node* current) {
		if (current->index == index) { pRand = current; return; }
		if (current->pLeft) { find_index(index, current->pLeft); }
		if (current->pRight) { find_index(index, current->pRight); }
	}
	node* find(std::string number, node* current) {
		int hash = 0;
		for (auto& el : number)
			hash += int(el);
		if (!current) return nullptr;
		if (current->airplane.number == number) return current;
		if (current->airplane.get_position() > hash) return find(number, current->pLeft);
		else return find(number, current->pRight);
	}
	void findAll_fragment(node* current, std::vector<node*>& All, std::string fragment) {
		if (!current) return;
		else {
			if (current->pLeft) { findAll_fragment(current->pLeft, All, fragment); }
			if (current->pRight) { findAll_fragment(current->pRight, All, fragment); }
			if (FindFragment(current->airplane.arrival.c_str(), fragment))
				All.push_back(current);
		}
	}
public:
	void show_header() {
		std::cout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl
			<< "| ÍÎÌÅÐ ÐÅÉÑÀ | ÄÀÒÀ ÂÛËÅÒÀ | ÂÐÅÌß ÂÛËÅÒÀ |     ÍÀÇÂÀÍÈÅ ÀÂÈÀÊÎÌÏÀÍÈÈ     |   ÀÝÐÎÏÎÐÒ ÎÒÁÛÒÈß   |   ÀÝÐÎÏÎÐÒ ÏÐÈÁÛÒÈß   |  ÌÅÑÒÀ  |" << std::endl;
		std::cout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
	}
	void show_node(node* current) {
		std::cout << "|   " << current->airplane.number << "    "
			<< "| " << current->airplane.dep_date << "  "
			<< "|    " << current->airplane.dep_time << "     ";

		int lenght = current->airplane.name.size();
		std::cout << "|"; for (int i = 0; i < abs(31 - lenght) / 2; ++i) { std::cout << " "; }
		std::cout << current->airplane.name;
		for (int i = 0; i < (abs(31 - lenght) % 2 == 0 ? abs(31 - lenght) / 2 : abs(31 - lenght) / 2 + 1); ++i) { std::cout << " "; }

		lenght = current->airplane.departure.size();
		std::cout << "|"; for (int i = 0; i < abs(22 - lenght) / 2; ++i) { std::cout << " "; }
		std::cout << current->airplane.departure;
		for (int i = 0; i < (abs(22 - lenght) % 2 == 0 ? abs(22 - lenght) / 2 : abs(22 - lenght) / 2 + 1); ++i) { std::cout << " "; }

		lenght = current->airplane.arrival.size();
		std::cout << "|"; for (int i = 0; i < abs(23 - lenght) / 2; ++i) { std::cout << " "; }
		std::cout << current->airplane.arrival;
		for (int i = 0; i < (abs(23 - lenght) % 2 == 0 ? abs(23 - lenght) / 2 : abs(23 - lenght) / 2 + 1); ++i) { std::cout << " "; }

		std::cout << "|"; (current->airplane.free < 10) ? (std::cout << "   ") : ((current->airplane.free < 100) ? (std::cout << "  ") : (std::cout << " "));
		std::cout << current->airplane.free << "/" << current->airplane.total;
		(current->airplane.total < 100) ? (std::cout << "  |") : (std::cout << " |");
		std::cout << std::endl;
		std::cout << " ----------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
	}
	FlightsTree() { HEAD = nullptr; SIZE = 0; AllFreeSeats = 0; }
	~FlightsTree() { clear(); }
	void insert() { HEAD = insert(HEAD); }
	node* get_rand() {
		while (true) {
			pRand = nullptr;
			if (!AllFreeSeats) return nullptr;
			int index = 1 + rand() % SIZE;
			find_index(index, HEAD);
			if (!pRand->airplane.free)
				continue;
			return pRand;
		}
	}
	Flight* GetHead() { return &HEAD->airplane; }
	bool remove(int key) {
		HEAD = remove(HEAD, key);
		if (is_removed)
		{
			--SIZE;
		}
		return is_removed;
	}
	void clear() { 
		while (SIZE) { 
			std::string HEAD_KEY = HEAD->airplane.number;
			remove(atoi(HEAD_KEY.erase(0, 3).c_str()));
		} 
	}
	node* find(std::string number) {
		return find(number, HEAD);
	}
	std::vector<node*> findAll_fragment(std::string fragment) {
		std::vector<node*> All;
		findAll_fragment(HEAD, All, fragment);
		return All;
	}
	void print() {
		std::ofstream* fout = new std::ofstream();
		fout->open("airplanes_tree.txt");
		show_header(fout);
		print(HEAD, fout);
		fout->close();
		system("start airplanes_tree.txt");
	}
	int size() { return SIZE; }
	void take_a_seat() { AllFreeSeats--; }
	void free_up_space() { AllFreeSeats++; }



	int GetAllFree() { return AllFreeSeats; }
#pragma region Setters
	bool set_number(std::string number) {
		toUpperString(number);
		if (AIRNUMBER_IS_COOL(number)) { CurrentAirplane.number = number; return true; }
		else { return false; }
	}
	bool set_name(std::string name) {
		toUpperString(name);
		if (AIRNAME_IS_COOL(name)) { CurrentAirplane.name = name; return true; }
		else { return false; }
	}
	bool set_departure(std::string departure) {
		toUpperString(departure);
		if (DEPARTURE_IS_COOL(departure)) { CurrentAirplane.departure = departure; return true; }
		else { return false; }
	}
	bool set_arrival(std::string arrival) {
		toUpperString(arrival);
		if (ARRIVAL_IS_COOL(arrival)) { CurrentAirplane.arrival = arrival; return true; }
		else { return false; }
	}
	bool set_dep_date(std::string dep_date) {
		if (DATE_IS_COOL(dep_date) && DATE_IS_REAL(dep_date, true)) { CurrentAirplane.dep_date = dep_date; return true; }
		else { return false; }
	}
	bool set_dep_time(std::string dep_time) {
		if (TIME_IS_COOL(dep_time)) { CurrentAirplane.dep_time = dep_time; return true; }
		else { return false; }
	}
	bool set_total(int total) {
		if (TOTAL_IS_COOL(total)) { CurrentAirplane.total = total; return true; }
		else { return false; }
	}
	bool set_free(int free) {
		if (FREE_IS_COOL(free, CurrentAirplane.total)) { CurrentAirplane.free = free; return true; }
		else { return false; }
	}
#pragma endregion
};