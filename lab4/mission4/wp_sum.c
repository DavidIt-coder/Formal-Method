/*@
    requires n <= 8;
    requires n == 0 || \valid_read(a + (0 .. n - 1));
    requires \forall integer i;
        0 <= i < n ==> -100 <= a[i] <= 100;
    assigns \nothing;
    ensures -800 <= \result <= 800;
*/
int sum_prefix(const int *a, unsigned n) {
    int sum = 0;
    /*@
        loop invariant 0 <= i <= n;
        loop invariant -100 * i <= sum <= 100 * i;
        loop assigns i, sum;
        loop variant n - i;
    */
    for (unsigned i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}