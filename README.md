
- **MyString( const char\* )**
- **MyString( std::string )**
- **MyString( const MyString& )**
- **MyString( MyString&& )**
- **~MyString()**
- **length()**
- **get()**


- **operator+**
- **operator-**
- **operator\***
- **operator=**
- **operator=**
- **operator==**
- **operator!=**
- **operator>**
- **operator<**
- **operator>=** 
- **operator<=**
- **operator!**
- **operator[]**
- **operator()**
- **operator>>**
- **operator>>**

```cpp
#include "MyString.h"

int main()
{
    MyString s1("Hello,");
    MyString s2("world!");
    MyString s3;
    s3 = s1 + s2;
    std::cout << s3;
    std::cout << s3("ll");   // 2
    std::cout << s3("mama"); // -1
}
```