#include <iostream>
#include <utility>
#include <vector>

bool validSchedule(
    int slotCount,
    const std::vector<int>& slot,
    const std::vector<std::pair<int, int>>& conflicts);

int main() {
    const int slotCount = 3;
    const std::vector<std::pair<int, int>> conflicts = {
        {0, 1}, {0, 2}, {1, 3},
        {2, 3}, {2, 4}, {3, 5}, {4, 5}};

    const std::vector<std::vector<int>> tests = {
        {0, 1, 2, 0, 0, 1},  // 合法方案
        {3, 1, 2, 0, 0, 1},  // 时间段越界
        {0, 0, 2, 0, 0, 1},  // 冲突课程同时上课
        {0, 1, 1, 0, 0, 1}   // 非冲突课程共享时间段
    };

    const std::vector<bool> expected = {
        true, false, false, true
    };

    bool allPassed = true;
    std::cout << std::boolalpha;

    for (int i = 0; i < static_cast<int>(tests.size()); ++i) {
        bool actual = validSchedule(slotCount, tests[i], conflicts);
        bool passed = (actual == expected[i]);

        std::cout << "Test " << i + 1 << ": slot = [";
        for (int j = 0; j < static_cast<int>(tests[i].size()); ++j) {
            if (j > 0) {
                std::cout << ", ";
            }
            std::cout << tests[i][j];
        }
        std::cout << "]\n";

        std::cout << "  expected = " << expected[i]
                  << ", actual = " << actual
                  << ", " << (passed ? "PASS" : "FAIL") << "\n";

        if (!passed) {
            allPassed = false;
        }
    }

    return allPassed ? 0 : 1;
}