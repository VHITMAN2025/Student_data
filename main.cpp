#define STUDENT_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#define Student_H

using namespace std;

class excep : public exception {
public:
    const char* what() const noexcept override {
        return "Student Number Not found\n";
    }
};

class Student {
public:
    int pin;
    string name, course;
    bool presence = true;

    void read() {
        cout << "Student Registration Number: ";
        cin >> pin;
        cout << "Student Name: ";
        cin.ignore();  // To handle the leftover newline character
        getline(cin, name);  // Read the name from input
        cout << "Course of the Student: ";
        getline(cin, course);  // Read the course from input
    }

    void save() {
        ofstream data("Student_Data.txt", ios::app);  // Open file in append mode
        if (data.is_open()) {
            data << pin << " " << name << " " << course << endl;
            data.close();
            cout << "Saved Successfully" << endl;
        } else {
            cout << "Unable to save" << endl;
        }
    }

    void display() {
        ifstream data_read("Student_Data.txt");
        string line;
        if (data_read.is_open()) {
            while (getline(data_read, line)) {
                cout << line << endl;
            }
            data_read.close();
        } else {
            cout << "Unable to Retrieve" << endl;
        }
    }

    void display_Specific(int search_pin) {
        ifstream Data_spec("Student_Data.txt");
        string line;
        bool found = false;
        if (Data_spec.is_open()) {
            while (getline(Data_spec, line)) {
                istringstream iss(line);
                int file_pin;
                string file_name, file_course;
                if (iss >> file_pin >> file_name >> file_course) {
                    if (file_pin == search_pin) {
                        cout << "Pin no: " << file_pin << endl;
                        cout << "Student Name: " << file_name << endl;
                        cout << "Course: " << file_course << endl;
                        pin = file_pin;
                        name = file_name;
                        course = file_course;
                        found = true;
                        presence = true;
                        return;  // Exit once found
                    }
                }
            }
            Data_spec.close();
            if (!found) {
                throw excep();  // Throw exception if not found
            }
        } else {
            throw excep();  // Throw exception if unable to retrieve
        }
    }

    void _course(string find_course) {
        ifstream cour("Student_Data.txt");
        string line;
        bool found = false;
        if (cour.is_open()) {
            while (getline(cour, line)) {
                istringstream iss(line);
                int file_pin;
                string file_name, file_course;
                if (iss >> file_pin >> file_name >> file_course) {
                    if (file_course == find_course) {
                        cout << "Pin no: " << file_pin << ", Name: " << file_name << ", Course: " << file_course << endl;
                        found = true;
                    }
                }
            }
            cour.close();
            if (!found) {
                cout << "No students found enrolled in " << find_course << "." << endl;
            }
        } else {
            cout << "Unable to Retrieve" << endl;
        }
    }
};

class Marks_Grades : public Student {
public:
    using Student::Student;  // Inherit the constructor
    int marks;
    char grade;

    void Grades_cal() {
        int marks_pin;
        cout << "Student Pin: ";
        cin >> marks_pin;
        cout << "Enter the marks of the student: ";
        cin >> marks;
        if (marks > 90) {
            grade = 'A';
        } else if (marks > 70) {
            grade = 'B';
        } else if (marks > 50) {
            grade = 'C';
        } else {
            grade = 'D';
        }
        try {
            display_Specific(marks_pin);  // Check if the student exists
            ofstream add_grade("Student_marks.txt", ios::app);
            if (add_grade.is_open()) {
                add_grade << "Pin: " << pin << " Name: " << name << " Grade: " << grade << endl;
                add_grade.close();
                cout << "Grade Saved Successfully" << endl;
            } else {
                cout << "Unable to upload Grade" << endl;
            }
        } catch (const excep& e) {
            cout << e.what();
        }
    }
};

class Attendance : public Student {
public:
    vector<char> attendance_list;
    char attendance;
    int attended_pin;
    float attendance_percentage;

    void attend_cal() {
        cout << "Enter Student pin number: ";
        cin >> attended_pin;
        display_Specific(attended_pin);
        cout << "Attendance of student (P for Present, A for Absent): ";
        cin >> attendance;
        attendance_list.push_back(attendance);
        ofstream attend("Student_attendance.txt", ios::app);
        if (attend.is_open()) {
            for (char att : attendance_list) {
                attend << "Pin: " << pin << " Attendance: " << att << endl;
            }
        }
        attend.close();
    }

    void view_attendance(int at_pin) {
        int total_days=0;
        ifstream attendance_sheet("Student_attendance.txt");
        if (attendance_sheet.is_open()) {
            string line;
            int total_attendance = 0;
            while (getline(attendance_sheet, line)) {
                int attendance_pin;
                char File_attendance;
                istringstream iss2(line);
                string temp;
                if (iss2 >> temp >> attendance_pin >> temp >> File_attendance) {
                    total_days++;
                    if (attendance_pin == at_pin && File_attendance == 'P') {
                        total_attendance++;
                    }
                   
                }
            }
             attendance_percentage = (total_attendance/100)*100;
            attendance_sheet.close();
            double attendance_percentage = (static_cast<double>(total_attendance) / total_days)*100;
            cout << "Total Attendance for pin " << at_pin << ": " << total_attendance << endl;
            cout<<"Attendance Percentage: "<<attendance_percentage<<endl;
        } else {
            cout << "Unable to open attendance file!" << endl;
        }
    }
};

int main() {
    Marks_Grades m;
    Attendance at;
    int choice;
    int at_pin;
    string find_course;

    do {
        cout << "Menu" << endl;
        cout << "1. Student Registration\n2. View Students List\n3. Group by Branch\n4. Search Student\n5. Grading Student\n6. Attendance Report\n7. View Attendance\n8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                m.read();
                m.save();
                break;
            case 2:
                m.display();
                break;
            case 3:
                cout << "Enter Course to List the Students: ";
                cin.ignore();  // Handle leftover newline
                getline(cin, find_course);
                m._course(find_course);
                break;
            case 4: {
                int searchPin;
                cout << "Enter pin number to search: ";
                cin >> searchPin;
                try {
                    m.display_Specific(searchPin);
                } catch (const excep& e) {
                    cout << e.what();
                }
                break;
            }
            case 5:
                m.Grades_cal();

                break;
            case 6:
               at.attend_cal();
               break;
            case 7:
              cout<<"Student pin: ";
               cin>>at_pin;
               at.view_attendance(at_pin);
               break;
            case 8:
                cout << "Exiting..." << endl;
                break;
            
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
