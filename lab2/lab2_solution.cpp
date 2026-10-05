#include <vector>
/*
通过两层嵌套循环实现对CNF范式的赋值检查：
外层循环遍历每一个子句，默认子句未满足；
内层循环遍历子句中的每个文字(带有正负号的布尔变量),
    先将文字取绝对值并减一映射为变量在数组中的下标，读取对应的布尔真值，
    然后根据文字的正负号计算出该文字的实际真假（正文字保持原值，负文字取反）。
    只要找到一个为真的文字，即刻将当前子句标记为满足并break跳出内层循环（短路优化）；
若内层循环结束子句仍未满足，则函数立即返回false。
只有当所有子句都被成功满足时，才最终返回true，证明该赋值是CNF的一个合法解。
*/
bool satisfiesCnf(
    int variableCount,//变量总数
    const std::vector<std::vector<int>>& clauses,//子句
    const std::vector<int>& assignment) {//保存变量的真假值

    //遍历所有的子句
    for(const auto& clause : clauses){//clause：本轮循环取出的一个子句;auto：让编译器自动推断类型
        bool clauseSatisfied = false;//假设这个子句目前是不满足的
        //遍历当前子句中的所有文字
        for(int literal : clause){//literal是当前取出的一个整数文字。
            int variable = literal > 0 ? literal : -literal;//取出文字对应的变量编号
            bool value = assignment[variable - 1] == 1;//读取该变量的真假值
            bool literalSatisfied = literal > 0 ? value : !value;//根据正负号计算文字的真假
            if(literalSatisfied){//判断这个布尔变量是否为真
                clauseSatisfied = true;
                break;
            }
        }
        if(!clauseSatisfied){
              return false;
        }
    }
    return true;
}