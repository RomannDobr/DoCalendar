// DoCALENDAR v1.3
//
// программа считает дни оставшиеся до какого-либо события
// либо дни прошедшие после какого-либо события

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <Windows.h>
#include <limits>

// #include <tchar.h>
// #include <shlobj.h>
// #include <objbase.h>
// #include <shellapi.h>

using namespace std;

void nowData(int w, int d, int m, int y);
void newSeason(int x, int y, string a);
void userCount(int x, int y, string message);
void manual();
int charToInt(char iks);
int spaceLength(string iks, string std, int dif);
int spaceLength(string iks);

int main() {
setlocale(LC_ALL, "RU");

SetConsoleOutputCP(65001);
SetConsoleCP(65001);

cout << "\n                 ---      ДоКалендарь      ---                 \n";

/// посмотреть как будет меняться сколнение недель на сезонах (difference+1 в строке 302)
/// добавить верификацию
/// добавить автозагрузку (добавить вопрос пользователю)
/// добавить иконку

//// остановился на: настраивал отступы при удалении Менее недели и Менее года

time_t now = time(0); // текущая дата/время, основанные на текущей системе <ctime>
tm *ltm = localtime(&now);
int year, month, day, wday;
year = 1900 + ltm->tm_year;
month = 1 + ltm->tm_mon;
day = ltm->tm_mday;
wday = ltm->tm_wday;

nowData(wday,day,month,year); // отображение текущей даты

int mesyac = month; // переменные там всякие
int oneoftvelve = 0;
int oneofyear = 0;
int oneyear = 0;
int oneofday = 0;
int question = 0;
int* const n = new int {54321}; // просто здоровое число для счётчиков (меньше не работает)
int j = 0;

struct tm a = {0,0,0,day,month-1,100,0,0,0}; // текущая дата
time_t x = mktime(&a); // 
struct tm b = {0,0,0,0,oneoftvelve,101,0,0,0}; // ожидаемая дата
time_t y = mktime(&b); // 


// ОТКРЫВАЕТ СОХРАНЕННЫЕ СОБЫТИЯ
ifstream firstfiles;
    firstfiles.open("DoCalendar_data.txt", ios::in);
    if(firstfiles.is_open())
    {
    string buffer1;
    string buffer0;
    char buf;
    int bu;
    int b;
    int j = 0;
         SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
    for(int i; j<4 && i<*n; i++) // поиск стартовой метки события - "|"
    {
    firstfiles >> buf;      
    if(buf == '|')
    {
    firstfiles >> buffer1; // считывание названия из одного слова
    for(int i; buf != '.'; i++) // считывание большого названия
    {
        firstfiles >> buf;
        if(buf == '.') break;
        firstfiles.seekg(-1, ios::cur);
        buffer1.append(" ");
        firstfiles >> buffer0;
        buffer1.append(buffer0);
    }
    }
    if(buf == '.')
    {
    firstfiles >> buf; // считывание и пропуск первого символа
    firstfiles >> buf; // считывание и пропуск второго символа
    firstfiles >> buf; // считывание третьго символа (20-2-5)
    bu = charToInt(buf);
    firstfiles >> buf; // считывание четвёртого символа (202-5)
    b = charToInt(buf);
    oneyear = bu*10+b+75;
    firstfiles >> buf;
    bu = charToInt(buf);
    firstfiles >> buf;
    if(buf == '-') oneoftvelve = bu;
    else
    {
    b = charToInt(buf);
    oneoftvelve = bu*10+b;
    }
    firstfiles >> buf;
    if(buf == '-') firstfiles >> buf;
    bu = charToInt(buf);
    firstfiles >> buf;
    if(buf == '-') oneofday = bu;
    else
    {
    b = charToInt(buf);
    oneofday = bu*10+b;
    }
struct tm c = {0,0,0,oneofday,oneoftvelve-1,oneyear,0,0,0};
time_t v = mktime(&c);
userCount(x,v,buffer1);
    }
    }
    firstfiles.close();
    }

        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);

