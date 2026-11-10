/*@
  assigns \nothing;
  ensures -1 <= \result && \result <= 1;
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
