/*@
  requires x < 2147483647;
  assigns \nothing;
  ensures \result == x + 1;
*/
int inc(int x) {
    return x + 1;
}
int main(void) {
    return inc(1) != 2;
}