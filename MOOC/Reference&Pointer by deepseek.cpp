/*
 * 指针与引用学习程序
 * 
 * 核心概念：
 * - 引用：对象的别名，必须初始化且不能重新绑定
 * - 指针：存储地址的变量，可以重新指向，可以为nullptr
 * - 常量限定：const在不同位置的语义差异
 */

#include <iostream>

// 全局变量和函数声明
int n = 4;
int& SetValue();  // 返回引用的函数

// 演示函数声明
void reference_demo();
void const_reference_demo();
void pointer_demo();
void const_pointer_demo();
void good_call(const char* ptr);  // 使用常量指针的函数参数

int main()
{
    std::cout << "=== 指针与引用学习程序 ===" << std::endl;
    
    reference_demo();
    const_reference_demo();
    pointer_demo();
    const_pointer_demo();
    
    // 演示函数参数中的常量指针用法
    good_call("Hello, this demonstrates const pointer in function parameters!\n");
    
    return 0;
}

/*
 * 返回全局变量引用的函数
 * 返回值是左值引用，因此可以出现在赋值语句的左侧
 */
int& SetValue()
{
    return n;
}

/*
 * 演示引用的基本特性
 * - 引用是对象的别名，必须在定义时初始化
 * - 引用一旦绑定就不能重新绑定到其他对象
 * - 通过引用可以修改原对象的值
 */
void reference_demo()
{
    std::cout << "\n--- 引用演示 ---" << std::endl;
    
    // 1. 引用必须在定义时初始化
    int m = 7;
    int& r = m;  // r是m的引用（别名）
    
    std::cout << "初始值: m = " << m << ", r = " << r << std::endl;
    
    // 2. 通过引用修改原对象
    r = 4;
    std::cout << "通过引用修改后: m = " << m << ", r = " << r << std::endl;
    
    // 3. 原对象修改会影响引用
    m = 8;
    std::cout << "修改原对象后: m = " << m << ", r = " << r << std::endl;
    
    // 4. 引用不能重新绑定，但可以赋值（修改所引用对象的值）
    int k = 15;
    r = k;  // 这不是重新绑定，而是把k的值赋给r引用的对象m
    std::cout << "赋值操作后: m = " << m << ", r = " << r << ", k = " << k << std::endl;
    
    // 5. 函数返回引用可以作为左值
    SetValue() = 20;  // 等价于 n = 20
    std::cout << "通过函数返回引用修改全局变量: n = " << n << std::endl;
}

/*
 * 演示常量引用
 * - 常量引用可以绑定到常量、非常量和字面量
 * - 不能通过常量引用修改所绑定的对象
 * - 常量引用可以延长临时对象的生命周期
 */
void const_reference_demo()
{
    std::cout << "\n--- 常量引用演示 ---" << std::endl;
    
    int x = 10;
    const int& cr1 = x;  // 常量引用绑定到非常量
    std::cout << "常量引用绑定到非常量: x = " << x << ", cr1 = " << cr1 << std::endl;
    
    // cr1 = 20;  // 错误！不能通过常量引用修改对象
    
    x = 20;  // 但可以直接修改原对象
    std::cout << "修改原对象后: x = " << x << ", cr1 = " << cr1 << std::endl;
    
    // 常量引用可以绑定到字面量
    const int& cr2 = 42;  // 绑定到字面量，创建临时对象
    std::cout << "常量引用绑定到字面量: cr2 = " << cr2 << std::endl;
    
    // 常量引用可以绑定到不同类型的对象（产生临时对象）
    double d = 3.14159;
    const int& cr3 = d;  // 绑定到转换后的临时int对象
    std::cout << "常量引用绑定到不同类型: d = " << d << ", cr3 = " << cr3 << std::endl;
}

/*
 * 演示指针的基本特性
 * - 指针存储地址，可以重新指向
 * - 指针可以为nullptr
 * - 通过指针可以间接访问和修改对象
 */
