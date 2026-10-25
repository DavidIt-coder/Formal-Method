from z3 import *
api_replicas = Int("api_replicas")# API 副本数
worker_replicas = Int("worker_replicas")# 任务处理器副本数
database_replicas = Int("database_replicas")# 数据库副本数
standby_nodes = Int("standby_nodes")# 预留备用节点数
total_nodes = Int("total_nodes")# 总节点数
monitoring = Bool("monitoring")# 是否启用监控
audit = Bool("audit")# 是否启用审计
domain_constraints = [
    api_replicas >= 0,
    worker_replicas >= 0,
    database_replicas >= 0,
    standby_nodes >= 0,
    total_nodes >= 0
]
requirements = [
    ("api_high_availability", api_replicas >= 2), # API ⾄少需要两个副本
    ("worker_matches_api", worker_replicas >= api_replicas), # 每个 API 副本都需要⼀个任务处理器副本
    ("database_high_availability", database_replicas >= 2), # 数据库⾄少需要两个副本
    ("standby_required", standby_nodes >= 1), # 系统还必须预留⼀个备⽤节点
    ("one_service_per_node",
    total_nodes == api_replicas + worker_replicas
                    + database_replicas + standby_nodes), # 每个服务副本和备⽤节点都独占⼀个节点
    ("maintenance_capacity", total_nodes <= 6), # 维护期间最多只有六个节点可⽤
    ("monitoring_required", monitoring), # 系统另外要求启⽤监控
    ("monitoring_requires_audit", Implies(monitoring, audit)) # 启⽤监控时必须启⽤审计
]

print("逐条删减检查：")

for removed_name, _ in requirements:
    # 每次创建新的求解器
    test_solver = Solver()

    # 保留全部领域约束
    test_solver.add(domain_constraints)

    # 只删除当前要求，其余要求全部保留
    for name, constraint in requirements:
        if name != removed_name:
            test_solver.add(constraint)

    result = test_solver.check()
    print("删除", removed_name, "后：", result)