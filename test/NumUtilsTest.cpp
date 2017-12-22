//
// Created by Sean Grimes on 11/21/17.
//

#include <iostream>
#include "catch.hpp"
#include "../src/NumUtils.hpp"

// All caps is killing me
#define require REQUIRE
#define test_case TEST_CASE
#define require_throws REQUIRE_THROWS

test_case("getMedian"){
    std::vector<int> int_vec_with_even_numbers{1, 10, 15, 3, 12, 4};
    require(NumUtils::getMedian(int_vec_with_even_numbers) == 7);

    std::vector<int> int_vec_with_odd_numbers{1, 10, 15, 3, 12, 4, 5};
    require(NumUtils::getMedian(int_vec_with_odd_numbers) == 5);

    std::vector<int> int_vec_single_element{10};
    require(NumUtils::getMedian(int_vec_single_element) == 10);

    std::vector<double> d_vec_even_numbers{1.1, 2.2, 4.5, 2.5};
    require(NumUtils::getMedian(d_vec_even_numbers) == 2.35);

    std::vector<double> d_vec_odd_numbers{1.1, 2.2, 4.5};
    require(NumUtils::getMedian(d_vec_odd_numbers) == 2.2);

    std::vector<int> int_vec_empty{};
    require_throws(NumUtils::getMedian(int_vec_empty));
}

test_case("maxValueVec"){
    std::vector<long> l_vec{100, 200, 300, 400};
    require(NumUtils::maxValueVec(l_vec) == 400);

    std::vector<double> d_vec{100.25, 200.12, 200.125, 200.50};
    Approx target = Approx(200.5).epsilon(0.0001);
    require(NumUtils::maxValueVec(d_vec) == target);

    std::vector<int> i_vec{100};
    require(NumUtils::maxValueVec(i_vec) == 100);

    std::vector<int> empty_vec{};
    require_throws(NumUtils::maxValueVec(empty_vec));
}

test_case("minValueVec"){
    std::vector<long> l_vec{100, 200, 300, 400};
    require(NumUtils::minValueVec(l_vec) == 100);

    std::vector<double> d_vec{100.25, 200.12, 200.125, 200.50};
    Approx target = Approx(100.25).epsilon(0.0001);
    require(NumUtils::minValueVec(d_vec) == target);

    std::vector<int> i_vec{100};
    require(NumUtils::minValueVec(i_vec) == 100);

    std::vector<int> empty_vec{};
    require_throws(NumUtils::minValueVec(empty_vec));
}

test_case("minMaxInVec"){
    std::vector<long> l_vec{100, 200, 300, 400};
    auto l_pair = NumUtils::minMaxInVec(l_vec);
    require(l_pair.first == 100);
    require(l_pair.second == 400);

    std::vector<double> d_vec{100.25, 200.12, 200.125, 200.50};
    Approx target_min = Approx(100.25).epsilon(0.0001);
    Approx target_max = Approx(200.50).epsilon(0.0001);
    auto d_pair = NumUtils::minMaxInVec(d_vec);
    require(d_pair.first == target_min);
    require(d_pair.second == target_max);

    std::vector<int> i_vec{100};
    auto i_pair = NumUtils::minMaxInVec(i_vec);
    require(i_pair.first == i_pair.second);

    std::vector<int> empty_vec{};
    require_throws(NumUtils::minValueVec(empty_vec));
}

test_case("strToInt"){
    std::string pass{"50"};
    std::string int_overflow{"10" + std::to_string(INT_MAX)};
    std::string int_underflow{"-10" + std::to_string(-INT_MAX)};
    std::string bad_number{"50a"};
    std::string zero{"0"};
    std::string not_an_int{"2.25"};

    require(NumUtils::strToInt(pass) == 50);
    require(NumUtils::strToInt(zero) == 0);
    require_throws(NumUtils::strToInt(int_overflow));
    require_throws(NumUtils::strToInt(int_underflow));
    require_throws(NumUtils::strToInt(bad_number));
    require_throws(NumUtils::strToInt(not_an_int));
}

