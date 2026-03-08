#include "FSM.h"
#include "util.h"
#include <bits/stdc++.h>


FSM::FSM(set<set<char>> charList) {
    this->charList = charList;
    this->num = 0;
    FSM_Node* firstNode = new FSM_Node(this->num, TokenCode::UNDIFNIE, false);
    this->nodes.push_back(firstNode);
    this->index.insert({firstNode->id, firstNode});
    this->first = firstNode;
    this->num++;
}

void FSM::addNode(FSM_Node* node) {
    this->nodes.push_back(node);
    this->index.insert({node->id, node});
    this->num++;
}

void FSM::addTrans(int from, int to, set<char> chSet){
    FSM_Node* fromNode = this->index[from];
    FSM_Node* toNode = this->index[to];
    if (fromNode ->trans.count(chSet)) {
        fromNode->trans[chSet].push_back(toNode);
    } else {
        vector<FSM_Node*> v;
        v.push_back(toNode);
        fromNode->trans.insert({chSet, v});
    }
}

FSM_Node* FSM::getFisrt() {
    return this->first;
}    

FSM_Node* FSM::getNode(int id){
    if(this->index.count(id) > 0) {
        return this->index[id];
    } else {
        return nullptr;
    }
}

set<set<char>> FSM::getCharList() {
    return this->charList;
}

int FSM::getNum() {
    return this->num;
}

// 该函数暂时仅用于debug
void FSM::printFSM() {
    cout << this->num << endl;
    cout << "================================================================" << endl;
    for(int i = 0; i < this->num; i++) {
        FSM_Node* node = this->index[i];
        cout << "Node : " << node->id << endl;
        if(node->isEndState) {
            cout << "END STATE!" << endl;
        }
        if(node->trans.count(getLetterList('a'))) {
            vector<FSM_Node*> v = node->trans[getLetterList('a')];
            for(FSM_Node* node : v) {
                cout << "Input Letters -> " << node->id << endl;
            }
        }
        if(node->trans.count(getLetterList('0'))) {
            vector<FSM_Node*> v = node->trans[getLetterList('0')];
            for(FSM_Node* node : v) {
                cout << "Input Number 0 -> " << node->id << endl;
            }
        }
        if(node->trans.count(getLetterList('1'))) {
            vector<FSM_Node*> v = node->trans[getLetterList('1')];
            for(FSM_Node* node : v) {
                cout << "Input Number 1-9 -> " << node->id << endl;
            }
        }
        cout << "================================================================" << endl;
    }
}

FSM createNFA() {
    set<set<char>> charList;
    set<char> letterList = {'_'};
    for (char c = 'a'; c <= 'z'; c++) {
        letterList.insert(c);
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        letterList.insert(c);
    }
    charList.insert(letterList);
    set<char> numList;
    for (char c = '1'; c <= '9'; c++) {
        numList.insert(c);
    }
    //numList.insert('-');  //////
    charList.insert(numList);
    set<char> zeroList = {'0'};
    charList.insert(zeroList);
    // charList={{1-9}, {_a-zA-Z}, {0}}

    FSM NFA = FSM(charList);

    NFA.addNode(new FSM_Node(1, TokenCode::INT, true));         // 只有一个数字
    NFA.addNode(new FSM_Node(2, TokenCode::UNDIFNIE, false));   // 过渡节点
    NFA.addNode(new FSM_Node(3, TokenCode::INT, true));         // 数字
    NFA.addNode(new FSM_Node(4, TokenCode::IDN, true));         // 只有一个字母
    NFA.addNode(new FSM_Node(5, TokenCode::UNDIFNIE, false));   // 过渡节点
    NFA.addNode(new FSM_Node(6, TokenCode::IDN, true));         // 标识符

    NFA.addTrans(0, 1, zeroList);
    NFA.addTrans(0, 1, numList);
    NFA.addTrans(0, 2, numList);
    NFA.addTrans(2, 2, zeroList);
    NFA.addTrans(2, 2, numList);
    NFA.addTrans(2, 3, zeroList);
    NFA.addTrans(2, 3, numList);

    NFA.addTrans(0, 4, letterList);
    NFA.addTrans(0, 5, letterList);
    NFA.addTrans(5, 5, letterList);
    NFA.addTrans(5, 5, zeroList);
    NFA.addTrans(5, 5, numList);
    NFA.addTrans(5, 6, zeroList);
    NFA.addTrans(5, 6, numList);
    NFA.addTrans(5, 6, letterList);

    cout << endl << "**************************NFA*************************" << endl;
    NFA.printFSM();
    return NFA;
}


