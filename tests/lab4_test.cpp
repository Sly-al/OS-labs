#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>

namespace fs = std::filesystem;

extern "C" {
    #include <lab4.h>
}

void RunTest(const std::vector<const char*>& input, const std::vector<long long>& expectedOutput) {
    const char* fileWithInput = "input.txt";
    const char* fileWithOutput = "result.txt";
    {
        auto inFile = std::ofstream(fileWithInput);

        for(const auto& line : input) {
            inFile<< line << '\n';
        }
    }

    auto deleter = [](FILE* file) {
        fclose(file);
    };

    std::unique_ptr<FILE, decltype(deleter)> inFile(fopen(fileWithInput, "r"), deleter);

    ParentRoutine(inFile.get());


    auto outFile = std::ifstream(fileWithOutput);

    ASSERT_TRUE(outFile.good());
    
    long long result;

    for(long long i : expectedOutput) {
        outFile >> result;
        EXPECT_EQ(result, i);
    }

     auto removeIfExists = [](const char* path) {
         if(fs::exists(path)) {
             fs::remove(path);
        }
    };
     
    removeIfExists(fileWithInput);
    removeIfExists(fileWithOutput);
    return;
}

TEST(FirstLabTests, TerminateOnNegative) {

    std::vector<const char*> input = {
            "800000",
            "4",
            "408",
            "4972627365458",
            "1234567891234",
            "1000000000000008",
            "9999999999999999",
            "-1337",
            "15",
            "30"
    };

    std::vector<long long> expectedOutput = {
            800000,
            4,
            408,
            4972627365458,
            1234567891234,
            1000000000000008,
            9999999999999999
    };

    RunTest(input, expectedOutput);
    
}

TEST(FirstLabTests, TerminateOnPrime){
    
     std::vector<const char*> input = {
            "80000110",
            "412",
            "408000915",
            "49726273654587778",
            "123456789123435",
            "199999999999000008",
            "777777777777775",
            "99377",
            "18",
            "39"
    };

    std::vector<long long> expectedOutput = {
            80000110,
            412,
            408000915,
            49726273654587778,
            123456789123435,
            199999999999000008,
            777777777777775
    };

    RunTest(input, expectedOutput);
}

TEST(FirstLabTests, TerminateOnEnd) {

    std::vector<const char*> input = {
            "806793",
            "840525306",
            "108496079",
            "29317987",
            "888368849",
            "5316212129",
            "8644366817"
    };

    std::vector<long long> expectedOutput = {
            806793,
            840525306,
            108496079,
            29317987,
            888368849,
            5316212129,
            8644366817
    };

    RunTest(input, expectedOutput);
    
}