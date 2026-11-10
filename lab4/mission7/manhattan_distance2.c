#include <stdlib.h>

/*@
  requires -1000 <= x1 <= 1000;
  requires -1000 <= y1 <= 1000;
  requires -1000 <= x2 <= 1000;
  requires -1000 <= y2 <= 1000;
  assigns \nothing;
  ensures \result == \abs(x1 - x2) + \abs(y1 - y2);
  ensures (x1 == x2 && y1 == y2) ==> \result == 0;
  ensures 0 <= \result <= 4000;
*/

int manhattan_distance(int x1, int y1, int x2, int y2) {
    /* 坐标已在 [-1000, 1000]，差值最大 2000，|差|之和最大 4000，
       远小于 int 上限，不会溢出。 */
    int dx = x1 - x2;
    int dy = y1 - y2;

    /* 用 long long 仅为形式上的安全，实际 int 足够 */
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    return dx + dy;
}