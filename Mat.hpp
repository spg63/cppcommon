//
// Mat.hpp
// cppcommon
//
// Created by Sean Grimes on 12/2/16
// Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once
#include <array>
#include <vector>
#include "NumUtils.hpp"
#include <x86intrin.h>

#define ROUND_UP(x, s) (((x)+((s)-1)) & -(s))
#define ROUND_DOWN(x, s) ((x) & ~((s)-1))

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

namespace{
    const size_t PARALLEL       {64 * 64}; // NOTE: This must remain larger than block_size^2
    const size_t DEFAULT_ROWS   {4};
    const size_t DEFAULT_COLS   {4};
    const int BLOCK_SIZE        {16};
    size_t get1D(size_t num_cols, size_t row, size_t col){ return (num_cols * row) + col; }
}

class Mat{
private:
    size_t rows_;
    size_t cols_;
    size_t size_;
    std::vector<num_t> mat_;
    
    void set1D(size_t row, size_t col, num_t val) { mat_[get1D(cols_, row, col)] = val; }
    void init_mat(){ mat_ = std::vector<num_t>(size_, 0.0); }
    
public:
    Mat()
    : rows_(DEFAULT_ROWS)
    , cols_(DEFAULT_COLS)
    , size_(rows_ * cols_)
    {
        init_mat();
    }
    
    Mat(const std::vector<std::vector<std::string>> &points)
    : rows_(points.size())
    , cols_(rows_ ? points[0].size() : 0)
    , size_(rows_ * cols_)
    {
        for(auto &&vec : points)
            for(auto &&pt : vec)
                mat_.emplace_back(NumUtils::strToDouble(pt));
    }
    
    template<class T>
    Mat(const std::vector<std::vector<T>> &points)
    : rows_(points.size())
    , cols_(rows_ ? points[0].size() : 0)
    , size_(rows_ * cols_)
    {
        for(auto &&vec : points)
            for(auto &&pt : vec)
                mat_.emplace_back(pt);
    }
    
    template<class T>
    Mat(const std::vector<T> &points, size_t rows = DEFAULT_ROWS, size_t cols = DEFAULT_COLS)
    : rows_(rows)
    , cols_(rows_ ? cols : 0)
    , size_(rows_ * cols_)
    {
        static_assert(std::is_arithmetic<T>::value, "Numeric types only");
        if(points.size() != size_)
            throw std::logic_error("rows * cols != size...why?");
        mat_ = points;
    }
    
    // Create a diagonal matrix based on val
    Mat(num_t val, size_t rows = DEFAULT_ROWS, size_t cols = DEFAULT_COLS)
    : rows_(rows)
    , cols_(cols)
    , size_(rows_ * cols_)
    {
        init_mat();
        
        // Create the diagonal matrix by default
        if(rows_ == cols_){
            for(auto i = 0; i < rows_; ++i)
                mat_[get1D(cols_, i, i)] = val;
        }
        // Just fill the matrix with values if rows_ != cols_
        else{
            for(auto i = 0; i < size_; ++i)
                mat_[i] = val;
        }
    }
    
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    size_t size() const { return size_; }
    
    num_t& operator()(size_t i, size_t j){ return mat_[get1D(cols_, i, j)]; }
    const num_t& operator()(size_t i, size_t j) const { return mat_[get1D(cols_, i, j)]; }
    
    num_t& operator[] (const size_t idx) { return mat_[idx]; }
    const num_t& operator[] (const size_t idx) const { return mat_[idx]; }
    
    std::vector<num_t> mat1d() const{ return mat_; }
    
    std::vector<std::vector<num_t>> mat2d() const {
        std::vector<std::vector<num_t>> mat;
        for(auto i = 0; i < rows_; ++i){
            std::vector<num_t> tmp(cols_, 0.0);
            mat.push_back(tmp);
        }
        
        for(auto i = 0; i < rows_; ++i)
            for(auto j = 0; j < cols_; ++j)
                mat[i][j] = mat_[get1D(cols_, i, j)];
        return mat;
    }
   
