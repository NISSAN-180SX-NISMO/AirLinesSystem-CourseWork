#pragma once
#include <fstream>
#include <vector>
#include "Trade.h"
#include "Checks.h"

enum class TYPE {SALE, REFUND};

class TradesList {
private:
	struct node {
		Trade trade;
		TYPE deal;
		node* pNext,* pPrev;
		node(Trade trade, TYPE deal, node* pPrev, node* pNext = nullptr) {
			this->trade = trade;
			this->deal = deal;
			this->pPrev = pPrev;
			this->pNext = pNext;
		}
	};
	Trade CurrentTrade;
	node* HEAD,* TAIL;
	int SIZE;
	void show_header(std::ofstream& fout) {
		fout << " ------------------------------------------------------------------------" << std::endl
			 << "| ÒÈÏ ÎÏÅÐÀÖÈÈ | ÑÅÐÈß È ÍÎÌÅÐ ÏÀÑÏÎÐÒÀ | ÍÎÌÅÐ ÐÅÉÑÀ | ÍÎÌÅÐ ÀÂÈÀÁÈÁÅÒÀ |" << std::endl
			 << " ------------------------------------------------------------------------" << std::endl;
	}
	void show_node(std::ofstream &fout, node*current) {
		fout << "|   "; (current->deal == TYPE::SALE) ? (fout << "ÏÐÎÄÀÆÀ") : (fout << "ÂÎÇÂÐÀÒ"); fout << "    ";
		fout << "|      "; 
		for (int i = 0; i < current->trade.passport_number.size(); ++i) {
			if (i == 4) { fout << " "; }
			fout << current->trade.passport_number[i];
		} fout << "       ";
		fout << "|   " << current->trade.air_number << "    ";
		fout << "|    " << current->trade.ticket_number << "     |" << std::endl;
		fout << " ------------------------------------------------------------------------" << std::endl;
	}

