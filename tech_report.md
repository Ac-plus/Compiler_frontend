# 天津大学编译原理与技术实验报告

> C--编译器前端实现与测试

## 1. 引言
本章将主要介绍本小组对于编译原理大作业课题的概要描述，包括问题描述、需求分析以及大致思路。
### 1.1 问题描述
本次大作业要求编写一个C--语言的编译器前端（包括词法分析器、语法分析器、语义分析及中间代码生成）：

- 使用自动机理论编写词法分析器；
- 自上而下或者自下而上的语法分析方法编写语法分析器；
- 补充完成中间代码生成部分代码。

### 1.2 顶端设计
任务流程图如图1-1所示。可见：整体来看，词法分析器位于最顶层，它输入源代码，输出的token序列送入语法分析器，从而生成对应语法树，生成预测分析表。语法树遍历序列送入下一层中间代码生成器，进而编译出LLVM IR的中间形式。

<img width="1014" height="511" alt="image" src="https://github.com/user-attachments/assets/44ea767c-6192-4df0-a5b0-cfafebd3d4ee" />

### 1.3 需求分析
需求分析分为3个部分：C--语言的单词符号类型与文法语言包含的操作，还有中间代码生成分析。分别介绍如下：

1. C--语言的单词符号类型：

关键字（KW，不区分大小写）包括：(1) int	(2) void (3) return	(4) const 

运算符（OP）包括：(5) +	(6) -	(7) *	(8) /	(9) \%	(10) =	(11) >	(12) <	(13) ==
(14) <=	(15) >=	(16) !=	(17) \&\&	(18) || 

界符（SE）包括：(19) ( (20) )  (21) {	(22) }	(23)； (24) ， 

标识符（IDN）定义与 C 语言保持相同，为字母、数字和下划线（\_）组成的不以数字开头的串。

整数（INT）定义与 C 语言类似。整数的正则表达式为

$$INT=-?(0\ |\ [1-9][0-9]^*)$$

2.语法语言包含的操作如下：

（1） 常量定义的声明（int a;）

（2）常量定义的初始化（int a = 3;）

（3）常类型数据的声明和定义（const int a = 3;）

（4）变量赋值传递（已声明的情况）（a = 3;/a = b;）

（5）函数名称，返回值类型，函数内部建立初始基本块并插入返回指令（int add(...)）

（6）函数传入参数的创建，管理（int add(int a, int b)）

（7）一元运算表达式（加法和乘法），（int a = 3 + 4; int c = a * b;）

（8）单目运算符号和变量数值结合（+/-/!）（!0/	-3）

（9）复合运算表达式（多个一元表达式结合 a = (b + c) * d）

（9）大小比较（ >/ >=/ </ <=） 

（10）双目运算符（|| \&\& <= == !=）

3.LLVM代码生成

在语法分析中与表达式有关的规则中构建语义分析，每个表达式都代表一种语义规则。我们需要构造生成四元式，采用后缀样式，新命名变量，记录在符号表中。之后，对符号表进行存取操作，从而生成中间代码。

需要注意的是，在Gitee上老师已经给出了遍历语法树过程中所需要调用的编译器中端。

## 2. 设计开发报告
### 2.1 概要设计
在本次实验中我们选择使用C++语言来实现C--语言的编译器。通过
仔细分析本次实验的需求，我们得到了如图2-1的设计流程图：

<img width="1042" height="636" alt="image" src="https://github.com/user-attachments/assets/c365e7e4-1c04-4c44-b2ea-fbf1fb1a2262" />


### 2.2 词法分析器的设计
下面将详细描述本次实验的NFA、DFA和符号表的设计思路。
#### 2.2.1 NFA
NFA的作用在于，后续将会把token输入到其中去，从而判断token的类别（如INT、IDN和UNDEFINED等）。

需要说明的是：

1. 其他类别诸如KW和OP等将在词法分析程序中判断，输入到有限自动机里的不会包含除了INT、IDN和UNDEFINED之外的token。

2. 负数涉及到符“-"，但是运算符不能被读入到NFA里。因此负数的判定将在之后的语法分析程序里单独添加一段实现逻辑，NFA只识别正整数。

我们所设计的NFA如图2-2所示。各个节点所表示的含义如下：
- Node 0. 起始节点
- Node 1. 状态为INT，表示只有一位的数字
- Node 2. 状态为UNDEFINED，表示输入一位数字后的过渡态，即将输入第二个数字
- Node 3. 状态为INT，表示多于1位的数字
- Node 4. 状态为IDN，表示只有1个字符组成的标识符
- Node 5. 状态为UNDEFINED，表示从起始态输入一个非数字字符后的过渡态，即将输入第二个字符
- Node 6. 状态为IDN，表示多于1个字符的标识符

