//
//  Vec.hpp
//  cppcommon
//
//  Created by Sean Grimes on 10/1/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once
#include <array>
#include <type_traits>
#include <iostream>
#include "NumUtils.hpp"

#if (defined(GL_TYPES_FLOAT) || defined(GL_TYPES_DOUBLE)) && defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif (defined(GL_TYPES_FLOAT) || defined(GL_TYPES_DOUBLE)) && !defined(__APPLE__)
    #include <GL/gl.h>
#endif

#if defined(GL_TYPES_DOUBLE)
    using num_t = GLdouble;
#elif defined(GL_TYPES_FLOAT)
    using num_t = GLfloat;
#else
    using num_t = float;
#endif


/**
    \brief Basic Vec class class for c++
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 10-1-16
    \details The Vec class is designed as a backing store for Vec2/3/4 classes, however it is 
publically exposed to the user. Documentation, however, is not provided for Vec class and it 
 operates in a non-standard way compared to the typical Vec2/3/4 classes. By default the values of
 each vector will be stored as c++ doubles; define GL_TYPES_FLOAT to use GLfloat, or GL_TYPES_DOUBLE
 to use GLdouble at compile time.
*/

enum POS {X, Y, Z, W};
namespace VecN{
    static const unsigned DEFAULT_POINTS{4};
}

class Vec{
private:
    size_t size_;
    std::vector<num_t> points_;
    void init_vec() { points_ = std::vector<num_t>(size_, 0.0); }
    
public:
    Vec() : size_(VecN::DEFAULT_POINTS) { init_vec(); }
    template<class T> Vec(const std::vector<T> &points) : size_(points.size()), points_(points) {}
 
    Vec(const std::vector<std::string> &points)
    : size_(points.size())
    {
        init_vec();
        
        for(auto i = 0; i < size_; ++i)
            points_[i] = static_cast<num_t>(NumUtils::strToDouble(points[i]));
    }
    
    
    Vec(num_t val, size_t num_points = VecN::DEFAULT_POINTS) : size_(num_points)
    {
        init_vec();
        for(auto i = 0; i < size_; ++i)
            points_[i] = val;
    }
    
    num_t& operator[] (const int idx) { return points_[idx]; }
    const num_t& operator[] (const int idx) const { return points_[idx]; }
    
    num_t x() const { return points_[POS::X]; }
    num_t y() const { return points_[POS::Y]; }
    num_t z() const { return points_[POS::Z]; }
    num_t w() const { return points_[POS::W]; }
   
    void x(num_t val){ points_[POS::X] = val; }
    void y(num_t val){ points_[POS::Y] = val; }
    void z(num_t val){ points_[POS::Z] = val; }
    void w(num_t val){ points_[POS::W] = val; }
    
    size_t size() const { return size_; }
    std::vector<num_t> points() const { return points_; }
    
    bool operator==(const Vec &other) const{
        if(size_ != other.size_)
            return false;
        for(auto i = 0; i < size_; ++i)
            if(points_[i] != other.points_[i])
                return false;
        return true;
    }
    
    bool operator!=(const Vec &other) const{
        return !(*this == other);
    }
    
    Vec& operator+=(const Vec &rhs){
        if(size_ != rhs.size_) {
            std::cerr << "size_: " << size_ << " | rhs.size_: " << rhs.size_ << std::endl;
            throw std::runtime_error("element-wise addition requires matrices of equal dimensions");
        }
        for(auto i = 0; i < size_; ++i)
            points_[i] += rhs.points_[i];
        return *this;
    }
    
    Vec& operator-=(const Vec &rhs){
        if(size_ != rhs.size_)
            throw std::runtime_error("element-wise subtraction requires matrices of equal dimensions");
        for(auto i = 0; i < size_; ++i)
            points_[i] -= rhs.points_[i];
        return *this;
    }
    
    Vec& operator*=(const Vec &rhs){
        if(size_ != rhs.size_)
            throw std::runtime_error("element-wise multiplication requires matrices of equal dimensions");            
        for(auto i = 0; i < size_; ++i)
            points_[i] *= rhs.points_[i];
        return *this;
    }
    
    Vec& operator/=(const Vec &rhs){
        if(size_ != rhs.size_)
            throw std::runtime_error("element-wise division requires matrices of equal dimensions");            
        for(auto i = 0; i < size_; ++i)
            points_[i] /= rhs.points_[i];
        return *this;
    }
    
    Vec operator-() const{
        Vec r;
        for(auto i = 0; i < size_; ++i)
            r.points_[i] = -points_[i];
        return r;
    }
    
    Vec& operator*=(num_t s){
        for(auto i = 0; i < size_; ++i)
            points_[i] *= s;
        return *this;
    }
    
    Vec& operator/=(num_t s){
        for(auto i = 0; i < size_; ++i)
            points_[i] /= s;
        return *this;
    }
    
