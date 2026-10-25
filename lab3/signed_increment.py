from z3 import *

x = BitVec("x", 8)
y = x + 1
s = Solver()

s.add(Not(y > x))

print(s.check())
print(s.model())