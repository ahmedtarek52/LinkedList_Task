#include <iostream>
using namespace std;

struct Student {
    string name;
    int id;
    Student* prev = NULL;
    Student* left = NULL;
    Student* right = NULL;
};

struct Queue {
    Student* first = NULL;
    Student* last = NULL;

    void enqueue(Student* newNode) {
        if (first == NULL) {
            first = last = newNode;
        }
        else {
            last->prev = newNode;
            last = newNode;
        }
    }

    Student* dequeue() {
        if (first == NULL) {
            cout << "Queue is empty." << endl;
            return NULL;
        }
        Student* temp = first;
        first = first->prev;
        if (first == NULL) {
            last = NULL;
        }
        return temp;
    }

    bool findInQueue(const string& name) {
        Student* current = first;
        while (current != NULL) {
            if (current->name == name) {
                cout << "Student " << name << " found in queue." << endl;
                return true;
            }
            current = current->prev;
        }
        cout << "Student " << name << " not found in queue." << endl;
        return false;
    }

    void enqueueAtIndex(Student* newNode, int index) {
        if (index == 0) {
            newNode->prev = first;
            first = newNode;
            if (last == NULL) 
                last = newNode;
            return;
        }

        Student* current = first;
        int count = 0;
        while (current != NULL && count < index - 1) {
            current = current->prev;
            count++;
        }

        if (current == NULL) {
            cout << "Index out of bounds. Adding at the end." << endl;
            enqueue(newNode);
        }
        else {
            newNode->prev = current->prev;
            current->prev = newNode;
            if (newNode->prev == NULL) last = newNode;
        }
    }


    void printQueue() {
        Student* current = first;
        while (current != NULL) {
            cout << "Name: " << current->name  << endl;
            current = current->prev;
        }
    }
};



struct StudentTree {
    Student* root = NULL;

    void insert(Student* newStudent) {
        if (root == NULL) {
            root = newStudent;
            return;
        }

        Student* current = root;
        Student* parent = NULL;

        while (current != NULL) {
            parent = current;
            if (newStudent->id < current->id) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (newStudent->id < parent->id) {
            parent->left = newStudent;
        }
        else {
            parent->right = newStudent;
        }
    }
    Student* find(int id) {
        Student* current = root;
        while (current != NULL) {
            if (id == current->id) {
                return current;
            }
            else if (id < current->id) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return NULL; 
    }

    void printTree(Student* node) {
        if (node == NULL) return;
        printTree(node->left);
        cout << "ID: " << node->id <<" " << "Name: " << node->name << '\n';
        printTree(node->right);
    }





};



int main() {
    Student s1{"first",1};
    Student s2{"second",2};
    Student s3{"third",3};
    Student s4{"fourth",4};

    Queue q;
    q.enqueue(&s1);
    q.enqueue(&s2);
    q.enqueue(&s3);

    cout << "Initial queue:" << '\n';
    q.printQueue();

    cout << "\n";
    q.findInQueue("second");
    q.findInQueue("fifth");


    q.enqueueAtIndex(&s4, 1);
    cout << "\nQueue after EnqueueAtIndex:" << '\n';
    q.printQueue();


    cout << "\nDequeuing all elements:" << '\n';
    for (int i = 0; i < 5; i++) {
        Student* temp = q.dequeue();
        if (temp != NULL) {
            cout << "Dequeued: " << temp->name << '\n';
        }
    }

    StudentTree tree;

    tree.insert(&s1);
    tree.insert(&s2);
    tree.insert(&s3);

    cout << " \n Inorder Tree:" << '\n';
    tree.printTree(tree.root);


    int searchId = 2;
    Student* foundStudent = tree.find(searchId);
    if (foundStudent != NULL) {
        cout << "\n tudent with ID " << foundStudent->id << ", Name: " << foundStudent->name << endl;
    }
    else {
        cout << "\n Student with ID " << searchId << " not found." << endl;
    }




    return 0;


}
