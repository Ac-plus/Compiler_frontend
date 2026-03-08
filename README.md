# TJU编译原理 C--

## 一、必要软件安装

1. 安装VSCode；
2. 安装MINGW64；
3. 在MINGW64里，导入make、gcc和g++。
## 二、编译步骤

如果使用Linux：
- 若不使用Makefile：
  - 编译lexical，使用以下命令：
    ```shell
    g++ -I include -g source/FSM.cpp source/SymbolTable.cpp source/util.cpp source/lexical.cpp -o lexical
    ```
  - 编译syntax，使用以下命令：
    ```shell
    g++ -I include -g source/syntax.cpp -o syntax
    ```
  - 运行lexical，使用以下命令：
    ```shell
    ./lexical
    ```
  - 运行syntax，使用以下命令：
    ```shell
    ./syntax
    ```
- 若使用Makefile：
  - 将run的后缀.exe去除即可
  
如果使用本机系统：
- 若不使用Makefile：
  - 编译lexical，使用以下命令：
    ```shell
    g++ -I include -g source/FSM.cpp source/SymbolTable.cpp source/util.cpp source/lexical.cpp -o lexical
    ```
  - 编译syntax，使用以下命令：
    ```shell
    g++ -I include -g source/syntax.cpp -o syntax
    ```
  - 运行lexical，使用以下命令：
    ```shell
    ./lexical.exe
    ```
  - 运行syntax，使用以下命令：
    ```shell
    ./syntax.exe
    ```
- 若使用Makefile：

  - 无需修改，安装`README.md`操作即可