// 原来的NFA确定化函数
/******
FSM NFAtoDFA(FSM NFA) {
    FSM DFA = FSM(NFA.getCharList());

    stack<set<int>> nodes;          // 遍历产生新节点的栈
    map<set<int>, int> statesMap;   // 存储当前的eplision-closure(move(I,a))对应的DFA中的标号，避免重复
    set<int> nowStates;             // 当前NFA状态集的标号集合
    nowStates.insert(NFA.getFisrt()->id);
    statesMap.insert({nowStates, DFA.getFisrt()->id});
    nodes.push(nowStates);
    int newNum = 0;
    while(!nodes.empty()) {
        nowStates = nodes.top();
        nodes.pop();
        for(int id : nowStates) {
            FSM_Node* node = NFA.getNode(id);
            for(set<char> chList : NFA.getCharList()) {
                if(node->trans.count(chList)) { // 通过当前符号集有边能够转移
                    set<int> temp;      // 当前状态集转移到的I的标号集
                    int tokenCode = TokenCode::UNDIFNIE;
                    bool isEnd = false;
                    for(unsigned int i = 0; i < node->trans[chList].size(); i++) {
                        FSM_Node* toNode = node->trans[chList][i]; // 当前转换到的
                        if(toNode->state != TokenCode::UNDIFNIE) {
                            tokenCode = toNode->state;
                        }
                        if(toNode->isEndState) {
                            isEnd = true;
                        }
                        int toId = toNode->id;
                        temp.insert(toId);
                    }
                    if(statesMap.count(temp) == 0) { // 到达了一个新的状态节点！
                        FSM_Node* newNode = new FSM_Node(++newNum, tokenCode, isEnd);
                        statesMap.insert({temp, newNode->id}); 
                        DFA.addNode(newNode);
                        DFA.addTrans(statesMap[nowStates], newNode->id, chList);
                        nodes.push(temp);   // 新发现的状态集需要加入栈中进行遍历
                    } else { // 之前就存在的状态集，添加边即可
                        int toId = statesMap[temp];
                        DFA.addTrans(statesMap[nowStates], toId, chList);
                    }
                }
            }
        }
    }
    return DFA;
}
********/


// 重构NFA确定化函数

//debug
void printSet(set<int> s) {
    cout<<"printSet"<<endl;
    for (auto it = s.begin(); it != s.end(); ++it) {
        printf("%d ", *it);                 // 使用迭代器遍历set并输出元素
    }
    cout<<endl;
}


int getKey(map<int, set<int>> m, set<int> s) {
    for (auto iter = m.begin(); iter != m.end(); ++iter) {
        if (iter->second == s) {
            return iter->first;
        }
    }
    // 如果未找到匹配的元素，则返回-1表示查找失败
    return -1;
}

bool isIn(map<int, set<int>> statesMap, set<int> nodes) {
    for (auto iter = statesMap.begin(); iter != statesMap.end(); ++iter) {
        if (iter->second == nodes) {
            return true;
        }
    }
    // 如果未找到匹配的元素，则返回false表示查找失败
    return false;
}

void printMap(map<int, set<int>> statesMap){
    for (auto iter = statesMap.begin(); iter != statesMap.end(); ++iter) {
        std::cout << "{" << iter->first << ",{";
        for (auto innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter) {
            std::cout << *innerIter << ",";
        }
        std::cout << "}},";
    }
}

