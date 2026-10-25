# Formal Methods Experiments

本仓库保存形式化方法课程的实验代码、实验报告、任务书和学习回顾。

## 实验列表

| 实验 | 主题 | 实验报告 | 学习回顾 |
| --- | --- | --- | --- |
| Lab 1 | 从软件测试到形式化验证 | [lab1/report.md](lab1/report.md) | — |
| Lab 2 | 命题逻辑与 SAT 求解 | [lab2/report.md](lab2/report.md) | [labReview/review2.md](labReview/review2.md) |
| Lab 3 | 谓词逻辑与 SMT 求解 | [lab3/report.md](lab3/report.md) | [labReview/lab3_review.md](labReview/lab3_review.md) |

## 项目结构

```text
Formal-Method/
├── lab1/                           # 实验 1：测试与形式化验证
│   ├── report.md
│   ├── midpoint_test.c
│   ├── midpoint_cbmc.c
│   ├── cbmc_smoke.c
│   └── image*.png                  # 报告截图
├── lab2/                           # 实验 2：命题逻辑与 SAT 求解
│   ├── report.md                  # 完整实验报告
│   ├── minisat_smoke.cnf           # MiniSAT 环境检查，当前为 -1 0 版本
│   ├── base.cnf                    # 任务 1：基础配置模型
│   ├── server_only.cnf             # 任务 1：强制启用服务器模式
│   ├── no_persistence.cnf          # 任务 1：禁止持久化
│   ├── honesty.cnf                 # 任务 2：诚实者与说谎者
│   ├── honesty_blocked.cnf         # 任务 2：排除唯一模型后的版本
│   ├── SelectClassPrompt.txt       # 任务 3：实际使用的选课建模提示词
│   ├── selection.cnf               # 任务 3：16 个变量、18 条子句的选课模型
│   ├── lab2_solution.cpp           # 任务 4：satisfiesCnf 函数，无 main
│   ├── test_42.cpp                 # 任务 4.2：六个批量测试
│   ├── test_fixed.cpp              # 任务 4.3：固定赋值与单变量翻转检查
│   ├── solution_explain.txt        # 函数学习笔记，不是独立可编译程序
│   ├── test_strategy.cnf           # 任务 5：原始规则 + 七条阻塞子句
│   ├── test_strategy2.cnf          # 任务 5：静态分析规则 + 两条阻塞子句
│   ├── test_strategy_check.cpp     # 任务 5.3：验证七个方案并检查重复
│   ├── encoding_a.cnf              # 任务 6：原始编码 A，8 条子句
│   ├── encoding_b.cnf              # 任务 6：原始编码 B，12 条子句
│   ├── encoding_b2.cnf             # 任务 6.4：删除一条子句后的 B，11 条子句
│   ├── test_equivalence.cpp        # 任务 6.2：穷举 64 种赋值比较原始 A/B
│   ├── *_result.txt                # 保存的求解结果，具体对应关系见下表
│   └── image*.png                  # 求解、测试和建模过程截图
├── lab3/                           # 实验 3：谓词逻辑与 SMT 求解
│   ├── report.md                   # 实验报告，包含运行结果和分析
│   ├── z3_smoke.py                 # Z3Py 环境检查
│   ├── mission1.py                 # 任务 1：布尔 CNF 部署模型
│   ├── mission1_2.py               # 任务 1：整数 SMT 部署模型
│   ├── lab3_solution.cpp           # 任务 2：validSchedule 排课检查函数
│   ├── test2.cpp                   # 任务 2：四类排课测试
│   ├── mission3.py                 # 任务 3：课程冲突排课求解
│   ├── mission3_2.py               # 任务 3：新增冲突后比较两个和三个时间段
│   ├── unsigned_increment.py       # 任务 4：8 位无符号加法反例
│   ├── signed_increment.py         # 任务 4：8 位有符号加法反例
│   ├── mission4.py                 # 任务 4：数学整数加法性质检查
│   ├── mission5.py                 # 任务 5：谓词、量词与模块发布性质检查
│   ├── mission5_2.py               # 任务 5：修改数据库测试状态后的性质检查
│   ├── deployment_core.py          # 任务 6：部署冲突的不可满足核
│   ├── mission6.py                 # 任务 6：逐条删除要求并重新求解
│   ├── mission7.py                 # 任务 7：最少时间段排课优化
│   ├── mission7_2.py               # 任务 7：新增冲突 (1,2) 后重新优化
│   └── images/                     # 报告截图
├── labManual/                      # 实验任务书
│   ├── 实验1_从软件测试到形式化验证_实验任务.pdf
│   ├── 实验2_命题逻辑与SAT求解_实验任务.pdf
│   └── 实验3_谓词逻辑与SMT求解_实验任务.pdf
├── labReview/                      # 实验过程回顾与知识总结
│   ├── review2.md
│   └── lab3_review.md              # 实验 3 的运行方法、操作过程和建模思路
├── .gitignore
└── README.md
```


## 实验环境

- Windows、PowerShell、Visual Studio Code。
- WSL / Ubuntu：运行 MiniSAT 和 Linux 下的编译测试。
- GCC / G++，实验 2、3 的 C++ 程序采用 C++17。
- CBMC 6.11：实验 1 的有界模型检查。
- MiniSAT：实验 2 的 SAT 求解。
- Python、Z3Py（`z3-solver`）：实验 3 的 SMT 建模、性质验证、不可满足核分析和优化求解。
