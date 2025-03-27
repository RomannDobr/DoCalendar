// DoCALENDAR v1.3
//
// программа считает дни оставшиеся до какого-либо события
// либо дни прошедшие после какого-либо события

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <string>
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

#define STRLEN(x) (sizeof(x)/sizeof(TCHAR) - 1)

void nowData(int w, int d, int m, int y);
void newSeason(int x, int y, string a);
void userCount(int x, int y, string message);
int charToInt(char iks);
void autorun(int tog);
void manual();

int main() {
    setlocale(LC_ALL, "RU");

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "\n ---    ДоКалендарь    ---\n";

    /// посмотреть как будут меняться счётчики на переходах
    /// настроить отображение консоли
    /// добавить верификацию
    /// убрать иероглифы из текстовика
    /// дорулить автозагрузку (вкл\выкл)

    //// остановился на: проверка автозагрузки

    time_t now = time(0); // текущая дата/время, основанные на текущей системе <ctime>
    struct tm* ltm = localtime(&now);
    int year, month, day, wday;
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    wday = ltm->tm_wday;

    int mesyac = month; // переменные там всякие
    int oneoftvelve = 0;
    int oneofyear = 0;
    int oneyear = 0;
    int oneofday = 0;
    int question = 7; // 0 занят
    int* const n = new int{ 54321 }; // просто здоровое число для счётчиков (меньше не работает)
    int j = 0;

    struct tm a = { 0,0,0,day,month - 1,101,0,0,0 }; // текущая дата
    time_t x = mktime(&a); // 
    struct tm b = { 0,0,0,0,oneoftvelve,101,0,0,0 }; // ожидаемая дата
    time_t y = mktime(&b); // 

    nowData(wday, day, month, year); // отображение текущей даты

    char re[MAX_PATH];
    string FP = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

    //   cout << "\n14:59\n";
    //   HKEY hkey;
    // LONG key = RegOpenKeyExA(HKEY_CURRENT_USER,
    // "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
    //   wcout << "\n key> " << key << "\n" << 

    //   fs::path(FP).replace_filename("DoCalendar_data.txt") << endl;

        // ОТКРЫВАЕТ СОХРАНЕННЫЕ СОБЫТИЯ
    for (int i = 0; i < 3; i++)
    {
        ifstream firstfiles;
        firstfiles.open(fs::path(FP).replace_filename("DoCalendar_data.txt"));
        if (firstfiles.is_open())
        {
            // cout << " >> file is open << \n";
            string buffer1;
            string buffer0;
            char buf;
            int bu{};
            int b{};
            int j{};
            SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
            SetConsoleCP(1251);       //         SetConsoleCP(1251);
            for (int i = 0; j < 4 && i < 10; i++) // поиск стартовой метки события - "|"
            {
                firstfiles >> buf;
                if (buf == '|')
                {
                    firstfiles >> buffer1; // считывание названия из одного слова
                    for (int i = 0; buf != '.'; i++) // считывание большого названия
                    {
                        firstfiles >> buf;
                        if (buf == '.') break;
                        firstfiles.seekg(-1, ios::cur);
                        buffer1.append(" ");
                        firstfiles >> buffer0;
                        buffer1.append(buffer0);
                    }
                }
                if (buf == '.')
                {
                    firstfiles >> buf; // считывание и пропуск первого символа
                    firstfiles >> buf; // считывание и пропуск второго символа
                    firstfiles >> buf; // считывание третьго символа (20->2<-5)
                    bu = charToInt(buf);
                    firstfiles >> buf; // считывание четвёртого символа (202->5)
                    b = charToInt(buf);
                    oneyear = bu * 10 + b + 75;
                    firstfiles >> buf;
                    bu = charToInt(buf);
                    firstfiles >> buf;
                    if (buf == '-') oneoftvelve = bu;
                    else
                    {
                        b = charToInt(buf);
                        oneoftvelve = bu * 10 + b;
                    }
                    firstfiles >> buf;
                    if (buf == '-') firstfiles >> buf;
                    bu = charToInt(buf);
                    firstfiles >> buf;
                    if (buf == '-') oneofday = bu;
                    else
                    {
                        b = charToInt(buf);
                        oneofday = bu * 10 + b;
                    }
                    struct tm c = { 0,0,0,oneofday,oneoftvelve - 1,oneyear,0,0,0 };
                    time_t v = mktime(&c);
                    userCount(x, v, buffer1);
                }
            }
            firstfiles.close();
            break;
        }

    }

    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
    SetConsoleCP(65001);     //       SetConsoleCP(65001);

    if (mesyac >= 9) // условие для запуска счётчика до нового года
    {
        newSeason(x, y, "  До нового года");
    }
    if (mesyac <= 2) // условие для запуска счётчика до весны
    {
        oneoftvelve = 3;
        oneofday = 0;
        oneyear = 100;
        struct tm c = { 0,0,0,oneofday,oneoftvelve - 1,oneyear + 1,0,0,0 };
        time_t spring = mktime(&c);
        newSeason(x, spring, "  До весны");
    }
    if (mesyac >= 3 && mesyac < 6) // условие для запуска счётчика до лета
    {
        oneoftvelve = 6;
        oneofday = 0;
        oneyear = 100;
        struct tm c = { 0,0,0,oneofday,oneoftvelve - 1,oneyear + 1,0,0,0 };
        time_t summer = mktime(&c);
        newSeason(x, summer, "  До лета");
    }
    if (mesyac >= 6 && mesyac < 9) // условие для запуска счётчика до конца лета
    {
        oneoftvelve = 9;
        oneofday = 0;
        oneyear = 100;
        struct tm c = { 0,0,0,oneofday,oneoftvelve - 1,oneyear + 1,0,0,0 };
        time_t fall = mktime(&c);
        newSeason(x, fall, "  До конца лета");
    }
    cout << "\n\n";

    // ИНДЕКСАЦИЯ СОБЫТИЙ
    ifstream efiles;
    string event1;
    string event2;
    string event3;
    string event4;
    string buffer0;
    char buf;
    efiles.open(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::in);
    if (efiles.is_open()) {
        SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
        for (int i = 0; i < *n; i++)
        {
            efiles >> buf;
            if (buf == '|') { // персчитывает колличество событий. Для отображения
                j++;
                if (j == 1) {
                    efiles >> event1;
                    for (int i = 0; buf != '.'; i++) // считывание большого названия
                    {
                        efiles >> buf;
                        if (buf == '.') break;
                        efiles.seekg(-1, ios::cur);
                        event1.append(" ");
                        efiles >> buffer0;
                        event1.append(buffer0);
                    }
                }
                if (j == 2)
                {
                    efiles >> event2;
                    for (int i = 0; buf != '.'; i++) // считывание большого названия
                    {
                        efiles >> buf;
                        if (buf == '.') break;
                        efiles.seekg(-1, ios::cur);
                        event2.append(" ");
                        efiles >> buffer0;
                        event2.append(buffer0);
                    }
                }
                if (j == 3)
                {
                    efiles >> event3;
                    for (int i = 0; buf != '.'; i++) // считывание большого названия
                    {
                        efiles >> buf;
                        if (buf == '.') break;
                        efiles.seekg(-1, ios::cur);
                        event3.append(" ");
                        efiles >> buffer0;
                        event3.append(buffer0);
                    }
                }
                if (j == 4)
                {
                    efiles >> event4;
                    for (int i = 0; buf != '.'; i++) // считывание большого названия
                    {
                        efiles >> buf;
                        if (buf == '.') break;
                        efiles.seekg(-1, ios::cur);
                        event4.append(" ");
                        efiles >> buffer0;
                        event4.append(buffer0);
                    }
                }
            }
        }
        efiles.close();
    }

    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
    SetConsoleCP(65001);     //       SetConsoleCP(65001);

    if (j < 4) cout << "  Создать событие? (нажмите 1)\n";
    if (j > 0) cout << "  Удалить событие? ";

    SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
    SetConsoleCP(1251);       //         SetConsoleCP(1251);

    if (j == 1) cout << "\n  ->> " << event1 << " (3).\n";
    if (j == 2) cout << "\n  ->> " << event1 << " (3).\n" << "  ->> " << event2 << " (4).\n";
    if (j == 3) cout << "\n  ->> " << event1 << " (3).\n" << "  ->> " << event2 << " (4).\n"
        << "  ->> " << event3 << " (5).\n";
    if (j == 4) cout << "\n  ->> " << event1 << " (3).\n" << "  ->> " << event2 << " (4).\n"
        << "  ->> " << event3 << " (5).\n" << "  ->> " << event4 << " (6).\n";

    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
    SetConsoleCP(65001);     //       SetConsoleCP(65001);

    cout << "  Инструкция (нажмите 2)\n";
    cout << "  Автозагрузка приложения (9-Добавить / 8-Удалить)\n";

    SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
    SetConsoleCP(1251);       //         SetConsoleCP(1251);

    cin >> question;

    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
    SetConsoleCP(65001);     //       SetConsoleCP(65001);

    // СОЗДАНИЕ
    if (j < 4 && question == 1)
    {
        string message;
        cout << " Введите название события:\n";

        SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);

        getline(cin, message);
        while (message.empty())
        {
            cin.clear();
            getline(cin, message);
        }

        SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);

        ofstream name(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::app);
        name << "|" << message << " . ";
        name.close();

        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
        SetConsoleCP(65001);     //       SetConsoleCP(65001); 

        cout << " Две последних цифры года события\n";
        ofstream name2(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::app);
        if (name2.is_open())
        {
            cin >> oneofyear;
            oneyear = oneofyear + 2001;
            ofstream name2(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::app);
            name2 << oneyear << " ";
            name2.close();
            cout << " Месяц события\n";
            ofstream name3(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::app);
            cin >> oneoftvelve;
            name3 << oneoftvelve << "-" << " ";
            name3.close();
            cout << " День\n";
            ofstream name4(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::app);
            cin >> oneofday;
            name4 << oneofday << "-" << " ";
            name4.close();
        }
        struct tm c = { 0,0,0,oneofday,oneoftvelve - 1,oneyear - 1925,0,0,0 };
        time_t w = mktime(&c);
        userCount(x, w, message);
    }

    // УДАЛЕНИЕ
    if (question >= 3 && question <= 6)
    {
        char* buff = new char[*n];
        char buf;
        int o = 0;
        ifstream delfil1(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::in);
        for (int i = 0; delfil1; i++)
        {
            delfil1.get(buf);
            buff[i] = buf;
            o++;
        }
        delfil1.close();
        ofstream delfiles1(fs::path(FP).replace_filename("DoCalendar_data.txt"), ios::out);
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
        SetConsoleCP(65001);     //       SetConsoleCP(65001);
        for (int i = 0, l = 0; i <= o; i++)
        {
            if (buff[i] == '|') l++;
            if (question == 3 && l != 1 && l != 0) delfiles1 << buff[i];
            if (question == 4 && l != 2 && l != 0) delfiles1 << buff[i];
            if (question == 5 && l != 3 && l != 0) delfiles1 << buff[i];
            if (question == 6 && l != 4 && l != 0) delfiles1 << buff[i];
        }
        SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
        if (question == 3)
        {
            cout << event1;
            SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
            SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if (question == 4)
        {
            cout << event2;
            SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
            SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if (question == 5)
        {
            cout << event3;
            SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
            SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if (question == 6)
        {
            cout << event4;
            SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
            SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
            delfiles1.close();
        }
        delete[] buff;
    }

    // АВТОЗАГРУЗКА
    if (question == 9)
    {
        autorun(question);
        MessageBox(NULL, _T("Autorun is ON"), _T("Autorun"), 0);
    }
    if (question == 8)
    {
        autorun(question);
        MessageBox(NULL, _T("Autorun is OFF"), _T("Autorun"), 0);
    }

    // ИНСТРУКЦИЯ
    if (j < 5 && question == 2) manual();

    delete n;

    for (int i = 0; i < 5; i++) cout << "\n";
    system("pause");
}

///////////////////////////////////////////////////////////////////////////////////

void nowData(int w, int d, int m, int y) // отображение текущей даты
{
    cout << "\n - Сегодня ";
    switch (w)
    {
    case 1: cout << "пн"; break;
    case 2: cout << "вт"; break;
    case 3: cout << "ср"; break;
    case 4: cout << "чт"; break;
    case 5: cout << "пт"; break;
    case 6: cout << "сб"; break;
    case 7: cout << "вс"; break;
    }
    cout << "." << d << "." << m << "." << y << "г. -" << endl;
}

void newSeason(int x, int y, string a) // счётчик до НГ, весны, лета, конца лета
{
    if (x != (time_t)(-1) && y != (time_t)(-1))
    {
        int difference = difftime(y, x) / (60 * 60 * 24);
        cout << " ___________________________________________________________________/\n\n";
        cout << a;
        cout << " " << difference << " дн. " << difference / 7;
        if ((difference / 7) % 10 == 1) cout << " полная нед." << endl;
        if ((difference / 7) == 0) cout << " меньше нед." << endl;
        if ((difference / 7) % 10 != 1 && (difference / 7) != 0) cout << " полных нед." << endl;
        cout << " ___________________________________________________________________/\n";
    }
}

void userCount(int x, int y, string message) // отображение первого счётчика "ДО"
{
    fstream name;
    char msg;
    char re[MAX_PATH];
    string FP = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));
    name.open(fs::path(FP).replace_filename("DoCalendar_data.txt"));
    SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
    SetConsoleCP(1251);       //         SetConsoleCP(1251);
    int j = 0;
    for (int i = 0; j < 1 && i < 54321; i++)
    {
        name >> msg;
        if (msg == '|' && x != (time_t)(-1) && y != (time_t)(-1))
        {
            if (y > x)
            {
                int difference = difftime(y, x) / (60 * 60 * 24);
                cout << " ___________________________________________________________________/\n\n";
                if ((difference) == 1)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " завтра. " << endl;
                }
                if ((difference) < 8 && (difference) > 1)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << endl;
                }

                if ((difference / 7) % 10 == 1 && (difference / 365) < 1 && (difference) >= 8)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полную нед. " << endl;
                }

                if ((difference / 7) % 10 != 1 && (difference / 7) != 0 && (difference / 365) < 1 && (difference) >= 8)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полных нед. " << endl;
                }

                if ((difference / 365) == 1 || ((difference / 365) % 10 == 1 && (difference / 365) != 11))
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " год." << endl;
                }

                if ((difference / 365) == 11)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " лет." << endl;
                }

                if ((difference / 365) % 10 > 1 && (difference / 365) % 10 < 5)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " годa." << endl;
                }

                if (((difference / 365) % 10 > 4 && (difference / 365) % 10 <= 9) || ((difference / 365) % 10 == 0 && (difference / 365) != 0))
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " через " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " лет." << endl;
                }
                cout << " ___________________________________________________________________/\n";
                j++;
            }

            if (y < x)
            {
                int difference = difftime(x, y) / (60 * 60 * 24);
                cout << " ___________________________________________________________________/\n\n";
                if ((difference) == 1)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " вчера. " << endl;
                }
                if ((difference) < 8 && (difference) > 1)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << endl;
                }

                if ((difference / 7) % 10 == 1 && (difference / 365) < 1 && (difference) >= 8)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полную нед. " << endl;
                }

                if ((difference / 7) % 10 != 1 && (difference / 7) != 0 && (difference / 365) < 1 && (difference) >= 8)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полных нед. " << endl;
                }

                if ((difference / 365) == 1 || ((difference / 365) % 10 == 1 && (difference / 365) != 11))
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " год." << endl;
                }

                if ((difference / 365) == 11)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " лет." << endl;
                }

                if ((difference / 365) % 10 > 1 && (difference / 365) % 10 < 5)
                {
                    cout << "  " << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " годa." << endl;
                }

                if (((difference / 365) % 10 > 4 && (difference / 365) % 10 <= 9) || ((difference / 365) % 10 == 0 && (difference / 365) != 0))
                {
                    cout << message;
                    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                    SetConsoleCP(65001);     //       SetConsoleCP(65001);
                    cout << " уже " << difference << " дн. " << difference / 7 << " полных нед. " << difference / 365 << " лет." << endl;
                }
                cout << " ___________________________________________________________________/\n";
                j++;
            }
            if (y == x)
            {
                int i = 1;
                cout << " ___________________________________________________________________/\n\n ";
                cout << " " << message;
                SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
                SetConsoleCP(65001);     //       SetConsoleCP(65001);
                cout << " сегодня! ";
                for (; i > 1; i--) cout << "<";
                cout << "\n ___________________________________________________________________/\n";
                j++;
            }
        }
    }
    name.close();
}

