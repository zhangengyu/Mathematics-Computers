package util;
public class ReverseArray {
    public static void main(String[] args) {
        int[] array = {1, 2, 3, 4, 5};
        reverse(array);
        for (int i : array) {
            System.out.print(i + " ");
        }
    }
    public static void reverse(int[] arr) {
        int left = 0;
        int right = arr.length - 1;
        while (left < right) {
            // 交换 arr[left] 和 arr[right]
            int temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            // 移动指针
            left++;
            right--;
        }
    }
}
