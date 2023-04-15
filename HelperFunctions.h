#pragma once
#include <string>
#include <algorithm> 
#include <cctype> 
#include <string>

const int MAX = 256;

void toUpperString(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return c == 'я' ? 'Я' : std::toupper(c); });
}

// Функция для поиска смещения
void badCharHeuristic(std::string str, int size, int badChar[MAX]) {

    for (int i = 0; i < MAX; i++)       // Инициализируем все значения как -1
        badChar[i] = -1;

    for (int i = 0; i < size; i++)      // Заполняем массив смещений для символов в шаблоне
        badChar[abs((int)str[i])] = i;
}

// Функция для поиска слова в строке по алгоритму Бойера-Мура
bool FindFragment(std::string text, std::string pattern) {

    int m = pattern.size();
    int n = text.size();

    int badChar[MAX];

    /* Заполняем массив смещений для символов в шаблоне
       используя функцию badCharHeuristic() */
    badCharHeuristic(pattern, m, badChar);
    int s = 0; // Смещение начала шаблона в строке
    while (s <= (n - m)) {
        int j = m - 1; // Индекс последнего символа шаблона
        // Проверяем, совпадает ли текущий символ текста с символом в шаблоне
        while (j >= 0 && pattern[j] == text[s + j]) j--;

        // Если все символы в шаблоне совпали с символами в тексте
        if (j < 0) {
            return true;
            //cout << "Шаблон найден в позиции " << s << endl;

            /* Смещаем шаблон, чтобы найти следующее вхождение в строке,
               используя таблицу смещений для шаблона */
            //s += (s + m < n) ? m - badChar[abs(text[s + m])] : 1;

        }

        else
        {
            /* Смещаем шаблон на основе смещения для "плохого" символа в шаблоне,
               иначе смещаем на 1 */
            s += max(1, j - badChar[abs(text[s + j])]);
        }
    }
    return false;
}