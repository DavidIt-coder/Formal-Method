/*@
    requires low <= high;
    assigns \nothing;
    ensures x < low ==> \result == low;
    ensures x > high ==> \result == high;
    ensures low <= x <= high ==> \result == x;
*/
int clamp_value(int x, int low, int high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}