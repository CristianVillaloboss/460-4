//
// Created by Cristian Villalobos on 02/04/2025.
//

#ifndef PROGRAMMINGASSIGNMENT3_SYMBOLTABLE_H
#define PROGRAMMINGASSIGNMENT3_SYMBOLTABLE_H


#include <iostream>
#include <string>

class SymbolTable {

public:
    SymbolTable() : head(nullptr), tail(nullptr),  size(0){}

    void insert(const std::string& name, const std::string& type, const std::string& DATATYPE,
                const bool& DATATYPE_IS_ARRAY, const int& DATATYPE_ARRAY_SIZE, const int& SCOPE, const std::string& parameterList) {

        Node* newNode = new Node(name, type, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE, SCOPE, parameterList);
        newNode->next = nullptr; //we are inserting a new tail. signified by next->nullptr
        if(tail == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else{
            //there are 1 or more nodes
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    int getSize() {
        return this->size;
    }

/*
 * IDENTIFIER_NAME: sum_of_first_n_squares
IDENTIFIER_TYPE: function
DATATYPE: int
DATATYPE_IS_ARRAY: no
DATATYPE_ARRAY_SIZE: 0
SCOPE: 1
 */
    void print() const {
        Node* curr = head;
        while (curr) {
            if(!curr->parameterList.empty()) {
                std::cout << "PARAMETER LIST FOR: " << curr->parameterList << "\n";
                std::cout << "IDENTIFIER_NAME: " << curr->name << "\n";
                std::cout << "DATATYPE: " <<curr->DATATYPE << "\n" << "DATATYPE_IS_ARRAY: " << curr->DATATYPE_IS_ARRAY << "\n"
                          << "DATATYPE_ARRAY_SIZE: " << curr->DATATYPE_ARRAY_SIZE << "\n" << "SCOPE: " << curr->SCOPE <<"\n";
            }
            if(curr->parameterList.empty()) {
                std::cout << "IDENTIFIER_NAME: " << curr->name << "\n";
                std::cout << "IDENTIFIER_TYPE: " << curr->type << "\n";
                std::cout << "DATATYPE: " <<curr->DATATYPE << "\n" << "DATATYPE_IS_ARRAY: " << curr->DATATYPE_IS_ARRAY << "\n"
                          << "DATATYPE_ARRAY_SIZE: " << curr->DATATYPE_ARRAY_SIZE << "\n" << "SCOPE: " << curr->SCOPE <<"\n";
            }


            curr = curr->next;
            std::cout << "\n";
        }

    }
    void printTailArrSize () {
        std::cout << "datatype_array_sizee: " << tail->DATATYPE_ARRAY_SIZE << "\n";
    }

    bool exists(std::string nameCheck, int& scope) { //checks if a node with the given name already exists, will use to track declarations
        Node* curr = head;
        while (curr) {
            if(nameCheck == curr->name) {
                scope = curr->SCOPE;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    std::string getName(int x) {
        Node* curr = head;
        int y = 0;
        while (curr) {
            y++;
            if(x == y){
                break;
            }
            curr = curr->next;
        }
        return curr->name;
    }
    int getScope(int x) {
        Node* curr = head;
        int y = 0;
        while (curr) {
            y++;
            if(x == y){
                break;
            }
            curr = curr->next;
        }
        return curr->SCOPE;
    }

    ~SymbolTable() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

private:
    class Node {
    public:
        std::string name;
        std::string type;
        std::string DATATYPE;
        bool DATATYPE_IS_ARRAY;
        int DATATYPE_ARRAY_SIZE;
        int SCOPE;
        std::string parameterList;


        Node* next;

        Node(const std::string& name, const std::string& type, const std::string& DATATYPE,
             const bool& DATATYPE_IS_ARRAY, const int& DATATYPE_ARRAY_SIZE, const int& SCOPE, const std::string& parameterList)
                : name(name), type(type), DATATYPE(DATATYPE), DATATYPE_IS_ARRAY(DATATYPE_IS_ARRAY), DATATYPE_ARRAY_SIZE(DATATYPE_ARRAY_SIZE),
                SCOPE(SCOPE), parameterList(parameterList), next(nullptr){}
    };

    Node* head;
    Node* tail;
    int size;
};


#endif //PROGRAMMINGASSIGNMENT3_SYMBOLTABLE_H
