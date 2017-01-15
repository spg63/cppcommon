//
//  Vec3.hpp
//  cppcommon
//
//  Created by Sean Grimes on 10/1/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once
#include <vector>
#include <type_traits>
#include "NumUtils.hpp"

/**
    \brief Basic 3D point class for c++
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 10-1-16
*/
class Vec3{
public:
    
    /**
        \brief Default c'tor
        \details Created object is useless until coordinates are set
    */
    Vec3() {}
    
    /**
        \brief c'tor takes a vector of strings
        \details vector.size() must == 3
        @param coords The vector holding 3 string coordinates
    */
    Vec3(const std::vector<std::string> &coords) {
        if(coords.size() != 3)
            throw std::invalid_argument("Vec3 takes a vector of size 3 only");
        try{
            init_pts(NumUtils::strToDouble(coords[0]),
                     NumUtils::strToDouble(coords[1]),
                     NumUtils::strToDouble(coords[2]));
        }
        catch(std::runtime_error &err){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "Couldn't convert string in coords to double\n");
#endif
            throw;
        }
    }
    
    /**
        \brief c'tor for all std::is_arithmetic types
        @param x The x coordinate
        @param y The y coordinate
        @param z The z coordinate
    */
    template<class T>
    Vec3(T x, T y, T z){
        static_assert(std::is_arithmetic<T>::value, "Vec3 can only be constructed from numerics");
        init_pts(x, y, z);
    }
    
    /**
        \brief c'tor for vector of std::is_arithmetic types
        \details vector.size() must == 3
        @param coords The vector holding 3 std::is_arithmetic coordinates
    */
    template<class T>
    Vec3(const std::vector<T> &coords){
        // Check that we're getting a numeric (or char)
        static_assert(std::is_arithmetic<T>::value, "Vec3 can only be constructed from std::vector<arithmetic_type>");
        
        // Insist on a vector size of 3
        if(coords.size() != 3)
            throw std::invalid_argument("Vec3 takes a vector of size 3 only");
        
        init_pts(coords[0], coords[1], coords[2]);
    }
    
    /**
        \brief Set x coordinate
        @param x The x coordinate, std::is_arithmetic type
    */
    template<class T>
    void x(T x){
        static_assert(std::is_arithmetic<T>::value, "Vec3 only accepts numerics for coordinates");
        x_ = x;
    }
    
    /**
        \brief Set y coordinate
        @param y The y coordinate, std::is_arithmetic type
    */
    template<class T>
    void y(T y){
        static_assert(std::is_arithmetic<T>::value, "Vec3 only accepts numerics for coordinates");
        y_ = y;
    }
    
    /**
        \brief Set z coordinate
        @param z The z coordinate, std::is_arithmetic type
    */
    template<class T>
    void z(T z){
        static_assert(std::is_arithmetic<T>::value, "Vec3 only accepts numerics for coordinates");
        z_ = z;
    }
    
    /**
        \brief Get x coordinate
        @return x coordinate as a double
    */
    double x() const {return x_;}
    
    /**
        \brief Get y coordinate
        @return y coordinate as a double
    */
    double y() const {return y_;}
    
    /**
        \brief Get z coordinate
        @return z coordinate as a double
    */
    double z() const {return z_;}
    
    /**
        \brief Get all points
        @return All coordinates as a vector of doubles, size == 3
    */
    std::vector<double> getAllPts() const {return std::vector<double>{x_, y_, z_};}
    
    /**
        \brief += operator
        @param rhs The other point
    */
    Vec3& operator+=(const Vec3 &rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }
    
    /**
        \brief -= operator
        @param rhs The other point
    */
    Vec3& operator-=(const Vec3 &rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }
    
    /**
        \brief *= operator
        @param rhs The other point
    */
    Vec3& operator*=(const Vec3 &rhs) {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        z_ *= rhs.z_;
        return *this;
    }
    
    /**
        \brief /= operator
        @param rhs The other point
    */
    Vec3& operator/=(const Vec3 &rhs) {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        z_ /= rhs.z_;
        return *this;
    }
    
    /**
        \brief negation operator
    */
    Vec3 operator-() const {return Vec3(-x_, -y_, -z_);}
    
    /**
        \brief *= scalar operator
        @param s The scalar
    */
    template<class T>
    Vec3& operator*=(const T &s){
        static_assert(std::is_arithmetic<T>::value, "Vec3 can only carry out arithmetic on numerics");
        
        x_ *= s;
        y_ *= s;
        z_ *= s;
        return *this;
    }
    
    /**
        \brief /= scalar operator
        @param s The scalar
    */
    template<class T>
    Vec3& operator/=(const T &s){
        static_assert(std::is_arithmetic<T>::value, "Vec3 can only carry out arithmetic on numerics");

        if(s == 0)
            throw std::invalid_argument("Divide by zero");
        x_ /= s;
        y_ /= s;
        z_ /= s;
        return *this;
    }
    
    /**
        \brief Write it
    */
    inline friend std::ostream &operator<<(std::ostream &out, const Vec3 &p3d);
    
private:
    double x_;
    double y_;
    double z_;
   
    // Generalized for floats && doubles
    template<class T>
    void init_pts(T x, T y, T z){
        x_ = x;
        y_ = y;
        z_ = z;
    }
    
};

/**
    \brief Addition
    @param lhs A point
    @param rhs Another point
*/
inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) {return Vec3(lhs) += rhs;}

/**
    \brief Subtraction
    @param lhs A point
    @param rhs Another point
*/
inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) {return Vec3(lhs) -= rhs;}

/**
    \brief Multiplication
    @param lhs A point
    @param rhs Another point
*/
inline Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs) {return Vec3(lhs) *= rhs;}

/**
    \brief Division
    @param lhs A point
    @param rhs Another point
*/
inline Vec3 operator/(const Vec3 &lhs, const Vec3 &rhs) {return Vec3(lhs) /= rhs;}

/**
    \brief Scalar multiplication
    @param s The scalar
    @param rhs The point
*/
template<class T>
Vec3 operator*(const T &s, const Vec3 &rhs){
    static_assert(std::is_arithmetic<T>::value, "Vec3 can only carry out arithmetic on numerics");

    return Vec3(rhs) *= s;
}

/**
    \brief Scalar multiplication
    @param lhs The point
    @param s The scalar
*/
template<class T>
Vec3 operator*(const Vec3 &lhs, const T &s){
    static_assert(std::is_arithmetic<T>::value, "Vec3 can only carry out arithmetic on numerics");

    return Vec3(lhs) *= s;
}

/**
    \brief Scalar division
    @param lhs The point
    @param s The scalar
*/
template<class T>
Vec3 operator/(const Vec3 &lhs, const T &s){
    static_assert(std::is_arithmetic<T>::value, "Vec3 can only carry out arithmetic on numerics");

    return Vec3(lhs) /= s;
}

inline std::ostream &operator<<(std::ostream &out, const Vec3 &p3d) {
    std::string x = std::to_string(p3d.x());
    std::string y = std::to_string(p3d.y());
    std::string z = std::to_string(p3d.z());
    out << x << " " << y << " " << z;
    return out;
}
