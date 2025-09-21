#include <bits/stdc++.h>

#define B 1
#define KB 1024
#define MB 1048576

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

random_device rd; //随机数生成
mt19937 gen(rd());

// void random_access(char *buffer, int size) {
//     int n = size / sizeof(char);
//     uniform_int_distribution<> dis(0, n - 1);

//     int test_times = 11451419 * 10;

//     vector<int> random_index;
//     for (int i = 0; i < test_times; i++) {
//         int index = dis(gen);
//         random_index.push_back(index);
//     }

//     int sum = 0;
//     high_resolution_clock::time_point t1 = high_resolution_clock::now();
//     for (int i = 0; i < test_times; i++) {
//         sum += buffer[random_index[i]];
//     }
//     high_resolution_clock::time_point t2 = high_resolution_clock::now();
//     duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
//     double dt = time_span.count();

//     cout << (size / 1024) << " " << (((double)sum / 1024.0) / dt) << endl;

//     delete[] buffer;
// }

// void test1() {  //不固定大小随机访问（不符合要求）
    
//     int size = 64 * KB;
//     int n = size / sizeof(char);
//     char *buffer = new char[n];
//     fill(buffer, buffer + n, 1);

//     vector<int> sizes{8 * KB, 16 * KB, 32 * KB, 64 * KB, 128 * KB, 192 * KB, 256 * KB, 384 * KB, 512 * KB, 1024 * KB, 1536 * KB, 2048 * KB, 3072 * KB, 4096 * KB, 6144 * KB, 8192 * KB};

//     for (auto s : sizes) {
//         random_access(buffer, s);
//     }
//     delete[] buffer;
// }

double ans[100][100];
void stride_access(char *buffer, int size, int stride, int ii, int jj, int MULP) {
    int n = size / sizeof(char);
    int sum = 0;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int z = 0; z < MULP; z++) {
        for (int j = 0; j < stride; j++) {      //使不同stride的遍历次数相同
            for (int i = 0; i < n; i += stride) {
                sum += buffer[i];
            }
        }

    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    double dt = time_span.count();

    //save
    ans[ii][jj] = (((double)sum / KB) / dt);
}

void test2() {  //固定大小，不固定步长
    int size = 400 * MB;
    int n = size / sizeof(char);
    char *buffer = new char[n];
    fill(buffer, buffer + n, 1);

    vector<int> strides{1 * B, 2 * B, 4 * B, 8 * B, 16 * B, 32 * B, 64 * B, 96 * B, 128 * B, 192 * B, 256 * B, 512 * B, 1024 * B, 1536 * B, 2048 * B};
    vector<int> sizes{8 * KB, 16 * KB, 32 * KB, 64 * KB, 128 * KB, 192 * KB, 256 * KB, 384 * KB, 512 * KB, 1024 * KB, 1536 * KB, 2048 * KB, 3072 * KB, 4096 * KB, 6144 * KB, 8192 * KB};
    vector<int> mulp{10000, 10000, 10000, 1000, 100, 10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1};
    // for (int i = sizes.size() - 1; i >= 0; i--) 
    //     for (int j = strides.size(); j >= 0; j--)  
    //         stride_access(buffer, sizes[i], strides[j], sizes.size() - 1 - i, strides.size() - 1 - j, mulp[i]);

    // for (int i = sizes.size() - 1; i >= 0; i--) 
    //     printf("\t%d", sizes[i] / KB);
    for (int i = 1, siz = 2; i <= 16; i++, siz *= 2) 
        printf("%d ", siz);
    // puts("");
    // for (int i = 0; i < sizes.size(); i++) {
    //     printf("size%d", sizes[i]/1024);
    //     for (int j = 0; j < strides.size(); j++) 
    //         printf("\t%.1lf", ans[i][j]);
    //     puts("");
    // }
    delete[] buffer;
}

int main() {
    // test1();
    test2();
    return 0;
}