test_case("strToDouble"){
    std::string pass{"2.25"};
    std::string overflow{"100" + std::to_string(DBL_MAX)};
    std::string underflow("-100" + std::to_string(-DBL_MAX));
    std::string bad_num("20.25bad");
    std::string zero{"0.0"};
    std::string an_int{"50"};

    Approx tar_pass = Approx(2.25).epsilon(0.0001);
    Approx tar_zero = Approx(0.0).epsilon(0.0001);
    Approx tar_int = Approx(50.0).epsilon(0.0001);
    require(NumUtils::strToDouble(pass) == tar_pass);
    require(NumUtils::strToDouble(zero) == tar_zero);
    require(NumUtils::strToDouble(an_int) == tar_int);
    require_throws(NumUtils::strToDouble(overflow));
    require_throws(NumUtils::strToDouble(underflow));
    require_throws(NumUtils::strToDouble(bad_num));
}

test_case("sumOfDigits"){
    int positive{12345};
    int zero{0};
    int negative{-12345};

    require(NumUtils::sumOfDigits(positive) == 15);
    require(NumUtils::sumOfDigits(zero) == 0);
    require(NumUtils::sumOfDigits(negative) == -15);
}

test_case("fibonacci_lowmemory"){
    require(NumUtils::fibonacci_lowmemory(0) == 0);
    require(NumUtils::fibonacci_lowmemory(1) == 1);
    require(NumUtils::fibonacci_lowmemory(2) == 1);
    require(NumUtils::fibonacci_lowmemory(5) == 5);
    require(NumUtils::fibonacci_lowmemory(10) == 55);
    require(NumUtils::fibonacci_lowmemory(30) == 832040);
}

test_case("fibonacci"){
    require(NumUtils::fibonacci(0) == NumUtils::fibonacci_lowmemory(0));
    require(NumUtils::fibonacci(1) == NumUtils::fibonacci_lowmemory(1));
    require(NumUtils::fibonacci(2) == NumUtils::fibonacci_lowmemory(2));
    require(NumUtils::fibonacci(5) == NumUtils::fibonacci_lowmemory(5));
    require(NumUtils::fibonacci(10) == NumUtils::fibonacci_lowmemory(10));
    require(NumUtils::fibonacci(30) == NumUtils::fibonacci_lowmemory(30));
    require(NumUtils::fibonacci(40) == 102334155);
    require(NumUtils::fibonacci(50) == 12586269025);
    require(NumUtils::fibonacci(60) == 1548008755920);
    require(NumUtils::fibonacci(75) == 2111485077978050);
    require(NumUtils::fibonacci(90) == 2880067194370816120);
}

test_case("randomInt") {
    auto rand_small_range = 0;
    for (auto i = 0; i < 100; ++i) {
        rand_small_range = NumUtils::randomInt(1, 2);
        require(rand_small_range >= 1);
        require(rand_small_range <= 2);
    }

    auto rand_0 = 5;
    for(auto i = 0; i < 100; ++i){
        rand_0 = NumUtils::randomInt(0, 0);
        require(rand_0 == 0);
    }

    auto rand_larger_range = 0;
    for(auto i = 0; i < 100; ++i){
        rand_larger_range = NumUtils::randomInt(0, 10000);
        require(rand_larger_range >= 0);
        require(rand_larger_range <= 10000);
    }
}

test_case("factorial"){
    require(NumUtils::factorial(0) == 1);
    require(NumUtils::factorial(1) == 1);
    require(NumUtils::factorial(2) == 2);
    require(NumUtils::factorial(3) == 6);
    require(NumUtils::factorial(10) == 3628800);
    require(NumUtils::factorial(15) == 1307674368000);
}

test_case("getRadians"){
    require(NumUtils::getRadians(45) == Approx(0.78539).epsilon(0.1));
    require(NumUtils::getRadians(90) == Approx(1.5708).epsilon(0.1));
    require(NumUtils::getRadians(180) == Approx(3.1415).epsilon(0.1));
    require(NumUtils::getRadians(360) == Approx(6.2831).epsilon(0.1));
    require(NumUtils::getRadians(7200) == Approx(125.6637).epsilon(0.1));
}
