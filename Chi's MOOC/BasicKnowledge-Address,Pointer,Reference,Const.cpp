/**
 * @file BasicKnowledge-Address,Pointer,Reference,Const.cpp
 * @brief 学习程序：关于指针、引用、地址、常量 (Pointer, Reference, Address, Const)
 * 
 * 本程序整理了关于 C++ 核心概念的学习笔记和示例代码。
 * 主要内容包括：
 * 1. 引用 (Reference): 别名机制、初始化规则、函数返回引用。
 * 2. 常引用 (Const Reference): 绑定规则、临时对象生命周期延长。
 * 3. 指针 (Pointer): 地址操作、解引用、空指针、数组与指针。
 * 4. 常量与指针 (Const & Pointer): 指向常量的指针 vs 常量指针。
 */

#include <iostream>
#include <cstdio> // for printf

// --- 全局变量与辅助函数声明 ---

int n = 4;

/**
 * @brief 返回全局变量 n 的引用
 * 
 * 函数签名 int &SetValue() 表示：函数返回一个 int 的左值引用（reference）。
 * 函数体 return n; 返回全局变量 n 的引用。因此 SetValue() 表达式的类型是 int&，是一个可被赋值的左值。
 * 所以 SetValue() = 10; 等价于 n = 10; —— 把右侧值赋给 n。
 */
int &SetValue() {
    return n;
}

// --- 演示函数声明 ---
void demonstrate_reference();
void demonstrate_const_reference();
void demonstrate_pointer(); 
void demonstrate_const_pointer();
void GoodCall(const char *ptr);

// --- 主函数 ---
int main()
{
    std::cout << "=== C++ 核心概念演示开始 ===" << std::endl;

    demonstrate_reference();
    demonstrate_const_reference();
    demonstrate_pointer();
    demonstrate_const_pointer();

    std::cout << "\n=== 演示结束 ===" << std::endl;
    return 0;
}

// --- 函数实现 ---

/**
 * @brief 1. 引用 (Reference) 演示
 * 
 * 核心概念：
 * - 引用是对象的别名 (alias)。
 * - 必须在定义时初始化，且之后不能更改绑定。
 * - 语义上不能为空 (null)。
 * - 底层通常实现为指针，但语法上像普通变量。
 */
void demonstrate_reference()
{
    std::cout << "\n--- 1. 引用 (Reference) ---" << std::endl;

    /*
    注解：
    您可以将左值引用视为对象的另一名称。左值引用声明由说明符的可选列表后跟一个引用声明符组成。
    引用必须初始化且无法更改。
    */

    // 演示：通过返回引用的函数修改全局变量
    std::cout << "全局变量 n 初始值: " << n << std::endl;
    SetValue() = 10; // 等价于 n = 10
    std::cout << "SetValue() = 10 后, n 的值: " << n << std::endl;

    int m = 7;
    int &r = m; // r 是 m 的引用
    
    /*
    定义引用时，一定要将其初始化成为引用某个变量。
    不能引用变量或者表达式（除非是常引用绑定临时对象）。
    初始化后，它就只能一直引用该变量，不能引用别的变量。
    // int &r=5; // 错误：非常量引用不能绑定到右值
    */

    std::cout << "初始化: m = " << m << ", r = " << r << std::endl;

    r = 4; // 修改 r 实际上是修改 m
    std::cout << "r = 4 后: m = " << m << ", r = " << r << std::endl;

    m = 8; // 修改 m 实际上也体现在 r
    std::cout << "m = 8 后: m = " << m << ", r = " << r << std::endl;

    /*
    注意：
    r = n * 4;  
    这里不是重新绑定引用，而是把右侧表达式的值赋给 r 所引用的对象 (m)。
    右侧是一个临时的整型值。
    */
    r = n * 4;  
    std::cout << "r = n * 4 后: m = " << m << ", r = " << r << std::endl;

    /*
    深入理解：
    “int &a = b;” 不是创建一个独立的指针变量；它是把名字 a 绑定到 b。
    在函数传参时，编译器通常以传地址（pointer）方式实现这个绑定。
    */
}

/**
 * @brief 2. 常引用 (Const Reference) 演示
 * 
 * 核心概念：
 * - const T& 可以绑定到：左值、const 左值、右值 (临时对象)。
 * - 能够延长临时对象的生命周期。
 * - 不能通过常引用修改其绑定的对象。
 */
void demonstrate_const_reference()
{
    std::cout << "\n--- 2. 常引用 (Const Reference) ---" << std::endl;

    /*
    常量引用可以引用常量，也可以引用非常量。
    但非常量引用只能引用非常量。
    */
    
    // 1. 绑定到右值 (临时对象)
    const int &c_r = 5; 
    std::cout << "const int &c_r = 5; // c_r = " << c_r << std::endl;
    // c_r = 6; // 错误：不能通过常引用去修改其引用的内容

    /*
    关于常引用的详细规则（笔记）：
    
    **概念区分**  
    - **初始化（绑定）**：`const T& r = expr;` —— 把引用 `r` 绑定到某个对象或临时上（只能在定义时绑定一次）。
    - **通过引用赋值（写入）**：`r = x;` —— 若 `r` 为 `const T&`，不能用 `r` 写入，被禁止。

    **哪些初始化是合法的（`const T&` 更宽松）**
    - 绑定到同类型的左值：`int x = 1; const int& r = x;` // 合法
    - 绑定到 const 左值：`const int cx = 2; const int& r2 = cx;` // 合法
    - 绑定到右值 / 临时（并延长临时寿命）：`const int& r3 = 42;` // 合法
    - 绑定到不同但可隐式转换的类型：`double d = 3.14; const int& ri = d;` // 合法（产生临时 int）

    **快速记忆要点**
    1. `const T&` 可以绑定到左值、右值与临时；而 `T&` 只能绑定到非 const 左值。
    2. `const T&` 能延长被绑定临时的寿命到引用的作用域。
    3. `const T&` 不能用于通过该引用修改对象。
    */
}