    inline friend std::ostream &operator<<(std::ostream &out, const Vec &v);
};

inline std::ostream &operator<<(std::ostream &out, const Vec &v){
    for(auto i = 0; i < v.size(); ++i)
        out << std::to_string(v[i]) << " ";
    return out;
}


inline Vec operator+(const Vec &lhs, const Vec &rhs) { return Vec(lhs) += rhs; }
inline Vec operator-(const Vec &lhs, const Vec &rhs) { return Vec(lhs) -= rhs; }
inline Vec operator*(const Vec &lhs, const Vec &rhs) { return Vec(lhs) *= rhs; }
inline Vec operator/(const Vec &lhs, const Vec &rhs) { return Vec(lhs) /= rhs; }

inline Vec operator*(num_t s, const Vec &rhs){ return Vec(rhs) *= s; }
inline Vec operator*(const Vec &lhs, num_t s){ return Vec(lhs) *= s; }
inline Vec operator/(const Vec &lhs, num_t s){ return Vec(lhs) /= s; }

// ----- Vec2 --------------------------------------------------------------------------------------

class Vec2{
private:
    Vec v_;
    
public:
    Vec2() {}
    Vec2(const std::vector<std::string> &points) : v_(points) {}
    template<class T> Vec2(const std::vector<T> &points) : v_(points) {}
    Vec2(num_t x, num_t y) : v_(std::vector<num_t>{x, y}) {}
    Vec2(num_t val) : v_(std::vector<num_t>{val, val}) {}
    
    num_t& operator[] (const int idx) { return v_[idx]; }
    const num_t& operator[] (const int idx) const { return v_[idx]; }
    
    num_t x() const { return v_.x(); }
    num_t y() const { return v_.y(); }
    num_t r() const { return v_.x(); }
    num_t g() const { return v_.y(); }
    void x(num_t val) { v_.x(val); }
    void y(num_t val) { v_.y(val); }
    void r(num_t val) { v_.x(val); }
    void g(num_t val) { v_.y(val); }
    
    bool operator==(const Vec2 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec2 &other) const { return !(*this == other); }
    Vec2& operator+=(const Vec2 &rhs) { v_ += rhs.v_; return *this; }
    Vec2& operator-=(const Vec2 &rhs) { v_ -= rhs.v_; return *this; }
    Vec2& operator*=(const Vec2 &rhs) { v_ *= rhs.v_; return *this; }
    Vec2& operator/=(const Vec2 &rhs) { v_ /= rhs.v_; return *this; }
    Vec2 operator-() const { return Vec2(-x(), -y()); }
    Vec2& operator*=(num_t s) { v_ *= s; return *this; }
    Vec2& operator/=(num_t s) { v_ /= s; return *this; }
    
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

inline Vec2 operator*(num_t s, const Vec2 &rhs){ return Vec2(rhs) *= s; }
inline Vec2 operator*(const Vec2 &lhs, num_t s){ return Vec2(lhs) *= s; }
inline Vec2 operator/(const Vec2 &lhs, num_t s){ return Vec2(lhs) /= s; }

inline num_t dot(const Vec2 &lhs, const Vec2 &rhs){ return lhs.x() * rhs.x() + lhs.y() * rhs.y(); }
inline num_t length(const Vec2 &v){ return std::sqrt(dot(v, v)); }
inline Vec2 normalize(const Vec2 &v){ return v / length(v); }


class Vec3{
private:
    Vec v_;
    
public:
    Vec3() : v_(std::vector<num_t>{0,0,0}) {}
    Vec3(const std::vector<std::string> &points) : v_(points) {}
    template<class T> Vec3(const std::vector<T> &points) : v_(points) {}
    Vec3(num_t x, num_t y, num_t z) : v_(std::vector<num_t>{x, y, z}) {}
    Vec3(Vec2 v, num_t z) : v_(std::vector<num_t>{v.x(), v.y(), z}) {}
    Vec3(num_t x, Vec2 v) : v_(std::vector<num_t>{x, v.x(), v.y()}) {}
    Vec3(num_t val) : v_(std::vector<num_t>{val, val, val}) {}

    num_t& operator[] (const int idx) { return v_[idx]; }
    const num_t& operator[] (const int idx) const { return v_[idx]; }
    
    num_t x() const { return v_.x(); }
    num_t y() const { return v_.y(); }
    num_t z() const { return v_.z(); }
    num_t r() const { return v_.x(); }
    num_t g() const { return v_.y(); }
    num_t b() const { return v_.z(); }
    void x(num_t val) { v_.x(val); }
    void y(num_t val) { v_.y(val); }
    void z(num_t val) { v_.z(val); }
    void r(num_t val) { v_.x(val); }
    void g(num_t val) { v_.y(val); }
    void b(num_t val) { v_.z(val); }
    
