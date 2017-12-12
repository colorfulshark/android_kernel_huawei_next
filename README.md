# 华为mate8内核源码

> 经过配置的华为mate8 内核源码，非官方获取，无法编译成可用的内核，只能用于模块编译，适用于Android6.0的mate8手机。

以下是模块编译的步骤：

1. clone源代码到本地：

  ```shell
  git clone https://github.com/colorfulshark/huawei-mate8-kernel-source.git
  ```

2. 确定手机内核的version magic：

  ```shell
  adb shell cat /proc/version
  ```

  我测试的设备的version magic如下：

  ```shell
  Linux version 3.10.90-g01f8576 (android@localhost) (gcc version 4.9.x-google 20140827 (prerelease) (GCC) ) #1 SMP PREEMPT Tue Oct 25 05:18:00 CST 2016
  ```

3. 根据编译器版本[下载对应的NDK](https://developer.android.google.cn/ndk/downloads/index.html)，比如4.9.x对应ndk-r14b

4. 修改源码 `include/generated/utsrelease.h` 中的 `UTS_RELEASE` 宏为指定的值，我这里是 `3.10.90-g01f8576`

5. 修改源码 `build/setenv.sh` 中的环境变量为你本地交叉编译器的值，也可以将交叉编译器设置到环境变量中

6. 设置环境变量

  ```
  source build/setenv.h
  ```

7. 执行clean，并开始编译模块

  ```
  make clean
  make modules
  ```
