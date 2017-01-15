#include "../../Vec3.hpp"
#include <iostream>
#include <vector>

int main(){
    //----- C'tors and setting points
    Vec3 v1;
    v1.x(5);
    v1.y(10);
    v1.z(15);

    Vec3 v2(5.0, 5.1, 5.2);
    std::vector<float> pts_f{1.0, 1.1, 1.2};
    std::vector<std::string> pts_str{"10.2", "11", "12.5"};
    Vec3 v3(pts_f);
    Vec3 v4(pts_str);

    //----- Getting points
    double str_x = v4.x();
    double str_y = v4.y();
    double str_z = v4.z();
    std::vector<double> float_pts = v3.getAllPts();

    //----- Streaming
    std::cout << "v1 (5, 10, 15): " << v1 << "\n";
    std::cout << "v2 (5.0, 5.1, 5.2): " << v2 << "\n";
    std::cout << "v3 (vec(1.0, 1.1, 1.2)): " << v3 << "\n";
    std::cout << "v4 (vec(10.2, 11, 12.5)): " << v4 << "\n";
    std::cout << std::endl;

    //----- Overloads
    std::cout << "v1 + v2: " << v1 + v2 << "\n";
    std::cout << "v2 - v1: " << v2 - v1 << "\n";
    std::cout << "v1 * v2: " << v1 * v2 << "\n";
    std::cout << "v1 / v2: " << v1 / v2 << "\n";
    std::cout << "v1 / 5: " << v1 / 5 << "\n";
    std::cout << "-v1: " << -v1 << "\n";

    Vec3 a(5, 10, 15);
    Vec3 b(5, 10, 15);

    if(a == b)
        std::cout << "a == b\n";
    else
        std::cout << "a != b\n";
    
    if(a != b)
        std::cout << "a != b\n";
    else
        std::cout << "a == b\n";

    if(a == v1)
        std::cout << "a == v1\n";
    else
        std::cout << "a != v1\n";

    if(b == v2)
        std::cout << "b == v2\n";
    else
        std::cout << "b != v2\n";

    return 0;
}
