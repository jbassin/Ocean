//
// Created by jbassin on 4/11/2018.
//

#ifndef OCEAN_OCEANTRANSFORM_H
#define OCEAN_OCEANTRANSFORM_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "oceanSyntacticTree.h"

using namespace std;

class oceanTransform {
public:
    oceanTransform();

    explicit oceanTransform(oceanSyntacticTree input);

    string getString(oceanSyntacticNode *input);

private:
    oceanSyntacticTree tree;

    vector<string> split(const string &s, char delim);

    vector<string> stringFile;

    string rtrim(std::string s);

    string ltrim(std::string s);

    string trim(std::string s);

    string format(string input);

    string replaceAll(string str, const string &from, const string &to);

    string nodeTrawl(oceanSyntacticNode *input);

    string parser(oceanSyntacticNode *input);

    string ifParser(oceanSyntacticNode *input);

    string elseParser(oceanSyntacticNode *input);

    string otherwiseParser(oceanSyntacticNode *input);

    string metParser(oceanSyntacticNode *input);

    string forParser(oceanSyntacticNode *input);
};


#endif //OCEAN_OCEANTRANSFORM_H
