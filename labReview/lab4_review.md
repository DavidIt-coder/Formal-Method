# Lab 4 实验回顾：程序规约与定理证明

实验报告：[lab4/report.md](../lab4/report.md)

本文整理 Lab 4 的运行方法、七个任务的验证主线、关键知识和常见错误。实验使用 ACSL 描述 C 程序契约，使用 Frama-C/WP 证明契约与运行时安全性质，使用 Eva 分析可能的数值范围和告警，并使用 Rocq 完成交互式定理证明。

## 1. 整体思路

实验主线是：自然语言需求 → 形式化规约 → 证明义务或抽象解释 → 分析成功目标与失败目标 → 判断问题来自实现、规约还是证明信息。

| 任务 | 核心问题 | 主要方法 |
| --- | --- | --- |
| 1 | 区间截断实现是否满足三种输入区域的要求？ | `requires`、分情况 `ensures`、错误实现对照 |
| 2 | 数组写入为什么需要合法地址和副作用边界？ | `\valid`、`assigns`、范围条件 |
| 3 | 如何证明循环范围、结果范围和终止性？ | 循环不变式、`loop assigns`、`loop variant` |
| 4 | 数组越界和整数溢出告警如何随输入范围变化？ | Eva 值分析与 WP 边界证明 |
| 5 | 为什么证明成功不等于规约完整？ | 规约削弱、前置条件收紧、边界变异 |
| 6 | 如何证明过滤后的列表长度不增加？ | Rocq 列表归纳、分类讨论、`lia` |
| 7 | 如何检查 LLM 生成的 C 程序？ | 独立需求、ACSL 契约、WP 与 Eva |

需要始终区分三件事：

- 程序安全：是否会越界、溢出或访问无效内存。
- 程序满足规约：在前置条件成立时，是否满足后置条件和副作用约束。
- 规约符合需求：契约是否完整、准确地表达了自然语言要求。

前两项可以由工具帮助检查，第三项仍需要人工判断。

## 2. 环境与运行方式

Frama-C 和 Rocq 安装在 WSL 的 opam `lab4` switch 中。进入实验目录后可运行：

```bash
cd /mnt/d/ME/FormalMethods/lab4
eval $(opam env --switch=lab4 --set-switch)
```

常用命令：

```bash
# WP：验证契约并生成运行时错误证明义务
frama-c -wp -wp-rte file.c

# Eva：进行抽象解释和值域分析
frama-c -eva -eva-slevel 20 file.c

# 只验证指定函数
frama-c -wp -wp-rte -wp-fct function_name file.c

# 编译 Rocq 证明
rocq compile file.v
```

WP 输出中的 `Proved goals: n / n` 表示所有生成的证明目标均已证明。`Timeout` 或 `Unknown` 只能说明目标未被当前证明过程证明，仍需结合代码和规约判断它是实际错误还是证明信息不足。

Eva 的 `alarm` 表示可能存在运行时问题；变量的区间输出属于抽象值结果，不是额外的运行时错误告警。Eva 没有产生 alarm，表示在当前入口状态和分析精度下未发现相关错误，并不自动等价于业务需求正确。

### 2.1 从仓库根目录开始的完整命令

如果当前位于仓库根目录，可按以下方式进入每个任务：

```bash
cd /mnt/d/ME/FormalMethods/lab4

# 查看工具版本
frama-c -version
rocq -v

# 分别进入任务目录
cd mission1   # 任务 1
cd ../mission2
cd ../mission3
cd ../mission4
cd ../mission5
cd ../mission6
cd ../mission7
```

如果新开的 WSL 终端找不到命令，先执行：

```bash
eval $(opam env --switch=lab4 --set-switch)
```

命令成功时通常通过退出码和摘要判断。WP 重点看 `Proved goals`；Eva 重点看 `alarms generated` 和最终变量区间；Rocq 成功编译时通常没有错误信息并生成 `.vo` 等编译产物。

## 3. 任务 1：区间截断契约

函数把输入 `x` 限制在闭区间 `[low, high]` 内。首先划分三个互斥且完备的输入区域：

| 输入区域 | 返回值 |
| --- | --- |
| `x < low` | `low` |
| `low <= x <= high` | `x` |
| `x > high` | `high` |

契约的核心结构是：