/**
 * @brief 3. 指针 (Pointer) 演示
 * 
 * 核心概念：
 * - 存放地址的变量。
 * - 可以为 nullptr。
 * - 可以重新赋值（改变指向）。
 * - 支持指针算术。
 */
void demonstrate_pointer()
{
    std::cout << "\n--- 3. 指针 (Pointer) ---" << std::endl;

    int a = -10;
    int v = 5;
    int *p; // 声明指针
    int *q;

    // 指针赋值：指向变量 a
    p = &a;
    std::cout << "p = &a, *p = " << *p << std::endl;

    // 改变指向：指向变量 v
    p = &v;
    std::cout << "p = &v, *p = " << *p << std::endl;

    // 通过指针修改值
    *p = 10;
    std::cout << "*p = 10 后, v = " << v << std::endl;

    // 复杂表达式
    *p = (*p) * (*p); // 等价于 v = v * v;
    std::cout << "*p = (*p) * (*p) 后: *p = " << *p << ", v = " << v << std::endl;

    // 指针赋值给指针
    q = &a;
    p = q; 
    std::cout << "p = q (指向 a) 后, *p = " << *p << std::endl;

    /*
    指针变量有一个值，是地址的开始。
    指针变量的类型决定了指针指向的对象占几个字节。
    */

    // 空指针
    p = nullptr; // 现代 C++ 推荐使用 nullptr 而非 0 或 NULL
    std::cout << "p = nullptr, p 的地址值: " << p << std::endl;
    // std::cout << *p << std::endl; // 错误：解引用空指针是未定义行为

    // 数组与指针
    int arr[5] = {20, 21, 22, 23, 24};
    int *array_p0 = arr;       // 数组名表示数组首元素地址
    int *array_p1 = &arr[1];
    int *array_p2 = &arr[2];

    std::cout << "数组与指针: " 
              << "*array_p0=" << *array_p0 << " " 
              << "*array_p1=" << *array_p1 << " " 
              << "*array_p2=" << *array_p2 << std::endl;
    
    /*
    建议：
    当你要求“必须有对象且语法更简单”用引用；
    当你需要“可空、可重指向或做数组遍历/所有权”用指针。
    */
}

/**
 * @brief 4. 常量与指针 (Const & Pointer) 演示
 * 
 * 核心概念：区分 "指向常量的指针" 和 "常量指针"。
 */
void demonstrate_const_pointer()
{    
    std::cout << "\n--- 4. 常量与指针 (Const & Pointer) ---" << std::endl;

    const int A = 10;  // const 第一种用法：定义常量  
    std::cout << "常量 A = " << A << std::endl;

    int a = 10;
    int b = 9;

    /*
    术语辨析 (Clarifying Pointer Terms):
    
    1. const T * p (Pointer to Const): 指向常量的指针
       - 记忆口诀：const 在 * 左边，修饰的是 *p (内容)。
       - 特性：指针 p 可以改变指向 (p = &other)；但不能通过 p 修改所指对象 (*p = ... 错误)。
    
    2. T * const p (Const Pointer): 常量指针
       - 记忆口诀：const 在 * 右边，修饰的是 p (指针本身)。
       - 特性：指针 p 初始化后不能改变指向 (p = &other 错误)；但可以通过 p 修改所指对象 (*p = ... 合法，前提是对象本身非 const)。

    3. const T * const p (Const Pointer to Const): 指向常量的常量指针
       - 特性：既不能改指向，也不能改内容。
    */

    // --- 演示 1: 指向常量的指针 (const int *) ---
    std::cout << "\n[演示 1] 指向常量的指针 (const int *p)" << std::endl;
    const int *p = &a; 
    std::cout << "p 指向 a (值为 " << *p << ")" << std::endl;

    // *p = 20; // 错误：不能通过指向常量的指针去修改其指向的内容
    // 解释：虽然 a 本身不是常量，但通过 p 看待 a 时，它是只读的。

    a = 20;     // 合法：可以通过变量名去修改变量的值
    std::cout << "a 修改为 20 后, *p = " << *p << " (p 随之变化)" << std::endl;
    
    // 改变指向是允许的
    p = &b; // 合法：p 是指向常量的指针，它自己不是常量，可以指向别处
    std::cout << "p 改指 b (值为 " << b << ") 后, *p = " << *p << std::endl;


    // --- 演示 2: 常量指针 (int * const) ---
    std::cout << "\n[演示 2] 常量指针 (int * const p)" << std::endl;
    int * const const_p = &a; // 必须初始化
    std::cout << "const_p 指向 a (值为 " << *const_p << ")" << std::endl;

    *const_p = 30; // 合法：指针本身是常量，但指向的内容可变
    std::cout << "通过 *const_p = 30 修改后, a = " << a << std::endl;

    // const_p = &b;  // 错误：不能改变常量指针的指向 (read-only variable 'const_p')


    // --- 演示 3: 指向常量的常量指针 (const int * const) ---
    std::cout << "\n[演示 3] 指向常量的常量指针 (const int * const p)" << std::endl;
    const int * const cp_cp = &a;
    std::cout << "cp_cp 指向 a (值为 " << *cp_cp << ")" << std::endl;

    // *cp_cp = 40; // 错误：不能改内容
    // cp_cp = &b;  // 错误：不能改指向
}

/**
 * @brief 辅助函数：演示 const char* 参数
 * 这样子可以防止“不小心”通过参数指针改变原变量的情况
 */
void GoodCall(const char *ptr)
{
    printf("%s", ptr);
}
