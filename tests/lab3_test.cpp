#include <gtest/gtest.h>

#include <lab3.h>
#include <utils.h>

#include <chrono>

const int THREE = 3;

namespace {
    TMatrix GenerateMatrix(int n, int m) {
        TMatrix result(n, std::vector<double>(m));

        std::srand(std::time(nullptr));

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                result[i][j] = std::rand() % 100;
            }
        }

        return result;
    }
}

bool operator==(const TMatrix& lhs, const TMatrix& rhs) {

    if(lhs.size() != rhs.size()) {
        return false;
    }

    for(int i = 0; i < isize(lhs); ++i) {
        if(lhs[i].size() != rhs[i].size()) {
            return false;
        }

        for(int j = 0; j < isize(lhs); ++j) {
            if(abs(lhs[i][j] - rhs[i][j]) > 1e-5) {
                return false;
            }
        }
    }

    return true;
}

bool EqualMatr(const TMatrix& lhs, const TMatrix& rhs) {

    if(lhs.size() != rhs.size()) {
        return false;
    }

    for(int i = 0; i < isize(lhs); ++i) {
        if(lhs[i].size() != rhs[i].size()) {
            return false;
        }

        for(int j = 0; j < isize(lhs); ++j) {
            if(abs(lhs[i][j] - rhs[i][j]) > 1e-5) {
                return false;
            }
        }
    }

    return true;
}

TEST(ThirdLabTests, SingleThreadYieldsCorrectResults) { // проверка правильности работы функции
    bool isSameMatr = EqualMatr(MatrixConvolution(TMatrix{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}},TMatrix{{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}}, 1, 1),
              (TMatrix{{0.888889, 1.33333, 0.888889}, {1.33333, 2, 1.33333}, {0.888889, 1.33333, 0.888889}}));
    EXPECT_TRUE(isSameMatr);
    
}

TEST(ThirdLabTest, ThreadConfigurations) { // проверка, что 1 тред считает также как и много тредов
    auto performTestForGivenSize = [](int n, int m, int maxThreadCount, int cycle) {
        auto m1 = GenerateMatrix(n, m);
        auto m2 = GenerateMatrix(THREE, THREE);
        auto result = MatrixConvolution(m1, m2, 1, cycle);

        for(int i = 2; i < maxThreadCount; ++i) {
            EXPECT_EQ(MatrixConvolution(m1, m2, i, cycle), result);
        }
    };

    performTestForGivenSize(3, 3, 2, 1);
    performTestForGivenSize(10, 10, 8, 3);
    performTestForGivenSize(100, 100, 10, 7);
    performTestForGivenSize(300, 300, 4, 8);
}

TEST(ThirdLabTest, PerfomanceTest) { // проверка среднего времени работы одного и нескольких тредов
    auto getAvgTime = [](int threadCount) {
        auto m1 = GenerateMatrix(300, 300);
        auto m2 = GenerateMatrix(THREE, THREE);

        constexpr int runsCount = 10;

        double avg = 0;

        for(int i = 0; i < runsCount; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();
            MatrixConvolution(m1, m2, threadCount, 2);
            auto end = std::chrono::high_resolution_clock::now();
            avg += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }

        return avg / runsCount;
    };

    auto singleThread = getAvgTime(1);
    auto multiThread = getAvgTime(4);

    std::cout << "Avg time for 1 thread: " << singleThread << '\n';
    std::cout << "Avg time for 4 threads: " << multiThread << '\n';

    EXPECT_GE(singleThread, multiThread);
}