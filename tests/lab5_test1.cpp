#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

extern "C" {
    #include <lab5.h>
}

const int ELEMENTS = 20;
const int TIMESTOTEST = 40;

void GenerateElements(std::vector<int> &correctVector){

    std::srand(std::time(nullptr));
    for (int i = 0; i < correctVector.size(); ++i){
        correctVector[i] = std::rand() % 1000;
    }
    return;
}

TEST(FiveLabTests, Translationfirst) { // проверка правильности работы функции 

    constexpr char* cor1  = "10011010010";
    EXPECT_STREQ(Translation(1234), cor1);

    constexpr char* cor2  = "1011101001000011101101100010110111111";
    EXPECT_STREQ(Translation(99999991231), cor2);
    
    constexpr char* cor3  = "10000111110010010000110000100000";
    EXPECT_STREQ(Translation(2278100000), cor3);

    constexpr char* cor4  = "1111";
    EXPECT_STREQ(Translation(15), cor4);

    constexpr char* cor5  = "110111100000101101101011001101111101101010101101100011111111";
    EXPECT_STREQ(Translation(999999999299999999), cor5);

}

TEST(FiveLabTests, SortOfArray) { // проверка правильности работы функции

    for(int j = 0; j < TIMESTOTEST; ++j){
        std::vector<int> genVector(ELEMENTS);
        std::vector<int> correctVector;
        int* p = &genVector[0];
        GenerateElements(genVector);
        correctVector.assign(genVector.begin(), genVector.end());
        Sort(p, ELEMENTS);
        sort(correctVector.begin(), correctVector.end());

        EXPECT_TRUE(correctVector == genVector);
    }
}