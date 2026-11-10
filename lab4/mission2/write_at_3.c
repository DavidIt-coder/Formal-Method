#include <stddef.h>
/*@
    requires \valid(a + (0 .. n-1));
    assigns a[index];
    ensures a[index] == value;
*/
void write_at(int *a, size_t n, size_t index, int value) {
    a[index] = value;
}