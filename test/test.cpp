#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// 语法树结点类型枚举
enum class ASTNodeType {
    program,
    compUnit,
    decl,
    varDecl,
    bType,
    int_,
    varDef,
    Ident,
    argVarDef,
    argVarDecl,
    compUnit_void,
    funcDef,
    funcType,
    void_,
    funcFParams,
    block,
    blockItem,
    stmt,
    exp,
    assignExp,
    eqExp,
    relExp,
    addExp,
    mulExp,
    unaryExp,
    callFunc,
    mulExpAtom,
    addExpAtom,
    relExpAtom,
    eqExpAtom,
    assignExpAtom,
    INT,
    number,
    EOF_
};

// 语法树结点类
class ASTNode {
public:
    ASTNode(ASTNodeType type, const string& text)
        : type(type), text(text), parent(nullptr) { }

    ~ASTNode() {
        for (auto& child : children) {
            delete child;
        }
    }

    // 添加子结点
    void addChild(ASTNode* node) {
        children.push_back(node);
        node->parent = this;
    }

    // 输出语法树
    void print(int indent = 0) {
        cout << string(indent, ' ') << "[" << text << "]" << endl;
        for (auto& child : children) {
            child->print(indent + 2);
        }
    }

    // 结点类型
    ASTNodeType type;
    // 结点文本
    string text;
    // 父结点指针
    ASTNode* parent;
    // 子结点向量
    vector<ASTNode*> children;
};

// 构建抽象语法树
ASTNode* buildAST() {
    vector<pair<ASTNodeType, string>> sequence = {
        { ASTNodeType::program, "#" }, { ASTNodeType::compUnit, "#" },
        { ASTNodeType::decl, "#" }, { ASTNodeType::varDecl, "#" },
        { ASTNodeType::bType, "#" }, { ASTNodeType::int_, "int" },
        { ASTNodeType::varDef, "" }, { ASTNodeType::Ident, "Ident" },
        { ASTNodeType::argVarDef, ";" }, { ASTNodeType::argVarDecl, ";" },
        { ASTNodeType::block, "{" }, { ASTNodeType::blockItem, "Ident" },
        { ASTNodeType::stmt, "Ident" }, { ASTNodeType::exp, "Ident" },
        { ASTNodeType::assignExp, "Ident" }, { ASTNodeType::eqExp, "Ident" },
        { ASTNodeType::relExp, "Ident" }, { ASTNodeType::addExp, "Ident" },
        { ASTNodeType::mulExp, "Ident" }, { ASTNodeType::unaryExp, "Ident" },
        { ASTNodeType::Ident, "Ident" }, { ASTNodeType::callFunc, "=" },
        { ASTNodeType::mulExpAtom, "INT" }, { ASTNodeType::addExpAtom, "INT" },
        { ASTNodeType::relExpAtom, "INT" }, { ASTNodeType::eqExpAtom, "INT" },
        { ASTNodeType::assignExpAtom, "INT" }, { ASTNodeType::INT, "INT" },
        { ASTNodeType::number, "INT" }, { ASTNodeType::mulExpAtom, ";" },
        { ASTNodeType::addExpAtom, ";" }, { ASTNodeType::relExpAtom, ";" },
        { ASTNodeType::eqExpAtom, ";" }, { ASTNodeType::assignExpAtom, ";" },
        { ASTNodeType::argVarDecl, ";" }, { ASTNodeType::blockItem, "int" },
        { ASTNodeType::decl, "int" }, { ASTNodeType::varDecl, "int" },
        { ASTNodeType::bType, "#" }, { ASTNodeType::int_, "int" },
        { ASTNodeType::varDef, "" }, { ASTNodeType::Ident, "Ident" },
        { ASTNodeType::argVarDef, "=" }, { ASTNodeType::INIT, "INT" },
        { ASTNodeType::exp, "INT" }, { ASTNodeType::assignExp, "INT" },
        { ASTNodeType::eqExp, "INT" }, { ASTNodeType::relExp, "INT" },
        { ASTNodeType::addExp, "INT" }, { ASTNodeType::mulExp, "INT" },
        { ASTNodeType::unaryExp, "INT" }, { ASTNodeType::Ident, "Ident" },
        { ASTNodeType::callFunc, "=" }, { ASTNodeType::mulExpAtom, ";" },
        { ASTNodeType::addExpAtom, ";" }, { ASTNodeType::relExpAtom, ";" },
        { ASTNodeType::eqExpAtom, ";" }, { ASTNodeType::assignExpAtom, ";" },
        { ASTNodeType::argVarDecl, ";" }, { ASTNodeType::blockItem, "int" },
        { ASTNodeType::decl, "int" }, { ASTNodeType::varDecl, "int" },
        { ASTNodeType::bType, "#" }, { ASTNodeType::int_, "int" },
        { ASTNodeType::varDef, "" }, { ASTNodeType::Ident, "Ident" },
        { ASTNodeType::argVarDef, "=" }, { ASTNodeType::INIT, "Ident" },
        { ASTNodeType::exp, "Ident" }, { ASTNodeType::assignExp, "Ident" },
        { ASTNodeType::eqExp, "Ident" }, { ASTNodeType::relExp, "Ident" },
        { ASTNodeType::addExp, "Ident" }, { ASTNodeType::mulExp, "Ident" },
        { ASTNodeType::unaryExp, "Ident" }, { ASTNodeType::Ident, "Ident" },
        { ASTNodeType::callFunc, "+" }, { ASTNodeType::mulExpAtom, "Ident" },
        { ASTNodeType::addExpAtom, "Ident" }, { ASTNodeType::IDENT, "Ident" },
        { ASTNodeType::callFunc, ";" }, { ASTNodeType::mulExpAtom, ";" },
        { ASTNodeType::addExpAtom, ";" }, { ASTNodeType::relExpAtom, ";" },
        { ASTNodeType::eqExpAtom, ";" }, { ASTNodeType::assignExpAtom, ";" },
        { ASTNodeType::argVarDecl, ";" }, { ASTNodeType::blockItem, "return" },
        { ASTNodeType::stmt, "return" }, { ASTNodeType::return_, "return" },
        { ASTNodeType::argExp, "Ident" }, { ASTNodeType::exp, "Ident" },
        { ASTNodeType::assignExp, "Ident" }, { ASTNodeType::eqExp, "Ident" },
        { ASTNodeType::relExp, "Ident" }, { ASTNodeType::addExp, "Ident" },
        { ASTNodeType::mulExp, "Ident" }, { ASTNodeType::unaryExp, "Ident" },
        { ASTNodeType::Ident, "Ident" }, { ASTNodeType::callFunc, ";" },
        { ASTNodeType::mulExpAtom, ";" }, { ASTNodeType::addExpAtom, ";" },
        { ASTNodeType::relExpAtom, ";" }, { ASTNodeType::eqExpAtom, ";" },
        { ASTNodeType::assignExpAtom, ";" }, { ASTNodeType::EOF_, "EOF" }
    };

    stack<ASTNode*> stack;
    for (auto& item : sequence) {
        ASTNode* node = new ASTNode(item.first, item.second);
        if (item.first != ASTNodeType::EOF_) {
            // 移进操作
            stack.push(node);
        } else {
            // 归约操作
            while (stack.size() > 1) {
                ASTNode* child = stack.top();
                stack.pop();
                stack.top()->addChild(child);
            }
            return stack.top();
        }
    }
    // 不会执行到这里
    return nullptr;
}

int main() {
    ASTNode* root = buildAST();
    root->print();
    delete root;
    return 0;
}
