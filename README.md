# easyToUseHisiSVP

海思的SVP包含IVE IVS NNIE等多个部分. 目前整理的内容以IVE模块为主,觉得有用请Star参与改进。
项目相关的芯片有3519av100、3516av300、3516av100、SS928V100.
参考资料来源于网络等
## 使用简要说明
1. 海思ive函数耗时记录，在 [**耗时数据.md**](https://github.com/YunKaiRiYueLang/easyToUseHisiSVP/blob/main/%E8%80%97%E6%97%B6%E6%95%B0%E6%8D%AE.md) 里。
2. ~~测试用的代码，在[**testcode**](https://github.com/YunKaiRiYueLang/easyToUseHisiSVP/tree/main/testcode/ive%E6%97%B6%E9%97%B4%E6%B5%8B%E8%AF%95)文件夹里。~~
3. 对ive图像格式的封装，仅支持8bit单通道类型，[**封装代码**](https://github.com/YunKaiRiYueLang/easyToUseHisiSVP/tree/main/%E5%AF%B9ive%E5%9B%BE%E5%83%8F%E6%A0%BC%E5%BC%8F%E7%9A%84%E5%B0%81%E8%A3%85)。

4. 编程指南：[**正确使用svp的关键点.md**](https://github.com/YunKaiRiYueLang/easyToUseHisiSVP/blob/main/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97%EF%BC%9A%E6%AD%A3%E7%A1%AE%E4%BD%BF%E7%94%A8svp%E7%9A%84%E5%85%B3%E9%94%AE%E7%82%B9.md) 目前写了ive nnie 等模块和mpp相关的注意事项
5. test文件夹包含一些测试的代码和程序。将分别存放。帮助确认和排除一些问题。
## 编译

### 方式1

源码位于 hisiSvpAuxiliaryClassAndFunction文件夹中,修改这个目录下的makefile,就能使用自己的sdk编译整理好了函数库了.

### 方式2
直接将inc和src的文件拷贝过去用好了.

## hisiImage类的使用

在文件夹 **对ive图像格式的封装** 中包含了对ive图像格式封装的源码。不使用opencv则不必使用 **cvive.hpp**

使用示例：

```c++
    //从本地读取图像
    hisiImage src1;
    src1.imread("/mnt/bjwang/f11024.bmp");
    //存储一个图像
    writeGrayBmpImage("/mnt/bjwang/ive.bmp", src1);
    //使用滤波函数
    HI_S8 mask[25] = {
        1,4,7,4,1,
        4,16,26,16,4,
        7,26,41,26,7,
        4,16,26,16,4,
        1,4,7,4,1
    };
    HI_U8 norm = 8;
    int needBlock = 1;
    hisiImage src;src.imread("/mnt/bjwang/f11024.bmp");
    hisiImage dst;dst.create(src);
    eive::iveFilter(src, dst, mask, norm, needBlock);


```

## 待完善事项

**注意：** 部分函数控制参数太多，若未完全实现会注明

下面是已经封装的35xx的ive函数的列表
- [ ] 对ive函数的封装，进一步使用封装的ive图像相关的类
  - [x] iveAnd
  - [x] iveAdd
  - [x] iveDMA, dma directly copy
  - [x] iveSobel
  - [x] iveFilter
  - [x] iveSub
  - [x] iveThresh  可以进行binary模式 trunc模式 ORI_MID_MAX模式
  - [x] iveThresh16
  - [x] iveOrdStatFilter
  - [x] iveCSC 封装BT709_YUV2RGB BT709_RGB2YUV
  - [x] iveInteg 
  - [x] ive16BitTo8Bit
- [ ] 完善智能指针，进一步优化内存管理
- [ ] SS928V100上的ive相关的代码

代码会陆续整合。欢迎Star。

## 讨论