int getState(set<int> nodes, FSM NFA) { 
    // 获取新节点的state，例如{1,2}的state为INT
    int newState = TokenCode::UNDIFNIE;
    for (int nid : nodes) {
        FSM_Node* node = NFA.getNode(nid);
        if (node->state != TokenCode::UNDIFNIE) {
            newState = node->state;
        }
    }
    return newState;
}

bool getIsEnd(set<int> nodes, FSM NFA) {
    //获取新节点是否为终态
    //原则为只要集合里有一个endstate，那么整个就都是endstate
    bool isEnd = 0;
    for (int nid : nodes) {
        FSM_Node* node = NFA.getNode(nid);
        if (node->isEndState) {
            isEnd = 1;
            break;
        }
    }
    return isEnd;
}

FSM NFAtoDFA(FSM NFA) {
    
    set<char> zero = {'0'};
    set<char> numbers = {}, letters = {};
    for (char ch = '1'; ch <= '9'; ch++) {
        numbers.insert(ch);
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        letters.insert(ch);
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        letters.insert(ch);
    }
    letters.insert('_');

    FSM DFA = FSM(NFA.getCharList());
    int NFANumNodes = NFA.getNum();
    stack<set<int>> nodes;          // 遍历产生新节点的栈
    map<int, set<int>> statesMap;   // 存储当前的eplision-closure(move(I,a))对应的DFA中的标号，避免重复
    // set<int> nowStates;          // 当前NFA状态集的标号集合
    enum {
        NFASTATES,
        ZERO,   
        NUMBERS, 
        LETTERS,
        
    } ;  // DFA状态矩阵的纵坐标
    set<int> statesMatrix[10][4] = {set<int>{}};   // DFA状态变换矩阵
    nodes.push({NFA.getFisrt()->id});
    statesMap.insert({DFA.getFisrt()->id, {NFA.getFisrt()->id}});  //{0,{0}}
    statesMatrix[0][NFASTATES] = {NFA.getFisrt()->id};
    while(!nodes.empty()) 
    {
        set<int> cNodes = nodes.top();
        nodes.pop();
        set<int> I_letters = {}, I_numbers = {}, I_zero = {};
        for (std::set<int>::iterator it = cNodes.begin(); it != cNodes.end(); ++it) {
            int id = *it;
            FSM_Node *node4id = NFA.getNode(id);  //找到对应的NFA节点
            //map<set<char>, vector<FSM_Node*>> trans = {}
            vector<FSM_Node*>   objVecLetters = node4id->trans[letters], 
                                objVecNumbers = node4id->trans[numbers], 
                                objVecZero    = node4id->trans[zero];
            for (int i = 0; i < objVecLetters.size(); i++) 
                I_letters.insert(objVecLetters[i]->id);
            for (int i = 0; i < objVecNumbers.size(); i++) 
                I_numbers.insert(objVecNumbers[i]->id);
            for (int i = 0; i < objVecZero.size(); i++) 
                I_zero.insert(objVecZero[i]->id);
        }
        //添加矩阵元素
        statesMatrix[getKey(statesMap, cNodes)][NFASTATES] = cNodes;
        statesMatrix[getKey(statesMap, cNodes)][ZERO] = I_zero;
        statesMatrix[getKey(statesMap, cNodes)][NUMBERS] = I_numbers;
        statesMatrix[getKey(statesMap, cNodes)][LETTERS] = I_letters;
        

        // 若epsilon(I_{_a-zA-Z|1-9|0})不在statesMap里，则加入进去，且压栈
        // 空集不加入
        if (!isIn(statesMap, I_zero) && I_zero.size()>0){
            statesMap.insert({statesMap.size(), I_zero}); nodes.push(I_zero);
        }
        
        if (!isIn(statesMap, I_numbers) && I_numbers.size()>0) {
            statesMap.insert({statesMap.size(), I_numbers}); nodes.push(I_numbers);
        }
        if (!isIn(statesMap, I_letters) && I_letters.size()>0) {
            statesMap.insert({statesMap.size(), I_letters}); nodes.push(I_letters);
        }
    }

    //////// 打印状态矩阵，测试用 BEGIN ////////
    // printMap(statesMap);
    //cout<<endl;
    //for (int i = 0; i < 10; ++i) {
    //    for (int j = 0; j < 4; ++j) {
    //        cout << "{";
    //        for (auto iter = statesMatrix[i][j].begin(); iter != statesMatrix[i][j].end(); ++iter) {
    //            cout << *iter << ",";
    //        }
    //        cout << "}" << ",";
    //    }
    //    cout << endl;
    //} 
    //////// 打印状态矩阵，测试用  END  ////////
    
    // 给DFA添加节点
    // statesMap里的每个key都是一个新节点,但起始节点0不重复添加
    for (auto iter = statesMap.begin(); iter != statesMap.end(); ++iter) {
        if (iter->first!=0) DFA.addNode(new FSM_Node(iter->first, getState(iter->second, NFA), getIsEnd(iter->second, NFA)));    
    }

    // 给DFA添加节点转换规则
    map<int, set<char>> chSetMap = { // 此为状态矩阵纵坐标和charSet的对应关系
        { ZERO,    zero},
        {  NUMBERS, numbers},
        {LETTERS, letters}
    };
    for (int i=0; i<10 && statesMatrix[i][NFASTATES].size()>0; i++) {
        for (int j=ZERO; j<=LETTERS && statesMatrix[i][j].size()>0; j++) {
            DFA.addTrans(i, getKey(statesMap, statesMatrix[i][j]), chSetMap[j]);
        }
    }
    
    cout << "***********************DFA*******************" << endl;
    DFA.printFSM();
    return DFA;
} 


