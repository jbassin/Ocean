#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include "oceanSyntacticTree.h"
#include "oceanTransform.h"

using namespace std;

void replaceAll(string &str, const string &from, const string &to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

int main() {

    ifstream infile(R"(C:\Users\jbassin\CLionProjects\Ocean\test.ocn)");
    ofstream outfile(R"(C:\Users\jbassin\CLionProjects\Ocean\test.cpp)");

    oceanSyntacticTree syntacticTree;
    oceanSyntacticNode *currentNode = syntacticTree.root;
    int currentLevel = 0;

    string line;
    while (getline(infile, line)) {
        string curLine = line;

        rtrim(curLine);
        while (curLine.find_first_of(' ', 0) < curLine.find_first_not_of(" ~", 0)) {
            curLine.replace(curLine.find_first_of(' ', 0), 1, "~");
        }
        unsigned int numOfTokens = 0;
        for (unsigned int i = 0; i < curLine.length(); i++) {
            if (curLine.at(i) == '~') numOfTokens++;
            else break;
        }

        if (currentLevel < numOfTokens) {
            syntacticTree.insert(currentNode, numOfTokens, curLine.substr(numOfTokens));
            currentLevel = numOfTokens;
        } else if (currentLevel == numOfTokens) {
            currentNode = syntacticTree.insert(currentNode, numOfTokens, curLine.substr(numOfTokens));
        } else {
            while (currentNode->level != numOfTokens) {
                currentNode = currentNode->parent;
            }
            currentNode = syntacticTree.insert(currentNode, numOfTokens, curLine.substr(numOfTokens));
            currentLevel = numOfTokens;
        }
    }
    currentNode = syntacticTree.insert(syntacticTree.root, 0, "");
    currentNode->terminate = 1;
    infile.close();
    oceanTransform tester(syntacticTree);
    string testString = tester.getString(syntacticTree.root);
    outfile << R"(
#include <iostream>

using namespace std;

int main() {)" << endl;
    outfile << testString;
    outfile << "}" << endl;
}