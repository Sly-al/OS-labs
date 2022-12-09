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

    constexpr char* cor1  = "1012011202";
    EXPECT_STREQ(Translation(23456), cor1);

    constexpr char* cor2  = "10022210102112110002010100";
    EXPECT_STREQ(Translation(939272781231), cor2);
    
    constexpr char* cor3  = "120212122221120112";
    EXPECT_STREQ(Translation(227810003), cor3);

    constexpr char* cor4  = "212";
    EXPECT_STREQ(Translation(23), cor4);

    constexpr char* cor5  = "2012012222202120122202001011211210122102";
    EXPECT_STREQ(Translation(8888888888774271290), cor5);

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