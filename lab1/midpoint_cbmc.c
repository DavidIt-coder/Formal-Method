extern int nondet_int(void);

int midpoint(int low, int high) {
 return low + (high - low) / 2;
}

int main(void) {
    int low = nondet_int();
    int high = nondet_int();
    __CPROVER_assume(0 <= low && low <= high);
    int mid = midpoint(low, high);
    __CPROVER_assert(//检查结果是否仍在原区间中
        low <= mid && mid <= high,
        "midpoint is inside the interval");
    __CPROVER_assert(//检查结果与 low 的距离是否等于区间长度的一半
        mid - low == (high - low) / 2,
        "midpoint has the expected offset");
    return 0;
}