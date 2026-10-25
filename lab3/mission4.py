from z3 import *

x = Int("x")
y = x + 1
s = Solver()

s.add(Not(y > x))

result = s.check()
print(result)

if result == sat:
    print(s.model())
