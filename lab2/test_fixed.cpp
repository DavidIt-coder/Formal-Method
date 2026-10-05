#include <iostream>
#include <vector>
#include "lab2_solution.cpp"

int main() {
    std::vector<std::vector<int>> clauses = {
        {1, 2}, {-1, 3}, {-2, -3}
    };

    std::cout << std::boolalpha;
    bool allMatch = true;

    // 分别检查 x3=true 和 x3=false，x1、x2 保持不变。
    for (int x3 : {1, 0}) {
        std::vector<int> assignment = {1, 0, x3};

        // 待验证函数的计算结果。
        bool actual = satisfiesCnf(3, clauses, assignment);

        // 独立按原始逻辑公式计算，不调用 satisfiesCnf。
        bool x1Value = assignment[0] == 1;
        bool x2Value = assignment[1] == 1;
        bool x3Value = assignment[2] == 1;

        bool c1 = x1Value || x2Value;
        bool c2 = !x1Value || x3Value;
        bool c3 = !x2Value || !x3Value;
        bool direct = c1 && c2 && c3;

        bool match = actual == direct;
        allMatch = allMatch && match;

        std::cout << "Assignment: "
                  << x1Value << ", "
                  << x2Value << ", "
                  << x3Value << '\n';

        std::cout << "Clause 1: " << c1 << '\n';
        std::cout << "Clause 2: " << c2 << '\n';
        std::cout << "Clause 3: " << c3 << '\n';

        std::cout << "Direct evaluation: " << direct << '\n';
        std::cout << "satisfiesCnf: " << actual << '\n';
        std::cout << "Match: " << match << "\n\n";
    }

    return allMatch ? 0 : 1;
}