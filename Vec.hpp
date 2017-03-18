//
//  Vec.hpp
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
    \brief Basic Vec class class for c++
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 10-1-16
    \details The Vec class is designed as a backing store for Vec2/3/4 classes, however it is 
publically exposed to the user. Documentation, however, is not provided for Vec class and it 
 operates in a non-standard way compared to the typical Vec2/3/4 classes.
*/

enum POS{ X, Y, Z, W };
const unsigned MAX_POINTS{4};

class Vec{
private:
    
    std::vector<double> points_;
    
    template<class T>
    void init_pts(std::vector<T> pts){
        static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
        for(auto i = 0; i < pts.size(); ++i)
            points_[i] = pts[i];
    }
    
    void init_store(){
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_.push_back(0);
    }
    
    template<class T>
    void update(POS position, T val){
        static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
        double v = static_cast<double>(val);
        points_[position] = v;
    }
    
public:
    
    Vec(){ init_store(); }
    Vec(const std::vector<std::string> &points){
        if(points.size() > MAX_POINTS)
            throw std::invalid_argument("Too many points");
 
        init_store();
        
        std::vector<double> pts;
        try{
            for(auto &&pt : points)
                pts.emplace_back(NumUtils::strToDouble(pt));
        }
        catch(std::runtime_error &err){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "Couldn't convert string in points to double\n");
#endif
            throw;
        }
        
        init_pts(pts);
    }
    
    template<class T>
    Vec(const std::vector<T> &points){
        if(points.size() > MAX_POINTS)
            throw std::invalid_argument("Too many points");
        
        init_store();
        init_pts(points);
    }
    
    double x() const { return points_[POS::X]; }
    double y() const { return points_[POS::Y]; }
    double z() const { return points_[POS::Z]; }
    double w() const { return points_[POS::W]; }
   
    template<class T> void x(T val){ update(POS::X, val); }
    template<class T> void y(T val){ update(POS::Y, val); }
    template<class T> void z(T val){ update(POS::Z, val); }
    template<class T> void w(T val){ update(POS::W, val); }
    
    bool operator==(const Vec &other) const{
        for(auto i = 0; i < MAX_POINTS; ++i)
            if(points_[i] != other.points_[i])
                return false;
        return true;
    }
    
    bool operator!=(const Vec &other) const{
        return !(*this == other);
    }
    
    Vec& operator+=(const Vec &rhs){
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] += rhs.points_[i];
        return *this;
    }
    
    Vec& operator-=(const Vec &rhs){
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] -= rhs.points_[i];
        return *this;
    }
    
    Vec& operator*=(const Vec &rhs){
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] *= rhs.points_[i];
        return *this;
    }
    
    Vec& operator/=(const Vec &rhs){
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] /= rhs.points_[i];
        return *this;
    }
    
    Vec operator-() const{
        Vec r;
        for(auto i = 0; i < MAX_POINTS; ++i)
            r.points_[i] = -points_[i];
        return r;
    }
    
    template<class T>
    Vec& operator*=(const T &s){
        static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] *= s;
        return *this;
    }
    
    template<class T>
    Vec& operator/=(const T &s){
        static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
        for(auto i = 0; i < MAX_POINTS; ++i)
            points_[i] /= s;
        return *this;
    }
};

inline Vec operator+(const Vec &lhs, const Vec &rhs) { return Vec(lhs) += rhs; }
inline Vec operator-(const Vec &lhs, const Vec &rhs) { return Vec(lhs) -= rhs; }
inline Vec operator*(const Vec &lhs, const Vec &rhs) { return Vec(lhs) *= rhs; }
inline Vec operator/(const Vec &lhs, const Vec &rhs) { return Vec(lhs) /= rhs; }

template<class T>
Vec operator*(const T &s, const Vec &rhs){
    static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
    return Vec(rhs) *= s;
}

template<class T>
Vec operator*(const Vec &lhs, const T &s){
    static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
    return Vec(lhs) *= s;
}

template<class T>
Vec operator/(const Vec &lhs, const T &s){
    static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
    return Vec(lhs) /= s;
}

// ----- Vec2 --------------------------------------------------------------------------------------

class Vec2{
private:
    Vec v_;
    
public:
    Vec2() {}
    Vec2(const std::vector<std::string> &points) : v_(points) {}
    template<class T>Vec2(const std::vector<T> &points) : v_(points) {}
    template<class T>Vec2(T x, T y){
        std::vector<T> pts{x, y};
        v_ = Vec(pts);
    }
    
