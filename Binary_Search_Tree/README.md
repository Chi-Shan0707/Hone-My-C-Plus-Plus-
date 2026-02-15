# Self-Balancing Binary Search Trees (AVL & Red-Black) | 自平衡二叉搜索树

[English Version](#english-version) | [中文版本](#中文版本)

---

<a name="english-version"></a>
## English Version

### 1. Why Self-Balancing Trees Exist
Plain BSTs are fast only when height is small. In worst case (sorted insert), BST height becomes $N$, and operations degrade to $O(N)$.

Self-balancing BSTs guarantee height near logarithmic, so search/insert/delete remain $O(\log N)$.

Core idea:
- Keep BST order invariant (`left < root < right`)
- Maintain additional balance constraints
- Repair violations via local rotations/recoloring

### 2. Rotation Is the Fundamental Primitive
All balancing algorithms use rotations.

![Tree Rotation Diagram](rotations.svg)

A rotation changes parent-child relationships while preserving in-order sequence.

For left rotation around `A` with right child `B`:
- `B` becomes parent
- `A` becomes `B.left`
- old `B.left` becomes `A.right`

This local transformation restores balance without rebuilding the tree.

### 3. AVL vs Red-Black: High-Level Comparison

| Feature | AVL Tree | Red-Black Tree |
| :--- | :--- | :--- |
| Balancing criterion | strict height balance | color + black-height rules |
| Height upper bound | tighter | looser but logarithmic |
| Search speed | slightly better on average | very good |
| Update cost | more frequent rotations | fewer rotations on average |
| Typical use | query-heavy workloads | standard libraries, general-purpose maps/sets |

### 4. AVL Tree (Strictly Balanced)

#### 4.1 Invariant
Balance factor:

$$
bf(u)=height(u.left)-height(u.right)
$$

Must satisfy:

$$
bf(u)\in\{-1,0,1\}
$$

for every node `u`.

#### 4.2 Repair Cases
When insertion/deletion causes $|bf|=2$, use one of four patterns:

- LL: right rotation
- RR: left rotation
- LR: left rotation on child, then right rotation on root
- RL: right rotation on child, then left rotation on root

#### 4.3 Procedure (Insertion)
1. Insert like BST.
2. Backtrack to root, update heights.
3. At first unbalanced ancestor, choose LL/LR/RL/RR by subtree direction.
4. Rotate and stop (in insertion case one fix is usually enough).

Deletion may trigger multiple rebalances on path to root.

### 5. Red-Black Tree (Looser but Practical)

#### 5.1 Invariants
1. Node color is red or black.
2. Root is black.
3. Null leaves are black.
4. No red node has a red child.
5. Every path from a node to descendant null leaves has same black count (black-height).

These rules imply tree height is $O(\log N)$.

#### 5.2 Why It Works Intuitively
RBT corresponds to a 2-3-4 tree encoding:
- red links “glue” nodes into multi-key B-tree nodes
- black-height consistency simulates equal-depth leaves in B-tree

So updates stay efficient and balanced enough for production use.

#### 5.3 Insertion Fix-up (Classic Cases)
After BST insertion, new node is red. If parent is red, violation occurs:

- **Case A (uncle red)**: recolor parent+uncle to black, grandparent to red, continue upward.
- **Case B (uncle black, triangle)**: rotate parent to convert into line.
- **Case C (uncle black, line)**: rotate grandparent and recolor.

### 6. Complexity and Constants
- AVL: search $O(\log N)$, updates $O(\log N)$; better search constants.
- RBT: search $O(\log N)$, updates $O(\log N)$; often better write-side constants.

In systems where writes are frequent, RBT is usually preferred.

### 7. Choosing Between AVL and RBT
- Choose **AVL** when:
  - read-heavy
  - strict latency on lookup
  - key distribution causes frequent near-worst cases
- Choose **RBT** when:
  - mixed read/write workload
  - implementation simplicity through mature templates/libraries
  - compatibility with `std::map/std::set` style behavior

### 8. Engineering Implementation Notes
1. Use sentinel NIL node in RBT to simplify edge cases.
2. Keep parent pointers for easier rotations/fix-up.
3. For AVL, update height in strict post-order after rotations.
4. Write rotation as standalone reusable helper; bugs often live there.
5. Add invariant-check debug function (height/balance or black-height) for testing.

---

<a name="chinese-version"></a>

## 中文版本 

### 1. 为什么需要自平衡树
普通 BST 在顺序插入时会退化成链表，高度接近 $N$，查找/插入/删除都可能退化到 $O(N)$。

自平衡树在维持 BST 有序性的同时，引入额外约束，使树高保持在 $O(\log N)$。

核心动作只有两类：
- 旋转（改结构）
- 染色/高度更新（改元数据）

### 2. 旋转：所有平衡算法的“螺丝刀”

![树旋转示意图](rotations.svg)

旋转是局部变换，不改变中序遍历顺序，因此不会破坏 BST 的大小关系。

以左旋为例（以 `A` 为轴，`B` 为右孩子）：
- `B` 上升为子树根
- `A` 下沉为 `B.left`
- 原 `B.left` 挪到 `A.right`

### 3. AVL 与红黑树总览对比

| 特性 | AVL 树 | 红黑树 (Red-Black Tree) |
| :--- | :--- | :--- |
| 平衡标准 | 严格高度差 | 黑高 + 颜色约束 |
| 树高 | 更矮更紧凑 | 略高但仍对数级 |
| 查询性能 | 通常略优 | 很好 |
| 更新性能 | 旋转更频繁 | 平均旋转更少 |
| 工程落地 | 读多写少场景 | 通用容器/系统内核常用 |

### 4. AVL 树（严格平衡）

#### 4.1 不变量
平衡因子定义：

$$
bf(u)=height(u.left)-height(u.right)
$$

要求每个节点都满足：

$$
bf(u)\in\{-1,0,1\}
$$

#### 4.2 失衡修复四大类
- LL：右旋
- RR：左旋
- LR：先左旋子节点，再右旋祖先
- RL：先右旋子节点，再左旋祖先

#### 4.3 插入流程
1. 按 BST 插入。
2. 回溯更新高度。
3. 找到首个失衡点。
4. 按 LL/LR/RL/RR 做旋转修复。

删除时可能沿路径多次失衡，通常需要持续回溯修复。

### 5. 红黑树（工程实战强）

#### 5.1 五条规则
1. 节点非红即黑。
2. 根是黑色。
3. 空叶子（NIL）视为黑色。
4. 红节点不能有红孩子。
5. 任意节点到其所有 NIL 路径上的黑节点数相同（黑高一致）。

这些规则保证了树高上界是对数级。

#### 5.2 插入修复直觉
新插入默认红色，若父节点也是红色则违规。按叔叔节点颜色分情况：

- 叔叔红：变色，上推到祖父继续处理
- 叔叔黑且“折线”：先把折线转成直线
- 叔叔黑且“直线”：旋转祖父并重新染色

#### 5.3 为什么红黑树常见
它可看作 2-3-4 树的二叉表示形式，平衡限制比 AVL 稍宽松，因此写操作成本更平滑，工程上常作为默认有序映射实现。

### 6. 复杂度与常数
- AVL：查询/插入/删除均为 $O(\log N)$，查询常数通常更优。
- 红黑树：查询/插入/删除均为 $O(\log N)$，更新常数通常更优。

### 7. 如何选择
- 选 **AVL**：查询占绝大多数、追求更低查询延迟。
- 选 **红黑树**：读写混合更均衡、希望更新更稳。

### 8. 实现建议
1. 红黑树使用统一 NIL 哨兵可极大减少空指针分支。
2. 旋转函数独立封装，确保只改有限指针。
3. AVL 旋转后先更新低层高度再更新高层。
4. 写调试检查器：AVL 检查 `bf`，红黑树检查黑高与红红冲突。
5. 随机测试与升序/降序极端数据都要覆盖。

---

[回到顶部 / Back to Top](#self-balancing-binary-search-trees-avl--red-black--自平衡二叉搜索树)
