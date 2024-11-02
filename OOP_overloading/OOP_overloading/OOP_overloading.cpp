#include <iostream>
using namespace std;

class vector3 {
public:
    float x, y, z;

    vector3(float xVal = 0, float yVal = 0, float zVal = 0) : x(xVal), y(yVal), z(zVal) {}

    vector3(const vector3& original) : x(original.x), y(original.y), z(original.z) {}


    vector3& operator++() {
        ++x;
        ++y;
        ++z;
        return *this;
    }

   
    vector3 operator++(int) {
        vector3 temp(*this); 
        x++;
        y++;
        z++;
        return temp; 
    }


    bool operator==(const vector3& right) const {
        return (x == right.x && y == right.y && z == right.z);
    }

    
    bool operator!=(const vector3& right) const {
        return !(*this == right);
    }

    
    vector3 operator+(const vector3& right) const {
        return vector3(x + right.x, y + right.y, z + right.z);
    }

    vector3 operator-(const vector3& right) const {
        return vector3(x - right.x, y - right.y, z - right.z);
    }

    
    vector3& operator=(const vector3& right) {
        if (this != &right) { 
            x = right.x;
            y = right.y;
            z = right.z;
        }
        return *this;
    }

    explicit operator float() const {
        return x;
    }


    void print() const {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
};

int main() {
    vector3 vec1(1, 2, 3);
    vector3 vec2(4, 5, 6);

    cout << "Initial vec1: ";
    vec1.print();


    cout << "Using post-increment vec1++: ";
    vector3 tempVec1 = vec1++;
    tempVec1.print(); 

    cout << "After post-increment, vec1: ";
    vec1.print(); 


    cout << "Using pre-increment ++vec1: ";
    ++vec1;
    vec1.print(); // Should print (3, 4, 5)


    cout << "vec1 == vec2: " << (vec1 == vec2 ? "true" : "false") << endl;
    cout << "vec1 != vec2: " << (vec1 != vec2 ? "true" : "false") << endl;


    vector3 resultAdd = vec1 + vec2;
    cout << "Addition result: ";
    resultAdd.print();

    vector3 resultSub = vec2 - vec1;
    cout << "Subtraction result: ";
    resultSub.print();


    vec1 = vec2;
    cout << "After assignment (vec1 = vec2), vec1: ";
    vec1.print();


    cout << "Casting as float: " << static_cast<float>(vec1) << endl;

    return 0;
}