// 原有的DFA最小化函数
/******
FSM minimizeDFA(FSM DFA) {
    set<set<int>> partition; // 状态的划分
    set<int> endState, notEndState; // 初始两个状态
    for(int i = 0; i < DFA.getNum(); i++) {
        if(DFA.getNode(i)->isEndState) {
            endState.insert(i);
        } else {
            notEndState.insert(i);
        }
    }
    partition.insert(endState);
    partition.insert(notEndState);
    bool isChanged = true;
    while(isChanged) { // 循环划分
        isChanged = false;
        for(set<int> state : partition) { // 遍历每一个集合
            if(state.size() == 1) {
                continue;
            }
            for(set<char> inputSet : DFA.getCharList()) { // 对每种输入都进行尝试
                map<int, int> m; // 记录每一个节点达到的最终节点对应关系
                set<int> emptySet; // 当前输入不能转换的节点集合
                for(int id : state) { // 当前输入需要对每个状态都进行尝试
                    FSM_Node* node = DFA.getNode(id);
                    if(node->trans.count(inputSet)) {
                        while(node->trans.count(inputSet)) {
                            node = node->trans[inputSet][0];
                            if(node->isEndState) {
                                break; // 到达终态就可以退出了，否则会死循环捏
                            }
                        } // 此时node是当前id能一直转换到的节点
                        int toId = node->id;
                        m.insert({id, toId});
                    } else { // 当前输入无法转换，单独落入一个集合
                        emptySet.insert(id);
                    }
                }

                map<int, set<int>> tempMap; //当前id与其终态点划分的对应关系
                for(int id : state) {
                    if(emptySet.count(id)) {
                        continue;
                    }
                    int toId = m[id];
                    for(set<int> nowStateSet : partition) { // 去找当前这个id落在了哪个划分里
                        if(nowStateSet.count(toId)) {
                            tempMap.insert({id, nowStateSet});
                            break;
                        }
                    }
                } // 到现在tempMap已经存储了该state里的id分类

                map<set<int>, set<int>> partToId; // 上面这个tempMap的反向操作
                for(int id : state) {
                    if(emptySet.count(id)) {
                        continue;
                    }
                    if(partToId.count(tempMap[id]) == 0) {
                        set<int> s;
                        s.insert(id);
                        partToId.insert({tempMap[id], s});
                    } else {
                        partToId[tempMap[id]].insert(id);
                    }
                } // 到现在partToId已经把state里的id分好类了

                if((emptySet.size() && partToId.size() >= 1) || (emptySet.size() == 0 && partToId.size() > 1)) {
                    isChanged = true;
                    partition.erase(state);
                    map<set<int>, set<int>>::iterator iter;
                    for(iter = partToId.begin(); iter != partToId.end(); iter++) {
                        partition.insert(iter->second);
                    }
                    partition.insert(emptySet);
                    break;
                }
            }
            if(isChanged) {
                break;
            }
        }
    } 

    // 到现在完成了划分，下面要根据这些划分构造新的miniDFA
    FSM miniDFA = FSM(DFA.getCharList());
    for (set<int> s : partition) { // 先设置好所有的点
        if(s.count(miniDFA.getFisrt()->id)) {
            continue;
        }
        miniDFA.addNode(new FSM_Node(*s.begin(), DFA.getNode(*s.begin())->state, DFA.getNode(*s.begin())->isEndState));
    }
    for(set<int> s : partition) { // 为每个点添加边
        FSM_Node* node; // node是当前划分的代表
        if(s.count(miniDFA.getFisrt()->id)) {
            node = miniDFA.getFisrt();
        } else {
            node = miniDFA.getNode(*s.begin());
        }
        for(int id : s) { // 遍历当前划分的每个点
            FSM_Node* rawNode = DFA.getNode(id);
            map<set<char>, vector<FSM_Node*>>::iterator iter;
            // 遍历这个点的每条边
            for(iter = rawNode->trans.begin(); iter != rawNode->trans.end(); iter++) {
                if(node->trans.count(iter->first)) { // 因为是DFA，如果有转换了就有边了，不用再往下执行了
                    continue;
                }
                int rawToId = iter->second[0]->id; // 原DFA中这个节点的id
                int newToId = rawToId;
                for(set<int> tempS : partition) {
                    if(tempS.count(rawToId)) {
                        newToId = *tempS.begin(); // 找到划分的代表
                        break;
                    }
                }
                miniDFA.addTrans(node->id, newToId, iter->first);
            }
        }
    }
    cout << endl << "************************miniDFA***********************" << endl;
    miniDFA.printFSM();
    return miniDFA;
}
**/

