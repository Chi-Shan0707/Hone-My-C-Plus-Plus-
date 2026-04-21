
# Hone My C++（C++ 进阶学习仓库）
[中文](#中文) | [English](#english)

---

<a name="中文"></a>
## 中文

### 这是什么？
这是我的 **C++ 进阶学习/复盘仓库**：以“算法与数据结构 + C++ 工程基础”为主线，包含我在学习过程中整理的笔记、可运行的代码实现、以及一些训练/比赛相关内容。整体风格偏“自用知识库”，会不断增长与迭代。

> 仓库名：`Chi-Shan0707/Hone-My-C-Plus-Plus-`

---

## 仓库结构概览（你能在这里找到什么）

### 1) 字符串算法与自动机：`Autonomaton/`
- **AC 自动机（Aho–Corasick）**：从 Trie 到 fail 指针、输出继承、复杂度分析，并配有结构示意图与代码实现  
- **KMP**：单模式匹配的实现，用于对比理解 AC 的“多模式一次扫描”

内容形式：
- `Autonomaton/README.md`：中英双语的深度笔记（含 SVG 图）
- `Autonomaton/ACAutomaton.cpp`、`Autonomaton/KMP.cpp`：实现代码

---

### 2) 平衡树与自平衡 BST：`Binary_Search_Tree/`
覆盖常见自平衡结构与其工程/竞赛实现要点：
- **AVL / Red-Black Tree**：旋转、性质约束、插入修复、复杂度推导（配图）
- **Splay Tree（伸展树）**
- **FHQ Treap**（分裂/合并 Treap 的一种常见竞赛写法）
- 额外的笔记与图示资源（`rotations.svg`、`Red_Black Tree ...png` 等）

内容形式：
- `Binary_Search_Tree/notes.md`：学习笔记
- 多个 `.cpp/.h`：实现与尝试

---

### 3) 排序与分治：`Sort/`
- 手写 **Quick Sort**（含头文件与示例调用）


---

### 4) 堆（优先队列思想）：`Heap/`
- `BasicHeap.cpp`：基础堆结构/操作的练习实现（偏“从零写一遍”的理解型代码）

---

### 5) 排列：`NextPermutation/`
- `next_permutation.h`：实现/拆解“下一个排列”逻辑
- `use_my_next_permutation.cpp`：使用示例

---

### 6) 网络流：`NetworkFlow/`
- **Dinic 最大流**：头文件实现 + wrapper + demo
- Graph abstractions via `flow_solver.hpp` (abstract base) + `dinic.hpp` (template implementation)

适合用来作为：
- 竞赛模板雏形
- 练习“把算法写成可复用组件”的工程化训练

---

### 7) 多项式/FFT：`Polynominal/`
- 面向大数/多项式乘法的练习（含 *A\*B Problem* 相关实现）
- `Polynominal/README.md` + SVG 图：FFT 管线/分治树等可视化辅助理解资料

---

### 8) C/C++ 基础回炉与零散专题：`MOOC/`
包含大量“从基础到易错点”的练习与笔记型代码，例如：
- 地址 / 指针 / 引用 / const
- `extern`、宏、分区（partition）找第 k 小等
- 以及配套的课程 PDF、README 汇总

---

### 10) 调试工具：`GDB/`
- `GDB/README.md`：GDB 使用笔记（适合快速查表/回忆命令）

---

### 11) 训练/比赛与其他：`Competition/`、`GoodDeed/`
- `Competition/FDUPC/`：比赛/训练相关内容（以目录形式沉淀）
- `GoodDeed/`：做点好事

---

### 12) 书
- `Introduction to Algorithm 算法导论.pdf`：
[算法导论中文版](https://github.com/Chi-Shan0707/Hone-My-C-Plus-Plus-/blob/main/Introduction%20to%20Algorithm%20%E7%AE%97%E6%B3%95%E5%AF%BC%E8%AE%BA.pdf) 感谢好人开源

---

## 如何使用本仓库（建议）
- **想系统学算法**：从 `Autonomaton/`、`Binary_Search_Tree/`、`NetworkFlow/`、`Polynominal/` 进入（它们含较完整笔记/实现）
- **想练 C++ 基础与语法细节**：看 `MOOC/`
- **想练“可复用模板”**：重点参考 `NetworkFlow/`、`Sort/`、`NextPermutation/`

---

## 下一阶段路线图（Plan / Reminder）

这份不是“已经完成”的目录，而是我和 GPT 讨论后整理出来的下一阶段探索清单，主要用来提醒自己后续优先补哪些更现代、更高级、也更适合用 modern C++ 去实现的方向。

| 数据结构 | 算法 |
| --- | --- |
| **动态连通 / 动态树**<br>Rollback DSU；带权/势能 DSU；Link-Cut Tree；Euler Tour Tree；Top Tree | **动态图 / 树高级技巧**<br>离线动态连通性；DSU on tree；Rerooting DP；Centroid Decomposition；Virtual Tree |
| **字符串 / 压缩索引**<br>Suffix Automaton；Palindromic Tree（Eertree）；Wavelet Tree；Wavelet Matrix；FM-index；Succinct Bitvector（rank/select） | **字符串算法**<br>SA-IS / Suffix Array；Manacher；Z-Algorithm；Booth 最小表示；Duval / Lyndon Decomposition；BWT |
| **顺序统计 / 优先队列 / 序列维护**<br>Order-statistics tree；Implicit Treap；Pairing Heap；Radix Heap；Linear Basis（线性基） | **图匹配 / 流 / 割**<br>Hopcroft–Karp；Hungarian；Blossom；Min-cost Max-flow；Stoer–Wagner 全局最小割；Gomory–Hu Tree；Edmonds Arborescence；Dominator Tree |
| **持久化 / 函数式**<br>Persistent Treap；HAMT；Purely Functional RB-Tree；Rope；Piece Table；Gap Buffer | **离线 / 分治 / 查询技巧**<br>Mo's Algorithm（含修改）；CDQ 分治；Parallel Binary Search；Offline Divide-and-Conquer on Time；Tarjan 离线 LCA |
| **空间 / 检索 / 近邻**<br>KD-Tree；Ball Tree；VP-Tree；R-Tree；BVH；HNSW | **路径 / 搜索 / 检索**<br>A*；Bidirectional Dijkstra；Contraction Hierarchies；Yen K Shortest Paths；Suurballe；ANN Search |
| **流式 / 近似 / Sketch**<br>Bloom Filter；Counting Bloom；Cuckoo Filter；Xor Filter；Count-Min Sketch；HyperLogLog；Roaring Bitmap | **流式 / 相似性**<br>Reservoir Sampling；Misra–Gries；MinHash；SimHash；LSH |
| **工程 / 存储**<br>B+ Tree；LSM Tree；Skip List；Concurrent Skip List；Robin Hood Hashing；Cuckoo Hashing | **外存 / 系统算法**<br>External Merge Sort；Cache-Oblivious Search/Sort；Consistent Hashing；LSM Compaction |
| **并发 / 现代 C++ 味很重**<br>Lock-free Queue；MPMC Ring Buffer；Concurrent Hash Map；Intrusive List；Arena / Pool Allocator | **并发 / 回收 / 调度**<br>Hazard Pointers；Epoch-Based Reclamation；RCU；Work-Stealing |
| **位运算 / 压缩 / 整数宇宙**<br>Succinct Trie；Compressed Suffix Array；van Emde Boas Tree；X-fast Trie；Y-fast Trie | **数论 / 线代 / 变换**<br>Berlekamp–Massey；Kitamasa；FWHT/FWT；CRT/Garner；Miller–Rabin；Pollard–Rho；GF(2) Gaussian Elimination |
| **偏理论但非常启发**<br>Retroactive Data Structures；Finger Tree；Succinct Tree / LOUDS | **偏理论但非常启发**<br>Matroid Intersection；SMAWK；SOS DP；Subset Convolution；Half-plane Intersection；Delaunay / Voronoi |

后续如果要继续补内容，优先按这个清单去开新目录、补 README 和实现文件。

---

## MicroGPT
C++ from-scratch GPT implementation has been migrated to its own repository:
**https://github.com/Chi-Shan0707/microgpt.cpp**


## 许可与声明

本仓库采用 **MIT 许可证**（详见 [`LICENSE`](./LICENSE) 文件）。

### 许可范围说明（重要）
- MIT 许可证**仅适用于本仓库中由我本人创作的内容**，包括：
  - 源代码（如 `.cpp`、`.h`、`.hpp` 等文件）
  - 我自己撰写的笔记和图表（除非另有说明）
- **第三方材料不受 MIT 许可证约束**，其版权仍归各自的权利持有人所有。

### 第三方材料
本仓库包含（或可能包含）以下第三方学习资源，例如：

- `Introduction to Algorithm 算法导论.pdf`  
  版权归该书作者及出版方所有。  
  此文件**仅用于学习与参考目的**，**并未以 MIT 许可证重新授权分发**。  
  若该文件侵犯了任何权利，请联系我，我将立即移除。
---

<a name="english"></a>
## English

### What is this?
This is my **advanced C++ learning & review repository**. It focuses on **algorithms/data structures** and **C++ engineering fundamentals**, containing notes, runnable implementations, and some contest/training materials. Think of it as a growing personal knowledge base.

> Repo: `Chi-Shan0707/Hone-My-C-Plus-Plus-`

---

## Repository Overview (What’s inside)

### 1) String Algorithms & Automata: `Autonomaton/`
- **AC Automaton (Aho–Corasick)**: from Trie to fail links, output inheritance, complexity analysis, with diagrams and implementation
- **KMP**: single-pattern matching implementation as a baseline for comparison

Format:
- `Autonomaton/README.md`: bilingual deep notes (with SVG diagrams)
- `Autonomaton/ACAutomaton.cpp`, `Autonomaton/KMP.cpp`: implementation code

---

### 2) Self-balancing BSTs: `Binary_Search_Tree/`
Covers major self-balancing structures and their key engineering/contest implementation details:
- **AVL / Red-Black Tree**: rotations, invariants, insertion fix-up, complexity (with diagrams)
- **Splay Tree**
- **FHQ Treap** (a common split/merge Treap style in contests)
- Extra notes and image assets (`rotations.svg`, `Red_Black Tree ...png`, etc.)

Format:
- `Binary_Search_Tree/notes.md`: learning notes
- multiple `.cpp/.h`: implementations and experiments

---

### 3) Sorting & Divide-and-Conquer: `Sort/`
- Handwritten **Quick Sort** (with header + usage example)


---

### 4) Heap (Priority Queue ideas): `Heap/`
- `BasicHeap.cpp`: a basic heap implementation for “build it from scratch” understanding

---

### 5) Permutation: `NextPermutation/`
- `next_permutation.h`: implementation / breakdown of “next permutation”
- `use_my_next_permutation.cpp`: usage example

---

### 6) Network Flow: `NetworkFlow/`
- **Dinic’s Max Flow**: header implementation + wrapper + demo
- `graph.hpp` etc.: attempts at graph abstractions/interfaces

Useful as:
- a contest template seed
- practice to turn algorithms into reusable components

---

### 7) Polynomial / FFT: `Polynominal/`
- Practice for big integer / polynomial multiplication (including *A\*B Problem* variants)
- `Polynominal/README.md` + SVG diagrams for FFT pipeline / divide-and-conquer visualization

---

### 9) C/C++ Fundamentals & Misc Topics: `MOOC/`
A collection of “back-to-basics” and tricky-topic exercises, for example:
- address / pointer / reference / const
- `extern`, macros, partition-based kth selection, etc.
- plus course PDFs and a summary README

---

### 10) Debugging Tools: `GDB/`
- `GDB/README.md`: GDB notes (quick command reference)

---

### 11) Contest/Training & Others: `Competition/`, `GoodDeed/`
- `Competition/FDUPC/`: contest/training materials organized by directory
- `GoodDeed/`: do good deeds

---

### 12) References
- `Introduction to Algorithm 算法导论.pdf`: CLRS (Chinese) PDF 
[stored/linked in the repo](https://github.com/Chi-Shan0707/Hone-My-C-Plus-Plus-/blob/main/Introduction%20to%20Algorithm%20%E7%AE%97%E6%B3%95%E5%AF%BC%E8%AE%BA.pdf) 感谢好人开源!!
---

## How to use this repo (suggested paths)
- **For systematic algorithm learning**: start with `Autonomaton/`, `Binary_Search_Tree/`, `NetworkFlow/`, `Polynominal/` (they contain more complete notes/implementations)
- **For C++ fundamentals & language details**: check `MOOC/`
- **For reusable templates**: focus on `NetworkFlow/`, `Sort/`, `NextPermutation/`

---

## Next Roadmap (Plan / Reminder)

This is not a list of finished content. It is a reminder of the next directions I want to explore in this repo, with a bias toward more modern, advanced, and modern-C++-friendly implementations.

| Data Structures | Algorithms |
| --- | --- |
| **Dynamic connectivity / dynamic trees**<br>Rollback DSU; weighted/potential DSU; Link-Cut Tree; Euler Tour Tree; Top Tree | **Dynamic graph / tree techniques**<br>Offline dynamic connectivity; DSU on tree; Rerooting DP; Centroid Decomposition; Virtual Tree |
| **Strings / compressed indexes**<br>Suffix Automaton; Palindromic Tree (Eertree); Wavelet Tree; Wavelet Matrix; FM-index; Succinct Bitvector (rank/select) | **String algorithms**<br>SA-IS / Suffix Array; Manacher; Z-Algorithm; Booth minimum representation; Duval / Lyndon Decomposition; BWT |
| **Order statistics / priority queues / sequence maintenance**<br>Order-statistics tree; Implicit Treap; Pairing Heap; Radix Heap; Linear Basis | **Graph matching / flow / cuts**<br>Hopcroft–Karp; Hungarian; Blossom; Min-cost Max-flow; Stoer–Wagner global min cut; Gomory–Hu Tree; Edmonds Arborescence; Dominator Tree |
| **Persistence / functional**<br>Persistent Treap; HAMT; Purely Functional RB-Tree; Rope; Piece Table; Gap Buffer | **Offline / divide-and-conquer / query tricks**<br>Mo's Algorithm (with modifications); CDQ Divide and Conquer; Parallel Binary Search; Offline Divide-and-Conquer on Time; Tarjan offline LCA |
| **Spatial / retrieval / nearest neighbor**<br>KD-Tree; Ball Tree; VP-Tree; R-Tree; BVH; HNSW | **Path / search / retrieval**<br>A*; Bidirectional Dijkstra; Contraction Hierarchies; Yen K Shortest Paths; Suurballe; ANN Search |
| **Streaming / approximation / sketching**<br>Bloom Filter; Counting Bloom; Cuckoo Filter; Xor Filter; Count-Min Sketch; HyperLogLog; Roaring Bitmap | **Streaming / similarity**<br>Reservoir Sampling; Misra–Gries; MinHash; SimHash; LSH |
| **Engineering / storage**<br>B+ Tree; LSM Tree; Skip List; Concurrent Skip List; Robin Hood Hashing; Cuckoo Hashing | **External / system algorithms**<br>External Merge Sort; Cache-Oblivious Search/Sort; Consistent Hashing; LSM Compaction |
| **Concurrency / very modern C++ flavored**<br>Lock-free Queue; MPMC Ring Buffer; Concurrent Hash Map; Intrusive List; Arena / Pool Allocator | **Concurrency / reclamation / scheduling**<br>Hazard Pointers; Epoch-Based Reclamation; RCU; Work-Stealing |
| **Bit tricks / compression / integer universe**<br>Succinct Trie; Compressed Suffix Array; van Emde Boas Tree; X-fast Trie; Y-fast Trie | **Number theory / linear algebra / transforms**<br>Berlekamp–Massey; Kitamasa; FWHT/FWT; CRT/Garner; Miller–Rabin; Pollard–Rho; GF(2) Gaussian Elimination |
| **Theoretical but very inspiring**<br>Retroactive Data Structures; Finger Tree; Succinct Tree / LOUDS | **Theoretical but very inspiring**<br>Matroid Intersection; SMAWK; SOS DP; Subset Convolution; Half-plane Intersection; Delaunay / Voronoi |

If I keep expanding this repo, these are the directions I should prioritize for new folders, READMEs, and implementations.

---

## MicroGPT
C++ from-scratch GPT implementation has been migrated to its own repository:
**https://github.com/Chi-Shan0707/microgpt.cpp**


## License 

This repository is licensed under the **MIT License** (see [`LICENSE`](./LICENSE)).

### Scope clarification (important) 
- The MIT License **only applies to content created by me** in this repository, including:
  - source code (`.cpp`, `.h`, `.hpp`, etc.)
  - my own notes and diagrams unless otherwise stated
- **Third-party materials are NOT covered by the MIT License** and remain the property of their respective copyright holders.

### Third-party materials 
This repository contains (or may contain) third-party learning resources, for example:

- `Introduction to Algorithm 算法导论.pdf`  
  Copyright belongs to the book’s authors/publisher.  
  It is provided here **for study/reference purposes only** and is **NOT redistributed under MIT**.  
  If this file violates any rights, please contact me and I will remove it.
