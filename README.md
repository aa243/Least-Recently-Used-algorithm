# LRU

详情见tutorial

# 复杂度要求

所有要求实现的函数都要求O(1)

O(n)酌情给分

# 补充信息

（由于toturial写的匆忙，有些重要细节没能在展示细节的地方展示，这里结合同学的反馈加以补充）

1. 时间复杂度：所有要求实现的函数为O(1)
2. 对于文档里未提到的未定义行为，如一个实例erase了另一个实例的迭代器，just do nothing（没提到就说明测试代码里大概率不会出现，提到了就会出现）
3. 对于迭代器失效的情况（如insert之后hashmap扩容的情况，remove一个迭代器，这个迭代器不能继续使用的情况等）交给用户(测试代码)即可
4. 如果不特别指出用默认构造函数构造的迭代器，空迭代器就是指end()
5. linked\_hashmap继承的hashmap的模板不一定是\<Key,T,Hash,Equal>
6. 希望在提交之前都本地用valgrind测试一下,valgrind的参数如下 
~~~
   '--tool=memcheck',
    '--leak-check=full',
    '--exit-on-first-error=yes',
    f'--error-exitcode=250',
    '--quiet',
~~~
7. lru的成员函数后面的const是可以去掉的(如果你的成员变量没有使用指针)
8. 手册里的print()函数可能不符合实际的成员变量，大概格式正确即可，中间那个符号是空格(latex的代码会把空格以这种方式渲染)
9. 3.4的测试代码4.cpp 5.cpp在3.5已经修改，commit并且push
