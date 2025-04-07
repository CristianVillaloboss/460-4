//
// Created by Cristian Villalobos on 26/03/2025.
//
#ifndef PROGRAMMINGASSIGNMENT3_GRAPH_H
#define PROGRAMMINGASSIGNMENT3_GRAPH_H

#include <string>
#include "TreeNode.h"

class Graph {
public:
    Graph(TreeNode* TreeNode)
    : root(TreeNode), currNode(TreeNode), outfile("cstOutput.txt") {}

    void addSibling (TreeNode* newNode) {
        TreeNode* temp = currNode;
        while (temp->getSibling()) {
            temp = temp->getSibling();
        }
        temp->setSibling(newNode);
        currNode = newNode;
    }
    void addChild (TreeNode* newNode) {
        if (!currNode->getChild()) {
            currNode->setChild(newNode);
        } else {
            TreeNode* temp = currNode->getChild();
            while (temp->getSibling()) {
                temp = temp->getSibling();
            }
            temp->setSibling(newNode);
        }
        currNode = newNode;
    }
    void printTree() {
        printLCRS(root);
    }
    TreeNode*& getCurrNode() {
        return currNode;
    }


private:
    TreeNode* root;
    TreeNode* currNode;
    std::ofstream outfile;

    void printLCRS(TreeNode* node) {
        if (!node) return;

        std::string val = node->getName();

        if (val == "{") {
            outfile << "\n" << val << " \n";  // { on its own line
        } else if (val == "}") {
            outfile << val << "\n";  // } on its own line, maybe newline before
        } else if (val == ";") {
            outfile << val << "\n";  // semicolon ends the current line
        } else {
            outfile << val << "\t";  // print inline with tab
        }

        printLCRS(node->getChild());
        printLCRS(node->getSibling());

    }
};


#endif //PROGRAMMINGASSIGNMENT3_GRAPH_H
