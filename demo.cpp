#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    const int pin;
    string name;

    // Constructor to initialize const variable
    Student(int p) : pin(p) {}

    void read() {
        cout << "Enter the name: ";
        cin >> name;
    }

    void display() {
        cout << "Pin: " << pin << ", Name: " << name << endl;
    }
};

int main() {
    int pinNumber;
    cout << "Enter the pin number: ";
    cin >> pinNumber;

    Student obj(pinNumber);
    obj.read();
    obj.display();

    return 0;
}
