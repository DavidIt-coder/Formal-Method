#include <iostream>
#include <vector>
#include "lab2_solution.cpp"

int main() {
    // 只检查原始四条业务规则，不加入阻塞子句
    const std::vector<std::vector<int>> clauses = {
        {1, 2},
        {-3, 1},
        {-4, 2},
        {-3, -4}
    };
    // 每行依次表示U、I、S、C，1为真，0为假
    const std::vector<std::vector<int>> models = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 1, 0, 1}
    };

    bool allPassed = true;
    //让bool类型以true/false的形式输出，而不是1/0
    std::cout << std::boolalpha;
    for (int i = 0; i < static_cast<int>(models.size()); i++){
        // 检查当前模型是否满足原始CNF
        bool satisfies = satisfiesCnf(4, clauses, models[i]);
        // 将当前模型与此前的每一个模型比较
        bool isNew = true;
        for (int j = 0; j < i; j++){
            if (models[i] == models[j]) {
                isNew = false;
                break;
            }
        }
        //
        std::cout << "Model " << i + 1 << ": ";
        for (int value : models[i]) {
            std::cout << value << ' ';
        }
        std::cout << "| satisfies=" << satisfies
                  << " | new=" << isNew << '\n';
        //只要有一项不通过，整体就记为失败
        if (!satisfies || !isNew) {
            allPassed = false;
        }
    }
    std::cout << "All checks passed: " << allPassed << '\n';
    return allPassed ? 0 : 1;
}