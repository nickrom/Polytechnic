#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

bool db_isopened = 0;
string db_name = "";

struct Dog
{
    long id;
    string nickname;
    string breed;
    string owner;
    long father_id;
    long mother_id;
    Dog() {
        id = -1;
    }
    Dog(long id, const string &nickname, const string &breed, const string &owner, long father_id,
        long mother_id) :
            id(id), nickname(nickname), breed(breed), owner(owner), father_id(father_id), mother_id(mother_id) { }
};

class Kennel {
private:
    int count;
    vector<struct Dog> kennel;
public:
    Kennel() {
        count = 0;
    }

    struct Dog get(int id) {
        for (struct Dog dog : kennel) {
            if (dog.id == id)
                return dog;
        }
        return Dog();
    }

    int getCount() {
        return count;
    }

    bool unique_id(long id) {
        for (struct Dog dog : kennel) {
            if (dog.id == id)
                return false;
        }
        return true;
    }

    bool put(struct Dog dog) {
        if (unique_id(dog.id)) {
            kennel.push_back(dog);
            count++;
            return true;
        }
        return false;
    }

    bool del(long id) {
        int i=0;
        for (auto it = kennel.begin(); it != kennel.end() ; it++, i++)
            if (kennel.at(i).id == id) {
                kennel.erase(it);
                count--;
                return true;
            }
        return false;
    }

    vector<struct Dog> get_owner_dogs(string owner) {
        vector<struct Dog> dogs;
        for (struct Dog dog : kennel) {
            if (dog.owner == owner)
                dogs.push_back(dog);
        }
        return dogs;
    }

    void print_all() {
        cout << "ID\tКличка\tПорода\tХозяин\tID отца\tID матери" << endl;
        for (struct Dog dog : kennel) {
            print(dog);
        }
    }

    void print_all(vector<struct Dog> dogs) {
        cout << "ID\tКличка\tПорода\tХозяин\tID отца\tID матери" << endl;
        for (struct Dog dog : dogs) {
            print(dog);
        }
    }

    void print_all(queue<struct Dog> dogs) {
        cout << "ID\tКличка\tПорода\tХозяин\tID отца\tID матери" << endl;
        cout << "Dog\t";
        print(dogs.front());
        dogs.pop();
        cout << "\tFather\t";
        print(dogs.front());
        dogs.pop();
        cout << "\tMother\t";
        print(dogs.front());
        dogs.pop();
        cout << "\t\tGrandFather\t";
        print(dogs.front());
        dogs.pop();
        cout << "\t\tGrandMother\t";
        print(dogs.front());
        dogs.pop();
        cout << "\t\tGrandFather\t";
        print(dogs.front());
        dogs.pop();
        cout << "\t\tGrandMother\t";
        print(dogs.front());
        dogs.pop();
    }

    void print(struct Dog dog) {
        if (-1 == dog.id) {
            cout << " NO DOG IN DATABASE" << endl;
            return;
        }
        cout << dog.id << "\t" << dog.nickname << "\t" << dog.breed << "\t" << dog.owner << "\t" << dog.father_id << "\t\t" <<
        dog.mother_id << "\t" << endl;
    }

    queue<struct Dog> pedigree(long id) {
        queue<struct Dog> family;
        queue<long> id_dogs;
        struct Dog dog = get(id);
        family.push(dog);
        for (int i=0; i<3; i++) {
            struct Dog f_dog, m_dog;
            if (i!=0) {
                dog = get(id_dogs.front());
                id_dogs.pop();
            }
            f_dog = get(dog.father_id);
            id_dogs.push(f_dog.id);
            family.push(f_dog);
            m_dog = get(dog.mother_id);
            id_dogs.push(m_dog.id);
            family.push(m_dog);
        }
        return family;
    }
    void save() {
        ofstream fout(db_name);
        for(int i=0; i<count; i++) {
            fout << kennel.at(i).id << "  " << kennel.at(i).nickname << "  " << kennel.at(i).breed << "  " <<
                    kennel.at(i).owner << "  " << kennel.at(i).father_id << "  " << kennel.at(i).mother_id;
        }
        count=0;
        kennel.clear();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Kennel ken;
    int choice;
    char s[20];
    do
    {
        cout << endl;
        if (db_isopened) {
            cout << db_name << " открыта!" << endl;
            cout << "1 - Добавление записи" << endl;
            cout << "2 - Удаление записи" << endl;
            cout << "3 - Данные по всем собакам" << endl;
            cout << "4 - Все собаки выбранного хозяина" << endl;
            cout << "5 - Родославная выбранной собаки" << endl;
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
                    Dog d;
                    while (fin >> d.id >> d.nickname >> d.breed >> d.owner >> d.father_id >> d.mother_id) {
                        ken.put(d);
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
                    Dog d;
                    cout << "Добавление записи: " << endl;
                    cout << "ID: " << endl;
                    cin >> d.id;
                    cout << "Кличка:" << endl;
                    cin >> d.nickname;
                    cout << "Порода:" << endl;
                    cin >> d.breed;
                    cout << "Хозяин:" << endl;
                    cin >> d.owner;
                    cout << "ID отца:" << endl;
                    cin >> d.father_id;
                    cout << "ID матери:" << endl;
                    cin >> d.mother_id;
                    if (ken.put(d))
                        cout << "Запись успешно добавлена!" << endl;
                    else
                        cout << "Собака с таким ID уже сущемствует" << endl;
                }
                break;
            case 2:
                if (db_isopened) {
                    int del_id;
                    cout << "Удаление записи: " << endl;
                    cout << "ID собаки: " << endl;
                    cin >> del_id;
                    if (ken.del(del_id))
                        cout << "Запись успешно удалена!" << endl;
                    else
                        cout << "Запись не найдена!" << endl;
                }
                break;
            case 3:
                if (db_isopened) {
                    cout << db_name << " содержит следующие " << ken.getCount() << " записи: " << endl;
                    ken.print_all();
                }
                break;
            case 4:
                if (db_isopened) {
                    string owner;
                    cout << "Введите хозяина: " << endl;
                    cin >> owner;
                    ken.print_all(ken.get_owner_dogs(owner));
                }
                break;
            case 5:
                if (db_isopened) {
                    long long ID;
                    cout << "Введите ID собаки: " << endl;
                    cin >> ID;
                    ken.print_all(ken.pedigree(ID));
                }
                break;
            case 0:
                if (db_isopened) {
                    ken.save();
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
    return 0;
}