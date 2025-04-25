/*************************************************************
 * ДЗ-3 Нормальный алгоритм Маркова (Вариант 3.23)
 * Студент: Батычков Вячеслав Геннадьевич, КТбо1-7
 * Программа: реализация нормального алгоритма Маркова
 * по преобразованию входного слова через подстановки строк
 * Схема: последовательное применение первой подходящей подстановки
 * Правила заданы в таблице из 22 пар «LEFT→RIGHT»
 * Завершение: применение самой последней пары
 *************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// Структура представляет правило подстановки в НАМ
struct Rule {
    string left;  // Левая часть правила подстановки
    string right; // Правая часть правила подстановки
};

// Подпрограмма проверяет строку на корректность символов (0-3)
// Вход: строка, введённая пользователем
// Выход: 0 – корректно; 1 – обнаружена ошибка формата
bool validInput(string *w);

// Подпрограмма применяет подходящее правило НАМ
// Вход: текущее слово, вектор структуры подстановок, номер правила,
// позиция в строке, длина замененной части, слово до применения
// Выход: 0 – продолжить; 1 – завершить работу алгоритма
bool applyFirst(string *w, vector<Rule> *rules, int *rule, int *pos, int *len, string *before);

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD   mode;
    if (GetConsoleMode(hOut, &mode))
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    const WORD clrNorm = 7, clrRed = WORD(FOREGROUND_RED | FOREGROUND_INTENSITY);

    vector<Rule> rules = {
        {"00|","|0"}, {"01|","|1"}, {"02|","|2"}, {"03|","|3"},
        {"10|","|4"}, {"11|","|5"}, {"12|","|6"}, {"13|","|7"},
        {"20|","|8"}, {"21|","|9"}, {"22|","|A"}, {"23|","|B"},
        {"30|","|C"}, {"31|","|D"}, {"32|","|E"}, {"33|","|F"},
        {"m|0","0m|"}, {"m|1","1m|"}, {"m|2","2m|"}, {"m|3","3m|"},
        {"m|","|"},   {"|",""}
    };

    bool work = true;            
    while (work) {
        string word;
        bool wordReady = false;
        while (!wordReady && work) {
            cout << "Введите входное слово: ";
            if (cin >> word) {
                if (validInput(&word))
                    wordReady = true;
                else
                    cout << "Ошибка: допустимы только цифры 0–3 без ведущего нуля. Попробуйте ещё раз.\n";
            } else
                work = false;
        }

        if (work) {
            word = "m|" + word;
            bool done = false;
            while (!done) {
                int idx, pos, len; string before;
                done = applyFirst(&word, &rules, &idx, &pos, &len, &before);

                cout << '(' << idx + 1 << ") ";
                for (int i = 0; i < (int)before.size(); i++) {
                    if (i == pos)          
                        SetConsoleTextAttribute(hOut, clrRed);
                    if (i == pos + len)    
                        SetConsoleTextAttribute(hOut, clrNorm);
                    cout << before[i];
                }
                SetConsoleTextAttribute(hOut, clrNorm);
                cout << " -> " << word << endl;
            }

            cout << "Результат: " << word << endl;
            bool gotChoice = false;
            while (!gotChoice) {
                cout << "Хотите повторить ввод? (1 - да, 2 - нет): ";
                string choice; cin >> choice;

                if (choice == "1")
                    gotChoice = true;     
                else if (choice == "2") {
                    gotChoice = true;
                    work = false;
                    cout << "Программа завершена" << endl;
                } else
                    cout << "Допустимы только значения 1 или 2. Попробуйте ещё раз." << endl;
            }
        }
    }
    return 0;
}

bool validInput(string *w) {
    bool good = !w -> empty() && !(w -> size() > 1 && (*w)[0] == '0');
    for (char c : *w) {
        if (good && (c < '0' || c > '3')) 
            good = false;
    }
    return good;
}

bool applyFirst(string *w, vector<Rule> *rules, int *rule, int *pos, int *len, string *before) {
    bool isDone = true;
    *rule = *pos = *len = -1;
    for (int p = 0, n = (int)w -> size(); p < n && isDone; p++)
        for (int r = 0, m = (int)rules -> size(); r < m && isDone; r++) {
            int l = (int)(*rules)[r].left.size();
            if (p + l <= n && w -> compare(p, l, (*rules)[r].left) == 0) {
                *before = *w;
                w -> replace(p, l, (*rules)[r].right);
                *rule = r; *pos = p; *len = l;
                isDone = (r == m - 1);
            }
        }
    return isDone;
}