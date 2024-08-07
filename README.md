# 用 C++ 实现常用的数据结构和算法

## TODO

- [x] 实现MyString
- [x] 实现MySharedPtr
- [x] 实现MyWeakPtr
- [x] 实现MyUniquePtr
- [x] 实现MutexLock
- [x] 实现MyMatrix(strassen实现矩阵乘法)
- [x] 实现Singleton
- [x] 实现MyBST(binary search tree)
- [x] 实现MyError
- [x] 实现MyHashMap
- [x] 实现常见的排序算法以及二分查找
- [x] 实现MyFileReader
- [x] 实现MyFileWriter
- [x] 实现MyRBTree
- [x] 实现MyProfiler
- [x] 实现MyVector
- [x] 实现MySkipList
- [x] 实现MyThreadPool
- [x] 实现MyMemoryPool
- [x] 实现MyJsonParser
- [ ] 实现多线程相关 Demo

## dependencies

to support C++ 20 feature

- required gcc-10 g++-10 or higher
- required cmake 3.21 or higher

## build

```shell
git clone https://github.com/MedivhGO/MyCPP.git
cd MyCPP
mkdir build
cd build
cmake ..
make
```

modify test/CMakeList.txt file to generate correspond single unit test executable file.

add below code:

- add_executable

- target_link_libraries

## run

```shell
cd build/test
./test_case.test
```

## valgrind

to check memory leak, you can use blow command.

```shell
/usr/bin/valgrind --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no ./build/test/MyCpp.test
```

## profiler

```C++
// use MyProfile.h measure function to get runtime data
// example
 measure(function_name, param1, param2).count()
```

## doctest

The fastest feature-rich C++11/14/17/20/23 single-header testing framework

## nanobench

Simple, fast, accurate single-header microbenchmarking functionality for C++11/14/17/20

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
