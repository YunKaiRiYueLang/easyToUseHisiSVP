# hisiSvp编程中的一个辅助函数和类的整理。

在使用时，只需要inc和lib文件即可。

特性说明：
1. hisiImage类用于实现ive图像的快速申请和释放。
2. 辅助函数的实现，打包为libhisiUtils.a静态库。
3. 存取图片封装了stb库。可以存储hisiImage类型图片或指针指向的图片
4. sample_开头的头文件是海思svp例子代码中的文件，移植过来的