<img width="1218" height="704" alt="image" src="https://github.com/user-attachments/assets/f849a9b2-3817-402c-ad20-60039594a991" />



#### 2.2.2 DFA

1. 确定化的DFA

将NFA确定化后得到DFA。DFA的构成如图2-3所示。

<img width="909" height="568" alt="image" src="https://github.com/user-attachments/assets/ab709779-719a-406c-9eb0-438d918d1740" />

各个节点的含义为如下。和NFA相比，DFA整合了2个过渡态，新增了一个数字0节点。
- Node 0. 起始节点
- Node 1. 状态为INT，表示数字0
- Node 2. 状态为INT，表示数字1-9
- Node 3. 状态为IDN，表示由`[a-zA-Z\_]`组成的标识符
- Node 4. 状态为IDN，表示长度大于1的标识符
- Node 5. 状态为INT，表示大于等于10的整数


2. 最小化的DFA

将DFA最小化后得到miniDFA。其构成如图2-4所示。

<img width="908" height="672" alt="image" src="https://github.com/user-attachments/assets/392e6d42-94a1-4dd6-9b6f-3337795d6a85" />

各个节点的含义为：
- Node 0. 起始节点
- Node 1. 状态是INT，表示整数0
- Node 2. 状态是INT，表示除了0的数字
- Node 3. 状态是IDN，表示全部标识符

可以看出，最精简的DFA删除了全部过渡态，且将全部IDN整合为了一个单独的节点。而INT还是划分为了0和其他数字，这是因为INT的正则表达式规定整数不能以0开头。

#### 2.2.3 符号表

符号表用于存储每个IDN的变量名（name）和存储在内存里的位置（value）。它由每一个表项（Record）连接而成。如图2-5所示，$a$,$main$,...,$e,f$为name，0x105acd8等为value示意值。

<img width="646" height="720" alt="image" src="https://github.com/user-attachments/assets/5aa24511-5d2f-44b2-8003-507174130614" />


1. 表项

这是一个name-value对，其中name表示IDN的名称，value表示对应的属性，通常指的是地址。

2. 符号表

这是一个unordered\_map数据结构，记录了每一个name-value对。由于它是无序的，因此需要通过哈希表的形式查找。

### 2.3 词法分析器的实现
本节将介绍实现词法分析器的过程中，各种算法的实现过程，以伪代码的形式给出。同时，还对具体操作进行说明，包括输出格式、源程序编译步骤等。
#### 2.3.1 数据结构的编码实现

(1) FSM节点

该结构（Node）用于表示NFA、DFA节点。之后，将各个加点串联起来再加上一些额外的属性和方法即可得到完整的FSM。

Node 所具有的属性如下：
```
• id：节点在NFA内的编号
• state：节点的状态，如TokenCode::INT，表示该节点下 token 表示整型变量
• trans：状态转换矩阵，根据输入的字符在哪个集合中决定。注意，为了简化代码量，NFA和DFA都只用这个转移矩阵，区别在于DFA的vector 只存储一个指针，因为DFA是单值函数
```

(2) FSM

该数据结构用于表示NFA和DFA。为此，我们设计了如下所示的数据属性。
```
• num：整型数，表示FSM中状态结点的个数。
• charList：以字符集为元素的集合，表示NFA的有穷字母表。例如：{numberList, letterList}。其中的每一个字符集可以表示 NFA 结点的输入。
• nodes：一个存放NFA节点的列表，表示当前NFA的全部状态节点。
• index：一个全部节点与其id组合而成的map列表，可根据id索引 NFA 节点。
• first：FSM 的首节点指针。
```
除此之外，FSM还包括一些类内函数（方法），用以实现后续操作：
```
• FSM()：初始化
• addNode()：添加节点
• addTrans()：添加状态转换规则
• getFirst()：获取第一个节点指针
• getNode()：根据节点编号获得节点指针
• getNum()：获取当前 FSM 节点数量
```

####  2.3.2 NFA的创建

使用createNFA实现。首先构造字母表，之后初始化FSM，然后参考NFA图2-2所示，依次添加7个节点即可：
```
/* createNFA算法伪代码
 * 输出：识别IDN和INT等的NFA
 */
letterList := [a-zA-Z_];
numberList := [1-9];
zeroList := {0};
charList := {letterList, numberList, zeroList};
NFA := FSM(charList);
定义节点Node 0-6;
for (i from 0 to 6) do:
    NFA.addNode(id=i, state(i), isEnd(i));
end for
根据NFA图添加边;
```

#### 2.3.3 NFA确定化过程