    double x() const { return v_.x(); }
    double y() const { return v_.y(); }
    double r() const { return v_.x(); }
    double g() const { return v_.y(); }
    template<class T> void x(T val) { v_.x(val); }
    template<class T> void y(T val) { v_.y(val); }
    template<class T> void r(T val) { v_.x(val); }
    template<class T> void g(T val) { v_.y(val); }
    
    bool operator==(const Vec2 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec2 &other) const { return !(*this == other); }
    Vec2& operator+=(const Vec2 &rhs) { v_ += rhs.v_; return *this; }
    Vec2& operator-=(const Vec2 &rhs) { v_ -= rhs.v_; return *this; }
    Vec2& operator*=(const Vec2 &rhs) { v_ *= rhs.v_; return *this; }
    Vec2& operator/=(const Vec2 &rhs) { v_ /= rhs.v_; return *this; }
    Vec2 operator-() const { return Vec2(-x(), -y()); }
    template<class T> Vec2& operator*=(const T &s) { v_ *= s; return *this; }
    template<class T> Vec2& operator/=(const T &s) { v_ /= s; return *this; }
    
    inline friend std::ostream &operator<<(std::ostream &out, const Vec2 &v);
};

inline std::ostream &operator<<(std::ostream &out, const Vec2 &v){
    std::string x = std::to_string(v.x());
    std::string y = std::to_string(v.y());
    out << x << " " << y << " ";
    return out;
}

inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs) += rhs; }
inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs) -= rhs; }
inline Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs) *= rhs; }
inline Vec2 operator/(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs) /= rhs; }

template<class T> Vec2 operator*(const T &s, const Vec2 &rhs){ return Vec2(rhs) *= s; }
template<class T> Vec2 operator*(const Vec2 &lhs, const T &s){ return Vec2(lhs) *= s; }
template<class T> Vec2 operator/(const Vec2 &lhs, const T &s){ return Vec2(lhs) /= s; }

inline double dot(const Vec2 &lhs, const Vec2 &rhs){ return lhs.x() * rhs.x() + lhs.y() * rhs.y(); }
inline double length(const Vec2 &v){ return std::sqrt(dot(v, v)); }
inline Vec2 normalize(const Vec2 &v){ return v / length(v); }


class Vec3{
private:
    Vec v_;
    
public:
    Vec3() {}
    Vec3(const std::vector<std::string> &points) : v_(points) {}
    template<class T> Vec3(const std::vector<T> &points) : v_(points) {}
    template<class T> Vec3(T x, T y, T z) {
        std::vector<T> pts{x, y, z};
        v_ = Vec(pts);
    }
    
    double x() const { return v_.x(); }
    double y() const { return v_.y(); }
    double z() const { return v_.z(); }
    double r() const { return v_.x(); }
    double g() const { return v_.y(); }
    double b() const { return v_.z(); }
    template<class T> void x(T val) { v_.x(val); }
    template<class T> void y(T val) { v_.y(val); }
    template<class T> void z(T val) { v_.z(val); }
    template<class T> void r(T val) { v_.x(val); }
    template<class T> void g(T val) { v_.y(val); }
    template<class T> void b(T val) { v_.z(val); }
    
    bool operator==(const Vec3 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec3 &other) const { return !(*this == other); }
    Vec3& operator+=(const Vec3 &rhs) { v_ += rhs.v_; return *this; }
    Vec3& operator-=(const Vec3 &rhs) { v_ -= rhs.v_; return *this; }
    Vec3& operator*=(const Vec3 &rhs) { v_ *= rhs.v_; return *this; }
    Vec3& operator/=(const Vec3 &rhs) { v_ /= rhs.v_; return *this; }
    Vec3 operator-() const { return Vec3(-x(), -y(), -z()); }
    template<class T> Vec3& operator*=(const T &s) { v_ *= s; return *this; }
    template<class T> Vec3& operator/=(const T &s) { v_ /= s; return *this; }
    
    inline friend std::ostream &operator<<(std::ostream &out, const Vec3 &v);
};

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v){
    std::string x = std::to_string(v.x());
    std::string y = std::to_string(v.y());
    std::string z = std::to_string(v.z());
    out << x << " " << y << " " << z;
    return out;
}

inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs) += rhs; }
inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs) -= rhs; }
inline Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs) *= rhs; }
inline Vec3 operator/(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs) /= rhs; }

template<class T> Vec3 operator*(const T &s, const Vec3 &rhs){ return Vec3(rhs) *= s; }
template<class T> Vec3 operator*(const Vec3 &lhs, const T &s){ return Vec3(lhs) *= s; }
template<class T> Vec3 operator/(const Vec3 &lhs, const T &s){ return Vec3(lhs) /= s; }