    bool operator==(const Vec3 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec3 &other) const { return !(*this == other); }
    Vec3& operator+=(const Vec3 &rhs) { v_ += rhs.v_; return *this; }
    Vec3& operator-=(const Vec3 &rhs) { v_ -= rhs.v_; return *this; }
    Vec3& operator*=(const Vec3 &rhs) { v_ *= rhs.v_; return *this; }
    Vec3& operator/=(const Vec3 &rhs) { v_ /= rhs.v_; return *this; }
    Vec3 operator-() const { return Vec3(-x(), -y(), -z()); }
    Vec3& operator*=(num_t s) { v_ *= s; return *this; }
    Vec3& operator/=(num_t s) { v_ /= s; return *this; }
    
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

inline Vec3 operator*(num_t s, const Vec3 &rhs){ return Vec3(rhs) *= s; }
inline Vec3 operator*(const Vec3 &lhs, num_t s){ return Vec3(lhs) *= s; }
inline Vec3 operator/(const Vec3 &lhs, num_t s){ return Vec3(lhs) /= s; }

inline num_t dot(const Vec3 &lhs, const Vec3 &rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

inline num_t length(const Vec3 &v) { return std::sqrt(dot(v, v)); }
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
    Vec4(num_t x, num_t y, num_t z, num_t w) : v_(std::vector<num_t>{x, y, z, w}) {}
    Vec4(Vec3 v, num_t w = 1) : v_(std::vector<num_t>{v.x(), v.y(), v.z(), w}) {}
    Vec4(num_t x, Vec3 v) : v_(std::vector<num_t>{x, v.x(), v.y(), v.z()}) {}
    Vec4(Vec2 vl, Vec2 vr) : v_(std::vector<num_t>{vl.x(), vl.y(), vr.x(), vr.y()}) {}
    Vec4(num_t val) : v_(std::vector<num_t>{val, val, val, val}) {}
    Vec4(Vec3 v) : Vec4(v, 1) {}
    
    num_t& operator[] (const int idx) { return v_[idx]; }
    const num_t& operator[] (const int idx) const { return v_[idx]; }
    
    num_t x() const { return v_.x(); }
    num_t y() const { return v_.y(); }
    num_t z() const { return v_.z(); }
    num_t w() const { return v_.w(); }
    num_t r() const { return v_.x(); }
    num_t g() const { return v_.y(); }
    num_t b() const { return v_.z(); }
    num_t a() const { return v_.w(); }
    void x(num_t val) { v_.x(val); }
    void y(num_t val) { v_.y(val); }
    void z(num_t val) { v_.z(val); }
    void w(num_t val) { v_.w(val); }
    void r(num_t val) { v_.x(val); }
    void g(num_t val) { v_.y(val); }
    void b(num_t val) { v_.z(val); }
    void a(num_t val) { v_.w(val); }
    
    
    bool operator==(const Vec4 &other) const { return v_ == other.v_; }
    bool operator!=(const Vec4 &other) const { return !(*this == other); }
    Vec4& operator+=(const Vec4 &rhs) { v_ += rhs.v_; return *this; }
    /*
    Vec4& operator+=(const Vec4 &rhs){
        
    }
    */
    Vec4& operator-=(const Vec4 &rhs) { v_ -= rhs.v_; return *this; }
    Vec4& operator*=(const Vec4 &rhs) { v_ *= rhs.v_; return *this; }
    Vec4& operator/=(const Vec4 &rhs) { v_ /= rhs.v_; return *this; }
    Vec4 operator-() const { return Vec4(-x(), -y(), -z(), -w()); }
    Vec4& operator*=(num_t s) { v_ *= s; return *this; }
    Vec4& operator/=(num_t s) { v_ /= s; return *this; }
    
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

inline Vec4 operator*(num_t s, const Vec4 &rhs){ return Vec4(rhs) *= s; }
inline Vec4 operator*(const Vec4 &lhs, num_t s){ return Vec4(lhs) *= s; }
inline Vec4 operator/(const Vec4 &lhs, num_t s){ return Vec4(lhs) /= s; }

inline num_t dot(const Vec4 &lhs, const Vec4 &rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() + lhs.w() * rhs.w();
}

inline num_t length(const Vec4 &v) { return std::sqrt(dot(v, v)); }
inline Vec4 normalize(const Vec4 &v) { return v / length(v); }
inline Vec3 cross(const Vec4 &lhs, const Vec4 &rhs){
    auto a = lhs.y() * rhs.z() - lhs.z() * rhs.y();
    auto b = lhs.z() * rhs.x() - lhs.x() * rhs.z();
    auto c = lhs.x() * rhs.y() - lhs.y() * rhs.x();
    return Vec3(a, b, c);
}
