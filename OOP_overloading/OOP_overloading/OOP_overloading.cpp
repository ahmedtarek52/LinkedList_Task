#include <iostream>


using namespace std;

class vector3 {
public:
    float x, y, z;

    
    //vector3() {};
    vector3(float xVal=0, float yval=0, float zVal=0) {
        x = xVal;
        y = yval;
        z = zVal;
    }

    vector3(const vector3& original) {
        x = original.x;
        y = original.y;
        z = original.z;
    }

 
    bool operator==( vector3& right) {
        return (x == right.x && y == right.y && z == right.z);
        
        //if(this->x ==right.x&& this->x == right.x&& this->x == right.x){
        //}
        //
    }


    bool operator!=(const vector3& right) const {
        return (x != right.x || y != right.y || z != right.z);

    }

    
  
    vector3 operator +(const vector3& right) {
        vector3 result ;
        result.x = this->x + right.x;
        result.y = this->y + right.y;
        result.z = this->z + right.z;
        //cout << "plus operator";
        return result;
    }

    vector3 operator -(const vector3& right) {
        vector3 result;
        result.x = this->x - right.x;
        result.y = this->y - right.y;
        result.z = this->z - right.z;
        //cout << "Subtraction operator";
        return result;

    }
   

    
    void  operator=(const vector3& right) {
      
            x = right.x;
            y = right.y;
            z = right.z;
        
        
    }

    
    explicit operator float()  {
        return x ;
    }

    
    void print() const {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
};

int main() {
    vector3 vec1(1, 2, 3);
    vector3 vec2(4, 5, 6);



    
    cout << "vec1 == vec2: " << (vec1 == vec2 ? "true" : "false") << endl;
    cout << "vec1 != vec2: " << (vec1 != vec2 ? "true" : "false") << endl;
    

    
    vector3 resultAdd = vec1 + vec2;
    cout << "Addition result: ";
    resultAdd.print();

    vector3 resultSub = vec2 - vec1;
    cout << "Subtraction result: ";
    resultSub.print();

    vec1 = vec2;
    cout << "Equal result: " << vec1.x << '\n';


   
    cout << (float)resultAdd;

    return 0;
}
