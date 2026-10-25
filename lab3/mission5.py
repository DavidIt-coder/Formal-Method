from z3 import *

Module, (Parser, Database, UI) = EnumSort(
 "Module", ["Parser", "Database", "UI"])

Tested = Function("Tested", Module, BoolSort())
Releasable = Function("Releasable", Module, BoolSort())
m = Const("m", Module)

base_constraints = [
    Tested(Parser),
    Not(Tested(Database)),
    Tested(UI),
    ForAll([m], Implies(Tested(m), Releasable(m))),
    ForAll([m], Implies(Releasable(m), Tested(m)))
]

def check_property(name, constraints, property_to_check):

    s = Solver()
    s.add(constraints)

    s.add(Not(property_to_check))

    result = s.check()
    print(name, ":", result)

    if result == sat:
        model = s.model()

        for module in [Parser, Database, UI]:
            print(
                module,
                "Tested =", model.eval(Tested(module)),
                "Releasable =", model.eval(Releasable(module))
            )


p1 = Exists([m], Releasable(m))
p2 = Not(Releasable(Database))
p3 = ForAll([m], Releasable(m))

check_property("存在可发布模块", base_constraints, p1)
check_property("Database 不可发布", base_constraints, p2)
check_property("所有模块都可发布", base_constraints, p3)