该部分的算法需要对NFA确定化成为DFA。对此也使用一个NFAtoDFA()实现。需要说明的是，这个算法是NFA确定化的通用算法，而不是仅仅输出一个本作业所需要的DFA。

算法的伪代码如下。首先构造DFA矩阵，然后构造一个栈结构存储还未遍历完的NFA节点集，以遍历NFA的每一个状态。此外，用statesMap存储NFA节点集合到DFA新节点之间的映射关系。每当遍历产生出一个新的NFA节点集，就将其加入到statesMap中，同时添加DFA节点编号。在遍历的每一步中，也将新节点加入DFA矩阵中。最后根据DFA矩阵构造出FSM即可。
```
/* NFA确定化算法伪代码
 * 输入：原始NFA
 * 输出：DFA
 */
letters := [a-zA-Z_];
numbers := [1-9];
zero := {0};
FSM DFA := FSM(NFA.getCharList());
stack<set<int>> nodes; // 遍历产生新节点的栈
map<int, set<int>> statesMap; // 存储当前的eplision-closure(move(I,a))对应的DFA中的标号，避免重复
set<int> statesMatrix[10][4] = {set<int>{}};   // DFA状态变换矩阵
nodes.push({0});
statesMap.insert({0,{0}});
statesMatrix[0][NFASTATES] = {0};
while (node非空) do:
    cNodes = nodes.top();
    nodes.pop();
    for (node in cNodes) do:
        根据node找到对应的NFA节点
        往I_letters, I_numbers, I_zero里添加这些节点输入letters,numbers,zero的全部节点
    end for
    //添加矩阵元素
    statesMatrix[cNodes的DFA编号][NFASTATES] = cNodes;
    for (chList in {ZERO,LETTERS,NUMBERS}) do:
        statesMatrix[cNodes的DFA编号][chList] = 对应的I;
    end for
    if (epsilon-closure(I_{_a-zA-Z|1-9|0}) not in statesMap) then:
        往statesMap里添加
        同时压入栈中
    end if
end while
// 给DFA添加节点
// statesMap里的每个key都是一个新节点,但起始节点0不重复添加
for ({id, nodes} in statesMap) do :
    DFA.addNode(new FSM_Node(id, nodes状态, 是否含有终态节点));    
end for
// 给DFA添加节点转换规则
for ((i, j) in statesMatrix and 非空集) do:
    DFA.addTrans(i, 矩阵元素对应的DFA节点id, 对应的输入chSet)
end for
return DFA;
```

#### 2.3.4 DFA最小化过程 

算法步骤如下：

1. 首先将原始节点划分为终态集合和非终态集合，进入到partition集合内。

2. 使用nodeStack栈同时存储还能继续划分的节点集合。每当完成一次划分，就将划分后的节点集合压入栈和partition中，同时将原集合从栈和partition中删除。在遍历的过程中，如果判定一个集合不可划分，则将其从nodeStack弹出。

3. 当栈空后，现行partition即为划分的集合。根据划分的结果构造miniDFA即可。

上述算法中，判定一个节点集合（设为nodes）可否划分的逻辑为：

1. 对于nodes中每个节点，求出其经zeroList,numList和letterList到达的节点集合；

2. 如果这些集合中有一个不包含于现行partition中的任一个节点集，则认为nodes不可划分；否则认为可划分。

对节点集合（同样设为nodes）拆分的步骤为：

1. 找到那个在判定可否划分的算法中，使nodes里的所有结点输入后不能到达现行partitition的任一节点集的charList；

2. 对于该charList，求出nodes经其到达的节点集里的节点分别位于partition的哪一节点集。注意，空集需要单独分出；

3. 根据这些节点集的不同对nodes里的节点聚类。分出的每一类即为一个划分后的集合。

综上所述，DFA最小化的算法伪代码如下所示。