```c
/*@
  requires low <= high;
  assigns \nothing;
  ensures x < low ==> \result == low;
  ensures low <= x <= high ==> \result == x;
  ensures x > high ==> \result == high;
*/
```

正确实现全部目标通过。把 `x > high` 分支错误地改成返回 `low` 后，对应后置条件无法证明。这说明契约描述的是外部行为，而不是某一种代码写法；同一契约可以区分满足需求和违反需求的实现。

### 3.1 代码分析

- `clamp_value.c` 是正确版本。第一个 `if` 处理下界外输入，第二个 `if` 处理上界外输入，最后的 `return x` 处理闭区间内部及两个端点。
- `clamp_value_wrong.c` 只把 `x > high` 分支改成返回 `low`，其他代码和契约保持不变。这样可以把证明结果的变化唯一归因到这一处实现变异。
- `assigns \nothing` 表示函数不修改调用前可观察的内存状态；局部控制流和返回值不违反该约束。

### 3.2 运行命令与结果

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission1

# 正确实现：预期全部通过
frama-c -wp -wp-rte clamp_value.c

# 错误实现：预期 x > high 对应的 ensures 无法证明
frama-c -wp -wp-rte clamp_value_wrong.c
```

本实验记录中，正确版本为 `8 / 8`；错误版本为 `7 / 8`，未证明目标是 `typed_clamp_value_ensures_2`。目标名称中的编号用于区分生成的后置条件，最终仍应回到契约原文判断它对应哪条需求。

## 4. 任务 2：内存有效性与副作用

数组写入需要回答三个不同问题：

```c
requires index < n;          // index 是否属于逻辑数组范围
requires \valid(a + index);  // 目标元素是否存在且可写
assigns a[index];            // 函数允许修改哪些内存
```

只有 `ensures a[index] == value` 不能保证写入本身合法。`\valid` 描述可访问性，`assigns` 描述副作用边界，两者不能替代。

整段条件 `\valid(a + (0 .. n-1))` 要求整个数组区域可写；单元素条件 `\valid(a + index)` 只要求实际访问的位置可写，通常更贴近本函数的真实需求。删除 `index < n` 会失去下标与数组长度的关系；删除 `assigns` 后功能目标可能仍能证明，但调用者无法从契约得知函数只修改了目标元素。

### 4.1 文件与代码分析

| 文件 | 作用 | 预期现象 |
| --- | --- | --- |
| `write_at.c` | 只有功能后置条件的初始版本 | 无法证明内存访问安全，且缺少 `assigns` |
| `write_at_new.c` | 整段数组有效、下标合法、限制副作用 | 完整契约应通过 |
| `write_at_2.c` | 把整段有效性改为单元素有效性 | 更精确描述实际访问，完整契约应通过 |
| `write_at_3.c` | 删除 `index < n` | 无法从整段有效性推出 `a[index]` 合法 |
| `write_at_4.c` | 删除 `assigns a[index]` | 功能目标可通过，但出现副作用说明不精确的警告 |

函数体只有一条 `a[index] = value`，但这一条语句同时依赖下标语义、地址有效性和写权限。后置条件中的 `a[index]` 还需要在函数返回后合法读取，因此不能只写结果相等而忽略内存前提。

### 4.2 运行命令与观察顺序

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission2

frama-c -wp -wp-rte write_at.c
frama-c -wp -wp-rte write_at_new.c
frama-c -wp -wp-rte write_at_2.c
frama-c -wp -wp-rte write_at_3.c
frama-c -wp -wp-rte write_at_4.c
```

建议按上述顺序比较：先看到初始版本缺少什么，再观察补全契约后的变化，最后通过删除一条约束判断每条契约的独立作用。`size_t index` 天然非负，所以 `0 <= index` 可以帮助阅读，但在 C 类型语义下是冗余信息。

## 5. 任务 3：循环不变式与终止性

`count_nonzero` 的关键循环注解为：

```c
/*@
  loop invariant 0 <= i <= n;
  loop invariant 0 <= count <= i;
  loop assigns i, count;
  loop variant n - i;
*/
```

各项作用如下：