int charToInt(char iks) // преодразование char в int (от 0 до 9)
{
    char character[10];
    int x;
    for (int i = 0; i < 10; i++)
    {
        character[i] = '0' + i;
        if (character[i] == iks)
        {
            x = i;
            break;
        }
    }
    return x;
}

void autorun(int tog)
{
    char re[MAX_PATH];
    string FP = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

    RegGetValueA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        "DoCalendar", RRF_RT_REG_SZ, 0, 0, 0);

    if (tog == 8)
    {
        cout << "OFF" << endl;
        HKEY hkey = HKEY_CURRENT_USER;
        RegOpenKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            &hkey);
        RegDeleteValueA(hkey, "DoCalendar");
        RegCloseKey(hkey);
    }
    else if (tog == 9)
    {
        cout << "ON" << endl;
        HKEY hkey;
        LONG key = RegOpenKeyExA(HKEY_CURRENT_USER,
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
        if (ERROR_SUCCESS == key) key = RegSetValueExA(hkey, "DoCalendar", 0, REG_SZ,
            (BYTE*)FP.c_str(), strlen(FP.c_str()) + 111);
    }
}

void manual()
{
    cout << " --------------------------------------------------------------------\n";
    cout << "       Программа считает дни оставшиеся до какого-либо события,\n";
    cout << "           а также дни прошедшие после какого-либо события.\n";
    cout << "        Чтобы создать новый отсчёт перезапустите ДоКалендарь,\n";
    cout << "      затем нажмите \"1\". Можно создать не более четырёх событий.\n";
    cout << " --------------------------------------------------------------------";
}