	void swap(node* L, node* R) {
		if (L == R || !L->pNext || !R->pPrev) return;
		if (L->pNext == R) {
			if (L->pPrev == nullptr && R->pNext == nullptr) {
				L->pPrev = R;
				L->pNext = nullptr;
				R->pPrev = nullptr;
				R->pNext = L;
				HEAD = R; TAIL = L;
			}
			else if (L->pPrev == nullptr) {
				L->pPrev = R;
				L->pNext = R->pNext;
				R->pNext->pPrev = L;
				R->pPrev = nullptr;
				R->pNext = L;
				HEAD = R;
			}
			else if (R->pNext == nullptr) {
				R->pPrev = L->pPrev;
				L->pPrev->pNext = R;
				R->pNext = L;
				L->pPrev = R;
				L->pNext = nullptr;
				TAIL = L;
			}
			else {
				R->pNext->pPrev = L;
				L->pPrev->pNext = R;
				R->pPrev = L->pPrev;
				L->pNext = R->pNext;
				R->pNext = L;
				L->pPrev = R;
			}
		}
		else {
			if (L->pPrev == nullptr && R->pNext == nullptr) {
				L->pNext->pPrev = R;
				R->pPrev->pNext = L;
				L->pPrev = R->pPrev;
				R->pNext = L->pNext;
				L->pNext = nullptr;
				R->pPrev = nullptr;
				HEAD = R; TAIL = L;
			}
			else if (L->pPrev == nullptr) {
				node t = *L;
				L->pNext->pPrev = R;
				R->pPrev->pNext = L;
				R->pNext->pPrev = L;
				L->pPrev = R->pPrev;
				L->pNext = R->pNext;
				R->pNext = t.pNext;
				R->pPrev = nullptr;
				HEAD = R;
			}
			else if (R->pNext == nullptr) {
				node t = *L;
				L->pNext->pPrev = R;
				L->pPrev->pNext = R;
				R->pPrev->pNext = L;
				L->pPrev = R->pPrev;
				L->pNext = nullptr;
				R->pNext = t.pNext;
				R->pPrev = t.pPrev;
				TAIL = L;
			}
			else {
				node t = *L;
				L->pNext->pPrev = R;
				L->pPrev->pNext = R;
				R->pPrev->pNext = L;
				R->pNext->pPrev = L;
				L->pPrev = R->pPrev;
				L->pNext = R->pNext;
				R->pNext = t.pNext;
				R->pPrev = t.pPrev;
			}
		}
	}

public:
	void show_header() {
		std::cout << " ---------------------------------------------------------" << std::endl
			<< "| ÑÅÐÈß È ÍÎÌÅÐ ÏÀÑÏÎÐÒÀ | ÍÎÌÅÐ ÐÅÉÑÀ | ÍÎÌÅÐ ÀÂÈÀÁÈÁÅÒÀ |" << std::endl
			<< " ---------------------------------------------------------" << std::endl;
	}
	void show_node(Trade* current) {
		std::cout << "|      ";
		for (int i = 0; i < current->passport_number.size(); ++i) {
			if (i == 4) { std::cout << " "; }
			std::cout << current->passport_number[i];
		} std::cout << "       ";
		std::cout << "|   " << current->air_number << "    ";
		std::cout << "|    " << current->ticket_number << "     |" << std::endl;
		std::cout << " ---------------------------------------------------------" << std::endl;
	}
	TradesList() { 
		SIZE = 0; 
		HEAD = TAIL = nullptr;
	}
	void sort() {
		if (!SIZE) return;
		if (SIZE == 1) return;
		int stage = 0;
		node* current,* border, * max, * min;
		do {
			border = HEAD;
			for (int i = 0; i < stage; ++i) {
				border = border->pNext;
			}
			if (!border->pNext) break;
			max = nullptr;
			min = nullptr;
			current = HEAD;
			do {
				if (!max || current->trade.get_position() > max->trade.get_position())
					max = current;
				if (current == border) break;
				else current = current->pNext;
			} while (true);
			
			current = TAIL;
			do {
				if (!min || current->trade.get_position() < min->trade.get_position())
					min = current;
				if (current == border->pNext) break;
				else current = current->pPrev;
			} while (true);
			
			++stage;
			if (max->trade.get_position() > min->trade.get_position())
				swap(max, min);
		} while (true);
	}
	void push_back(TYPE deal) {
		if (SIZE == 0) { HEAD = TAIL = new node(CurrentTrade, deal, nullptr, TAIL); }
		else if (SIZE == 1) { HEAD->pNext = TAIL = new node(CurrentTrade, deal, HEAD, nullptr); }
		else {
			TAIL->pNext = new node(CurrentTrade, deal, TAIL);
			TAIL = TAIL->pNext;
		}
		++SIZE;
		CurrentTrade.clear();
	}
	void print() {
		std::ofstream fout("TradesList.txt");
		node* current = HEAD;
		show_header(fout);
		while (current != nullptr) {
			show_node(fout, current);
			current = current->pNext;
		}
		fout.close();
		system("start TradesList.txt");
	}
	node* find(std::string passport) {
		node* forward = HEAD;
		node* back = TAIL;
		while (forward) {
			if (forward->trade.passport_number == passport) return forward;
			if (back->trade.passport_number == passport) return back;
			forward = forward->pNext;
			back = back->pPrev;
		}
		return nullptr;
	}
	std::vector<Trade*> findAll(std::string air_numnber) {
		std::vector<Trade*> output;
		node* current = HEAD;
		while (current) {
			if (current->trade.air_number == air_numnber && current->deal == TYPE::SALE)
				output.push_back(&current->trade);
			current = current->pNext;
		}
		return output;
	}
	int size() { return SIZE; }
#pragma region Setters
	bool set_passport(string number) {
		if (KEY_IS_COOL(number)) { CurrentTrade.passport_number = number; return true; }
		else return false;
	}
	bool set_airplane(string number) {
		toUpperString(number);
		if (AIRNUMBER_IS_COOL(number)) { CurrentTrade.air_number = number; return true; }
		else return false;
	}
	bool set_ticket(string number) {
		if (TICKET_NUMBER_IS_COOL(number)) { CurrentTrade.ticket_number = number; return true; }
		else return false;
	}
#pragma endregion
};