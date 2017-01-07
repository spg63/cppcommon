#include "../../Point3D.hpp"
#include <iostream>
#include <vector>

int main(){
    //----- C'tors and setting points
    Point3D p1;
    p1.x(5);
    p1.y(10);
    p1.z(15);

    Point3D p2(5.0, 5.1, 5.2);
    std::vector<float> pts_f{1.0, 1.1, 1.2};
    std::vector<std::string> pts_str{"10.2", "11", "12.5"};
    Point3D p3(pts_f);
    Point3D p4(pts_str);

    //----- Getting points
    double str_x = p4.x();
    double str_y = p4.y();
    double str_z = p4.z();
    std::vector<double> float_pts = p3.getAllPts();

    //----- Streaming
    std::cout << "p1 (5, 10, 15): " << p1 << "\n";
    std::cout << "p2 (5.0, 5.1, 5.2): " << p2 << "\n";
    std::cout << "p3 (vec(1.0, 1.1, 1.2)): " << p3 << "\n";
    std::cout << "p4 (vec(10.2, 11, 12.5)): " << p4 << "\n";
    std::cout << std::endl;

    //----- Overloads
    std::cout << "p1 + p2: " << p1 + p2 << "\n";
    std::cout << "p2 - p1: " << p2 - p1 << "\n";
    std::cout << "p1 * p2: " << p1 * p2 << "\n";
    std::cout << "p1 / p2: " << p1 / p2 << "\n";
    std::cout << "p1 / 5: " << p1 / 5 << "\n";
    std::cout << "-p1: " << -p1 << "\n";
    



    return 0;
}
