#include <iostream>
#include <string>
using namespace std;

struct Employee {
    int id;
    string name;
    Employee* next = nullptr;
    Employee* prev = nullptr;

    void printEmployee() {
        cout << "id: " << id << ", name: " << name << '\n';
    }

    void setElements(int id, string name) {
        this->id = id;
        this->name = name;
    }
};

struct LinkedList {
    Employee* first = nullptr;
    Employee* lastInserted = nullptr;

    void pushBack(Employee* employee) {
        if (first == nullptr) {
            first = employee;
            lastInserted = employee;
        }
        else {
            lastInserted->next = employee;
            employee->prev = lastInserted;
            lastInserted = employee;
        }
    }

    void deleteEmployee(Employee* employee) {
        if (employee == nullptr) return;

        if (employee->prev != nullptr) {
            employee->prev->next = employee->next;
        }
        else {
            first = employee->next;
        }

        if (employee->next != nullptr) {
            employee->next->prev = employee->prev;
        }
        else {
            lastInserted = employee->prev;
        }

        delete employee;
    }


    Employee* findEmployeeById(int id) {
        Employee* current = first;
        while (current != nullptr) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void replaceEmployee(int id, string newName) {
        Employee* employee = findEmployeeById(id);
        if (employee != nullptr) {
            employee->name = newName;
        }
    }

    void printList() {
        Employee* current = first;
        while (current != nullptr) {
            current->printEmployee();
            current = current->next;
        }
    }

    Employee* merge(Employee* first, Employee* second) {
        if (!first) return second;
        if (!second) return first;

        if (first->id <= second->id) {
            first->next = merge(first->next, second);
            first->next->prev = first;
            first->prev = nullptr;
            return first;
        }
        else {
            second->next = merge(first, second->next);
            second->next->prev = second;
            second->prev = nullptr;
            return second;
        }
    }

    Employee* split(Employee* head) {
        Employee* fast = head->next;
        Employee* slow = head;

        while (fast != nullptr && fast->next != nullptr) {
            fast = fast->next->next;
            slow = slow->next;
        }

        Employee* temp = slow->next;
        slow->next = nullptr;
        return temp;
    }

    Employee* mergeSort(Employee* head) {
        if (!head || !head->next) return head;

        Employee* second = split(head);

        head = mergeSort(head);
        second = mergeSort(second);

        return merge(head, second);
    }

    void sortByID() {
        if (!first || !first->next) return;
        first = mergeSort(first);
    }
};

int main() {
    LinkedList empList;

    Employee* emp1 = new Employee;
    Employee* emp2 = new Employee;
    Employee* emp3 = new Employee;

    emp1->setElements(3, "Ahmed");
    emp2->setElements(1, "tarek");
    emp3->setElements(2, "mohamed");

    empList.pushBack(emp1);
    empList.pushBack(emp2);
    empList.pushBack(emp3);

    cout << "Original list:\n";
    empList.printList();

    cout << "\nFind employee with ID 2:\n";
    Employee* found = empList.findEmployeeById(2);
    if (found) found->printEmployee();

    cout << "\nReplace employee with ID 1:\n";
    empList.replaceEmployee(1, "ali");
    empList.printList();

    cout << "\nSort by ID:\n";
    empList.sortByID();
    empList.printList();

    cout << "\nDelete employee with ID 2:\n";
    Employee* toDelete = empList.findEmployeeById(2);
    if (toDelete) empList.deleteEmployee(toDelete);
    empList.printList();

    // Clean up remaining memory
    while (empList.first != nullptr) {
        Employee* temp = empList.first;
        empList.first = empList.first->next;
        delete temp;
    }

    return 0;
}