    bool operator==(const Mat &other) const {
        if(size_ != other.size_ || rows_ != other.rows_ || cols_ != other.cols_)
            return false;
        return mat_ == other.mat_;
    }
    
    bool operator!=(const Mat &other) const {
        return !(*this == other);
    }
    
    Mat& operator+=(const Mat &rhs){
        if(rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("element-wise addition requires matrices of equal dimensions");
        for(auto i = 0; i < size_; ++i)
            mat_[i] += rhs.mat_[i];
        return *this;
    }
 
    Mat& operator-=(const Mat &rhs){
        if(rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("element-wise subtraction requires matrices of equal dimensions");
        for(auto i = 0; i < size_; ++i)
            mat_[i] -= rhs.mat_[i];
        return *this;
    }
    
    Mat& operator*=(const Mat &rhs){
        if(rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("element-wise multiplcation requires matrices of equal dimensions");
        for(auto i = 0; i < size_; ++i)
            mat_[i] *= rhs.mat_[i];
        return *this;
    }
    
    Mat& operator/=(const Mat &rhs){
        if(rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("element-wise division requires matrices of equal dimensions");
        for(auto i = 0; i < size_; ++i)
            mat_[i] /= rhs.mat_[i];
        return *this;
    }
    
    Mat operator-() const {
        Mat m(0, this->rows_, this->cols_);
        for(auto i = 0; i < size_; ++i)
            m.mat_[i] = -this->mat_[i];
        return m;
    }
    
    Mat& operator*=(num_t s){
        for(auto i = 0; i < size_; ++i)
            mat_[i] *= s;
        return *this;
    }
    
    Mat& operator/=(num_t s){
        for(auto i = 0; i < size_; ++i)
            mat_[i] /= s;
        return *this;
    }

    
    inline friend std::ostream &operator<<(std::ostream &out, const Mat &m);
};

inline std::ostream &operator<<(std::ostream &out, const Mat &m){
    for(auto i = 0; i < m.rows_; ++i){
        for(auto j = 0; j < m.cols_; ++j)
            out << m[get1D(m.cols_, i ,j)] << " ";
        out << "\n";
    }
    return out;
}

inline Mat operator+(const Mat &lhs, const Mat &rhs) { return Mat(lhs) += rhs; }
inline Mat operator-(const Mat &lhs, const Mat &rhs) { return Mat(lhs) -= rhs; }
inline Mat operator*(const Mat &lhs, const Mat &rhs) { return Mat(lhs) *= rhs; }
inline Mat operator/(const Mat &lhs, const Mat &rhs) { return Mat(lhs) /= rhs; }

inline Mat operator*(num_t s, const Mat &rhs) { return Mat(rhs) *= s; }
inline Mat operator*(const Mat &lhs, num_t s) { return Mat(lhs) *= s; }
inline Mat operator/(const Mat &lhs, num_t s) { return Mat(lhs) /= s; }

namespace{
    inline Mat simple_transpose_(const Mat &m){
        std::cout << "simple\n";
        auto cols = m.cols();
        auto rows = m.rows();
        Mat re(0, cols, rows);
        for(auto i = 0; i < cols; ++i)
            for(auto j = 0; j < rows; ++j)
                re(i, j) = m(j, i);
        return re;
    }
    
    inline Mat tiling_transpose_(const Mat &m){
        std::cout << "tiling\n";
        auto cols = m.cols();
        auto rows = m.rows();
        Mat re(0, cols, rows);
        
        for(auto i = 0; i < cols; i += BLOCK_SIZE){
            for(auto j = 0; j < rows; j += BLOCK_SIZE){
                int max_k = i + BLOCK_SIZE < cols ? i + BLOCK_SIZE : static_cast<int>(cols);
                int max_l = j + BLOCK_SIZE < rows ? j + BLOCK_SIZE : static_cast<int>(rows);
                for(auto k = i; k < max_k; ++k){
                    for(auto l = j; l < max_l; ++l){
                        re(k, l) = m(l, k);
                    }
                }
            }
        }
        return re;
    }
    
    inline void transpose4x4_SSE(const float *A, float *B, const int lda, const int ldb){
        __m128 row1 = _mm_loadu_ps(&A[0*lda]);
        __m128 row2 = _mm_loadu_ps(&A[1*lda]);
        __m128 row3 = _mm_loadu_ps(&A[2*lda]);
        __m128 row4 = _mm_loadu_ps(&A[3*lda]);
        _MM_TRANSPOSE4_PS(row1, row2, row3, row4);
        _mm_store_ps(&B[0*ldb], row1);
        _mm_store_ps(&B[1*ldb], row2);
        _mm_store_ps(&B[2*ldb], row3);
        _mm_store_ps(&B[3*ldb], row4);
    }
   
    
    inline Mat simd_transpose_(const Mat &m){
        std::cout << "simd\n";
        auto cols = m.cols();
        auto rows = m.rows();
        auto size = m.size();
        auto raw = m.mat1d();
        
        int lda = ROUND_UP(static_cast<int>(cols), BLOCK_SIZE);
        int ldb = ROUND_UP(static_cast<int>(rows), BLOCK_SIZE);
        
        //float *A = (float*)_mm_malloc(sizeof(float) * lda * ldb, 64);
        //if(!A) return tiling_transpose_(m);
        const float *A = m.mat1d().data();
        
        float *B = (float*)_mm_malloc(sizeof(float) * lda * lda, 64);
        //if(!B){ _mm_free(A); return tiling_transpose_(m); }
        
        /*
        for(auto i = 0; i < size; ++i)
            A[i] = raw[i];
        */
        
        for(int i = 0; i < cols; i += BLOCK_SIZE){
            for(int j = 0; j < rows; j += BLOCK_SIZE){
                int max_k = i + BLOCK_SIZE < cols ? i + BLOCK_SIZE : static_cast<int>(cols);
                int max_l = j + BLOCK_SIZE < rows ? j + BLOCK_SIZE : static_cast<int>(rows);
                for(int k = i; k < max_k; k += 4){
                    for(int l = j; l < max_l; l += 4){
                        transpose4x4_SSE(&A[k * lda + l], &B[l * ldb + k], lda, ldb);
                    }
                }
            }
        }
        
        std::vector<float> transpose_vec(B, B + (cols * rows));
        Mat re(transpose_vec, cols, rows);
        
        //if(A) _mm_free(A);
        if(B) _mm_free(B);
        return re;
    }
}

inline Mat transpose(const Mat &m){
#if defined(GL_TYPES_FLOAT) || defined(GL_TYPES_DOUBLE)
    if(m.cols() < BLOCK_SIZE && m.rows() < BLOCK_SIZE)
        return simple_transpose_(m);
    else
        return tiling_transpose_(m);
#else
    // tiling_transpose_() could be called on matrices smaller than BLOCK_SIZE, but no performance
    // benefit
    if(m.cols() < BLOCK_SIZE && m.rows() < BLOCK_SIZE)
        return simple_transpose_(m);
    // simd can only be called on square matrices, this shouldn't be the case but I've got a bug
    // somewhere and no time to find it at the moment, so if it's not square just do a simple
    // tiling transpose and move on
    //else if((m.rows() != m.cols()) || m.size() < PARALLEL)
    //else
    //    return tiling_transpose_(m);
    // Finally, if all conditions are met, use SSE instructions to significantly speed things along
    else
        return simd_transpose_(m);
#endif
}

inline Mat identity(size_t dims = DEFAULT_ROWS) {
    return Mat(1, dims, dims);
}



























