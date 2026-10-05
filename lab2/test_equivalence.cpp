#include<iostream>
#include<vector>
#include "lab2_solution.cpp"

bool check_equivalence(int variableCount,
    const std::vector<std::vector<int>>& clausesA,
    const std::vector<std::vector<int>>& clausesB){
    int modelCountA = 0;
    int modelCountB = 0;
    //记录有多少个区分赋值
    int differenceCount = 0;

    std::cout << std::boolalpha;

    //计算循环总数，2^variableCount次
    int total = 1 << variableCount;
    //i可以表示0~2^variableCount-1，
    //转换成二进制的话可覆盖total种取值
    for(int i = 0;i < total;i++){
        std::vector<int> assignment(variableCount);
        for(int j = 0;j < variableCount;j++){
            //将读取的那一位移到最右边，只保留最后一位
            assignment[j] = (i >> j) & 1;
        }
        bool resultA = satisfiesCnf(variableCount, clausesA, assignment);
        bool resultB = satisfiesCnf(variableCount, clausesB, assignment);
        
        if(resultA)
            modelCountA++;
        if(resultB)
            modelCountB++;
        if(resultA != resultB){
            differenceCount++;
            std::cout << "Distinguishing assignment (A W D C L M): "; 
            for(int value : assignment)
                std::cout << value << ' ';
            std::cout << "| encoding A=" << resultA
                      << " | encoding B=" << resultB << '\n';
        }
    }

    bool equivalent = differenceCount == 0;
    std::cout << "Assignments checked: " << total << '\n';
    std::cout << "Models of encoding A: " << modelCountA << '\n';
    std::cout << "Models of encoding B: " << modelCountB << '\n';
    std::cout << "Distinguishing assignments: " << differenceCount << '\n';
    std::cout << "Equivalent: " << equivalent << '\n';

    return equivalent;
}

int main(){
    const std::vector<std::vector<int>> clausesA = {
        {1, 2},
        {-1, 3},
        {-3, 1},
        {-2, 4},
        {-4, 3},
        {-3, 5},
        {-6, 5},
        {-5, 6}
    };
    const std::vector<std::vector<int>> clausesB = {
        {-4, 3},
        {-3, 5},
        {-6, 5},
        {-5, 6},
        {1, 2, 4},
        {1, 2, -4},
        {-1, 3, 6},
        {-1, 3, -6},
        {-3, 1, 2},
        {-3, 1, -2},
        {-2, 4, 5},
        {-2, 4, -5}
    };

    check_equivalence(6, clausesA, clausesB);

    return 0;
}
