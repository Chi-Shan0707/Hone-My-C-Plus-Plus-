#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers> // C++20 标准数学常量

// 兼容性处理：如果编译器不支持 C++20，可以使用下方宏定义
const double pi = std::acos(-1.0);

// 定义复数类型
using Complex = std::complex<double>;
using Poly = std::vector<Complex>;

/**
 * 递归 FFT 核心函数
 * 
 * @param a 待变换的多项式系数数组（复数形式）
 * @param invert 是否进行逆变换 (IDFT)
 */
void fft_recursive(Poly& a, bool invert) {
    size_t n = a.size();

    // 【基准情况】：如果只有一个元素，直接返回
    // 因为 DFT 只有 1 个点时，值就是它本身
    if (n == 1) return;

    // 【分治步骤】：分离奇数次项和偶数次项
    // a0 存放偶数索引项 (0, 2, 4...)，对应 x^0, x^2...
    // a1 存放奇数索引项 (1, 3, 5...)，对应 x^1, x^3...
    Poly a0(n / 2);
    Poly a1(n / 2);

    for (auto i = 0; i < n >> 1 ; ++i) {
        a0[i] = a[2 * i];     // 提取偶数位
        a1[i] = a[2 * i + 1]; // 提取奇数位
    }

    // 【递归求解】
    // 计算 A0(x) 和 A1(x) 的 DFT
    fft_recursive(a0, invert);
    fft_recursive(a1, invert);

    // 【合并步骤】
    // 利用单位复根的性质合并两个子问题的解
    
    // 计算主单位根
    // 角度 = 2*pi/n，如果是逆变换则取负
    double ang = 2 * pi / n * (invert ? -1 : 1);
    Complex w(1, 0);             // 初始旋转因子 w_n^0 = 1
    Complex wn(std::cos(ang), std::sin(ang)); // 主单位根 w_n^1

    // 蝴蝶操作
    // 根据公式：
    // y_k       = y0_k + w_n^k * y1_k
    // y_{k+n/2} = y0_k - w_n^k * y1_k
    for (auto k = 0; k < n / 2; ++k) 
    {
        // 计算奇数项的贡献
        Complex t = w * a1[k];
        
        // 合并结果
        a[k] = a0[k] + t;           // 前半部分
        a[k + n / 2] = a0[k] - t;   // 后半部分
        
        // 更新旋转因子：w = w * wn
        w *= wn;
    }

    // 【逆变换特殊处理】
    // 在递归的最外层统一除以 n 是比较麻烦的，
    // 所以我们在每一层合并时都除以 2，效果等同于最终除以 n
    if (invert) {
        for (auto i = 0; i < n; ++i) a[i] /= 2.0;
        //这里的a无法>>=1，因为a是复数数组，除以2需要使用复数除法，而不是位移操作
        
    }
}

/**
 * 多项式乘法包装函数
 * 
 * @param a 多项式 A 的系数 (实数)
 * @param b 多项式 B 的系数 (实数)
 * @return 乘积结果的系数 (实数)
 */
std::vector<double> multiply(const std::vector<double>& a, const std::vector<double>& b) {
    // 1. 确定长度：结果长度最大为 len_a + len_b - 1
    // FFT 要求长度必须是 2 的幂，因此向上取整到最近的 2 的幂
    size_t n = 1;
    while (n < a.size() + b.size() - 1) n <<= 1;
    

    // 2. 数据转存：将实系数转为复数，并补零至长度 n（2的幂）
    Poly fa(a.begin(), a.end());
    Poly fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);

    // 3. 正向 FFT (系数 -> 点值)
    fft_recursive(fa, false);
    fft_recursive(fb, false);

    // 4. 点值相乘
    // 在点值表示法下，多项式乘法是 O(n) 的点对点乘法
    Poly fc(n); 

    for (size_t i = 0; i < n; ++i) {
        fc[i] = fa[i] * fb[i];
    }

    // 5. 逆向 FFT (点值 -> 系数)
    fft_recursive(fc, true);

    // 6. 结果提取
    // 取实部作为最终系数
    std::vector<double> result(a.size() + b.size() - 1);//一定是a.size() + b.size() - 1，因为乘积的最高次项是a的最高次项和b的最高次项之和;不是fa.size() + fb.size() - 1，因为fa和fb是补零后的长度，可能大于a.size() + b.size() - 1
    
    for (size_t i = 0; i < result.size(); i++) result[i] = fc[i].real(); //精度可控
    
    
    return result;
}

int main() 
{
    std::string a,b;
    std::cin>>a>>b;
    std::vector<double> poly_a ;
    std::vector<double> poly_b ;
    
    for(auto rit = a.rbegin(); rit != a.rend(); ++rit) poly_a.push_back(*rit - '0');
    for(auto rit = b.rbegin(); rit != b.rend(); ++rit) poly_b.push_back(*rit - '0');

    

    std::vector<double> res = multiply(poly_a, poly_b);
    std::vector<long long> digits(res.size() + 16, 0);

    for (size_t i = 0; i < res.size(); ++i) 
    {
        digits[i] += static_cast<long long>(std::llround(res[i]));
    }
    for (size_t i = 0; i + 1 < digits.size(); ++i) 
    {
        if (digits[i] >= 10) 
        {
            digits[i + 1] += digits[i] / 10;
            digits[i] %= 10;
        }
    }
    
    while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    for (auto it = digits.rbegin(); it != digits.rend(); ++it)std::cout << *it;
    std::cout << '\n';
    
    return 0;
}
