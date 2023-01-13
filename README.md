# 实现一些 C++ 中的常用类或函数

## TODO

- 实现MyString
- 实现MySharedPtr
- 实现MyWeakPtr
- 实现MyUniquePtr
- 实现MutexLock
- 实现MyMatrix(strassen实现矩阵乘法)
- 实现Singleton
- 实现MyMalloc
- 实现MyBST(binary search tree)
- 实现MyError
- 实现MyHashMap
- 实现常见的排序算法以及二分查找
- 实现MyFileReader
- 实现MyRBTree
- 实现异步日志

## dependencies

- C++ 20

## build

```shell
git clone https://github.com/MedivhGO/MyCppImplement.git
cd MyCppImplement
mkdir build
cd build
cmake ..
make
```

modify test/CMakeList.txt file to generate correspond single unit test executable file.

- add_executable

- target_link_libraries

## run

```shell
cd build/test
./test_case
```

## valgrind

to check memory leak, you can use blow command.

```shell
/usr/bin/valgrind --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no ./build/test/MyCppImplement.test
```


## References

See also:

- https://github.com/rangelak/Strassen-Matrix-Multiplication
- https://github.com/aronszanto/strassen
- https://github.com/danek0100/Cpp-Ex08-Implementation-of-MyString-class
- https://github.com/shakhovm/MySharedPtr
- https://github.com/DanesH-Abdollahi/DanesH-Abdollahi-AP_1400_HW4
- https://github.com/heysulo/MyMalloc
- https://github.com/0xf3cd/Compiler-Tokenizer

