from z3 import Int, Solver, sat

s0 = Int("slot[0]")
s1 = Int("slot[1]")
s2 = Int("slot[2]")
s3 = Int("slot[3]")
s4 = Int("slot[4]")
s5 = Int("slot[5]")

def check_schedule(slot_count):
    # 每次调用都创建新的求解器，使两次检查相互独立
    s = Solver()

    # 合法时间段：0 到 slot_count - 1
    for course in [s0, s1, s2, s3, s4, s5]:
        s.add(course >= 0, course < slot_count)

    # 原来的七条冲突
    s.add(s0 != s1)
    s.add(s0 != s2)
    s.add(s1 != s3)
    s.add(s2 != s3)
    s.add(s2 != s4)
    s.add(s3 != s5)
    s.add(s4 != s5)

    # 新增冲突
    s.add(s0 != s3)

    result = s.check()
    print(slot_count, "个时间段：", result)

    if result == sat:
        model = s.model()
        print("模型：", model)

        schedule = [
            model.eval(s0).as_long(),
            model.eval(s1).as_long(),
            model.eval(s2).as_long(),
            model.eval(s3).as_long(),
            model.eval(s4).as_long(),
            model.eval(s5).as_long()
        ]
        print("slot =", schedule)


check_schedule(2)
check_schedule(3)