# easyToUseHisiSVP

海思svp关键点整理，觉得有用请Star或者直接参与整理

## 使用简要说明

1. 海思ive函数耗时记录，在 **耗时数据.md** 里。
2. 测试用的代码，在testcode文件夹里。
3. 对ive数据格式进行了封装以便于内存管理，在hisiImage中，仅支持8bit单通道类型。

## hisiImage类的使用

在文件夹 **对ive图像格式的封装** 中包含了对ive图像格式封装的源码。不使用opencv则不必使用 **cvive.hpp**

存取图像的代码。

```c++
    //从本地读取图像
    hisiImage src1;
    src1.imread("/mnt/bjwang/f11024.bmp");
    //存储一个图像
    writeGrayBmpImage("/mnt/bjwang/ive.bmp", src1);
    //配合ive函数的数据转换接口等见源码
```

## 待完善事项

**注意：** 部分函数控制参数太多，若未完全实现会注明

- [ ] 对ive函数的封装，进一步使用封装的ive图像相关的类
  - [x] iveAnd
  - [x] iveAdd
  - [ ] iveSobel
  - [ ] iveSub
  - [ ] iveThresh
- [ ] 完善智能指针，进一步优化内存管理

代码会陆续整合。欢迎Star。

## 和我讨论

mail:1510627880@qq.com

wechat: wbj095067
