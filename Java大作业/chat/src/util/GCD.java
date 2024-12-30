package util;

public class GCD {
    public static void main(String[] args) {
        int num1 = 56;
        int num2 = 98;
        System.out.println("GCD of " + num1 + " and " + num2 + " is: " + findGCD(num1, num2));
    }

    public static int findGCD(int a, int b) {
        while (b != 0) { // 当b不为0时，继续循环
            int temp = b; // 保存b的值，因为接下来b会被新的值覆盖
            b = a % b;    // 计算a除以b的余数，并将结果赋给b
            a = temp;     // a现在应该是上一个b的值
        }
        return a; // 最后返回的就是GCD（因为当b变为0时，a即为最大公约数）
    }
}