- `0 <= i <= n`：限制索引范围，支持数组访问和循环退出推理。
- `0 <= count <= i`：把计数与已处理元素数联系起来，支持结果范围和无溢出证明。
- `loop assigns i, count`：说明循环只修改索引和计数，其他状态保持不变。
- `loop variant n - i`：始终非负并在每轮严格减小，用于证明终止。

前置条件 `n <= INT_MAX` 与计数不变式组合后得到 `count <= i <= n <= INT_MAX`。删除计数不变式后，整数溢出和最终后置条件会再次失去必要的证明信息。

### 5.1 代码如何逐步增加证明信息

| 文件 | 新增注解 | 主要作用 |
| --- | --- | --- |
| `count_nonzero.c` | 无循环注解 | 初始对照版本 |
| `count_nonzero1.c` | `0 <= i <= n` | 给出索引范围 |
| `count_nonzero2.c` | 再加 `0 <= count <= i` | 给出计数与进度关系 |
| `count_nonzero3.c` | 再加 `loop assigns i, count` | 限制循环副作用 |
| `count_nonzero4.c` | 再加 `loop variant n - i` | 证明循环终止，最终完整版本 |
| `count_nonzero5.c` | 从完整版本删除计数不变式 | 观察溢出与后置条件再次失败 |

循环体每次读取 `a[i]`，若元素非零则将 `count` 增加 1，最后将 `i` 增加 1。因此进入下一轮时，`i` 恰好等于已经处理的元素数量，而非零计数最多为 `i`。这就是 `count <= i` 的程序含义。

### 5.2 运行命令与预期变化

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission3

frama-c -wp -wp-rte count_nonzero.c
frama-c -wp -wp-rte count_nonzero1.c
frama-c -wp -wp-rte count_nonzero2.c
frama-c -wp -wp-rte count_nonzero3.c
frama-c -wp -wp-rte count_nonzero4.c
frama-c -wp -wp-rte count_nonzero5.c
```

实验记录中，完整版本 `count_nonzero4.c` 为 `13 / 13`。删除计数不变式后的 `count_nonzero5.c` 为 `9 / 11`，重新失去 `rte_signed_overflow` 和最终 `ensures` 的证明。前几个版本目标总数会随注解增加而变化，因此不能只比较分母，还应查看具体未证明目标。

## 6. 任务 4：Eva 告警与数值边界

初始程序中：

- 数组只有 8 个元素，而 `n` 可取 `0..12`，因此 `n = 9..12` 时可能越界。
- 每个元素可取 `-1000000000..1000000000`，多次累加可能超出 `int` 范围。

将 `n` 缩小为 `0..8` 后，数组访问告警消失；再将元素范围缩小为 `-100..100` 后，整数溢出告警也消失，最终和落在 `-800..800`。

WP 版本使用前置条件和循环不变式证明同一边界：

```c
loop invariant 0 <= i <= n;
loop invariant -100 * i <= sum <= 100 * i;
```

固定的 `-800 <= sum <= 800` 不足以作为保持性不变式，因为它没有记录当前和与已处理元素数量之间的关系。动态边界 `-100*i..100*i` 能在每轮累加后自然保持，并在 `i == n <= 8` 时推出最终结果范围。

### 6.1 三个 Eva 版本的代码差异

| 文件 | `n` 的范围 | 元素范围 | 预期结果 |
| --- | --- | --- | --- |
| `eva_sum.c` | `0..12` | `-10^9..10^9` | 越界读取和正、负溢出告警 |
| `eva_sum1.c` | `0..8` | `-10^9..10^9` | 越界告警消失，溢出仍可能发生 |
| `eva_sum2.c` | `0..8` | `-100..100` | 两类告警均消失，`sum` 落在 `-800..800` |

`Frama_C_interval` 为 Eva 提供不确定但有边界的输入。初始版本最终状态中出现 `n ∈ [0..8]`，表示只有这些安全路径到达程序末尾，不代表初始输入已经从 `0..12` 自动改成了 `0..8`。

### 6.2 Eva 和 WP 命令

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission4

frama-c -eva -eva-slevel 20 eva_sum.c
frama-c -eva -eva-slevel 20 eva_sum1.c
frama-c -eva -eva-slevel 20 eva_sum2.c

frama-c -wp -wp-rte wp_sum.c
```

