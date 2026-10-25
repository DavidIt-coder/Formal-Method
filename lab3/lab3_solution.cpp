#include <utility>
#include <vector>

bool validSchedule(
    int slotCount,
    const std::vector<int>& slot,
    const std::vector<std::pair<int, int>>& conflicts) {
    //1. 检查每门课程的时间段编号是否有效
    for(int s : slot){
        if(s < 0 || s >= slotCount)
            return false;
    }
    //2. 检查每对冲突课程是否位于不同时间段
    for(const auto& conflict : conflicts){
        if(slot[conflict.first] == slot[conflict.second])
            return false;
    }
    return true;
}