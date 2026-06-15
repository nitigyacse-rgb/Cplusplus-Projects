#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

// Data structure representing an individual Employee
class Employee
{
private:
    int id;
    std::string name;
    std::string role;

public:
    // Constructor
    Employee(int empId, std::string empName, std::string empRole)
        : id(empId), name(empName), role(empRole) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getRole() const { return role; }

    // Helper method to display a single employee row
    void displayRow() const
    {
        std::cout << std::left << std::setw(10) << id
                  << std::setw(25) << name
                  << std::setw(20) << role << "\n";
    }
};

// Class handling the Employee Management System functionalities
class EmployeeManagementSystem
{
private:
    std::vector<Employee> employeeTable;

    // Helper to find an employee index by ID
    int findEmployeeIndex(int id) const
    {
        for (size_t i = 0; i < employeeTable.size(); ++i)
        {
            if (employeeTable[i].getId() == id)
            {
                return i; // Found
            }
        }
        return -1; // Not found
    }

public:
    // 1. Built The Employee Table (Display all records)
    void displayEmployeeTable() const
    {
        if (employeeTable.empty())
        {
            std::cout << "\n--- Employee Table is Empty ---\n";
            return;
        }

        std::cout << "\n------------------ EMPLOYEE TABLE ------------------\n";
        std::cout << std::left << std::setw(10) << "ID"
                  << std::setw(25) << "Name"
                  << std::setw(20) << "Role" << "\n";
        std::cout << "----------------------------------------------------\n";

        for (const auto &emp : employeeTable)
        {
            emp.displayRow();
        }
        std::cout << "----------------------------------------------------\n";
    }

    // 2. Insert New Entries
    void insertEntry()
    {
        int id;
        std::string name, role;

        std::cout << "\nEnter Employee ID: ";
        while (!(std::cin >> id))
        {
            std::cout << "Invalid input. Please enter a numerical ID: ";
            std::cin.clear();
            std::cin.ignore(123, '\n');
        }
        std::cin.ignore(); // Clear newline character from buffer

        // Check if ID already exists
        if (findEmployeeIndex(id) != -1)
        {
            std::cout << "Error: An employee with ID " << id << " already exists!\n";
            return;
        }

        std::cout << "Enter Employee Name: ";
        std::getline(std::cin, name);

        std::cout << "Enter Employee Role: ";
        std::getline(std::cin, role);

        // Add to our table
        employeeTable.push_back(Employee(id, name, role));
        std::cout << "Employee record inserted successfully!\n";
    }

    // 3. Delete The Entries
    void deleteEntry()
    {
        if (employeeTable.empty())
        {
            std::cout << "\nNo records available to delete.\n";
            return;
        }

        int id;
        std::cout << "\nEnter the Employee ID to delete: ";
        std::cin >> id;

        int index = findEmployeeIndex(id);
        if (index != -1)
        {
            employeeTable.erase(employeeTable.begin() + index);
            std::cout << "Employee record with ID " << id << " deleted successfully.\n";
        }
        else
        {
            std::cout << "Employee with ID " << id << " not found.\n";
        }
    }

    // 4. Search A Record
    void searchRecord() const
    {
        if (employeeTable.empty())
        {
            std::cout << "\nNo records available to search.\n";
            return;
        }

        int id;
        std::cout << "\nEnter Employee ID to search: ";
        std::cin >> id;

        int index = findEmployeeIndex(id);
        if (index != -1)
        {
            std::cout << "\n--- Record Found ---\n";
            std::cout << std::left << std::setw(10) << "ID"
                      << std::setw(25) << "Name"
                      << std::setw(20) << "Role" << "\n";
            std::cout << "----------------------------------------------------\n";
            employeeTable[index].displayRow();
            std::cout << "----------------------------------------------------\n";
        }
        else
        {
            std::cout << "Employee with ID " << id << " not found.\n";
        }
    }
};

int main()
{
    EmployeeManagementSystem ems;
    int choice;

    do
    {
        std::cout << "\n===== EMPLOYEE MANAGEMENT SYSTEM =====\n";
        std::cout << "1. Built / Display Employee Table\n";
        std::cout << "2. Insert New Entries\n";
        std::cout << "3. Delete The Entries\n";
        std::cout << "4. Search A Record\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";

        if (!(std::cin >> choice))
        {
            std::cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            std::cin.clear();
            std::cin.ignore(123, '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            ems.displayEmployeeTable();
            break;
        case 2:
            ems.insertEntry();
            break;
        case 3:
            ems.deleteEntry();
            break;
        case 4:
            ems.searchRecord();
            break;
        case 5:
            std::cout << "\nExiting System. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid selection! Choose an option between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}