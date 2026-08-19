# MyCPP

用 C++20 实现常用数据结构与算法的学习项目。所有组件均为手写实现，使用 googletest 编写单元测试，并支持 AddressSanitizer 内存检测。

## 已实现

- 字符串与容器：`MyString`、`MyVector`、`MyHashMap`
- 智能指针与内存管理：`MySharedPtr`、`MyWeakPtr`、`MyUniquePtr`、`MyMemoryPool`
- 树与跳表：`MyBST`（二叉搜索树）、`MyRBTree`（红黑树）、`MySkipList`
- 排序与查找：冒泡/堆/插入/归并/快排/选择排序、二分查找
- 工具类：`MyJsonParser`、`MyFileReader`、`MyFileWriter`、`MyMatrix`（Strassen 矩阵乘法）、`MyProfiler`、`MyError`、`MySingleton`、`MyLog`
- 并发：`MutexLock`、`FixedThreadPool`（线程池）、`MyThread`（并行测试辅助）

## 环境要求

- C++20 编译器：GCC 10+ / Clang 12+
- CMake 3.16+
- Ninja（可选，推荐，构建更快）
- Git（用于克隆仓库和子模块）

## 获取代码

仓库使用 git submodule 引入第三方依赖，克隆后需要初始化子模块：

```bash
git clone --recurse-submodules https://github.com/MedivhGO/MyCPP.git
cd MyCPP
```

如果已经克隆，可单独补初始化：

```bash
cd MyCPP
git submodule update --init --recursive
```

## 构建

```bash
cmake -S . -B build         # 配置
cmake --build build         # 构建
```

推荐使用 Ninja 生成器并指定构建类型：

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

构建产物统一输出到构建目录的 `bin/`（可执行文件）和 `lib/`（库）下：

```
build/
├── bin/MyCpp.info    # 演示程序
├── bin/MyCpp.test    # 测试程序
└── lib/libMyCpp.so   # 库
```

常用配置选项：

| 选项 | 说明 | 默认值 |
|---|---|---|
| `-DCMAKE_BUILD_TYPE=Debug/Release` | 构建类型 | 空 |
| `-DBUILD_TESTING=OFF` | 关闭测试构建（googletest 也不会编译） | ON |
| `-DMYCPP_ENABLE_ASAN=ON` | 开启 AddressSanitizer 内存检测 | OFF |

## 运行测试

测试使用 googletest，并通过 CTest 集成，每个用例独立注册：

```bash
cd build
ctest                       # 运行全部用例
ctest --output-on-failure   # 失败时打印详细输出
ctest -R "SkipList"         # 只跑名字匹配的用例
```

也可以直接运行测试程序：

```bash
./build/bin/MyCpp.test                                 # 全部用例
./build/bin/MyCpp.test --gtest_filter=SkipListTest.*   # 只跑某个用例组
./build/bin/MyCpp.test --gtest_also_run_disabled_tests # 强制运行禁用用例
```

### 单独编译一个测试文件

在 `test/CMakeLists.txt` 中按需添加单测目标：

```cmake
add_executable(test_myskiplist.test test_all.cpp container/test_myskiplist.cpp)
target_link_libraries(test_myskiplist.test PRIVATE ${PROJECT_NAME} GTest::gmock)
```

然后构建并运行：

```bash
cmake --build build --target test_myskiplist.test
./build/bin/test_myskiplist.test
```

## 内存检测（AddressSanitizer）

推荐使用独立的构建目录，避免与普通构建互相污染：

```bash
cmake -S . -B build-asan -G Ninja -DCMAKE_BUILD_TYPE=Debug -DMYCPP_ENABLE_ASAN=ON
cmake --build build-asan
cd build-asan && ./bin/MyCpp.test
```

运行时会检测越界读写、use-after-free、双重释放、ODR 违规等问题，出错立即终止并打印调用栈。

## 内存泄漏检查（valgrind）

```bash
valgrind --leak-check=full --leak-resolution=med --track-origins=yes ./build/bin/MyCpp.test
```

## 性能分析

使用 `util/MyProfile.h` 中的 `measure` 函数测量函数运行时间：

```c++
auto elapsed = measure(function_name, param1, param2).count();
```

## 安装（可选）

```bash
cmake --install build --prefix /your/install/prefix
```

默认安装到 `/usr/local`，产物布局：

```
<prefix>/
├── lib/libMyCpp.so
├── bin/MyCpp.info
└── include/          # 全部公共头文件
```

## 目录结构

```
├── CMakeLists.txt   # 顶层构建配置
├── include/         # 公共头文件（声明）
│   ├── container/   # 容器与数据结构
│   ├── memory/      # 智能指针与内存管理
│   ├── sort/        # 排序算法
│   ├── thread/      # 并发组件
│   └── util/        # 工具类
├── src/             # 实现
├── test/            # googletest 单元测试
├── resource/        # 资源与配置模板
└── third_party/     # 第三方依赖（git submodule）
```

## 第三方依赖

- [googletest](https://github.com/google/googletest) — 单元测试框架
- [doctest](https://github.com/doctest/doctest) — 单头文件测试框架
- [nanobench](https://github.com/martinus/nanobench) — 微基准测试

## References

See also:

- https://github.com/rangelak/Strassen-Matrix-Multiplication
- https://github.com/aronszanto/strassen
- https://github.com/danek0100/Cpp-Ex08-Implementation-of-MyString-class
- https://github.com/shakhovm/MySharedPtr
- https://github.com/DanesH-Abdollahi/DanesH-Abdollahi-AP_1400_HW4
- https://github.com/heysulo/MyMalloc
- https://github.com/0xf3cd/Compiler-Tokenizer
- https://github.com/Xue64/The-Dive-Library
- https://github.com/Dynmi/RedBlackTree
- https://github.com/percentcer/VectorTest
- https://github.com/taskflow/taskflow
- https://github.com/AleksanderBrzozowski/AAL
- https://github.com/shejialuo/topKProblem
- https://github.com/marcodiri/cpp-java-threadpool
- https://github.com/green-anger/MemoryPool
