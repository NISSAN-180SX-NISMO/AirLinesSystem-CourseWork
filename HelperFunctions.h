#pragma once
#include <string>
#include <algorithm> 
#include <cctype> 
#include <string>

const int MAX = 256;

void toUpperString(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return c == '�' ? '�' : std::toupper(c); });
}

// ������� ��� ������ ��������
void badCharHeuristic(std::string str, int size, int badChar[MAX]) {

    for (int i = 0; i < MAX; i++)       // �������������� ��� �������� ��� -1
        badChar[i] = -1;

    for (int i = 0; i < size; i++)      // ��������� ������ �������� ��� �������� � �������
        badChar[abs((int)str[i])] = i;
}

// ������� ��� ������ ����� � ������ �� ��������� ������-����
bool FindFragment(std::string text, std::string pattern) {

    int m = pattern.size();
    int n = text.size();

    int badChar[MAX];

    /* ��������� ������ �������� ��� �������� � �������
       ��������� ������� badCharHeuristic() */
    badCharHeuristic(pattern, m, badChar);
    int s = 0; // �������� ������ ������� � ������
    while (s <= (n - m)) {
        int j = m - 1; // ������ ���������� ������� �������
        // ���������, ��������� �� ������� ������ ������ � �������� � �������
        while (j >= 0 && pattern[j] == text[s + j]) j--;

        // ���� ��� ������� � ������� ������� � ��������� � ������
        if (j < 0) {
            return true;
            //cout << "������ ������ � ������� " << s << endl;

            /* ������� ������, ����� ����� ��������� ��������� � ������,
               ��������� ������� �������� ��� ������� */
            //s += (s + m < n) ? m - badChar[abs(text[s + m])] : 1;

        }

        else
        {
            /* ������� ������ �� ������ �������� ��� "�������" ������� � �������,
               ����� ������� �� 1 */
            s += max(1, j - badChar[abs(text[s + j])]);
        }
    }
    return false;
}