#include <iostream>
#include <vector>
#include "lab2_solution.cpp"

int main() {
    // 每个测试记录：变量数、子句、赋值和预期结果。
    struct TestCase {
        int variableCount;
        std::vector<std::vector<int>> clauses;
        std::vector<int> assignment;
        bool expected;
    };

    std::vector<TestCase> tests = {
        {1, {{1}}, {1}, true},
        {1, {{1}}, {0}, false},
        {1, {{-1}}, {0}, true},
        {1, {{-1}}, {1}, false},
        {3, {{1, -2}, {-1, 3}, {2, 3}}, {1, 0, 1}, true},
        {3, {{1, -2}, {-1, 3}, {2, 3}}, {1, 0, 0}, false}
    };

    int passed = 0;
    std::cout << std::boolalpha;

    for (int i = 0; i < static_cast<int>(tests.size()); ++i) {
        const auto& test = tests[i];

        // 实际调用函数，得到检查结果。
        bool actual = satisfiesCnf(
            test.variableCount,
            test.clauses,
            test.assignment
        );

        bool match = actual == test.expected;

        if (match) {
            ++passed;
        }

        std::cout << "Test " << i + 1
                  << ": expected=" << test.expected
                  << ", actual=" << actual
                  << ", " << (match ? "PASS" : "FAIL")
                  << '\n';
    }

    std::cout << "Passed: " << passed
              << "/" << tests.size() << '\n';

    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}