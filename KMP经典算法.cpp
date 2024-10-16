#include "bits/stdc++.h"
using namespace std;

class myString{
private:
    string mainstr;
    int size;
    // 构建next数组
    void GetNext(string p, int next[]) {
        int len = p.length();
        next[0] = -1;
        int k = -1;  // 前缀指针
        int j = 0;   // 后缀指针

        while (j < len - 1) {
            if (k == -1 || p[j] == p[k]) {
                ++j;
                ++k;
                next[j] = k;
            } else {
                k = next[k];  // 回退
            }
        }
    }
    // KMP算法实现
    int KMPFind(string p, int pos, int next[]) {
        int i = pos;  // 主串指针
        int j = 0;    // 模式串指针
        int n = mainstr.length();
        int m = p.length();

        while (i < n && j < m) {
            if (j == -1 || mainstr[i] == p[j]) {
                ++i;
                ++j;
            } else {
                j = next[j];  // 使用next数组避免重复匹配
            }
        }

        if (j == m) return i - j + 1;  // 匹配成功返回匹配位置（从1开始）
        return 0;  // 匹配失败返回0
    }
public:
    myString() {
        size = 0;
    }

    ~myString() {
        size = 0;
    }

    void SetVal(string sp) {
        mainstr.assign(sp);
        size = mainstr.length();
    }

    // KMP查找子串
    int KMPFindSubstr(string p, int pos) {
        int L = p.length();
        int *next = new int[L];
        GetNext(p, next);  // 生成next数组

        for (int i = 0; i < L; i++) {
            cout << next[i] << ' ';
        }
        cout << endl;

        int result = KMPFind(p, pos, next);  // 查找子串
        delete[] next;  // 释放next数组
        return result;
    }
};

int main() {
    int t;
    cin >> t;  // 读入实例数量

    for (int i = 0; i < t; i++) {
        string mainstr, pattern;
        cin >> mainstr >> pattern;  // 读入主串和模式串

        myString ms;
        ms.SetVal(mainstr);

        int result = ms.KMPFindSubstr(pattern, 0);  // 查找模式串
        cout << result << endl;  // 输出匹配结果
    }
    return 0;
}
