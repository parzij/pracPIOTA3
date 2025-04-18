/*************************************************************
 * ДЗ‑3 Нормальный алгоритм Маркова (Вариант 3.23)
 * Студент: Батычков Вячеслав Геннадьевич, КТбо1‑7
 * Программа: реализация нормального алгоритма Маркова
 * по преобразованию входного слова через подстановки строк
 * Схема: последовательное применение первой подходящей подстановки
 * Правила заданы в таблице из 21 пары «LEFT→RIGHT»
 * Завершение: отсутствие применимых правил или пустая замена маркера '|'
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

// Подпрограмма проверяет строку на корруктность символов (0-3)
// Вход: строка, введённая пользователем
// Выход: 0 – корректно; 1 – обнаружена ошибка формата
bool validInput(string *w);

// Подпрограмма применяет подходящее правило НАМ
// Вход: текущее слово, ветор структуры подстановок, номер правила, 
// позиция в строке, длина замененной части, слово до применения, слово после применения
// Выход: 0 – продолжить; 1 – завершить работу алгоритма
bool applyFirst(string *w, vector<Rule> *rules, int *rule, int *pos, int *len, string *before);

int main() {
    SetConsoleOutputCP(65001);  
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;  
    if (GetConsoleMode(hOut, &mode))
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    const WORD clrNorm = 7, clrRed = WORD(FOREGROUND_RED | FOREGROUND_INTENSITY);

    /* -------- таблица подстановок -------- */
    vector<Rule> rules = {
        {"00|","|0"},{"01|","|1"},{"02|","|2"},{"03|","|3"},
        {"10|","|4"},{"11|","|5"},{"12|","|6"},{"13|","|7"},
        {"20|","|8"},{"21|","|9"},{"22|","|A"},{"23|","|B"},
        {"30|","|C"},{"31|","|D"},{"32|","|E"},{"33|","|F"},
        {"0|","|0"},{"1|","|1"},{"2|","|2"},{"3|","|3"},{"|",""}
    };

    bool workFlag = true;
    while (workFlag) {
        cout << "Введите входное слово: ";
        string word; 
        bool validWord = false, exitWordLoop = false;
        while (!validWord && !exitWordLoop) {
            cin >> word;
            validWord = validInput(&word);
            if (!validWord) {
                cout << "Ошибка: допустимы только цифры 0–3 без ведущего нуля.\nПовторить ввод? (1 - да, 2 - нет): ";
                bool validChoice = false;
                string choice;
                while (!validChoice) {
                    cin >> choice;
                    if (choice == "1") {
                        validChoice = true;
                        cout << "Введите входное слово: ";
                    } else if (choice == "2") {
                        validChoice = true;
                        workFlag = false;
                        exitWordLoop = true;
                    } else
                        cout << "Ошибка: введите только 1 или 2: ";
                }
            }
        }
        
        if (workFlag) {
            word.push_back('|');
            bool isDone = false;
            while (!isDone) {
                int rule = -1, pos = -1, len = 0; 
                string before;
                isDone = applyFirst(&word, &rules, &rule, &pos, &len, &before);

                if (rule != -1) {
                    cout << '(' << rule + 1 << ") ";
                    for (size_t i = 0; i < before.size(); i++) {
                        if (i == (size_t)pos) 
                            SetConsoleTextAttribute(hOut, clrRed);
                        if (i == pos + len) 
                            SetConsoleTextAttribute(hOut, clrNorm);
                        cout << before[i];
                    }
                    SetConsoleTextAttribute(hOut, clrNorm);
                    cout << " -> " << word << endl;
                }
            }
            cout << "Результат: " << word << "\nПродолжить? (1 - да, 2 - нет): ";
            
            bool validAgain = false;
            while (!validAgain) {
                string again; cin >> again;
                if (again == "1") 
                    validAgain = true;
                else if (again == "2") {
                    validAgain = true;
                    workFlag = false;
                    cout << "Программа завершена\n";
                } else
                    cout << "Ошибка: введите только 1 или 2: ";
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