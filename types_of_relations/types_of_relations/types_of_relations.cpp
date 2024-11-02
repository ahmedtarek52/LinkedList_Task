#include <iostream>
#include <string>
using namespace std;

class Book {
public:
    Book() {};
    string title;

    Book(string title) : title(title) {}
    virtual void show() {
        cout << "Book Title: " << title << "\n";
    }
};

class Funbooks : public Book {
public:
    Funbooks() {};
    Funbooks(string title) : Book(title) {}
    void download() {
        cout << "Downloading Book: " << title << "\n";
    }
};

// Aggregation
class Author {
public:
    Author() {};
    string name;
    Author(string name) : name(name) {}
    void showAuthor() {
        cout << "Author: " << name << "\n";
    }
};

// Association
class Reader {
public:
    Reader() {};
    string name;
    int age;
    Reader(string name, int age) : name(name), age(age) {}
    void Readerdetails() {
        cout << "Readerdetails: " << name << " , " << "his age is: " << age << "\n";
    }
};

// Composition
class Library {
public:
    Library() {};
    string location;
    Library(string location) : location(location) {}
    void Librarylocation() {
        cout << "Library location: " << location << "\n";
    }
    void storeBook(Book& book) {
        cout << "The book \"" << book.title << "\" is stored at " << location << " library.\n";
    }
};

int main() {
    Book* ptr;
    Funbooks fun("bobby");
    ptr = &fun;
    //ptr->show();
    fun.download();

    Author author("mohamed");
    author.showAuthor();

    Reader reader("ahmed", 12);
    reader.Readerdetails();


    Library lib("Alex");
    lib.Librarylocation();
    lib.storeBook(fun);

    return 0;
}
