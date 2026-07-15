# c++编译基础 -- https://zhuanlan.zhihu.com/p/618037867

## 1  预处理（Preprocess）
    g++ -E -I include/ main.cpp -o main.i
    # 或者直接调用 cpp 命令
    cpp -I include/ main.cpp -o main.i
    上述命令中： - g++ -E 是让编译器在预处理之后就退出，不进行后续编译过程，等价于cpp指令 - -I include/用于指定头文件目录 - main.cpp是要预处理的源文件 - -o main.i用于指定生成的文件名

## 2 编译（Compile）
    编译只是把我们写的代码转为汇编代码，它的工作是检查词法和语法规则，所以，如果程序没有词法或则语法错误，那么不管逻辑是怎样错误的，都不会报错。
    编译不是指程序从源文件到二进制程序的全部过程，而是指将经过预处理之后的程序转换成特定汇编代码(assembly code)的过程。

    编译的指令如下：

    g++ -S -I include/ main.cpp -o main.s
    
    与预处理类似，上述命令中： - g++ -S是让编译器在编译之后停止，不进行后续过程 - -I include/用于指定头文件目录 - main.cpp是要编译的源文件 - -o main.s用于指定生成的文件名

## 3 汇编（Assemble）
    汇编过程将上一步的汇编代码(main.s)转换成机器码(machine code)，这一步产生的文件叫做目标文件(main.o)，是二进制格式。

    gcc/g++的汇编过程通过 as 命令完成，所以我们可以通过g++ -c或as命令完成汇编：
    g++ -c -I include/ main.cpp -o main.o

    # 或者直接调用 as 命令
    as main.s -o main.
    
    上述指令中： - g++ -c让编译器在汇编之后退出，等价于 as - -I include/仍是用于指定头文件目录 - main.cpp是要汇编的源文件 - -o main.o用于指定生成的文件名

    汇编这一步需要为每一个源文件（本文示例代码中为main.cpp、func.cpp）产生一个目标文件。因此func.cpp也需要执行一次这个汇编过程产生一个func.o文件:

    # 可以用 g++ -c 命令一步生成 func.o
    g++ -c -I include/ src/func.cpp -o src/func.o
    # 当然也可以按照上面的预处理、编译、汇编三个步骤生成func.o

## 4 链接（Link）
    C/C++代码经过汇编之后生成的目标文件(*.o)并不是最终的可执行二进制文件，而仍是一种中间文件(或称临时文件)，目标文件仍然需要经过链接(Link)才能变成可执行文件。

    既然目标文件和可执行文件的格式是一样的（都是二进制格式），为什么还要再链接一次呢？
    因为编译只是将我们自己写的代码变成了二进制形式，它还需要和系统组件（比如标准库、动态链接库等）结合起来，这些组件都是程序运行所必须的。
    链接（Link）其实就是一个“打包”的过程，它将所有二进制形式的目标文件(.o)和系统组件组合成一个可执行文件。完成链接的过程也需要一个特殊的软件，叫做链接器（Linker）。

    此外需要注意的是：C++程序编译的时候其实只识别.cpp文件，每个cpp文件都会分别编译一次，生成一个.o文件。这个时候，链接器除了将目标文件和系统组件组合起来，还需要将编译器生成的多个.o或者.obj文件组合起来，生成最终的可执行文件(Executable file)。
    以本文中的代码为例，将func.o和main.o链接成可执行文件main.out，指令如下:

    g++ src/func.o main.o -o main.out

    -o main.out用于指定生成的可执行二进制文件名
    由于g++自动链接了系统组件，所以我们只需要把自定义函数的目标文件与main.o链接即可。
    运行main.out，结果如下：

    ./main.out
    a + b = 3

## 5 小结
    从上面的介绍可以看出，从C++源代码到最终的可执行文件的中间过程并不简单。了解预处理、编译、汇编、链接各个步骤的作用有助于我们处理更加复杂的项目工程。

    不过也不必被这么麻烦的编译过程劝退，当我们编译简单.cpp代码时，

    // hello.cpp
    #include <iostream>
    using namespace std;

    int main(){
        cout << "Hello, world!" << endl;
        return 0;
    }
    仍然可以直接使用g++命令生成可执行文件，而不必考虑中间过程：

    g++ hello.cpp -o hello
    ./hello
    Hello, world!