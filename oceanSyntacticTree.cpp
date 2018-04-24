//
// Created by jbassin on 4/10/2018.
//

#include "oceanSyntacticTree.h"

oceanSyntacticTree::oceanSyntacticTree() {
    root = new oceanSyntacticNode;
}

oceanSyntacticTree::oceanSyntacticTree(string data) {
    root = new oceanSyntacticNode;
    root->data = data;
}

oceanSyntacticNode *oceanSyntacticTree::insert(oceanSyntacticNode *root, int level, string data) {
    oceanSyntacticNode *currentNode = root;
    while (currentNode->level != level) {
        if (currentNode->child != nullptr) {
            currentNode = currentNode->child;
        } else {
            currentNode->child = new oceanSyntacticNode;
            currentNode->child->level = level;
            currentNode->child->data = data;
            currentNode->child->parent = currentNode;
            return currentNode->child;
        }
    }
    while (currentNode->next != nullptr) {
        currentNode = currentNode->next;
    }
    currentNode->next = new oceanSyntacticNode;
    currentNode->next->level = level;
    currentNode->next->data = data;
    currentNode->next->back = currentNode;
    currentNode->next->parent = currentNode->parent;
    return currentNode->next;
}