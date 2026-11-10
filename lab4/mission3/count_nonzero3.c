#include <limits.h>
#include <stddef.h>
/*@
  requires n == 0 || \valid_read(a + (0 .. n-1));
  requires n <= INT_MAX;
  assigns \nothing;
  ensures 0 <= \result <= n;
*/
int count_nonzero(const int *a, size_t n) {
    size_t i = 0;
    int count = 0;
    /*@
        loop invariant 0 <= i <= n;
        loop invariant 0 <= count <= i;
        loop assigns i, count;
     */
    while (i < n) {
        if (a[i] != 0) {
            ++count;
        }
        ++i;
    }
    return count;
}