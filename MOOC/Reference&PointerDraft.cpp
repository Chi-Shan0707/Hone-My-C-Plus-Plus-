/*
注解
您可以将左值引用视为对象的另一名称。左值引用声明由说明符的可选列表后跟一个引用声明符组成。
引用必须初始化且无法更改。

地址可转换为给定指针类型的任何对象也可转换为相似的引用类型。例如，地址可转换为类型 char * 的
任何对象也可转换为类型 char &。

不要将引用声明与 address-of 运算符的用法混淆。&identifier 前面有 char 或 int 之类的类型时，
identifier 将声明为对该类型的引用。&identifier 前面没有类型时，用法就是 address-of 运算符的用法。
*/

/*
- char&（引用）  
  - 是某个 char 对象的别名（alias），必须在定义时初始化（绑定到已有对象）；之后不能改为引用别的对象。  
  - 不能为 null，语义上总是一个有效的对象。  
  - 在内存/ABI 上通常实现为指针，但在类型系统里 sizeof(char&) 等同于 sizeof(char)。  
  - 常用于函数参数传递需要保证存在且想直接修改实参时（更安全、语法像普通变量）。

- char*（指针）  
  - 存放地址的变量，可以为 nullptr，可以指向数组、字符串或单个 char。  
  - 可以重新赋值（指向不同地址）、可以做指针算术（遍历数组）。  
  - 使用 * 解引用访问/修改所指对象；使用 & 取地址得到指针。  
  - 常用于可选地址、动态分配或表示 C 风格字符串（以 '\0' 结尾的 char 数组）。


建议：当你要求“必须有对象且语法更简单”用引用；当你需要“可空、可重指向或做数组遍历/所有权”用指针。
*/

#include<iostream>
int n = 4;
int &SetValue();
void reference();
void const_reference();
void pointer(); 
void const_pointer();

int main()
{
    reference();
    const_reference();
    pointer();
    const_pointer();
    return 0;
}

int &SetValue()
{
    return n;
}

void reference()
{
    std::cout << "Here is reference;" << std::endl;
    /*
    定义引用时，一定要将其初始化成为引用某个变量
    不能引用变量或者表达式
    初始化后，它就只能一直引用该变量，不能引用别的变量
    */
    // int &r=5;错误
    SetValue() = 10;
    /*
    解释 SetValue 在你代码里的用法（简短）

函数签名 int &SetValue() 表示：函数返回一个 int 的左值引用（reference）。
函数体 return n; 返回全局变量 n 的引用。因此 SetValue() 表达式的类型是 int&，是一个可被赋值的左值。
所以 SetValue() = 10; 等价于 n = 10; —— 把右侧值赋给 n。这也是你在程序中看到的行为（输出 10）。
    
    */
    std::cout << n << std::endl;
    // 输出 10
    int m = 7;
    int &r = m;
    r = 4;
    std::cout << r << std::endl;
    std::cout << m << std::endl;
    m = 8;
    std::cout << r << std::endl;
    r = n * 4;  
    // 这里不是重新绑定引用，而是把右侧表达式的值赋给r，右侧是一个临时的整型值
    std::cout << r << std::endl;
    std::cout << m << std::endl;
    /*
    “int &a = b; 不是创建一个独立的指针变量；它是把名字 a 绑定到 b。在函数传参时，编译器通常以传地址（pointer）方式实现这个绑定。
    
    */
}

