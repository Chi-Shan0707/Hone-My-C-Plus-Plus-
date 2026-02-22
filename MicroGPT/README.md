# MicroGPT（纯 Python 最小 GPT）

这个目录现在包含：
- `microgpt.py`：训练 + 推理一体的最小 GPT 实现。
- `microgpt_explainer.py`：完整可运行的 microGPT（详注版），包含【代码意图】+【LLM算法原理】+【Transformer思想】注释。

## 1) 详注版 microGPT 使用

在 `MicroGPT/` 目录下运行：

```bash
python microgpt_explainer.py
```

说明：
- 该文件本身就是“完整 microGPT 训练 + 推理代码”，不是外部解释器。
- 你可以直接在这个文件上继续改造（例如后续对齐 C++ 重写逻辑）。

## 2) microgpt.py 训练方法

在 `MicroGPT/` 目录下运行：

```bash
python microgpt.py
```

训练流程说明：
1. 若当前目录没有 `input.txt`，脚本会自动下载 names 数据集。
2. 按字符级 tokenizer 构建词表，加入 BOS token。
3. 初始化 1 层、16 维 embedding 的极简 GPT 参数。
4. 用自定义 `Value` 自动求导 + Adam 优化器训练 `num_steps=1000`。
5. 训练结束后执行采样推理，生成 20 个新名字。

你可以修改这些超参数进行实验：
- `n_layer`, `n_embd`, `block_size`, `n_head`
- `learning_rate`, `beta1`, `beta2`, `num_steps`, `temperature`

## 3) 未来 C++ 重写计划（To-Do）

- [ ] 设计 `Value` 标量自动求导类（含计算图、拓扑反传、运算符重载）。
- [ ] 实现张量/矩阵容器与基础算子（matmul、softmax、rmsnorm、relu）。
- [ ] 按当前结构实现 GPT 前向：embedding -> attention -> MLP -> lm_head。
- [ ] 实现训练循环：tokenize、loss、backward、Adam 更新、梯度清零。
- [ ] 对齐 Python 版本超参，确保 loss 曲线趋势一致。
- [ ] 加入推理采样（temperature、BOS 截断）。
- [ ] 提供最小可复现脚本：build + run + 对比输出。
- [ ] 后续再做性能版：批处理、向量化、并行、内存池。