```
/* DFA最小化算法伪代码
 * 输入：DFA
 * 输出：minimized-DFA
 */
int DFAMatrix[DFA.getNum()][NUM_CHARLISTS] = {};  //构造原DFA的等价状态矩阵
根据DFA给矩阵赋值，空集值为-1;
// 初始化操作
partition = {{DFA终态节点id}, {剩余节点}}; // 状态的划分
nodeStack = {{DFA终态节点id}, {剩余节点}}; // 用于临时存储划分的节点，(每一步)只保存还能继续划分的
while (nodeStack不空) do: // 栈非空，表示还有可以继续划分的
    cNodes = nodeStack.top();
    nodeStack.pop();
    map<from:int, to:set<int>> nodeMap; // 指示move(node, chList)位于PI的哪个子集
    for (charListId in {ZERO, NUMBERS, LETTERS}) do:
        nodesMap添加从node出发输入chList到达的节点在现行Partition的哪一元素集里
        res = reduceByTo(nodeMap);  //根据子集的不同归类
        if (res仅一个元素) then:
            清空nodesMap;
            continue;
        else:  
            将划分好的nodes压进集合和栈;
            break;
        end if
    end for
    从partition删除cNodes;
end while
for (nodes in partition) do:
    if (nodes.size() < 2) continue;
    int minNodeId = nodes里的最小值
    for (chSetId in ALL_LISTS) do:
        for (int nodeId in nodes) do:
            if (元素在nodes里) then: 
                DFAMatrix对应值归并为minNodeId;
            else: 
                DFAMatrix对应值不变
                continue;
            end if
        end for
    end for
    把modes里除了最小元素对应的矩阵行全部删除 
end for
FSM miniDFA = FSM(DFA.getCharList());
for (当前DFA矩阵第一列的非空元素 id) do:
    给miniDFA添加对应节点(id)
end for
// 给miniDFA添加变换规则
for (DFA矩阵里的非-1元素) do:
    给miniDFA添加对应边;
end for
return miniDFA;
```

#### 2.3.5 词法分析过程 
本节将介绍词法分析器的整体流程。如图 2-6 所示。在词法分析过程中，首先读入源代码文件，然后经过lexicalAnalysis()将其拆分成为tokens，对于这些token是否是OP、KW和SE都可以直接与标准符号进行字符串比较后得出。对于INT和IDN则需要送入DFA后，通过自动机分析得到具体的token类别。

<img width="1168" height="361" alt="image" src="https://github.com/user-attachments/assets/d6678d36-d65c-47a3-894b-4f6957d5ddae" />

对于lexicalAnalysis方法，实现tokenize的逻辑如下：

1. 除了分割符和已经定义好的界符和运算符之外，其余所有token序列都应交给自动机判断；

2. 利用空格、分隔符、界符和运算符将源码程序分开，因为带有运算符的字符串不应该进入自动机。

3、这样一来，我们就可以通过逐个字符读取源码，遇到空格、分隔符、界符和运算符就将之前的部分整合为一个token分出即可。

实现负数判定的逻辑如下：

1. 当读到“-”运算符时，如果它的上一个字符为字母、下划线、数字或“）”，则将“-”当作减号，否则当作负号。使用全局布尔型变量neg保存当前“-”是否为负号。

2. 如果识别到诸如“-5”这样的token，由于送入自动机的没有运算符，因此若neg为true，则直接将“-5”当作INT输出，不送入自动机。

#### 2.3.6 输出格式说明 
词法分析器的输出包括单词符号序列和符号表。单词符号序列的输出格式如下所示：
```
[token 1] [TAB] <[tokenCode 1]> 
[token 2] [TAB] <[tokenCode 2]> 
...
[token n] [TAB] <[tokenCode n]> 
```

例如：
```
int <KW>
a   <IDN>
=   <OP>
10  <INT>
;   <SE>
```

最后将其打印到txt文件中。

符号表的输出格式如下所示：
```
name [TAB] value
[token 1] [TAB] [value 1]
[token 2] [TAB] [value 2]
...
[token n] [TAB] [value n]
```

例如：
```
name    value
b       0x119ef80
main	0x119ef18
a       0x119c3d8
```
最后将其打印到txt文件中。

\subsection{ 源程序编译步骤}

词法分析器的编译运行步骤如下：

1. 实验环境与软件准备

您需要安装VSCode、MINGW等工具，在Windows或Linux环境下均可运行。

2. 编译

如果您安装了make，那么输入如下指令即可完成词法分析器的编译：
```
make lexical
```
如果您未安装make，那么输入如下指令也可完成词法分析器的编译：
```
g++ -I include -g source/FSM.cpp source/SymbolTable.cpp     source/util.cpp source/lexical.cpp -o lexical
```
3. 运行

使用下列指令，运行词法分析器。之后，您就可以得到两个txt文件的输出。
```
./lexical [test_file_path]
```

### 2.4 语法分析器的设计

语法分析器我们采用LL(1)自顶向下分析法实现。LL(1)语法分析器是自顶向下的一种语法分析方法，也称为递归下降分析器。它根据文法直接推导句子的过程来分析语言文法。

#### 2.4.1 LL(1)文法要满足的条件

LL(1)语法分析器中的LL(1)文法必须满足这些条件：

1. 不存在左递归；

2. 不存在二义性；

3. 没有公共左因子。

#### 2.4.2 LL(1)分析法的主要步骤

LL(1)分析法的流程图如下图2-7所示：