void pointer_demo()
{
    std::cout << "\n--- 指针演示 ---" << std::endl;
    
    int a = 100;
    int b = 200;
    int* p = &a;  // p指向a
    
    std::cout << "指针初始化: a = " << a << ", *p = " << *p << std::endl;
    
    // 通过指针修改对象
    *p = 150;
    std::cout << "通过指针修改后: a = " << a << ", *p = " << *p << std::endl;
    
    // 指针可以重新指向
    p = &b;
    std::cout << "指针重新指向b: b = " << b << ", *p = " << *p << std::endl;
    
    // 指针运算
    *p = (*p) * 2;
    std::cout << "指针运算后: b = " << b << ", *p = " << *p << std::endl;
    
    // 空指针
    p = nullptr;
    std::cout << "指针设置为nullptr: p = " << p << std::endl;
    // *p = 10;  // 危险！解引用空指针是未定义行为
    
    // 数组与指针
    int arr[3] = {10, 20, 30};
    int* arr_ptr = arr;  // 数组名退化为指向首元素的指针
    
    std::cout << "数组元素通过指针访问: ";
    for(int i = 0; i < 3; ++i) {
        std::cout << *(arr_ptr + i) << " ";
    }
    std::cout << std::endl;
}

/*
 * 演示常量指针和指针常量
 * - const int* p: 指向常量的指针（指针可以变，指向的内容不能通过指针变）
 * - int* const p: 指针常量（指针不能变，指向的内容可以通过指针变）
 * - const int* const p: 指向常量的指针常量（都不能变）
 */
void const_pointer_demo()
{
    std::cout << "\n--- 常量指针演示 ---" << std::endl;
    
    int x = 1;
    int y = 2;
    const int z = 3;
    
    // 1. 指向常量的指针 - 指针可以重新指向，但不能通过指针修改对象
    const int* ptr_to_const = &x;
    std::cout << "指向常量的指针: x = " << x << ", *ptr_to_const = " << *ptr_to_const << std::endl;
    
    // *ptr_to_const = 10;  // 错误！不能通过指向常量的指针修改对象
    ptr_to_const = &y;     // 正确！指针可以重新指向
    std::cout << "指针重新指向后: *ptr_to_const = " << *ptr_to_const << std::endl;
    
    ptr_to_const = &z;     // 正确！可以指向真正的常量
    std::cout << "指向真正常量: z = " << z << ", *ptr_to_const = " << *ptr_to_const << std::endl;
    
    // 2. 指针常量 - 指针不能重新指向，但可以通过指针修改对象
    int* const const_ptr = &x;
    std::cout << "指针常量: x = " << x << ", *const_ptr = " << *const_ptr << std::endl;
    
    *const_ptr = 99;       // 正确！可以通过指针修改对象
    std::cout << "通过指针修改后: x = " << x << ", *const_ptr = " << *const_ptr << std::endl;
    
    // const_ptr = &y;     // 错误！指针不能重新指向
    
    // 3. 指向常量的指针常量 - 既不能修改指针，也不能通过指针修改对象
    const int* const const_ptr_to_const = &z;
    std::cout << "指向常量的指针常量: z = " << z << ", *const_ptr_to_const = " << *const_ptr_to_const << std::endl;
    
    // *const_ptr_to_const = 100;  // 错误！不能通过指针修改对象
    // const_ptr_to_const = &x;    // 错误！指针不能重新指向
}

/*
 * 演示函数参数中使用常量指针的好习惯
 * - 防止意外修改指针指向的数据
 * - 明确表达函数的意图（只读取不修改）
 */
void good_call(const char* ptr)
{
    std::cout << "\n--- 函数参数中的常量指针演示 ---" << std::endl;
    std::cout << "传递的消息: " << ptr;
    
    // ptr[0] = 'h';  // 错误！不能通过常量指针修改数据
    // 这保证了函数不会意外修改调用者传递的数据
}

/*
 * 关键要点总结：
 * 
 * 引用 (T&):
 * - 必须初始化，是对象的别名
 * - 不能重新绑定，语法更简洁安全
 * - 不能为null，总是指向有效对象
 * - 常用于函数参数和返回值，避免拷贝
 * 
 * 指针 (T*):
 * - 可以重新指向，可以为nullptr
 * - 需要显式解引用(*)
 * - 支持指针算术，可以遍历数组
 * - 用于动态内存管理、可选参数等场景
 * 
 * 常量限定:
 * - const T&: 可以绑定到任何对象，但不能通过引用修改
 * - const T*: 指向常量的指针，不能通过指针修改数据
 * - T* const: 指针常量，指针本身不能改变指向
 * - const T* const: 指向常量的指针常量，最严格的限制
 * 
 * 使用建议:
 * - 需要别名且对象必须存在时用引用
 * - 需要可选、重指向或数组遍历时用指针
 * - 函数参数尽量使用const引用或const指针表明意图
 */