from z3 import Int, Solver

x = Int("x")
s = Solver()
s.add(x > 1, x < 3)

assert str(s.check()) == "sat"
assert s.model()[x].as_long() == 2
print("Z3 smoke test passed")