<img width="958" height="537" alt="image" src="https://github.com/user-attachments/assets/4e8ad94d-9151-42dd-af94-4171de9bcef7" />


LL(1)分析法主要的步骤如下：

1. 对数据、语法定义派生规则，构建语法分析表；

2. 从输入文本的字符串中读入一个字符，生成输入串str；

3. 取分析栈的栈顶$T$，如果$T$属于终结符，则匹配str进入读入下一字符；

4. 如果$T$属于非终结符，根据LL(1)文法所定义的分析表中，首符从左到右往下搜索，找出相关规则模板，若所搜索到的非终结符号$V_n$的首符符合str中的文本，那么就在分析栈和输出栈中弹出已经匹配的值，并将由$V_n$生成的产生式逐一压入分析栈，一直执行到分析栈为空为止。如果搜索到分析表中对应的位置为空，则说明语法错误，分析失败；

5. 循环执行步骤3和步骤4，直到处理完整个输入串str，如果str被正确匹配，那么分析成功，否则分析失败。

本次实验中文法由老师所提供，并且符合LL(1)文法。在此次实验中，我们根据LL(1)文法的主要步骤，在语法分析器中构建了主要函数并成功实现了语法分析器的功能。

### 2.5 语法分析器的实现

在语法分析器的实现环节，我们定义了void getVnVt()、void getFirst(string token)、bool getFollow()、bool getTable()、bool doAnalysis() 以及一些其他的辅助函数来实现语法分析器，接下来我们将从这些函数的具体实现来展开介绍。

#### 2.5.1 语法分析整体流程

如图2-8所示。

1.首先使用读入语法规则，检测左递归；

2.构建非终结符First集和Follow集；

3.构建预测分析表；

4.将栈顶加入program，每次取栈顶元素；

5.如果是非终结符则按照预测分析表对终结符进行规约；

6.如果是终结符则读入词法分析结果并出栈；

7.如果第六步中有不对应或者预测错误，则说明语法错误，否则如果读完整个词法分析栈中也没有元素，则说明正确。


<img width="1058" height="665" alt="image" src="https://github.com/user-attachments/assets/f65ce946-37e8-4d6e-a9ee-62c7866f928e" />


**左递归的检测与排除**

排除左递归的算法步骤如下所述：

