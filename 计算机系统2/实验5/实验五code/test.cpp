#include <bits/stdc++.h>

int test(int elems, int stride) {
    long i, sx2 = stride * 2, sx3 = stride * 3, sx4 = stride * 4;
    long acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
    long length = elems;
    long limit = length - sx4;

    /*Combine 4 elements at a time*/
    for (i = 0; i < limit; i += sx4) {
        acc0 = acc0 + data[i];
        acc1 = acc1 + data[i + stride];
        acc2 = acc2 + data[i + sx2];
        acc3 = acc3 + data[i + sx3];
    }

    /*Finish any remaining elements*/
    for (; i < length; i += stride) 
        acc0 = acc0 + data[i];
    return ((acc0 + acc1) + (acc2 + acc3));
}

/* run - Run test(elems, stride) and return read throughput
*        "size" is in bytes, "stride" is in array elements, 
*        and Mhz is CPU clock frequency in Mhz.
*/
double run(int size, int stride, double Mhz) {
    double cycles;
    int elems = size / sizeof(long);

    test(elems, stride);  // Warm up the cache
    cycles = fcy2(test, elems, stride, 0);  // Call test(elems, stride)
    return (size / stride) / (cycles / Mhz);  // Convert cycles to MB/s
}

int main() {
    

    return 0;
}