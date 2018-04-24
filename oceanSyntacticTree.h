//
// Created by jbassin on 4/10/2018.
//

#ifndef OCEAN_OCEANSYNTACTICTREE_H
#define OCEAN_OCEANSYNTACTICTREE_H

#include <string>

using namespace std;

struct oceanSyntacticNode {
    string data = "";
    int level = 0;
    int terminate = 0;
    oceanSyntacticNode *parent = nullptr;
    oceanSyntacticNode *back = nullptr;
    oceanSyntacticNode *next = nullptr;
    oceanSyntacticNode *child = nullptr;
};

class oceanSyntacticTree {
private:


public:
    oceanSyntacticNode *root;

    oceanSyntacticTree();

    explicit oceanSyntacticTree(string data);

    oceanSyntacticNode *insert(oceanSyntacticNode *root, int level, string data);
};


#endif //OCEAN_OCEANSYNTACTICTREE_H
