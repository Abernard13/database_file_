#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Declare variables for accessing the file and filename
fstream file;
string filename = "data.txt";

// Declare a struct to save the data
struct Person {
    string id, firstName, lastName;
    float income;
};

// Function to search for an employee in the file
struct Person* search(string id) {
    struct Person* p2 = new Person;
    char empId[6], firstName[16], lastName[16], incomeStr[11];
    
    file.clear();
    file.seekg(0, ios::beg);

    while (file.read(empId, 5)) {
        empId[5] = '\0';
        p2->id = empId;

        if (p2->id == id) {
            // Reading rest of data
            file.read(firstName, 15);
            file.read(lastName, 15);
            file.read(incomeStr, 10);

            // Null-terminate strings
            firstName[15] = '\0';
            lastName[15] = '\0';
            incomeStr[10] = '\0';

            // Save to structure Person
            p2->firstName = firstName;
            p2->lastName = lastName;
            p2->income = stof(incomeStr); // convert to float

            return p2; // return pointer to Person struct
        }
        file.seekg(40, ios::cur); // Move to the next record
    }
    return nullptr;
}

// Function to get an employee's information
void get_employee() {
    string empId;
    
    // Ask the user to enter the employee id
    cout << "Enter employee ID: ";
    cin >> empId;

    // Call search function
    struct Person* p = search(empId);

    // Check if employee exists
    if (p == nullptr) {
        cout << "Employee not found." << endl;
    } else {
        // Display employee details
        cout << "Employee Found!" << endl;
        cout << "ID: " << p->id << endl;
        cout << "First Name: " << p->firstName << endl;
        cout << "Last Name: " << p->lastName << endl;
        cout << "Income: " << p->income << endl;
    }
}

// Function to list all employees
void list() {
    char id[6], firstName[16], lastName[16], incomeStr[11];
    
    file.clear();
    file.seekg(0, ios::beg); // rewind file to beginning

    cout << "\nList of Employees:\n";
    while (file.read(id, 5)) {
        // Read rest of data
        file.read(firstName, 15);
        file.read(lastName, 15);
        file.read(incomeStr, 10);

        // Null-terminate strings
        id[5] = '\0';
        firstName[15] = '\0';
        lastName[15] = '\0';
        incomeStr[10] = '\0';

        // Convert income string to float
        float income = atof(incomeStr);

        // Display employee record
        cout << id << " " << firstName << " " << lastName << " " << income << endl;
    }
}

// Function to write data to file
void write_data(Person p) {
    // Move to the end of file for writing
    file.clear();
    file.seekp(0, ios::end);

    // Write data to file
    file.write(p.id.c_str(), 5);
    file.write(p.firstName.c_str(), 15);
    file.write(p.lastName.c_str(), 15);

    // Format income as 10 characters
    string incomeStr = to_string(p.income);
    while (incomeStr.length() < 10) {
        incomeStr = " " + incomeStr;
    }
    file.write(incomeStr.c_str(), 10);
    file.flush();
}

// Function to add a new employee
void new_employee() {
    string empId;
    while (true) {
        cout << "\n*** New Employee ***\n";
        cout << "Enter employee ID (-1 to end): ";
        cin >> empId;

        if (empId == "-1") break; // Exit if user enters -1

        // Check if employee already exists
        if (search(empId) != nullptr) {
            cout << "Employee with ID " << empId << " already exists!\n";
            continue;
        }

        // Enter employee details
        Person p;
        p.id = empId;
        cout << "Enter first name: ";
        cin >> p.firstName;
        cout << "Enter last name: ";
        cin >> p.lastName;
        cout << "Enter income: ";
        cin >> p.income;

        // Save data to file
        cout << "Saving data...\n";
        write_data(p);
    }
}

// Main function
int main() {
    file.open(filename, ios::in | ios::out | ios::app);

    // If file doesn't exist, create it
    if (!file) {
        file.open(filename, ios::out);
        file.close();
        file.open(filename, ios::in | ios::out | ios::app);
    }

    // Menu
    int option = -1;
    while (true) {
        cout << "\n*** Menu ***" << endl;
        cout << "1) New employee\n";
        cout << "2) Search employee information\n";
        cout << "3) List employees\n";
        cout << "4) Exit\n";
        cout << "Option: ";
        cin >> option;

        switch (option) {
            case 1: new_employee(); break;
            case 2: get_employee(); break;
            case 3: list(); break;
            case 4: 
                file.close();
                return 0;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }
}