// 下面是重构的DFA最小化函数

bool isSubSet(set<int> S, set<int> T) { //判断S是否是T的子集
    for (int elm : S) {
        if (T.find(elm) == T.end()) { // S某一元素不在T里
            return false;
        }
    }
    return true;
}

bool isInOne(set<int> nodes, set<set<int>> PiSet) {  //判断nodes是否包含于PiSet的某一元素里
    // 加一条特殊规则，如果nodes={-1}（纯空集），则也认为它位于PiSet全部元素的子集
    if (nodes == set<int>{-1}) return true;

    for (auto s : PiSet) {
        if (isSubSet(nodes, s)) return true;
    }
    return false;
}

bool isDevilable(set<int>nodes, set<set<int>> PiSet, auto DFAMatrix) {
    enum { DFASTATES, ZERO, NUMBERS, LETTERS };
    if (nodes.size()==1) return false;
    set<int> zeroSet, numSet, letSet;
    for (int node : nodes) {
        zeroSet.insert(DFAMatrix[node][ZERO]);
        numSet.insert(DFAMatrix[node][NUMBERS]);
        letSet.insert(DFAMatrix[node][LETTERS]);
    }
    if (!isInOne(zeroSet, PiSet) || !isInOne(numSet, PiSet) || !isInOne(letSet, PiSet)) {
        return true;
    }
    return false;
}

