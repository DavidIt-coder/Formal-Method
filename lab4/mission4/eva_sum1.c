#include "__fc_builtin.h"
int sum_prefix(const int *a, unsigned n) {
    int sum = 0;
    for (unsigned i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}
int main(void) {
    int a[8];
    for (unsigned i = 0; i < 8; ++i) {
        a[i] = Frama_C_interval(-1000000000, 1000000000);
    }
    unsigned n = (unsigned)Frama_C_interval(0, 8);
    return sum_prefix(a, n);
}