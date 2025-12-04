## Ford-Fulkerson Algorithm

**Algorithm Steps:**

1. **Initialization**: For all edges `e ∈ E`, set `f(e) = 0`
2. **Construct Residual Graph**: Build residual graph `G_f = (V, E_f)`
3. **Find Augmenting Path**: Find an s-t path `P` in `G_f` (terminate if no such path exists)
4. **Update Flow**:
   - Compute `δ = min{C_f(e) | e ∈ P}` where `C_f` represents residual capacity
   - For each edge `e` in path `P`, update `f(e) = f(e) + δ`
5. **Loop**: Return to step 2

---

## Max-Flow Min-Cut Theorem Proof

**Key Equations:**
<br>
val(f) = (sum of flows on saturated S → T* edges) - (sum of flows on zero-flow T* → S* edges)
<br>
val(f) = (sum of capacities of all S → T* edges) - 0
<br>
val(f) = c(S, T*)