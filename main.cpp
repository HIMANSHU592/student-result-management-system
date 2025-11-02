#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>   
using namespace std;

class Student {
    int rollNo;
    string name;
    int p_marks, c_marks, m_marks, e_marks, cs_marks;
    double per;
    char grade;

public:
    void getData();
    void showData() const;
    void calculate();
    int getRollNo() const { return rollNo; }
};

void Student::calculate() {
    per = (p_marks + c_marks + m_marks + e_marks + cs_marks) / 5.0;
    if (per >= 90)
        grade = 'A';
    else if (per >= 75)
        grade = 'B';
    else if (per >= 50)
        grade = 'C';
    else
        grade = 'F';
}

void Student::getData() {
    cout << "\nEnter Roll Number: ";
    cin >> rollNo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter marks in Physics: ";
    cin >> p_marks;
    cout << "Enter marks in Chemistry: ";
    cin >> c_marks;
    cout << "Enter marks in Maths: ";
    cin >> m_marks;
    cout << "Enter marks in English: ";
    cin >> e_marks;
    cout << "Enter marks in Computer Science: ";
    cin >> cs_marks;

    calculate();
}

void Student::showData() const {
    cout << "\nRoll Number: " << rollNo;
    cout << "\nName: " << name;
    cout << "\nMarks (Phy, Chem, Math, Eng, CS): "
         << p_marks << ", " << c_marks << ", " << m_marks << ", " << e_marks << ", " << cs_marks;
    cout << "\nPercentage: " << per;
    cout << "\nGrade: " << grade << endl;
}

// --- File Handling Functions ---
void writeStudent();
void displayAll();
void searchStudent(int);
void deleteStudent(int);
void modifyStudent(int);

int main() {
    int choice;
    do {
        cout << "\n\n===== STUDENT RESULT MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Search Student by Roll No\n";
        cout << "4. Modify Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeStudent();
            break;
        case 2:
            displayAll();
            break;
        case 3: {
            int num;
            cout << "Enter Roll No to search: ";
            cin >> num;
            searchStudent(num);
            break;
        }
        case 4: {
            int num;
            cout << "Enter Roll No to modify: ";
            cin >> num;
            modifyStudent(num);
            break;
        }
        case 5: {
            int num;
            cout << "Enter Roll No to delete: ";
            cin >> num;
            deleteStudent(num);
            break;
        }
        case 6:
            cout << "\nExiting... Thank you!\n";
            break;
        default:
            cout << "Invalid choice!";
        }
    } while (choice != 6);
    return 0;
}

void writeStudent() {
    Student st;
    ofstream outFile;
    outFile.open("student.dat", ios::binary | ios::app);
    st.getData();
    outFile.write(reinterpret_cast<char*>(&st), sizeof(Student));
    outFile.close();
    cout << "\nStudent record added successfully!\n";
}

void displayAll() {
    Student st;
    ifstream inFile("student.dat", ios::binary);
    if (!inFile) {
        cout << "File not found!";
        return;
    }
    cout << "\n===== ALL STUDENT RECORDS =====\n";
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        st.showData();
        cout << "---------------------------------\n";
    }
    inFile.close();
}

void searchStudent(int n) {
    Student st;
    ifstream inFile("student.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (st.getRollNo() == n) {
            st.showData();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\nRecord not found!\n";
}

void modifyStudent(int n) {
    Student st;
    fstream file("student.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof() && file.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (st.getRollNo() == n) {
            cout << "\nEnter new details:\n";
            st.getData();
            int pos = (-1) * static_cast<int>(sizeof(st));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&st), sizeof(Student));
            cout << "\nRecord Updated Successfully!\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found)
        cout << "\nRecord not found!\n";
}

void deleteStudent(int n) {
    Student st;
    ifstream inFile("student.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (st.getRollNo() != n)
            outFile.write(reinterpret_cast<char*>(&st), sizeof(Student));
    }
    inFile.close();
    outFile.close();
    remove("student.dat");
    rename("temp.dat", "student.dat");
    cout << "\nRecord Deleted Successfully!\n";
}
