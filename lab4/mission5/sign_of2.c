/*@
    requires x != 0;
    assigns \nothing;
    ensures x < 0  ==> \result == -1;
    ensures x == 0 ==> \result == 0;
    ensures x > 0  ==> \result == 1;
*/
int sign_of(int x) {
    if (x < 0) {
        return -1;
    }
    if (x > 0) {
        return 1;
    }
    return 0;
}

int main(void) {
    return sign_of(0);
}
