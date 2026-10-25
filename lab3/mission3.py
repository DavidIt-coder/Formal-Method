from z3 import Int, Solver, sat

s0 = Int("slot[0]")
s1 = Int("slot[1]")
s2 = Int("slot[2]")
s3 = Int("slot[3]")
s4 = Int("slot[4]")
s5 = Int("slot[5]")

s = Solver()

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

result = s.check()
print(result)

if result == sat:
    model = s.model()
    print("model = ", model)

    schedule = [
        model.eval(s0).as_long(),
        model.eval(s1).as_long(),
        model.eval(s2).as_long(),
        model.eval(s3).as_long(),
        model.eval(s4).as_long(),
        model.eval(s5).as_long()
    ]

    print("slot =", schedule)

    for t in range(3):
        courses = [
            i for i in range(6)
            if schedule[i] == t
        ]
        print("时间段", t, "：课程", courses)