if (mesyac >= 9) // условие для запуска счётчика до нового года
{
newSeason(x,y,"До нового года");
}
if (mesyac <= 2) // условие для запуска счётчика до весны
{
oneoftvelve = 3;
oneofday = 0;
oneyear = 100;
struct tm c = {0,0,0,oneofday,oneoftvelve-1,oneyear,0,0,0};
time_t spring = mktime(&c);
newSeason(x,spring,"До весны");
}
if (mesyac >= 3 && mesyac < 6) // условие для запуска счётчика до лета
{
oneoftvelve = 6;
oneofday = 0;
oneyear = 100;
struct tm c = {0,0,0,oneofday,oneoftvelve-1,oneyear,0,0,0};
time_t summer = mktime(&c);
newSeason(x,summer,"До лета");
}
if (mesyac >= 6 && mesyac < 9) // условие для запуска счётчика до конца лета
{
oneoftvelve = 9;
oneofday = 0;
oneyear = 100;
struct tm c = {0,0,0,oneofday,oneoftvelve-1,oneyear,0,0,0};
time_t fall = mktime(&c);
newSeason(x,fall,"До конца лета");
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
    efiles.open("DoCalendar_data.txt", ios::in);
    if(efiles.is_open()) {
         SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
    for(int i; i<*n; i++)
    { 
        efiles >> buf;       
        if(buf == '|') { // персчитывает колличество событий. Для отображения
        j++;
        if(j == 1) {
        efiles >> event1;
            for(int i; buf != '.'; i++) // считывание большого названия
            {
                efiles >> buf;
                if(buf == '.') break;
                efiles.seekg(-1, ios::cur);
                event1.append(" ");
                efiles >> buffer0;
                event1.append(buffer0);
            }
        }
        if(j == 2)
        {
        efiles >> event2;
                    for(int i; buf != '.'; i++) // считывание большого названия
                    {
                efiles >> buf;
                if(buf == '.') break;
                efiles.seekg(-1, ios::cur);
                event2.append(" ");
                efiles >> buffer0;
                event2.append(buffer0);
            }
        }
        if(j == 3)
        {
        efiles >> event3;
                    for(int i; buf != '.'; i++) // считывание большого названия
                    {
                efiles >> buf;
                if(buf == '.') break;
                efiles.seekg(-1, ios::cur);
                event3.append(" ");
                efiles >> buffer0;
                event3.append(buffer0);
            }
        }
        if(j == 4)
        {
        efiles >> event4;
                    for(int i; buf != '.'; i++) // считывание большого названия
                    {
                efiles >> buf;
                if(buf == '.') break;
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

if(j < 4) cout << "      ->>> Создать событие?  (нажмите 1)\n";
if(j > 0) cout << "      <<<- Удалить событие? ";

         SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);

if(j == 1) cout << " " << event1 << " (3).\n";
if(j == 2) cout << " " << event1 << " (3).\n" << "                             " 
                << event2 << "(4).\n";
if(j == 3) cout << " " << event1 << " (3).\n" << "                             "
<< event2 << " (4).\n"<< "                             " << event3 << " (5).\n";
if(j == 4) cout << " " << event1 << " (3).\n" << "                             "
<< event2 << " (4).\n" << "                             " << event3 << " (5).\n"
<< "                             " << event4 << " (6).\n";

        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);

cout << "       ->>  Инстркуция  <<-  (нажмите 2)\n";

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

ofstream name("DoCalendar_data.txt", ios::app);
name << "|" << message << " . ";
name.close();

        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001); 

cout << " Две последних цифры года события\n";
ofstream name2("DoCalendar_data.txt", ios::app);
if(name2.is_open())
{
cin >> oneofyear;
oneyear = oneofyear+2000;
ofstream name2("DoCalendar_data.txt", ios::app);
name2 << oneyear << " ";
name2.close();
cout << " Месяц события\n";
ofstream name3("DoCalendar_data.txt", ios::app);
cin >> oneoftvelve;
name3 << oneoftvelve << "-" << " ";
name3.close();
cout << " День\n";
ofstream name4("DoCalendar_data.txt", ios::app);
cin >> oneofday;
name4 << oneofday << "-" << " ";
name4.close();
}
struct tm c = {0,0,0,oneofday,oneoftvelve-1,oneyear-1925,0,0,0};
time_t w = mktime(&c);
userCount(x,w,message);
}
         
// УДАЛЕНИЕ
if (question >= 3 && question <= 6)
    {
    char *buff = new char[*n];
    char buf;
    ifstream delfil1("DoCalendar_data.txt", ios::in);
    for(int i, l=0; delfil1; i++)
        { 
         SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
        delfil1.get(buf);
        if(buf == '|') l++;
        if(question == 3 && l != 1 && l != 0) buff[i] = buf;
        if(question == 4 && l != 2 && l != 0) buff[i] = buf;
        if(question == 5 && l != 3 && l != 0) buff[i] = buf;
        if(question == 6 && l != 4 && l != 0) buff[i] = buf;
        }
        delfil1.close();
    ofstream delfiles1("DoCalendar_data.txt", ios::out);
    for(int i=0; i<321; i++)
        {
        if(buff[i] != '\0')
        {
        delfiles1 << buff[i];
        }
        }
        if(question == 3)
        {
            cout << event1;
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if(question == 4)
        {
            cout << event2;
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if(question == 5)
        {
            cout << event3;
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        }
        if(question == 6)
        {
            cout << event4;
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
            cout << " удалёно";
        delfiles1.close();
        }
            delete[] buff;
        }

// ИНСТРУКЦИЯ
if (j < 5 && question == 2) manual();

delete n;

for (int i = 0; i<5; i++) cout << "\n";
system("pause");
return 0;
}

///////////////////////////////////////////////////////////////////////////////////

void nowData(int w, int d, int m, int y) // отображение текущей даты
    {
    cout << "                                        \n";
    cout << "                    Сегодня ";
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
    cout << "," << d << "." << m << "." << y << "г." << endl;
}

void newSeason(int x, int y, string a) // счётчик до НГ, весны, лета, конца лета
{
   if ( x != (time_t)(-1) && y != (time_t)(-1) )
{
        int difference = difftime(y, x) / (60 * 60 * 24);
        cout << " ______________________________________________________________\n\n";
        for(int i=0; i<spaceLength(a); i++) cout << " ";
        cout << a;
        cout << " " << difference+1 << " дн. " << difference/7;
        if((difference/7) % 10 == 1) cout  << " полная нед." << endl;
        if((difference/7) == 0) cout  << " меньше нед." << endl;
        if((difference/7) % 10 != 1 && (difference/7) != 0) cout  << " полных нед." << endl;
        cout << " ______________________________________________________________\n";
    }
}

void userCount(int x, int y, string message) // отображение первого счётчика "ДО"
    {
    fstream name;
    char msg;
    name.open("DoCalendar_data.txt", ios::in);
         SetConsoleOutputCP(1251); // SetConsoleOutputCP(1251);
        SetConsoleCP(1251);       //         SetConsoleCP(1251);
    int j = 0;
    for(int i; j<1 && i<54321; i++)
    { 
        name >> msg;
    if (msg == '|' && x != (time_t)(-1) && y != (time_t)(-1) )
    {
        if (y > x)
        {
        int difference = difftime(y, x) / (60 * 60 * 24);
        cout << " ______________________________________________________________\n\n";
        for(int i=0; i<=spaceLength(message, " через ", difference)-1; i++) cout << " ";
        cout << message; 
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
        cout << " через " << difference << " дн. ";
        if((difference/7) % 10 == 1) cout  << " полная нед. ";
        // if((difference/7) == 0) cout  << " меньше нед. ";
        if((difference/7) % 10 != 1 && (difference/7) != 0) cout << difference/7 << " полных нед. ";
        if((difference/365) == 0) cout << endl;
        if((difference/365) == 1 || ((difference/365) % 10 == 1 && (difference/365) != 11))
                        cout << difference/365  << " год." << endl;
        if((difference/365) == 11) cout << difference/365  << " лет." << endl;
        if((difference/365) % 10 > 1 && (difference/365) % 10 < 5)
                        cout << difference/365  << " годa." << endl;
        if(((difference/365) % 10 > 4 && (difference/365) % 10 <= 9) 
        || ((difference/365) % 10 == 0 && (difference/365) != 0))
                        cout << difference/365  << " лет." << endl;
        if((difference/365) < 0) cout << difference/365  << " лет." << endl; // лишнее в последствии
        cout << " ______________________________________________________________\n";
        j++;
        }
        if (y < x)
        {
        int difference = difftime(x, y) / (60 * 60 * 24);
        cout << " ______________________________________________________________\n\n";
        for(int i=0; i<=spaceLength(message, " уже   ", difference)-1; i++) cout << " ";
        cout << message;
        SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
         SetConsoleCP(65001);     //       SetConsoleCP(65001);
        cout << " уже " << difference << " дн. ";
        if((difference/7) % 10 == 1) cout << difference/7  << " полная нед. ";
        // if((difference/7) == 0) cout  << " меньше нед. ";
        if((difference/7) % 10 != 1 && (difference/7) != 0) cout << difference/7  << " полных нед. ";
        if((difference/365) == 0) cout << endl;
        if((difference/365) == 1 || ((difference/365) % 10 == 1 && (difference/365) != 11))
                        cout << difference/365  << " год." << endl;
        if((difference/365) == 11) cout << difference/365  << " лет." << endl;
        if((difference/365) % 10 > 1 && (difference/365) % 10 < 5)
                        cout << difference/365  << " годa." << endl;
        if(((difference/365) % 10 > 4 && (difference/365) % 10 <= 9) 
        || ((difference/365) % 10 == 0 && (difference/365) != 0))
                        cout << difference/365  << " лет." << endl;
        if((difference/365) < 0) cout << difference/365  << " лет." << endl; // лишнее в последствии
        cout << " ______________________________________________________________\n";
        j++;
            }
        }
    }
    name.close();
}

int charToInt (char iks) // преодразование char в int (от 0 до 9)
{
    char character[10];
    int x;
    for(int i=0; i<10; i++)
        {
        character[i] = '0'+i;
        if(character[i] == iks)
        {
            x = i; 
            break;
        } 
    }
    return x;
}

int spaceLength(string iks, string std, int dif)
{
    int x=0, s=0, d=0, j=1, k=0;
    for(size_t i=0; iks[i]; i++) x++;
    for(size_t i=0; i<std.size(); i++) s++;
    for(int i=1; i<dif; i++)
    {
        if(dif/j < 10 && dif/j > 0)
        {
            d = i;
            break;
        }
        j *= 10;
    }
    if(dif < 356) k=2;
        x = (40-(x+s+d+k))/2;
    return x;
}

int spaceLength(string iks)
{
    int x = 0;
    for(size_t i=0; i<iks.size(); i++) x++;
        x = (50-x)/2;
    return x;
}

void manual()
{
    cout << " --------------------------------------------------------------\n";
    cout << "    Программа считает дни оставшиеся до какого-либо события,\n";
    cout << "        а также дни прошедшие после какого-либо события.\n";
    cout << "     Чтобы создать новый отсчёт перезапустите ДоКалендарь,\n";
    cout << "                      затем нажмите \"1\".\n";
    cout << " --------------------------------------------------------------";
}