set<set<int>> trans2set(stack<set<int>> s) {
    auto s_ = s;
    set<set<int>> res = {};
    while(!s_.empty()) {
        auto elm = s_.top(); s_.pop();
        res.insert(elm);
    }
    return res;
}

set<int> findSubset(int elm, set<set<int>> S) {  
    // 作用：寻找elm(eg.1)位于S(eg.{{1},{2,3},{4,5}})的哪个子集里
    for (set<int> subset : S) {
        if (subset.find(elm) != subset.end())  {  // 如果elm在subset里
            return subset;
            break;
        }
    }
    set<int> notFound;
    notFound.insert(-1);
    return notFound;
}

set<set<int>> reduce(map<int, set<int>> nodeMap) {
    //根据nodeMap中每个pair second属性的不同，将他们的first属性归为不同的类输出。
    //例如，nodeMap={{1,{0}}, {2,{3,4}}, {3,{3,4}}, {4,{0}}}，则返回{{1}, {2,3}}
    std::map<int, std::set<int>> classMap;

    // 遍历 nodeMap 中的每一个 pair
    for (auto& p : nodeMap) {
        // 将 pair 的 first 和 second 分别保存到变量 node 和 cls 中
        int node = p.first;
        auto cls = p.second;

        // 查找是否已经存在以 cls 为 key 的 set，如果不存在则创建一个新的 set
        if (classMap.find(cls.size()) == classMap.end()) {
            classMap[cls.size()] = std::set<int>();
        }

        // 将 node 添加到以 cls 为 key 的 set 中
        classMap[cls.size()].insert(node);
    }

    std::set<std::set<int>> result;

    // 将 classMap 中的各个 set 添加到 result 中
    for (auto& p : classMap) {
        result.insert(p.second);
    }

    return result;
}

