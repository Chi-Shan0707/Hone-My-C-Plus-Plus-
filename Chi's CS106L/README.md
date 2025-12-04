[CS106L Lectures in 2025?](https://web.stanford.edu/class/cs106l/)





[CS106L Lectures in 2020?](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1204/lectures.html)



## 流的链式操作（Chaining）

因为 `<<` / `>>` 返回的是对同一流对象的引用（`ostream&` / `istream&`），每次调用返回流本身，所以可以从左到右链式调用。

- 写入+读取示例：
```cpp
#include <sstream>
#include <string>

std::ostringstream oss;
oss << "123" << "456"; // oss.str() == "123456"

#include <sstream>
#include <string>

std::string str1, str2;
std::istringstream iss(oss.str());
iss >> str1 >> str2; // 从左到右依次提取到 str1, str2
```

（可选补充句）链式调用的另一个便利点是：流对象在被转换为布尔值时能反映错误状态，因此你也可以写成 `if (iss >> a >> b) { ... }` 以同时执行提取并检查是否成功。


## statebits