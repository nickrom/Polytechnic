#include <iostream>
//#include <conio.h>
#include <iomanip>
#include <string.h>
#include <fstream>
#include <ostream>
#include <unistd.h>
#include <string> 
using namespace std;


bool db_isopened = 0;
string db_name = "";

struct Product
{
    int storage;
    string name;
    int price;
    int num;
};

class Inventory
{
private:
    static const int SIZE = 20;
    Product inv[SIZE];
    int count;
public:
    Inventory(): count(0) {}
    //~Inventory()
    //grow()
    long long account(int);
    int getCount();
    void add(Product);
    bool del(int, string);
    int find(int, string);
    void find_user(int, string);
    void show();
    void save();
};

int Inventory::getCount() {
    return count;
}

long long Inventory::account(int stor) {
    long long acc=0;
    for(int i=0; i<count; i++) {
        if (inv[i].storage == stor)
            acc+=inv[i].num*inv[i].price;
    }
    return acc;
}

void Inventory::add(Product p) {
    inv[count++]=p;
    if (count == SIZE) {
        //call grow()!!!
    }
}

bool Inventory::del(int del_stor, string del_name) {
    int del_num = find(del_stor, del_name);
    if (del_num != -1) {
        inv[del_num].storage = -2;
        return true;
    }
    else
        return false;
}

int Inventory::find(int stor, string name) {
    for (int i=0; i<count; i++) {
        if ((inv[i].storage == stor) && (inv[i].name == name))
            return i;
    }
    return -1;
}

void Inventory::find_user(int find_stor, string find_name) {
    Inventory find_inv;
    for(int i=0;i<count;i++) {
        int stor=inv[i].storage;
        string name=inv[i].name;
        if (stor != -2 && (stor == find_stor || find_stor == -1) && ((name == find_name) || (find_name == "*")))
            find_inv.add(inv[i]);
    }
    find_inv.show();
}

void Inventory::show() {
    if (count==0) {
        cout << "Ничего не найдено!" << endl;
        return;
    }
    for(int i=0; i<count; i++) {
        if (inv[i].storage != -2)
            cout << inv[i].storage << "  " << inv[i].name << "  " << inv[i].price << "  " << inv[i].num << endl;
    }
}

void Inventory::save() {
    ofstream fout(db_name);
    for(int i=0; i<count; i++) {
        fout << inv[i].storage << "  " << inv[i].name << "  " << inv[i].price << "  " << inv[i].num << endl;
    }
    count=0;
}

int main(int argc, char** argv)
{

    setlocale(LC_ALL, "Russian");
    Inventory inv;
    int choice;
    char s[20];
    do
    {
        cout << endl;
        if (db_isopened) {
            cout << db_name << " открыта!" << endl;
            cout << "1 - Добавление записи" << endl;
            cout << "2 - Удаление записи" << endl;
            cout << "3 - Показать все записи" << endl;
            cout << "4 - Поиск" << endl;
            cout << "5 - Стоимость товаров на складе" << endl;
        }
        else
            cout << "1 - Открытие базы данных" << endl;
        cout << "0 - Выход" << endl;
        cout << endl;

        cin >> s;
        choice = static_cast<int>(s[0]) - 48;
        switch(choice)
        {
            case 1:
                if (!db_isopened) {
                    cout << "Введите путь к базе данных: " << endl;
                    cin >> db_name;
                    ifstream fin(db_name);
                    if (!fin.is_open()) {
                        cout << "Нет такой базы данных" << endl;
                        break;
                    }
                    Product p;
                    while (fin >> p.storage >> p.name >> p.price >> p.num) {
                        inv.add(p);
                    }
                    if (fin.bad()) {
                        std::cout << "I/O error\n";  // Пока файл читали, с ним что-то случилось.
                        return 1;
                    }
                    if (!fin.eof()) {
                        std::cout << "Invalid data\n";  // Не удалось распарсить число.
                        return 1;
                    }
                    fin.close();
                    db_isopened = true;
                }
                else {
                    Product p;
                    cout << "Добавление записи: " << endl;
                    cout << "1) Введите номер склада: " << endl;
                    cin >> p.storage;
                    cout << "2) Название товара: " << endl;
                    cin >> p.name;
                    cout << "3) Цена: " << endl;
                    cin >> p.price;
                    cout << "4) Кол-во: " << endl;
                    cin >> p.num;
                    inv.add(p);
                    cout << "Запись успешно добавлена!" << endl;
                }
                break;
            case 2:
                if (db_isopened) {
                    int del_storage;
                    string del_name;
                    cout << "Удаление записи: " << endl;
                    cout << "1) Введите номер склада: " << endl;
                    cin >> del_storage;
                    cout << "2) Название товара: " << endl;
                    cin >> del_name;
                    if (inv.del(del_storage, del_name))
                        cout << "Запись успешно удалена!" << endl;
                    else
                        cout << "Запись не найдена!" << endl;
                }
                break;
            case 3:
                if (db_isopened) {
                    cout << db_name << " содержит следующие " << inv.getCount() << " записи: " << endl;
                    inv.show();
                }
                break;
            case 4:
                if (db_isopened) {
                    int find_storage;
                    string find_name;
                    cout << "Поиск: " << endl;
                    cout << "(для полного поиска используйте '-1' для номера склада или '*' для названия )" << endl;
                    cout << "1) Номер склада: " << endl;
                    cin >> find_storage;
                    cout << "2) Название товара: " << endl;
                    cin >> find_name;
                    inv.find_user(find_storage, find_name);
                }
                break;
            case 5:
                if (db_isopened) {
                    long long storage;
                    cout << "Введите номер склада: " << endl;
                    cin >> storage;
                    cout << "Стоимость товаров на складе " << storage << " равна " << inv.account(storage) << endl;
                }
                break;
            case 0:
                if (db_isopened) {
                    inv.save();
                    db_isopened = false;
                }
                else {
                    cout << "Good Bye!" << endl;
                    return 0;
                }
                break;
            default: break;
        }
    }while(true);

    //_getch();
    return 0;
}


