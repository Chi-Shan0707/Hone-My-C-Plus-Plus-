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
    指针初始化和赋值运算是有约束条件的，只能使用一下四种植
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
    const int a = 10;  // const第一种用法：定义常量  
    int v;
    std::cout << a << std::endl;
    const int *b;
    
    b = &a;
    std::cout << *b << std::endl;
}