inline double dot(const Vec3 &lhs, const Vec3 &rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

inline double length(const Vec3 &v) { return std::sqrt(dot(v, v)); }
inline Vec3 normalize(const Vec3 &v) { return v / length(v); }
inline Vec3 cross(const Vec3 &lhs, const Vec3 &rhs){
    auto a = lhs.y() * rhs.z() - lhs.z() * rhs.y();
    auto b = lhs.z() * rhs.x() - lhs.x() * rhs.z();
    auto c = lhs.x() * rhs.y() - lhs.y() * rhs.x();
    return Vec3(a, b, c);
}


class Vec4{
private:
    Vec v_;
    
public:
    Vec4() {}
    Vec4(const std::vector<std::string> &points) : v_(points) {}
    template<class T> Vec4(const std::vector<T> &points) : v_(points) {}
    template<class T> Vec4(T x, T y, T z, T w) {
        std::vector<T> pts{x, y, z, w};
        v_ = Vec(pts);
    }
    
    double x() const { return v_.x(); }
    double y() const { return v_.y(); }
    double z() const { return v_.z(); }
    double w() const { return v_.w(); }
    double r() const { return v_.x(); }
    double g() const { return v_.y(); }
    double b() const { return v_.z(); }
    double a() const { return v_.w(); }
    template<class T> void x(T val) { v_.x(val); }
    template<class T> void y(T val) { v_.y(val); }
    template<class T> void z(T val) { v_.z(val); }
    template<class T> void w(T val) { v_.w(val); }
    template<class T> void r(T val) { v_.x(val); }
    template<class T> void g(T val) { v_.y(val); }
    template<class T> void b(T val) { v_.z(val); }
    template<class T> void a(T val) { v_.w(val); }
    
    
    bool operator==(const Vec4 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec4 &other) const { return !(*this == other); }
    Vec4& operator+=(const Vec4 &rhs) { v_ += rhs.v_; return *this; }
    Vec4& operator-=(const Vec4 &rhs) { v_ -= rhs.v_; return *this; }
    Vec4& operator*=(const Vec4 &rhs) { v_ *= rhs.v_; return *this; }
    Vec4& operator/=(const Vec4 &rhs) { v_ /= rhs.v_; return *this; }
    Vec4 operator-() const { return Vec4(-x(), -y(), -z(), -w()); }
    template<class T> Vec4& operator*=(const T &s) { v_ *= s; return *this; }
    template<class T> Vec4& operator/=(const T &s) { v_ /= s; return *this; }
    
    inline friend std::ostream &operator<<(std::ostream &out, const Vec4 &v);
};

inline std::ostream &operator<<(std::ostream &out, const Vec4 &v){
    std::string x = std::to_string(v.x());
    std::string y = std::to_string(v.y());
    std::string z = std::to_string(v.z());
    std::string w = std::to_string(v.w());
    out << x << " " << y << " " << z << " " << w;
    return out;
}

inline Vec4 operator+(const Vec4 &lhs, const Vec4 &rhs) { return Vec4(lhs) += rhs; }
inline Vec4 operator-(const Vec4 &lhs, const Vec4 &rhs) { return Vec4(lhs) -= rhs; }
inline Vec4 operator*(const Vec4 &lhs, const Vec4 &rhs) { return Vec4(lhs) *= rhs; }
inline Vec4 operator/(const Vec4 &lhs, const Vec4 &rhs) { return Vec4(lhs) /= rhs; }

template<class T> Vec4 operator*(const T &s, const Vec4 &rhs){ return Vec4(rhs) *= s; }
template<class T> Vec4 operator*(const Vec4 &lhs, const T &s){ return Vec4(lhs) *= s; }
template<class T> Vec4 operator/(const Vec4 &lhs, const T &s){ return Vec4(lhs) /= s; }

inline double dot(const Vec4 &lhs, const Vec4 &rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() + lhs.w() * rhs.w();
}

inline double length(const Vec4 &v) { return std::sqrt(dot(v, v)); }
inline Vec4 normalize(const Vec4 &v) { return v / length(v); }
inline Vec3 cross(const Vec4 &lhs, const Vec4 &rhs){
    auto a = lhs.y() * rhs.z() - lhs.z() * rhs.y();
    auto b = lhs.z() * rhs.x() - lhs.x() * rhs.z();
    auto c = lhs.x() * rhs.y() - lhs.y() * rhs.x();
    return Vec3(a, b, c);
}