FSM minimizeDFA(FSM DFA) {
    set<char> zero = {'0'};
    set<char> numbers = {'1','2','3','4','5','6','7','8','9'};
    set<char> letters = {'_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    enum{ DFASTATES, ZERO, NUMBERS, LETTERS };
    map<int, set<char>> chSetMap = { // 此为状态矩阵纵坐标和charSet的对应关系
        { ZERO,     zero},
        {  NUMBERS, numbers},
        {LETTERS,   letters}
    };
    int DFAMatrix[DFA.getNum()][4] = {};  //构造原DFA的等价状态矩阵
    // int **DFAMatrix;
    for (int id = 0; id < DFA.getNum(); id++) {
        DFAMatrix[id][DFASTATES] = id;
        FSM_Node* crtNode = DFA.getNode(id);
        for (int j = ZERO; j <= LETTERS; j++) {
            vector<FSM_Node*> objNodeVec = crtNode->trans[chSetMap[j]];
            FSM_Node* objNode = objNodeVec.size() > 0 ? objNodeVec[0] : nullptr;
            DFAMatrix[id][j] = objNode != nullptr ? objNode->id : -1;
        }
    }

    // 初始化操作
    set<set<int>> partition; // 状态的划分
    set<int> endState, notEndState; // 初始两个状态
    for(int i = 0; i < DFA.getNum(); i++) {
        if(DFA.getNode(i)->isEndState) {
            endState.insert(i);
        } else {
            notEndState.insert(i);
        }
    }
    partition.insert(endState);
    partition.insert(notEndState);

    // 测试打印partition
    cout << "PARTITION" << endl << "{" ;
    for (auto elm : partition) {
        cout<<"{";
        for (int num : elm) {
            cout << num << ",";
        }
        cout << "}, ";
    }
    cout << "}" << endl;

    stack<set<int>> nodeStack;  // 用于临时存储划分的节点，(每一步)只保存还能继续划分的
    nodeStack.push(endState);   
    nodeStack.push(notEndState);
    while(!nodeStack.empty()) { // 栈非空，表示还有可以继续划分的
        set<int> cNodes = nodeStack.top();
        nodeStack.pop();

        // 测试用输出
        if (cNodes==set<int>{2,5}) cout<<endl<<isDevilable(cNodes, partition, DFAMatrix)<<"\n";

        if (!isDevilable(cNodes, partition, DFAMatrix)) {
            continue;
        } 

        map<int, set<int>> nodeMap; // 指示move(node, chList)位于PI的哪个子集
        for (int charListId : {ZERO, NUMBERS, LETTERS}) {
            for (auto node : cNodes) {
                nodeMap.insert({node, findSubset(DFAMatrix[node][charListId], partition)});
            }
            set<set<int>> res = reduce(nodeMap);  // 根据子集的不同归类
            if (res.size() <= 1) {
                nodeMap.clear();
                continue;
            } else {  // 将划分好的nodes压进集合和栈
                for (auto item : res) {
                    nodeStack.push(item);
                    partition.insert(item);
                }
                break;
            }
        }
        //if (nodeStack.size() > 1) 
        partition.erase(cNodes); // cout << "here";
    }

    // 测试打印partition
    cout << "PARTITION" << endl << "{" ;
    for (auto elm : partition) {
        cout<<"{";
        for (int num : elm) {
            cout << num << ",";
        }
        cout << "} , ";
    }
    cout << "}" << endl;

    // 下面根据partiton构造miniDFA
    // partition={{0},{1},{2,5},{3,4}}
    
    // test print
    //for (int i=0; i<6; i++) {
    //    for (int j=0; j<4; j++) {
    //        cout << DFAMatrix[i][j] << ",";
    //    }
    //    cout<<endl;
    //}

    for (auto& nodes : partition) {  // nodes = {2,5}
        if (nodes.size() < 2) continue;
        int minNodeId = *(nodes.lower_bound(INT_MIN));
        for (int chSetId = ZERO; chSetId <= LETTERS; chSetId++) { // chSetId = ZERO
            for (int nodeId : nodes) {  // nodeid = 5
                if (nodes.find(DFAMatrix[nodeId][chSetId]) != nodes.end()) { //元素在nodes里
                    DFAMatrix[minNodeId][chSetId] = minNodeId;
                }
                else {
                    DFAMatrix[minNodeId][chSetId] = DFAMatrix[nodeId][chSetId] ;
                    continue;
                }
            }
        }
        //把modes里除了最小元素对应的矩阵行全部删除
        for (int i : nodes) {
            if (i != minNodeId) {
                DFAMatrix[i][DFASTATES] = -1;   DFAMatrix[i][ZERO] = -1;    
                DFAMatrix[i][NUMBERS] = -1;     DFAMatrix[i][LETTERS] = -1;
            }
        }
        
    }

    // test print
    //for (int i=0; i<6; i++) {
    //    for (int j=0; j<4; j++) {
    //        cout << DFAMatrix[i][j] << ",";
    //    }
    //    cout<<endl;
    //}

    // 给miniDFA添加节点
    FSM miniDFA = FSM(DFA.getCharList());
    for (int id = 1; id < DFA.getNum(); id++) {
        if (DFAMatrix[id][DFASTATES] != -1) {
            miniDFA.addNode(new FSM_Node(id, DFA.getNode(id)->state, DFA.getNode(id)->isEndState));
        }
    }

    // 给miniDFA添加变换规则
    for (int id = 0; id < miniDFA.getNum(); id++) {
        for (int j = ZERO; j <= LETTERS; j++) {
            if (DFAMatrix[id][j] != -1) miniDFA.addTrans(id, DFAMatrix[id][j], chSetMap[j]);
        }
    }

    miniDFA.printFSM();
    return miniDFA;

}


int identity(FSM DFA, string token) {
    FSM_Node* node = DFA.getFisrt();
    bool success = true;
    for(unsigned int i = 0; i < token.length(); i++) {
        char c = token[i];
        if(node->trans.count(getLetterList(c))) {
            node = node->trans[getLetterList(c)][0];
        } else {
            success = false;
        }
    }
    if(success && node->isEndState) {
        return node->state;
    } else {
        return TokenCode::UNDIFNIE;
    }
}