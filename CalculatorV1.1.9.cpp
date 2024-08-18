// CalculatorV1.1.9.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <unordered_set>
#include <sstream>
#include <array>
#include <map>
#include "log.h" 
#include <cmath>
using namespace std;
// 定义计算器类
class calculator {
public:
    double output = 0.0; // 存储计算结果的成员变量

    inline calculator() {}; // 默认构造函数
    // 带有一个字符串参数的构造函数，用于初始化表达式
    inline calculator(std::string a) :expression(a) {}
    inline ~calculator() {}; // 析构函数

    // 计算表达式的函数
    
    bool evaluate() {
        std::string error;
        if (!isdigit(expression[0]) || !isdigit(expression[expression.size() - 1])) {
            if (expression[0] != '(' && expression[expression.size() - 1] != ')') {
                error = "错误：第一个字符和最后一个字符不能是运算符。";
                log(error, 2);
                return true;
            }
        }
        // 检查表达式中是否含有非法字符
        for (char i : expression) {
            if (find(pass_str.begin(), pass_str.end(), i) == pass_str.end()) {
                std::stringstream ss;
                ss << "错误：没有叫“" << i << "”的运算符。";
                error = ss.str();
                log(error, 2);
                return true;
            }
        }
        if (!isValid(expression)) {
            error = "警告：未闭合的括号。";
            log(error, 1);
            return true;
        }
        std::stack<char> op_stack; // 操作符栈  
        op_stack.push('\0'); // 向栈中压入一个空字符作为哨兵  
        double a1, a2;
        std::stack<double> value; // 操作数栈  
        std::string Postfix = infixToPostfix(expression); // 将中缀表达式转为后缀表达式  
#ifndef NDEBUG
        log(Postfix, 0);
#endif
        std::istringstream iss(Postfix);
        string cache;
        while (iss >> cache) {
            if (isnum(cache)) {
                value.push(stod(cache));
            }
            else {
                a1 = value.top(); value.pop();
                a2 = value.top(); value.pop();
                try{
                    value.push(apply_op(a2, cache, a1));
                }catch(std::runtime_error err) {
                    error = err.what();
                    log(error, 2);
                }
                output = value.top();
            }
        }if (value.size() != 1) {
            error = "内部错误：异常的表达式处理。";
            log(error, 2);
            return true;
        }
        return false;
    }
private:
    inline static bool isValid(std::string e) {
        std::array<char, 2> p = { '(', ')' };
        std::stack<char> ExStack;
        for (char i : e) {
            if (i == p[0]) {
                ExStack.push(i);
            }
            else if (i == p[1]) {
                if (ExStack.empty()) {
                    return false;
                }
                else {
                    ExStack.pop();
                }
            }
        }return ExStack.empty();
    }
    inline bool isnum(const std::string& a) const {
        try {
            int b = stod(a);
        }
        catch (std::invalid_argument err) {
            return false;
        }
        catch (std::out_of_range err) {
            return false;
        }return true;
    }
    std::string expression; // 存储表达式的成员变量

    // 执行计算操作的函数
    inline double apply_op(double a, std::string op, double b) const {
        if (op == "+") { return a + b; }
        if (op == "-") { return a - b; }
        if (op == "*") { return a * b; }
        if (op == "^") { return pow(a, b); }
        if (op == "/") {
            if (b == 0.0) { throw std::runtime_error("警告：你不能除以零。"); }
            else { return a / b; }
        }
    }
    const std::unordered_set<char> pass_str{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '-', '*', '/', '(', ')', '.', '^'};// 存储所有合法字符的无序集合
    const std::unordered_set<std::string> pass = { "+", "-", "*", "/" };// 存储所有操作符的无序集合
    // 将中缀表达式转为后缀表达式
    std::map<char, int> level = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2}, 
    {'^', 3}
    };
    std::string infixToPostfix(const std::string& infix) {
        std::ostringstream postfix;
        std::stack<char> op;
        for (int i = 0; i < infix.size(); i++) {
            if (isdigit(infix[i])) {
                std::ostringstream num;
                while (i < infix.size() && (isdigit(infix[i]) || infix[i] == '.')) {
                    num << infix[i];
                    i++;
                }
                postfix << num.str() << ' ';
                i--;
            }
            else if (infix[i] == '(') {
                op.push(infix[i]);
            }
            else if (infix[i] == ')') {
                while (!op.empty() && op.top() != '(') {
                    postfix << op.top() << ' ';
                    op.pop();
                }
                op.pop();
            }
            else {
                while (!op.empty() && level[infix[i]] <= level[op.top()]) {
                    postfix << op.top() << ' ';
                    op.pop();
                }op.push(infix[i]);
            }
        }while (!op.empty()) {
            postfix << op.top() << ' ';
            op.pop();
        }return postfix.str();
    }
};
int main() {
    // 输出欢迎信息
    std::cout << "欢迎使用计算器！" << std::endl;
    std::cout << "版本号：V1.1.92" << std::endl;
    std::string a;
    std::string error;
    int i = 0;
    while (true) {
        // 提示用户输入表达式
        std::cout << "输入一个算式:";
        std::getline(std::cin, a);
        // 如果用户输入'0'，则退出程序
        if (a == "") {
            error = "错误：空的表达式。";
            i++;
            log(error, 2);
            continue;
        }
        if (a == "0") {
            return 0;
        }
        if (a == "clear") {
            for (int j = 0; j < i; j++) {
                std::cout << "\033[A" << "\033[K";
                std::cout << "\033[A" << "\033[K";
#ifndef NDEBUG
                std::cout << "\033[A" << "\033[K";
#endif 
            }
            std::cout << "\033[A" << "\033[K";
            i = 0;
            continue;
        }
        // 创建一个计算器对象，并传入用户输入的表达式
        std::unique_ptr<calculator> c = std::make_unique<calculator>(a);
        if (c->evaluate()) {
            continue;
        }
        else {
            std::cout << "答案:" << c->output << std::endl;
        }
        i++;
    }
}
/*
* 更新历史：
* V0.0.1：不能使用
* V1.1.0：支持一位数的加减乘除，不支持小括号
* V1.1.2：优化错误提示，更适合中国人使用
* V1.1.5：支持多位数和小括号
* V1.1.7：支持浮点数
* V1.1.9：增加日志输出，更新历史，未闭合的括号错误提示
* V1.1.91：优化代码, 优化输出日志，增加clear功能（最新版）
* V2.0.1：尽请期待
*/

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
