#include <stddef.h>
/*@
    requires 0 <= index && index < n;
    requires \valid(a + index);
    assigns a[index];
    ensures a[index] == value;
*/
void write_at(int *a, size_t n, size_t index, int value) {
    a[index] = value;
}