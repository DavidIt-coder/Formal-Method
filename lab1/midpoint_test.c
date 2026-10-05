#include <stdio.h>

int midpoint(int low, int high) {
 return low + (high - low) / 2;
}

int main(void) {
    int low;
    int high;
    if (scanf("%d%d", &low, &high) != 2) {
        return 1;
    }
    if (low < 0 || low > high) {
        puts("invalid input");
        return 0;
    }
    printf("%d\n", midpoint(low, high));

    /* 任务 2 的反例：固定回归测试 */
    int regression_low = 177393664;
    int regression_high = 1981235200;
    int expected = 1079314432;
    int actual = midpoint(regression_low, regression_high);
    printf("regression input: (%d, %d)\n", regression_low, regression_high);
    printf("expected: %d, actual: %d\n", expected, actual);
    printf("%s\n", actual == expected ? "PASS" : "FAIL");
    
    return 0;
}