void const_reference()
{
    std::cout << "Here is const_reference;" << std::endl;
    /*
    常量引用可以引用常量，也可以引用非常量
    但非常量引用只能引用非常量
    */
    const int &c_r = 5;
    // 类型是 const int &
    // 不能通过常引用去修改其引用的内容
    
    // const T& 和 T&是不同的类型
    // const T 类型的常变量和const T& 类型的引用则不能用来初始化 T &类型的引用，除非进行强制类型转换
}
/*
计划（短）
- 说明“常引用”的两种语义场景：初始化（binding）与通过引用赋值（写入）。  
- 列出哪些初始化/赋值是合法的并给出代码示例。  
- 说明临时对象寿命延长与常见禁用情形的要点。

**概念区分**  
- **初始化（绑定）**：`const T& r = expr;` —— 把引用 `r` 绑定到某个对象或临时上（只能在定义时绑定一次，之后不能改绑定）。  
- **通过引用赋值（写入）**：`r = x;` —— 是把右侧的值赋给被引用的对象（若 `r` 为 `const T&`，不能用 `r` 写入，被禁止）。

**哪些初始化是合法的（`const T&` 更宽松）**
- 绑定到同类型的左值（非 const 也可以）：
  - `int x = 1; const int& r = x;` // 合法
- 绑定到 const 左值：
  - `const int cx = 2; const int& r2 = cx;` // 合法
- 绑定到右值 / 临时（并延长临时寿命到引用作用域）：
  - `const std::string& s = std::string("hi");` // 合法，临时寿命延长
  - `const int& r3 = 42;` // 合法，42 的临时被延长
- 绑定到不同但可隐式转换的类型（允许转换后绑定）：
  - `double d = 3.14; const int& ri = static_cast<int>(d);` // 可用隐式或显式转换后绑定（编译器可能插入临时）
  - 实际上 `const T&` 可绑定到需要一次隐式转换产生的临时（编译器构造临时，然后引用绑定到该临时）。
- 绑定到派生类对象的基类引用（多态场景常见）：
  - `struct D: B {}; D d; const B& br = d;` // 合法

**哪些初始化 / 赋值是非法或受限**
- 不能把右值绑定到非 const 左值引用：
  - `int&&` 或 `const int&` 可以绑定右值，`int& r = 42;` // 非法
- 不能把 `const T*` 的地址序列不安全地转换：类型系统关于 const 的丢弃规则仍然适用（不要把 `const T*` 转为 `T*` 然后写）。  
- 引用不能绑定到位域（bit-field）或某些不具地址性的表达式（例如不能对 bitfield 取地址来绑定引用）。  
- 对 `const T& r = expr;` 初始化后不能通过 `r` 写入：`r = new_val;` // 编译错误（read-only）

**写入（assignment）规则**
- 如果引用是 `const T&`，则不能通过该引用写入被引用对象：
  - `int x = 1; const int& r = x; r = 2;` // 错误，不能通过 `r` 改变 x
- 如果引用是非常量引用 `T&`，通过引用写入就是允许的并会修改实参：
  - `int &r2 = x; r2 = 3; // x 变为 3`
- 注意：即便 `r` 是 `const T&`，也可以在其它非 const 访问点（例如直接用变量名）修改该对象（若对象本身不是 const），`const` 只限制通过该引用的修改。

**临时对象寿命延长（重要）**
- `const T& r = T(...);` 会延长临时 `T(...)` 的寿命，使得该临时存活到引用 `r` 的作用域结束（不是只到表达式结束）。因此 `const T&` 常用于避免拷贝同时安全地绑定临时。
- 但注意：绑定到函数返回的临时也会被延长到引用的作用域（前提是引用是局部变量或全局变量等）。


**快速记忆要点（3 条）**
- `const T&` 可以绑定到左值、右值与临时；而 `T&` 只能绑定到非 const 左值。  
- `const T&` 能延长被绑定临时的寿命到引用的作用域。  
- `const T&` 不能用于通过该引用修改对象（不能写），且引用一旦初始化不能重新绑定。

*/
void pointer()
{
    std::cout << "Here is pointer;" << std::endl;
    int *p; int *q; int v; int a = -10;
    // 指针类型 * 指针变量名
    p = &a;
    std::cout << *p << std::endl;
    p = &v;
    v = 5;
    std::cout << *p << std::endl;
    *p = 10;
    std::cout << v << std::endl;
    *p = (*p) * (*p);
    // 等价于 v=v*v;
    std::cout << *p << " " << v << std::endl;
    q = &a;
    p = q;  // 指针赋值给指针
    std::cout << *p << std::endl;
    // 指针的作用是间接访问变量
    /*
    指针变量有一个值，是地址的开始
    指针变量的类型决定了指针指向的对象占几个字节（char 占一个字节，int 占四个字节 long long 占8字节）,
    这连着的这些东东都是这个对象的
    */
    /*
    指针初始化和赋值运算是有约束条件的，只能使用以下四种值
    地址初值不能是变量，即便是变量，即便是值=0的整型变量，也不能是整形数据
    int *p_error;
    int P=3;
    int *P_error=P;//错误
    p_error=5;//错误
    p_error=P;//错误
    */

    p = nullptr;
    // 虽然与p=0等价，但是还是现代一点
    std::cout << p << std::endl;
    // std::cout<< *p << std::endl;
    // 解引用空指针是未定义行为

    int arr[5]={20,21,22,23,24};
    int *array_p0, *array_p1, *array_p2;
    array_p0 = arr;
    array_p1 = &arr[1]; // 数组名表示数组首元素地址
    array_p2 = &arr[2];

    std::cout <<"数组与指针"<< *array_p0 << " " << *array_p1 << " " << *array_p2 << std::endl;
}

void const_pointer()
{    
    std::cout << "Here is const_pointer;" << std::endl;
    const int A = 10;  // const第一种用法：定义常量  
    int v;
    std::cout << A<< std::endl;
    


    /*
定义常量


*/
//定义常量指针：不可以通过常量指针去修改器指向的内容

    int a = 10;
    int b=9;
    const int *p = &a; //定义常量指针
    // *p = 20;          //错误，不能通过常量指针去修改器指向的内容
    a = 20;            //可以通过变量名去修改变量的值
    std::cout << "*p=" << *p << std::endl;
/*
根据哲学，我们既然使用了常量指针，就说明我们希望是直接修改变量本身，不能通过修改常量指针去动这个变量
所以我们不能把常量指针赋值给另一个非常量指针，不然你说那个非常量指针能不能被修改呢。
但反过来可以，我可以让一个非常量指针赋值给一个常量指针
*/
    int* q=&b;
    int *Q;
    //硬要搞事情可以吗，当然可以
    p=q;
    Q=(int *)p; //强制类型转换
   
}

void GoodCall(const char *ptr)
{
    printf("%s",ptr);
    //这样子可以防止“不小心”通过参数指针改变原变量的情况
}
/*

Clarifying Pointer Terms...
术语（中 / 英）

const T * p：指向常量的指针 / pointer to const T（也可写作 T const * p）
T * const p：常量指针 / const pointer to T（也常说 pointer-that-is-const）
补充：const T * const p：指向常量且自身为常量的指针 / const pointer to const T
逐项说明（含能否改指向 / 能否改所指）

const T * p（pointer to const T） :

中文：指向常量的指针（或“指向 T 的常量”）
含义：指针 p 可以改变指向（p = &other; 合法），但不能通过 p 修改所指对象（*p = ... 编译错误）。
能否修改所指对象：否（除非用不安全的 cast）
能否重新赋指向：是
示例：



T * const p（const pointer to T） :

中文：常量指针（指针本身是常量）
含义：指针 p 在初始化后不能改变指向（p = &other; 编译错误），但可以通过 p 修改所指对象（*p = ... 合法，前提 pointee 非 const）。
能否修改所指对象：是（如果 pointee 非 const）
能否重新赋指向：否


*/