`wp_sum.c` 不再依赖固定的 `main` 输入，而是通过函数契约声明 `n <= 8`、内存可读、元素范围为 `-100..100`。循环不变式把每一步部分和限制在 `-100*i..100*i`，最终版本记录为 `14 / 14`。

## 7. 任务 5：规约变异与非空洞性

本任务通过三种独立变异说明“证明成功”只代表实现满足当前规约：

1. 把三种输入区域削弱为 `-1 <= \result <= 1` 后，始终返回 0 的错误实现仍能通过，因为规约遗漏了输入与返回值的对应关系。
2. 增加 `requires x != 0` 后，函数在缩小的适用域内可以通过，但 `sign_of(0)` 的调用点无法满足前置条件。
3. 把 `x < 0` 错写为 `x <= 0` 后，`x == 0` 同时要求返回 `-1` 和 `0`，规约产生冲突。

因此，验证规约前应检查输入区域是否完整、是否意外重叠、前置条件是否排除了本应支持的业务输入，以及后置条件是否足够强。

### 7.1 各文件对应的规约变异

| 文件 | 变异 | 要观察的问题 |
| --- | --- | --- |
| `sign_of.c` | 原始三分支规约 | 正确实现是否全部通过 |
| `sign_of1.c` | 弱化为结果只需位于 `[-1,1]` | 正确实现仍可通过 |
| `sign_of1_1.c` | 弱规约下始终返回 0 | 错误实现也能通过，暴露规约过弱 |
| `sign_of2.c` | 增加 `requires x != 0`，并在 `main` 调用 0 | 函数通过但调用点违反前置条件 |
| `sign_of3.c` | 把 `x < 0` 错写为 `x <= 0` | 零值区域出现冲突后置条件 |

每种变异都应从原始规约独立开始，不能把上一种变异叠加到下一种，否则无法把证明结果变化归因到唯一修改。

### 7.2 运行命令

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission5

frama-c -wp -wp-rte -wp-fct sign_of sign_of.c
frama-c -wp -wp-rte -wp-fct sign_of sign_of1.c
frama-c -wp -wp-rte -wp-fct sign_of sign_of1_1.c

# 同一文件分别验证被调函数和调用者
frama-c -wp -wp-rte -wp-fct sign_of sign_of2.c
frama-c -wp -wp-rte -wp-fct main sign_of2.c

frama-c -wp -wp-rte -wp-fct sign_of sign_of3.c
```

关键目标分别是：`main` 中调用 0 时的 `call_sign_of_requires`，以及错误边界版本中的 `sign_of_ensures`。前者属于调用者违反前置条件，后者属于规约自身在 `x == 0` 区域提出冲突要求。

## 8. 任务 6：Rocq 列表归纳证明

定理要求证明：

```coq
length (filter keep xs) <= length xs
```

证明结构是对 `xs` 做列表归纳：

- 空列表：化简为 `0 <= 0`。
- 非空列表：对 `keep x` 分类。
  - 保留 `x` 时，两侧长度都增加 1。
  - 丢弃 `x` 时，过滤结果不增加，而原列表长度增加 1。

两个分支都可在归纳假设基础上交给 `lia` 完成。显式脚本适合观察目标与假设，短脚本用分号组合重复步骤，更简洁但隐藏了部分中间状态。

把结论临时改为严格不等式 `<` 后，空列表立即给出最小反例：两侧长度都是 0，目标变成 `0 < 0`。找到具体反例说明命题本身为假；如果命题没有反例但脚本失败，才继续检查证明策略或缺失引理。

### 8.1 证明脚本分析

显式证明先执行：

```coq
intros A keep xs.
induction xs as [| x xs IH].
```

这一步产生空列表和非空列表两个目标。非空分支的归纳假设 `IH` 正是剩余列表 `xs` 的定理。随后：

```coq
destruct (keep x) eqn:Hkeep.
```

把当前元素分为保留和丢弃两种情况。`simpl` 展开 `filter` 和 `length`，`lia` 再把化简后的自然数不等式与 `IH` 组合起来。

短证明：

```coq
induction xs as [| x xs IH]; simpl.
- lia.
- destruct (keep x); simpl; lia.
```

使用分号把同一化简步骤应用到多个子目标，因此代码更短，但不如显式版适合截图观察中间状态。

### 8.2 运行与检查命令

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission6

# 检查是否仍有未完成证明
grep -nE 'admit|Admitted|Abort' filter_length.v

# 编译最终证明；成功时不应出现 Error
rocq compile filter_length.v
```