1.将每个规则的第一个符号(非终结符）加入到该规则左侧非终结符的集合当中；

2.迭代第一遍后，对于每个非终结符的集合求其式子里所有符号的并集并进行更新；

3.通过对每个非终结符迭代计算；

4.当检测到含有自身时，说明有左递归。当迭代不再更新且未检测到自身，则说明没有左递归。

执行上述算法后，认为本次文法不包含左递归。

**getVnVt()函数的实现**

此函数的功能为读取提供的文法文件（grammar.txt），解析其中的语法规则并识别出终结符$V_t$与非终结符$V_n$。伪代码如下：
```
fileReader = freopen(GRAMMAR_FILE);
var c(char), right, left(bool), Id(int), str(string), line(int);
var cur(string) = NULL;
while (c in fileReader) do:
    if (c == '\n') then:
        Vt.insert(cur);
        right -> push_back(cur);
        将right添加到终结符列表Id位置处;
        left = TRUE, cur = NULL;
        continue;
    else if (c!=' ' and c!='\t') then:
        cur右侧添加c;
    else if (c==' ' or c=='\t') then:
        if (cur为空) continue;
        if (cur=="->") then:
            left = 0;
            cur = "";
        else if (left为真) then:
            获取该非终结符在集合Vn中的唯一标识编号Id
            if (Id == 0) then:
                新建一个编号并将其存储到Map和Map2中
                update Id;
            endif
            Vn.insert(cur);
            right = new vector<string>;
            cur = ""; 
        else ：
            将该符号添加到right所指向的动态数组中;
            将该符号添加到终结符集合Vt; 
            cur = "";
        endif
    endif
endwhile
将非终结符集合Vn中的元素从终结符集合Vt中移除;
close(fileReader);
```

对此的说明如下：

（1）使用标准C库函数freopen()打开指定路径的文本文件，并将其设置为标准输入流的方式，使函数可以从文件中读取数据；

（2）定义一个字符类型变量 c，一个动态数组指针变量 right，一个字符串类型变量 cur，一个布尔类型变量 left，一个整型变量 Id，和一个字符串类型变量 str，以及一个整型变量 line，并将 cur 的初始值设为空字符串；

（3）使用 while 循环读取文件中的每一个字符并存储到变量 c 中，对当前字符识别判断并做出相应的反应；

（4）如果当前字符c为换行符 “\string\n”，则说明已经读取了一个产生式规则。此时需要将当前字符串cur添加到终结符集合$V_t$中，将 right 所指向的动态数组根据当前 Id 值添加到该产生式左侧非终结符的推导式列表 v[Id] 中。并将 left 标记设为 1，表示接下来读取的是产生式左侧的符号。cur 的值设为空字符串。

（5）如果当前字符 c 不为空白字符，说明当前在读取一个符号，则将字符 c 添加到字符串 cur中；

（6）如果读取到了空格 “ ” 或制表符 “\string\t” ，则说明字符串 cur 已经读取了一个符号。此时需要根据当前 cur 的读取的符号来判断下一步；

（7）如果是 cur 为空集，则continue。如果 cur 为 “->”，说明接下来将要读取产生式右侧的符号，需要将 left 标记设为 0，并将 cur 的值设为空；

（8）如果 cur 既不为空也不是 “->”，说明 cur为一个产生式中的符号，此时需要根据 left 标记进行判断。如果 left 为 1，说明此时字符串 cur 中为产生式左侧的非终结符。此时需要获取该非终结符在集合$V_n$ 中的唯一标识编号 Id。如果该元素已经存在，则直接将其编号赋值给 Id，否则需要新建一个编号并将其存储到 Map 和 Map2 中，并将其编号赋值给 Id。然后将字符串 cur 添加到集合$V_n$ 中，并新建一个空的动态数组，将其赋值给 right 最后将 cur 的值设为空字符串；

（9）如果当前 left 的标记为 0，则说明 cur 为该左侧非终结符的一个推导式中的一个符号。此时需要将该符号添加到 right 所指向的动态数组中，并将该符号添加到终结符集合$V_t$中，并将 cur 的值设为空字符串；

（10）遍历完整个文件之后，需要将非终结符集合$V_n$ 中的元素从终结符集合$V_t$ 中删去，最后关闭文件。

该函数的主要思路为读取提供的文法文件，对读取出的字符进行存储并判断，最终求出非终结符以及终结符的集合，以及非终结符对应的所有产生式的集合以供后续算法使用。

**getFirst()函数的实现**

该函数的功能为通过算法求出当前符号 token 的First集。First集是指文法中每个非终结符号所能推导出的所有可能的终结符号的集合。它是用来进行语法分析的一种重要工具，可以用来判断一个输入串是否符合文法规则。

这个函数的作用是求出给定文法中每个非终结符的FIRST集合。函数接收一个字符串 token，表示要求FIRST集合的非终结符。伪代码如下：
```
if gotFirst(token) then:
    return;
endif
id = tokenId[token];  //获取符号id
for vector<string>vec in Rules[id]: //遍历这个符号为左式的规则
    bool hasEmpty = TRUE;
    for str in vec: 
        if str是终结符 then 在first[id]中加入str;
        getFirst(str);  // 递归获取右侧所有符号first集合
        first[id]中加入first[str]-'$';
        if first[str]不含有'$' then
            hasEmpty = FALSE;
            break;
        end if
    endfor
    if hasEmpty then first[id]中加入'$';
end for
    
```

对此的说明如下：

（1）首先判断当前 token 是否求过FIRST集合，如果已经求过则直接返回即可；

（2）如果 token 属于终结符，则创建一个只包含 token 的集合作为其FIRST集合加入到FIRST集中并返回；

（3）如果 token 属于非终结符，首先遍历所有 token 能够推导出的产生式集合。对于每一个产生式，遍历其中的符号并递归调用 getFirst() 函数求取每一个符号的FIRST集。定义 set 容器存储当前符号的 FIRST 集并且遍历当前容器 FIRST 集中的所有符号；

（4）将当前符号的 FIRST集加入到 token 的FIRST集中。并且判断当前符号的FIRST集中是否包含空集 \string$，如果包含，则继续遍历产生式的下一个符号，否则停止遍历当前产生式；

（5）每条产生式遍历结束后，判断是否该产生式所有符号的FIRST集合都包含空集，如果是，则将空集 \string$ 加入 token 的FIRST集中；

（6）所有产生式遍历完成后，将 token的FIRST集加入到FIRST中，并使用 for 循环递归调用 getFirst() 函数求出所有符号的FIRST集合。

该函数的主要思路是通过遍历产生式，对于每个符号求出其FIRST集合，然后根据FIRST集合中是否有空串来判断是否需要继续求下一个符号的FIRST集合，并将非空串加入该符号所在产生式的FIRST集合。这个过程需要多次递归，最终得到所有非终结符的FIRST集合。

**getFollow()函数的实现**

该函数的功能为通过算法求出并更新所有非终结符 $V_n$ 的 Follow 集。Follow集是指在语法分析中，对于每个非终结符号，其后继终结符号的集合。

对于一个非终结符号$V_n$，其 Follow 集合包含所有可以紧随 $V_n$ 出现的终结符号。Follow 集对于构建语法分析表格和进行语法分析非常重要，并且通过follow集，可以确定在某个非终结符号的右侧可能出现的终结符号，从而在语法分析时进行正确的推导和规约。

该函数的实现流程如图2-9所示。

<img width="867" height="771" alt="image" src="https://github.com/user-attachments/assets/5e7e6c86-7358-4281-9710-759119438082" />




该函数的主要实现步骤如下：

（1）首先遍历所有非终结符号 $V_n$，查看每一个非终结符是否有 Follow 集。如果其 Follow 集合不存在，则创建一个空的 Follow 集合。

（2）对于每个非终结符号 $V_n$，遍历所有产生式，找到其中包含该非终结符$V_n$的产生式；

（3）对于找到的产生式，从该非终结符号的下一个符号开始，依次计算每一个符号 First 集合，并将其加入到该非终结符号的 Follow 集合中；

（4）如果当前符号的 First 集合不包含空集 \string$，则直接停止计算，继续遍历下一个产生式；

（5）该产生式在该终结符之后的符号均可推出空集 \string$， 则将该产生式的头部符号的 Follow 集合加入到该非终结符号的 Follow 集合中；

（6）通过对比更新前后 Follow 集的大小判断该终结符 Follow 集是否被更新；

（7）如果更新了，说明所有的 Follow 集都需要更新。即最后返回 true 继续更新 Follow 集。

这个函数的主要思路是遍历所有的非终结符，对于每个非终结符，找到包含该非终结符的产生式，并计算该产生式后面的符号的First集合，根据该 First 集对该非终结符的 Follow 集进行更新。如果Follow集合有更新，则继续遍历，直到未再更新为止。

**getTable()函数的实现**

该函数的功能是通过 First 集与 Follow 集推导出LL(1) 文法的预测分析表。预测分析表通常是由语法分析器自动生成的，它包含了输入符号和栈顶符号的组合，以及对应的产生式或动作。

在语法分析过程中，分析器会根据输入符号和栈顶符号在预测分析表中查找对应的产生式或动作，并执行相应的操作，直到分析完成或出现错误。这个函数是用于构建预测分析表的函数。

该函数实现流程如图2-10所示。

<img width="888" height="896" alt="image" src="https://github.com/user-attachments/assets/eecce5d9-d9c4-4653-b952-30ab3a298464" />


该函数的主要实现步骤如下：


（1）首先将所有非终结符以及终结符进行编号映射；

（2）初始化预测分析表，将所有条目初始化为 \_00（\_00 = {0，-1}，-1表示该条目为空）；

（3）遍历所有非终结符，对于每一个非终结符遍历所有以该终结符位首的产生式；

（4）对于每一个产生式，为了求出其 First 集，需要从头开始遍历产生式其中的符号，然后求出符号的 First 集，并且加入到该产生式的 First 集中；

（5）遍历该 First 集中的符号，判断是否存在空集 \string$。如果不存在，则说明该产生式的 First 集不会包含之后符号的 First 集，就停止遍历该产生式，并将当前的 First 集加入到该产生式的 First 集中；如果存在则一直遍历该产生式到最后一个符号；

（6）当当前产生式停止遍历，则需要遍历该产生式的 First 集。对于 First 集中的每一个终结符，将该产生式加入到预测分析表的对应位置中（对应位置为对应的非终结符以及终结符，内容为该非终结符对应的产生式的编号）；

（7）同时判断该产生式的 First 集中是否包含空集 \string$，如果包含，则需要将该非终结符的 Follow 集以同样的方式将空集加入到预测分析表中；

（8）每次对预测分析表进行更新是需要判断该位置是否为空，如果不为空，说明该 LL(1) 文法出现错误，直接返回错误信息。

该函数的主要思路就是通过遍历所有产生式的 First 集对预测分析表进行填充，如果 First 集包含空集 \string$，还需要遍历 Follow 集进行填充。

**doAnalysis() 函数的实现**

该函数的功能主要为更具得到的预测分析表对存入队列中的符号序列进行语法分析的预测分析，并将预测分析过程输出到文件中。目的是判断输入符号序列是否可以通过该文法所推导出，主要过程为从队列中取出第一个符号，并将该符号与分析栈的栈顶符号进行比较。若它们相同，将其从队列中移除，继续读入下一个符号。否则，根据分析表格进行操作。伪代码如下：
```
fp = 将输出重定向至<syntax_analysis_path>
it = chars.begin()
while (true) do
    token = stack[top-1]
    if Vt 中存在 token then
        if token == *it then
            if token == "#" then
                输出相关信息
                刷新文件缓冲区，关闭文件指针
                return TRUE
            else
                输出++cnt_analysis, token + "#" + *it, "move"至文件
                更新top和id
            end if
        else
            输出 ++cnt_analysis, token + "#" + *it, "error" 至文件
            刷新文件缓冲区，关闭文件指针
            return FALSE
        end if
    else
        Id = Map[token]
        id = vt[*it]
        if table[Id][id].second == -1 then
            输出 ++cnt_analysis, token + "#" + *it, "error" 至文件
            刷新文件缓冲区，关闭文件指针
            将输出重定向回终端
            输出 Id, id 至终端
            输出 "error info : " + token + " " + *it 至终端
            return FALSE
        else
            输出 ++cnt_analysis, token + "#" + *it, "reduction" 至文件
            更新top-=1
            gId = table[Id][id].second
            if gId == 99 then continue end if
            vv = v[Id][gId]
            for i from vv.size()-1 to 0 do
                stack[top++] = vv[i]
            end for
        end if
    end if
end while
```

该函数的主要实现步骤如下：

（1）根据输入的符号栈 chars 解析，将指向符号集开头的指针 it 存储在 auto it = chars.begin() 中。同时将文件指针 fp 修改为所指向的 syntax\_analysis\_path 文件并打开，使得将来的输出将会输出到文件中；

（2）循环执行，直到语法分析结束返回 true 或者 false。其中，top 指向分析栈的栈顶，初始值为 1，表示栈中只有结束符 \string#；

（3）从栈中取出栈顶的符号 token，如果是终结符，则需进行字符匹配；

（4）如果匹配成功，则将栈顶弹出；如果匹配失败，则语法分析失败，退出分析并关闭写指针，直接返回 false。

（5）如果栈顶符号不是终结符，则需要在预测分析表中查找对应的产生式（table[Id][id]），其中 Id 和 id 分别是栈顶符号和输入字符的编号；

（6）若预测分析表该位置为空（table[Id][id] == -1），说明语法分析失败，直接返回 false退出分析；否则，将栈顶的非终结符出栈，并将产生式的右部逆序压入栈中；

（7）根据压入的符号生成输出，输出匹配的结果，包括序号，栈顶符号，输入字符和当前的动作（move/reduction）；

（8）如果输入已经检查完并且语法正确即匹配成功，则就输出 accept ，关闭文件指针并返回 true。

该函数的主要思路为通过预测分析表，使用 while 循环进行语法分析。每次分析首先从栈顶取出一个符号，判断该符号为终结符还是非终结符并对比输入符号来确定规约还是移动，直到语法分析结束。

**其他辅助函数**

我们除了构建以上函数来实现语法分析器的大部分功能外，也构建了其他一些函数进行辅助作用。由于功能比较简单，实现并不复杂，在这只简要介绍：
```
\begin{itemize}
    \item void getChars(string str)：对输入的token序列进行识别并压入队列中；
    \item bool analysis()：语法分析的入口，读取词法分析的输出，调用getChars函数识别并压入栈，最后返回doAnalysis函数；
    \item void print\_first()：将Fisrt集打印到指定文件中；
    \item void print\_follow()：将Follow集打印到指定文件中；
    \item void print\_table()：将预测分析表打印到指定文件中。
\end{itemize}
```

**输出格式说明**

1. First与Follow集

输出格式如下。每个符号下面一行输出集合元素，以空格进行划分。
```
addExp
 -- '!=' ')' ',' ';' '<' '<=' '=' '==' '>' '>=' 
addExpAtom
 -- '!=' ')' ',' ';' '<' '<=' '=' '==' '>' '>=' 
argConst
 -- ';' 
...
```

2. 预测分析表

由于表格规模较大，具体可见预测分析表的输出节“预测分析表的输出”所描述。

3. 移进-归约序列

输出的规范如下。
```
[栈顶] # [下一元素] <tab> [动作]
...
```

例如：

```
program#int  reduction
compUnit#int reduction
......
```

**源程序编译步骤**

1. 实验环境与软件准备

您需要安装VSCode、MINGW等工具，在Windows或Linux环境下均可运行。

2. 编译

如果您安装了make，那么输入如下指令即可完成语法分析器的编译：
```
make syntax
```
如果您未安装make，那么输入如下指令也可完成语法分析器的编译：
```
g++ -I include -g source/syntax.cpp -o syntax
```
3. 运行

使用下列指令，运行语法分析器。之后，您就可以得到多个txt文件的输出。
```
./syntax
```

