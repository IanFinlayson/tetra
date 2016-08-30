/* this program takes in Tetra code and produces a parse tree which allows it
 * to be tested and visualized.  It now produces a structured text dump.
 * along with graphical output with graphiz */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tetra.h"

/* returns a string representation of a node type */
tstring stringType(Node* node) {
    tstring ss;

    switch (node->kind()) {
        /* statements and groups */
        case NODE_FUNCTION:
            return "FUNC " + node->getStrval();
        case NODE_TOPLEVEL_LIST:
            return "TOP LEVELS";
        case NODE_STATEMENT:
            return "STMTS";
        case NODE_FORMAL_PARAM_LIST:
            return "PARAMS";
        case NODE_IDENTIFIERS:
            return "IDENTIFIERS";
        case NODE_OPEN:
            return "OPEN";
        case NODE_IMPORT:
            return "IMPORT";
        case NODE_PASS:
            return "PASS";
        case NODE_WAIT:
            return "WAIT";
        case NODE_RETURN:
            return "RETURN";
        case NODE_BREAK:
            return "BREAK";
        case NODE_CONTINUE:
            return "CONTINUE";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_ELIF:
            return "ELIF";
        case NODE_ELIF_CHAIN:
            return "ELIF CHAIN";
        case NODE_ELIF_CLAUSE:
            return "ELIF CLAUSE";
        case NODE_PARALLEL:
            return "PARALLEL";
        case NODE_PARFOR:
            return "PARFOR";
        case NODE_BACKGROUND:
            return "BACKGROUND";
        case NODE_LOCK:
            return "LOCK";
        case NODE_CONST:
            return "CONST";
        case NODE_GLOBAL:
            return "GLOBAL";
        case NODE_DECLARATION:
            return "DECLARATION: " + node->getStrval();
        case NODE_LAMBDA:
            return "LAMBDA";
        case NODE_CLASS:
            return "CLASS " + node->getStrval();
        case NODE_CLASS_PART:
            return "CLASS PART";
        case NODE_DOT:
            return "DOT";
        case NODE_METHOD_CALL:
            return "METHOD CALL";
        case NODE_SELF:
            return "SELF";

        /* operators */
        case NODE_ASSIGN:
            return "=";
        case NODE_OR:
            return "or";
        case NODE_AND:
            return "and";
        case NODE_LT:
            return "<";
        case NODE_LTE:
            return "<=";
        case NODE_GT:
            return ">";
        case NODE_GTE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NEQ:
            return "!=";
        case NODE_NOT:
            return "not";
        case NODE_BITXOR:
            return "^";
        case NODE_BITAND:
            return "&";
        case NODE_BITOR:
            return "|";
        case NODE_BITNOT:
            return "~";
        case NODE_SHIFTL:
            return "<<";
        case NODE_SHIFTR:
            return ">>";
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_TIMES:
            return "*";
        case NODE_DIVIDE:
            return "/";
        case NODE_MODULUS:
            return "%";
        case NODE_EXP:
            return "EXP";
        case NODE_IN:
            return "in";

        /* functions */
        case NODE_FUNCALL:
            return "CALL: " + node->getStrval();
        case NODE_ACTUAL_PARAM_LIST:
            return "ARGS";

        /* vectors */
        case NODE_INDEX:
            return "INDEX";
        case NODE_VECVAL:
            return "VECVAL";
        case NODE_TUPVAL:
            return "TUPVAL";
        case NODE_DICTVAL:
            return "DICTVAL";
        case NODE_VECRANGE:
            return "VECRANGE";

        /* leafs */
        case NODE_INTVAL:
            return "INT:" + node->getStrval();
        case NODE_REALVAL:
            return "REAL: " + node->getStrval();
        case NODE_STRINGVAL:
            return "\\\"" + node->getStrval() + "\\\"";
        case NODE_IDENTIFIER:
            return "ID " + node->getStrval();
        case NODE_BOOLVAL:
            return "BOOL: " + node->getStrval();
        case NODE_NONEVAL:
            return "NONE";
        default:
            throw Error("Unsupported node type!");
    }
}

/* generate a unique label for a node */
tstring genId() {
    static tint count = 0;
    count++;
    return tstring("n") + tstring(count);
}

/* dump a single node to a graphviz dot file */
void dumpNodeGraphviz(Node* node, tstring id, std::ofstream& out) {
    if (!node) {
        return;
    }

    /* dump the node designator */
    out << "  " << id << "[label=\"";
    out << stringType(node);
    out << "\", fontname=\"courier\"];\n";

    /* for each child */
    for (int i = 0; i < node->numChildren(); i++) {
        /* generate a new id */
        tstring childId = genId();

        /* dump the connection */
        out << "  " << id << " -> " << childId << ";\n";

        /* dump the child itself */
        dumpNodeGraphviz(node->child(i), childId, out);
    }
}

/* dump an entrie tree to a graphviz dot file */
void dumpTreeGraphviz(Node* node) {
    std::ofstream out("tree.gv");
    out << "digraph G {\n";
    dumpNodeGraphviz(node, "n0", out);
    out << "}\n";
    out.close();

    system("rm tree.png");
    system("dot -Tpng tree.gv -o tree.png");
    system("xdg-open tree.png");
}

/* dump a tree to stdout */
void dumpTreeStdout(Node* node, int level = 0) {
    /* base case */
    if (!node) {
        return;
    }

    /* dump leading spaces */
    for (int i = 0; i < level; i++) {
        std::cout << "    ";
    }

    /* dump this type of node */
    std::cout << stringType(node);

    /* dump the type and line if relevant */
    if (node->type()) {
        std::cout << " (TYPE:" << typeToString(node->type()) << ")";
    }

    if (node->getLine()) {
        std::cout << " (LINE:" << node->getLine() << ")";
    }

    std::cout << std::endl;

    /* dump children */
    for (int i = 0; i < node->numChildren(); i++) {
        dumpTreeStdout(node->child(i), level + 1);
    }
}
