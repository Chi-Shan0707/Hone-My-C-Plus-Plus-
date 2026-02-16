# Self-Balancing BST Notes (AVL & Red-Black) | 由浅入深讲透旋转、约束与复杂度

[English Version](#english-version) | [中文版本](#中文版本)

---

<a name="english-version"></a>
## English Version

### 1. Starting Point: Why plain BST is not enough
BST operations depend on height `h`:

$$
search,\ insert,\ delete = O(h)
$$

If insertion order is sorted, `h` can become `N`, so operations degrade to `O(N)`.
Self-balancing trees keep `h = O(logN)` after every update.

### 2. Shared Core Primitive: Rotation
All balancing methods use local pointer transforms that preserve in-order sequence.

![AVL and Red-Black Rotation/Fixup Map](rotations.svg)

Left rotation around `A` (right child `B`):
- `B` moves up
- `A` becomes `B.left`
- old `B.left` becomes `A.right`

No key ordering is broken.

### 3. AVL vs Red-Black at a glance

| Aspect | AVL | Red-Black |
| :--- | :--- | :--- |
| Balance policy | strict height difference | color + black-height constraints |
| Tree height | tighter bound | looser but still logarithmic |
| Lookup latency | usually slightly better | very good |
| Update overhead | more rotations in average | fewer rotations in average |
| Common usage | read-heavy indexes | general ordered maps/sets |

### 4. AVL: Strict local balance

#### 4.1 Invariant

$$
bf(u)=height(u.left)-height(u.right), \quad bf(u)\in\{-1,0,1\}
$$

#### 4.2 Four imbalance patterns
- LL: right rotate
- RR: left rotate
- LR: left rotate child, then right rotate root
- RL: right rotate child, then left rotate root

#### 4.3 Insertion walkthrough
1. Insert as normal BST.
2. Backtrack and update heights.
3. First node with `|bf|=2` is repaired by one of LL/LR/RL/RR.
4. In insertion, one local repair usually restores global validity.

Deletion may need repeated repairs while moving upward.

#### 4.4 Height bound sketch
Let `N(h)` be minimum nodes of AVL height `h`:

$$
N(h)=N(h-1)+N(h-2)+1
$$

This Fibonacci-like recurrence implies `h = O(logN)`.

### 5. Red-Black Tree: Looser constraints, strong engineering behavior

#### 5.1 Five invariants
1. Every node is red or black.
2. Root is black.
3. NIL leaves are black.
4. No red node has red child.
5. Every node-to-NIL path has equal black count.

#### 5.2 Insertion fix-up (classic 3 cases)
If parent is red after insertion:
- Uncle red: recolor and continue upward.
- Uncle black + triangle: rotate parent to line.
- Uncle black + line: rotate grandparent + recolor.

#### 5.3 Recolor + Rotation (fully step-by-step)
Assume newly inserted node is `N` (red), parent `P`, grandparent `G`, uncle `U`.

**Step 0: Insert as BST, color N red**
- If `P` is black: stop immediately.
- If `P` is red: violation (red-red) appears.

**Step 1: Classify by uncle color**
- `U` is red -> recolor branch (no immediate large rotation).
- `U` is black/NIL -> rotation branch.

**Step 2A: Recolor branch (`U` red)**
1. `P` and `U` recolor to black.
2. `G` recolor to red.
3. Move current pointer to `G` and continue upward.

Intuition: local black-height is preserved, but red-red may move upward.

**Step 2B: Rotation branch (`U` black/NIL)**
First normalize shape:
- If `(N,P,G)` is triangle (`LR` or `RL`), rotate at `P` once to convert into line.
- If line (`LL` or `RR`), rotate at `G` directly.

Then recolor after final rotation:
- new subtree root becomes black
- old `G` becomes red

This simultaneously removes red-red conflict and keeps black-height valid.

**Step 3: Force root black**
After loop, recolor root to black (mandatory invariant).

#### 5.4 Why height is logarithmic
If black-height is `bh`, shortest root-leaf path has at least `bh` black nodes.
By red-rule, longest path is at most `2*bh`.
Also subtree nodes satisfy `n >= 2^{bh}-1`.
Hence:

$$
h \le 2\log_2(n+1)
$$

### 6. Complexity summary
- AVL: search/insert/delete all `O(logN)`; better query constants.
- Red-Black: search/insert/delete all `O(logN)`; better update smoothness.

### 7. Selection guide
- Prefer **AVL** for strongly read-heavy workloads.
- Prefer **Red-Black** for mixed R/W, standard-library-like behavior.

### 8. Implementation checklist
1. Keep rotation helpers independent and thoroughly tested.
2. Store parent pointers for simpler fix-up.
3. AVL: update child height before parent height after rotation.
4. RBT: use sentinel NIL to reduce null-edge branching.
5. Add invariant checker in debug mode.

---

<a name="chinese-version"></a>
## 中文版本

### 1. 先看本质：普通 BST 为什么会“翻车”
二叉搜索树的时间复杂度取决于树高 `h`：

$$
查找,\ 插入,\ 删除 = O(h)
$$

若输入接近有序，树会退化成链表，`h \approx N`，复杂度退化到 `O(N)`。
自平衡树的目标就是：每次更新后都把高度压回 `O(logN)`。

### 2. 共同核心：旋转
AVL 和红黑树看起来规则不同，但“结构修复动作”几乎都靠旋转。

![AVL 与红黑树旋转修复总览图](rotations.svg)

左旋（以 `A` 为轴、`B` 为右儿子）可理解为：
1. `B` 上提成子树根
2. `A` 下沉为 `B.left`
3. 原 `B.left` 挪成 `A.right`

中序序列不变，所以 BST 的有序性不变。

### 3. AVL 与红黑树怎么选：先看约束强度

| 维度 | AVL | 红黑树 |
| :--- | :--- | :--- |
| 约束 | 严格高度差约束 | 颜色 + 黑高约束 |
| 树高上界 | 更紧 | 略松但仍对数级 |
| 查询性能 | 常数更优 | 足够优秀 |
| 更新成本 | 旋转更频繁 | 旋转更少更平滑 |
| 典型应用 | 读多写少 | 读写混合、通用容器 |

### 4. AVL：严格平衡的逐步推演

#### 4.1 不变量

$$
bf(u)=height(u.left)-height(u.right),\quad bf(u)\in\{-1,0,1\}
$$

#### 4.2 为什么只有 4 种修复？
失衡只会在祖先路径上出现，且只会表现为：
- LL / RR（单旋）
- LR / RL（双旋）

#### 4.3 插入算法流程
1. 按普通 BST 插入。
2. 回溯更新高度。
3. 找到第一个 `|bf|=2` 的节点。
4. 根据路径方向选择 LL/LR/RL/RR 并旋转。

插入通常修一次就够；删除可能一路向上多次修复。

#### 4.4 为什么 AVL 高度是对数级？
设高度为 `h` 的 AVL 最少节点数是 `N(h)`，则：

$$
N(h)=N(h-1)+N(h-2)+1
$$

这是类斐波那契递推，所以 `N(h)` 指数增长，反推 `h=O(logN)`。

### 5. 红黑树：约束更松、工程更稳

#### 5.1 五条规则（务必背熟）
1. 节点要么红要么黑
2. 根是黑色
3. NIL 叶子视为黑色
4. 红节点不能有红孩子
5. 任意到 NIL 的路径黑节点数相同（黑高一致）

#### 5.2 插入修复的 3 类情形
新节点默认红；若父红则违规：
- 叔叔红：变色 + 上推
- 叔叔黑且折线：先转成直线
- 叔叔黑且直线：旋转祖父 + 重新染色

#### 5.3 变色与旋转：完整推演
设新插入节点为 `N`（红），父为 `P`，祖父为 `G`，叔叔为 `U`。

**Step 0：先按 BST 插入，再染红**
- 若 `P` 是黑色，直接结束。
- 若 `P` 是红色，触发红红冲突。

**Step 1：按叔叔颜色分流**
- `U` 为红：走“纯变色上推”。
- `U` 为黑（或 NIL）：走“旋转 + 染色修复”。

**Step 2A：叔叔红（变色上推）**
1. `P` 与 `U` 染黑
2. `G` 染红
3. 当前指针上移到 `G` 继续检查

直觉：这一步不改大结构，只把冲突往上推一层。

**Step 2B：叔叔黑（旋转分两段）**
1. 若是折线（`LR`/`RL`），先在 `P` 处旋一次，变成直线。
2. 再在 `G` 处旋转（`LL`/`RR` 对应单旋）。
3. 旋转后把“新子树根”染黑，把旧 `G` 染红。

这一步同时修复红红冲突，并保持黑高一致。

**Step 3：根强制染黑**
循环结束后把根染黑，恢复规则 2。

#### 5.4 红黑树高度上界证明（常见面试点）
记黑高为 `bh`：
- 最短路径长度至少 `bh`
- 由于不能红红相连，最长路径长度至多 `2bh`
- 同时节点数满足 `n \ge 2^{bh}-1`

故有：

$$
h \le 2\log_2(n+1)
$$

所以仍是 `O(logN)`。

### 6. 复杂度与常数
- AVL：查找/插入/删除均 `O(logN)`，查找常数往往更小。
- 红黑树：查找/插入/删除均 `O(logN)`，更新更平滑。

### 7. 落地实现建议（很实用）
1. 旋转写成独立函数，专测指针连线正确性。
2. 维护 `parent` 指针，修复逻辑会简洁很多。
3. AVL 旋转后高度更新顺序：先低后高。
4. 红黑树统一 NIL 哨兵，避免大量空指针分支。
5. 写 `checkInvariant()`：AVL 检 `bf`，RBT 检黑高与红红冲突。

---

[回到顶部 / Back to Top](#self-balancing-bst-notes-avl--red-black--由浅入深讲透旋转约束与复杂度)