严格不等式版本用于观察失败，完成分析后必须恢复 `<=`，再重新编译最终文件。`.glob`、`.vo`、`.vok`、`.vos` 和辅助文件是编译产物，不是证明源代码。

## 9. 任务 7：验证 LLM 生成程序

任务流程应保持职责分离：先独立写需求，再让 LLM 生成第一版实现，最后由实验者编写契约并验证，不能根据第一轮验证结果反向修改原始需求。

曼哈顿距离实验把坐标限制在 `[-1000,1000]`，因此每个坐标差的绝对值最多为 2000，总距离最多为 4000。WP 需要同时证明：

- 结果满足曼哈顿距离公式；
- 重合点返回 0；
- 结果位于 `0..4000`；
- 减法、取负和加法不发生有符号整数溢出。

其中结果边界由后置条件证明义务支持，运算安全由 `rte,signed_overflow` 证明义务支持，二者相关但不是同一个目标。Eva 的返回值区间可以作为补充证据，但其结论仍依赖入口状态和分析设置。

### 9.1 两个代码版本

- `manhattan_distance1.c` 保存 LLM 未经人工修正的第一版实现，用于保证验证过程可追溯。
- `manhattan_distance2.c` 在相同实现上添加由实验者独立编写的 ACSL 契约。前置条件限制四个坐标，后置条件描述公式、重合点和结果范围。

函数先计算 `dx = x1 - x2` 和 `dy = y1 - y2`，再通过条件取绝对值，最后返回两者之和。坐标范围保证差值位于 `[-2000,2000]`，取绝对值后位于 `[0,2000]`，相加后位于 `[0,4000]`。

公式后置条件还能推出交换两点后结果不变，因为 `|x1-x2| = |x2-x1|`，纵坐标同理。重合点后置条件虽然可以由公式推出，但单独写出能够明确对应边界需求。

### 9.2 WP、Eva 与证明目标打印命令

```bash
cd /mnt/d/ME/FormalMethods/lab4/mission7

# 验证契约和运行时安全
frama-c -wp -wp-rte -wp-fct manhattan_distance manhattan_distance2.c

# 以该函数为入口做 Eva 分析
frama-c -eva -eva-slevel 20 -main manhattan_distance manhattan_distance2.c

# 打印详细证明义务，便于把目标对应到需求
frama-c -wp -wp-rte -wp-fct manhattan_distance -wp-print manhattan_distance2.c
```

WP 记录为 `16 / 16`。阅读 `-wp-print` 输出时，`Post-condition` 中的 `0 <= result <= 4000` 支持结果边界需求，`Assertion 'rte,signed_overflow'` 支持运算不溢出。Eva 以带参数函数作为入口时可能显示契约状态 `unknown`；应同时查看最终区间和 alarm 摘要，不能把 `unknown` 写成已经由 Eva 证明。

## 10. 常见错误与检查清单

- 把循环变量写进函数 `requires`：局部变量尚未进入作用域，应使用 `loop invariant`。
- 只写固定结果边界，不写随循环进度变化的不变式：通常无法证明保持性。
- 把 `\valid` 与 `assigns` 混淆：前者说明内存是否合法，后者限制允许修改的位置。
- 为了让证明通过而加强无关前置条件：可能排除本应验证的输入区域。
- 把 `Timeout` 直接等同于程序错误：它只表示当前证明过程没有完成目标。
- 把 Eva 的抽象区间输出称为额外 alarm：区间是分析结果，不是运行时错误告警。
- 在 Rocq 中保留 `admit` 或改用 `Admitted`：只能保存未完成证明，不符合本实验要求。
- 看到全部目标通过就认定业务正确：还必须人工检查规约是否完整准确。

提交前可依次检查：

1. 报告是否包含每个任务要求的代码、运行结果、截图和解释。
2. 正确版本是否全部证明，错误或变异版本是否出现预期失败。
3. 最终 Rocq 文件是否不含 `admit`、`Admitted` 或 `Abort`。
4. 图片引用是否存在，目录是否按任务整理。
5. 是否区分了实现错误、规约错误、调用者违反前置条件和证明信息不足。
