//
// Created by Cristian Villalobos on 27/03/2025.
//

#ifndef PROGRAMMINGASSIGNMENT3_TREENODE_H
#define PROGRAMMINGASSIGNMENT3_TREENODE_H
#include <iostream>

using namespace std;

class TreeNode {
public:
   TreeNode(const std::string& name, const std::string& type)
            : name(name), type(type), child(nullptr), sibling(nullptr) {}
   string getName() {return name;}
   string getType(){return type;}
   void setSibling(TreeNode* newSibling) {
       sibling = newSibling;
   }
   void setChild(TreeNode* newChild) {
       child = newChild;
   }
   TreeNode* getChild() {
       return child;
   }
   TreeNode* getSibling() {
       return sibling;
   }

private:
    string name;
    string type;
    TreeNode* child;
    TreeNode* sibling;
};


#endif //PROGRAMMINGASSIGNMENT3_TREENODE_H
