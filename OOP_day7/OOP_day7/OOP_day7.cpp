#include <iostream>
#include <vector>


class GeometricShape {
protected:
    int width; 
    int hight;
public:

    GeometricShape(int width, int hight) {
        this->width = width;
        this->hight = hight;
    }
   virtual float CalcArea() {
       return width * hight;
    }
};

class Circle : public GeometricShape {
private:
    float r;
    float bay;

public:
    Circle(float r, float bay) : GeometricShape(0, 0) {
        this->r = r;
        this->bay = bay;
    }

    float CalcArea() override {
        return bay * r * r;
    }
};

class Triangle :public GeometricShape {
public:
    Triangle(int width, int hight) : GeometricShape(width, hight) {}
    float CalcArea() {
        return  width * hight *.5;
    }    

};

class Rectangle :public GeometricShape {
public:

    Rectangle(int width, int hight) : GeometricShape(width, hight) {}
    float CalcArea() {
        return  (width + hight) * 2;
    }

};

class Square :public GeometricShape {
public:
    Square(int width, int hight) : GeometricShape(width, hight) {}
    float CalcArea() {
        return  width * hight;
    }
};


//float CalcArea(GeometricShape * GeometricShape) {
//   return  GeometricShape->CalcArea();
//}




float AverageArea(std::vector<GeometricShape*> shapes) {
    float totalArea = 0;
    for (GeometricShape* shape : shapes) {
        totalArea += shape->CalcArea();
    }
    return totalArea / shapes.size();
}





int main()
{

    //Triangle t4(4 , 5);
    //std::cout << CalcArea(&t4)<<'\n';

    //Circle c4(7, 3.14);
    //std::cout << CalcArea(&c4)<<'\n';
    
    


    GeometricShape* shape;

    Circle c1(7 , 3.14);
    shape = &c1;
    std::cout << "Circle area: " << shape->CalcArea() << std::endl;

    Triangle t1(7, 8);
    shape = &t1;
    std::cout << "Triangle area: " << shape->CalcArea() << std::endl;

    Rectangle r1(7, 8);
    shape = &r1;
    std::cout << "Rectangle area: " << shape->CalcArea() << std::endl;

    Square s1(7, 8);
    shape = &s1;
    std::cout << "Square area: " << shape->CalcArea() << std::endl;


    std::vector<GeometricShape*> shapes;

    shapes.push_back(&c1);
    shapes.push_back(&t1);
    shapes.push_back(&r1);
    shapes.push_back(&s1);

    std::cout << "Average area of shapes: " << AverageArea(shapes) << '\n';
}






