# AC Automaton (Aho-Corasick) 深度笔记 | 从 Trie 到线性多模式匹配

[English Version](#english-version) | [中文版本](#中文版本)

---

<a name="english-version"></a>
## English Version

### 1. What Problem Does AC Solve?
Given one text `T` and many patterns `P1...Pk`, AC automaton reports all matches in one scan.

Why this matters:
- Running single-pattern algorithms repeatedly is expensive.
- AC reuses shared prefixes and shared fallback paths.
- Real systems (security scanning, search highlighting, sensitive-word filtering) rely on this exact idea.

### 2. Intuition in One Sentence
AC = Trie (forward progress) + Fail links (smart fallback) + Output aggregation (report all endings).

![AC Automaton Build + Query Pipeline](ac_structure.svg)

### 3. State Model
For each trie node/state `u`:
- `next[u][c]`: transition by character `c`
- `fail[u]`: longest proper suffix state of current matched string
- `out[u]`: how many / which patterns end at this state

After preprocessing, query transition is always deterministic:

$$
u \leftarrow next[u][c]
$$

No backtracking on text index.

### 4. Step-by-Step Build Procedure

#### Step A: Insert patterns into Trie
Each pattern creates a path; endpoints store count or pattern IDs.

#### Step B: BFS build fail links
1. Root children: `fail = root`.
2. BFS pop `u`.
3. For every alphabet character `c`:
     - If child `v = next[u][c]` exists:
         - `fail[v] = next[fail[u]][c]`
         - merge outputs: `out[v] += out[fail[v]]` (or merge ID lists)
         - push `v`
     - Else:
         - `next[u][c] = next[fail[u]][c]` (automaton completion)

This completion is the key optimization: query becomes one array lookup per character.

#### Build timeline (micro-steps)
Use patterns `he`, `she`, `her`:

1. Insert `he`: create path `root -> h -> e`, mark end at `he`.
2. Insert `she`: reuse nothing at first char (`s` new), then create `s -> h -> e`, mark end at `she`.
3. Insert `her`: reuse `h -> e`, append `r`, mark end at `her`.
4. Start BFS from root children (`h`, `s`), set their `fail = root`.
5. Process `sh`: since suffix `h` exists under root, set `fail(sh)=h`.
6. Process `she`: since suffix `he` exists, set `fail(she)=he` and merge outputs.
7. Process `her`: fallback from `he` by `r` to root branch; set `fail(her)=root` (or corresponding state if exists).

After step 7, the automaton is complete for linear query.

### 5. Step-by-Step Query Procedure
Initialize `u = root`, then for each text char:
1. `u = next[u][ch]`
2. report / accumulate `out[u]`

If exact positions are needed, for a matched pattern length `len` ending at index `i`:

$$
start = i - len + 1
$$

#### Query timeline on text `ushers`

| i | char | state transition | outputs |
| :-- | :-- | :-- | :-- |
| 0 | `u` | `root -> root` | none |
| 1 | `s` | `root -> s` | none |
| 2 | `h` | `s -> sh` | none |
| 3 | `e` | `sh -> she` | `she`, inherited `he` |
| 4 | `r` | fail-assisted transition to `her` path | `her` |
| 5 | `s` | transition by completed automaton edges | (depends on dictionary) |

This table is the practical meaning of “text pointer never moves backward”.

### 6. Why Query is Linear (Proof Sketch)
After automaton completion:
- each text character performs exactly one transition `u = next[u][ch]`
- no inner `while` fallback loop during query

So query cost is:

$$
O(|T| + \#matches)
$$

### 7. Build Complexity (Where Cost Comes From)
Let total pattern length be

$$
L = \sum_{i=1}^{k}|P_i|
$$

Number of states is `O(L)`. Build includes:
- Trie insertion: `O(L)`
- BFS over states and alphabet transitions: `O(\Sigma \cdot states)` for dense arrays

Total:

$$
O(L + \Sigma \cdot states)
$$

For fixed small alphabet (like 26 lowercase letters), this is effectively linear in `L`.

### 8. Walkthrough Example: `he`, `she`, `her` on `ushers`
During scanning, when state reaches `she`, output includes:
- `she` itself
- plus inherited suffix match `he` through fail chain

Then reading next char can move to `her` branch via fail-assisted transition without restarting.

### 9. Practical Design Variants
- **Count total matches**: `out[u]` as integer; fastest.
- **Count per pattern**: store endpoint IDs; accumulate per ID.
- **Report all positions**: store pattern lengths/IDs; emit `(start,end)`.

### 10. Common Pitfalls
1. Forgetting output inheritance along fail links.
2. Not completing missing transitions in BFS.
3. Incorrect alphabet mapping (`a-z`, ASCII, Unicode mixed).
4. Wrong dedup strategy causing overcount/undercount.
5. Large-scale memory issues without pre-allocation.

### 11. Engineering Checklist
- Dense alphabet: `next[state][26]` is usually fastest.
- Large alphabet: sparse maps reduce memory.
- Reserve vectors for high state counts.
- Separate `build()` and `query()` APIs.
- Add tiny deterministic tests (overlap, prefix, duplicate pattern).

---

<a name="chinese-version"></a>
## 中文版本

### 1. 这题本质在解决什么？
给你一个文本 `T`，再给你很多模式串 `P1...Pk`，要求一次扫描找出所有命中。

如果每个模式串单独跑 KMP，代价通常接近：

$$
O(|T|\cdot k)
$$

AC 自动机把“多次匹配”压成“一台状态机 + 一次扫描”。

### 2. 先建立直觉：三件套
AC 自动机 = **Trie + Fail 指针 + 输出信息**。

- Trie：尽量向前走（匹配前缀）
- Fail：走不动时，跳到“最长可用后缀”
- 输出：当前状态对应哪些模式串命中

![AC 自动机构建与查询流程图](ac_structure.svg)

### 3. 形式化定义（代码里对应的字段）
把 Trie 的每个节点看成状态 `u`：

- `next[u][c]`：读入字符 `c` 后去哪里
- `fail[u]`：当前串的最长真后缀对应状态
- `out[u]`：在 `u` 结尾的模式串信息（计数/ID 列表）

构建完成后，查询阶段统一是：

$$
u \leftarrow next[u][c]
$$

注意：文本下标不回退。

### 4. 构建算法：逐步推演

#### Step A：插入所有模式串
每个模式串在 Trie 里走一遍；终点打标记（次数或 ID）。

#### Step B：BFS 建 `fail`
1. 根的直接孩子：`fail = root`
2. 队列弹出状态 `u`
3. 枚举字符 `c`
     - 若 `v = next[u][c]` 存在：
         - `fail[v] = next[fail[u]][c]`
         - 输出继承：`out[v] += out[fail[v]]`
         - `v` 入队
     - 否则：
         - `next[u][c] = next[fail[u]][c]`

这一步叫“补全自动机转移”，它直接决定查询能否做到线性。

#### 构建时间线（细粒度）
以 `he`, `she`, `her` 为例：

1. 插入 `he`：得到路径 `root -> h -> e`，在 `he` 节点打终点标记。
2. 插入 `she`：新增 `s -> h -> e`，在 `she` 节点打终点标记。
3. 插入 `her`：复用 `h -> e`，再接 `r`，在 `her` 节点打终点标记。
4. BFS 初始化：根的直接孩子（`h`,`s`）的 `fail` 全设为 `root`。
5. 处理 `sh`：其最长可用后缀是 `h`，故 `fail(sh)=h`。
6. 处理 `she`：其后缀命中 `he`，故 `fail(she)=he`，并做输出继承。
7. 处理 `her`：按 fail 规则找到可转移后缀状态并设置 `fail(her)`。

完成这些步骤后，查询期即可做到“每字符一次转移”。

### 5. 查询算法：一次扫描
初始化 `u = root`，遍历文本每个字符 `ch`：

1. `u = next[u][ch]`
2. 根据 `out[u]` 统计或输出答案

若要输出区间，模式串长度为 `len`、结束位置为 `i`，则起点：

$$
start = i - len + 1
$$

#### 查询时间线：`ushers`

| i | 字符 | 状态变化 | 输出 |
| :-- | :-- | :-- | :-- |
| 0 | `u` | `root -> root` | 无 |
| 1 | `s` | `root -> s` | 无 |
| 2 | `h` | `s -> sh` | 无 |
| 3 | `e` | `sh -> she` | 命中 `she`，并继承命中 `he` |
| 4 | `r` | 通过 fail 辅助转移进入 `her` 路径 | 命中 `her` |
| 5 | `s` | 按补全转移继续 | 视词典而定 |

这张时间线就是“文本指针永不回退”的可视化运行轨迹。

### 6. 复杂度为什么是线性？（核心证明）
补全转移后，每个字符只做一次状态转移，不再有查询期 `while(fail...)` 回退。

所以查询复杂度：

$$
O(|T| + \#matches)
$$

其中 `#matches` 是输出本身的体量，无法省略。

### 7. 构建复杂度拆解
令总模式串长度为：

$$
L = \sum_{i=1}^{k}|P_i|
$$

- 插入 Trie：`O(L)`
- BFS 建 fail + 补边：`O(\Sigma \cdot states)`（稠密字母表）

总计：

$$
O(L + \Sigma \cdot states)
$$

若字母表固定（如 26 个小写字母），可近似看作线性。

### 8. 例子推演：`he`, `she`, `her` 匹配 `ushers`
扫描到 `she` 的 `e` 状态时：
- 会命中 `she`
- 还会因为 fail 继承同时命中后缀 `he`

这就是 AC 自动机处理“重叠匹配”的关键优势。

### 9. 三种工程实现形态
1. **只要总命中数**：`out` 用整数，最省内存、最快。
2. **统计每个模式串次数**：终点存 ID，查询时按 ID 累加。
3. **输出所有位置**：存 `pattern_id` 和 `len`，在线恢复 `(start,end)`。

### 10. 高频坑点
1. 忘了做 `out[v] += out[fail[v]]`。
2. 没补全缺失转移，导致查询退化。
3. 字符映射不统一（大小写/ASCII/Unicode）。
4. 去重写错，出现重复计数或漏计。
5. 数据大时没 `reserve`，扩容频繁拖慢速度。

### 11. 建议你写代码时的检查清单
- 接口分成 `insert` / `build` / `query`
- 构建后随机抽查若干 `fail` 指向
- 用三类样例测：前缀重叠、完全重复、无匹配
- 大数据前先 `reserve`

---

[回到顶部 / Back to Top](#ac-automaton-aho-corasick-深度笔记--从-trie-到线性多模式匹配)