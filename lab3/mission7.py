from z3 import *

s0 = Int("slot[0]")
s1 = Int("slot[1]")
s2 = Int("slot[2]")
s3 = Int("slot[3]")
s4 = Int("slot[4]")
s5 = Int("slot[5]")

s = Optimize()

s.add(s0 >= 0, s0 <= 2)
s.add(s1 >= 0, s1 <= 2)
s.add(s2 >= 0, s2 <= 2)
s.add(s3 >= 0, s3 <= 2)
s.add(s4 >= 0, s4 <= 2)
s.add(s5 >= 0, s5 <= 2)

s.add(s0 != s1)  # 冲突 (0,1)
s.add(s0 != s2)  # 冲突 (0,2)
s.add(s1 != s3)  # 冲突 (1,3)
s.add(s2 != s3)  # 冲突 (2,3)
s.add(s2 != s4)  # 冲突 (2,4)
s.add(s3 != s5)  # 冲突 (3,5)
s.add(s4 != s5)  # 冲突 (4,5)

used0 = Bool("used[0]")
used1 = Bool("used[1]")
used2 = Bool("used[2]")

# used[t] == Or(slot[i] == t)
s.add(used0 == Or(s0 == 0, s1 == 0, s2 == 0, s3 == 0, s4 == 0, s5 == 0))
s.add(used1 == Or(s0 == 1, s1 == 1, s2 == 1, s3 == 1, s4 == 1, s5 == 1))
s.add(used2 == Or(s0 == 2, s1 == 2, s2 == 2, s3 == 2, s4 == 2, s5 == 2))

# Sum(If(used[t], 1, 0))
objective = Sum(If(used0, 1, 0), If(used1, 1, 0), If(used2, 1, 0))

s.minimize(objective)

print(s.check())
print(s.model())       # 最优方案
print(s.model().eval(objective).as_long()) # 最优值（最少时间段数）
