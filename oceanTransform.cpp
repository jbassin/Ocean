//
// Created by jbassin on 4/11/2018.
//

#include "oceanTransform.h"

oceanTransform::oceanTransform() = default;

oceanTransform::oceanTransform(oceanSyntacticTree input) {
    this->tree = input;
    this->stringFile;
}

vector<string> oceanTransform::split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(move(item));
    }
    return elems;
}

string oceanTransform::getString(oceanSyntacticNode *input) {
    string returnString = "";
    returnString += nodeTrawl(input->next);
    return returnString;
}

string oceanTransform::format(string input) {
    while (input.find(", ", 0) != string::npos) {
        input = this->replaceAll(input, ", ", ",");
    }
    while (input.find(". ", 0) != string::npos) {
        input = this->replaceAll(input, ". ", ".");
    }
    string returnToken = "";
    if (input == "") {
        return "";
    }
    if (input.find_first_of('|', 0) != string::npos) {
        int firstOf = input.find_first_of('|', 0);
        int lastOf = input.find_last_of('|', string::npos);
        string method = this->format(input.substr(0, firstOf));
        string parameters = this->format(input.substr(firstOf + 1, lastOf - firstOf - 1));
        returnToken += method + "(";
        returnToken += parameters + ") ";
    } else {
        vector<string> brokenLine = this->split(input, ' ');
        if (brokenLine.size() > 1) {
            for (auto i : brokenLine) {
                returnToken += this->format(i) + " ";
            }
        } else {
            if (brokenLine[0] == "") {
                return "";
            }
            brokenLine = this->split(brokenLine[0], '.');
            if (brokenLine.size() > 1) {
                bool toggle = false;
                for (auto i : brokenLine) {
                    if (toggle) {
                        returnToken += ".";
                    }
                    toggle = true;
                    returnToken += this->format(i);
                }
            } else {
                if (brokenLine[0] == "") {
                    return "";
                }

                if (brokenLine[0] == "") {
                    return "";
                }
                brokenLine = this->split(brokenLine[0], ',');
                if (brokenLine.size() > 1) {
                    bool toggle = false;
                    for (auto i : brokenLine) {
                        if (toggle) {
                            returnToken += ",";
                        }
                        toggle = true;
                        returnToken += this->format(i);
                    }
                } else {
                    if (brokenLine[0] == "") {
                        return "";
                    }
                    brokenLine = this->split(brokenLine[0], ':');
                    if (brokenLine.size() > 1) {
                        for (auto i : brokenLine) {
                            returnToken += this->format(i) + " ";
                        }
                    } else {
                        if (brokenLine[0] == "") {
                            return "";
                        }
                        if (input == "is") {
                            returnToken += "== ";
                        } else if (input == "and") {
                            returnToken += "&& ";
                        } else if (input == "or") {
                            returnToken += "|| ";
                        } else if (input == "num") {
                            returnToken += "int ";
                        } else if (input == "dec") {
                            returnToken += "double ";
                        } else if (input == "string") {
                            returnToken += "string ";
                        } else if (input == "nothing") {
                            returnToken += "nullptr ";
                        } else if (input == "print") {
                            returnToken += "cout << ";
                        } else if (input.front() == '\'' && input.back() == '\'') {
                            input.front() = '\"';
                            input.back() = '\"';
                            returnToken += input + " ";
                        } else {
                            returnToken += input + " ";
                        }
                    }
                }
            }
        }
    }

    return returnToken;
}

string oceanTransform::replaceAll(string str, const string &from, const string &to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

string oceanTransform::nodeTrawl(oceanSyntacticNode *input) {
    string returnString = "";
    if (input->terminate == 1) return returnString;
    returnString += this->parser(input);
    if (input->next != nullptr) returnString += this->nodeTrawl(input->next);
    return returnString;
}

string oceanTransform::parser(oceanSyntacticNode *input) {
    vector<string> brokenLine = this->split(input->data, ':');
    string returnToken = "";
    if (input->data == "") return returnToken;
    if (brokenLine[0] == "if") {
        returnToken += this->ifParser(input);
    } else if (brokenLine[0] == "else") {
        returnToken += this->elseParser(input);
    } else if (brokenLine[0] == "otherwise") {
        returnToken += this->otherwiseParser(input);
    } else if (brokenLine[0] == "for") {
        returnToken += this->forParser(input);
    } else if (brokenLine[0] == "met") {
        returnToken += this->metParser(input);
    } else {
        returnToken += this->format(input->data) + ";";
    }
    returnToken += "\n";
    return returnToken;
}

string oceanTransform::ifParser(oceanSyntacticNode *input) {
    string returnToken = "if (";
    returnToken += this->format(input->data.substr(input->data.find_first_of(':', 0), string::npos));
    returnToken += ") {\n";
    returnToken += this->nodeTrawl(input->child);
    returnToken += "\n}";
    return returnToken;
}

string oceanTransform::elseParser(oceanSyntacticNode *input) {
    string returnToken = "else if (";
    returnToken += this->format(input->data.substr(input->data.find_first_of(':', 0), string::npos));
    returnToken += ") {\n";
    returnToken += this->nodeTrawl(input->child);
    returnToken += "\n}";
    return returnToken;
}

string oceanTransform::otherwiseParser(oceanSyntacticNode *input) {
    string returnToken = "else {\n";
    returnToken += this->nodeTrawl(input->child);
    returnToken += "\n}";
    return returnToken;
}

string oceanTransform::forParser(oceanSyntacticNode *input) {
    string returnToken = "for (";
    string statement = this->format(input->data.substr(input->data.find_first_of(':', 0), string::npos));
    while (statement.find(", ", 0) != string::npos) {
        statement = this->replaceAll(statement, ", ", ",");
    }
    while (statement.find(". ", 0) != string::npos) {
        statement = this->replaceAll(statement, ". ", ".");
    }
    while (statement.find(" ", 0) != string::npos) {
        statement = this->replaceAll(statement, " ", "");
    }
    vector<string> brokenLine = this->split(statement, '=');
    string variable = brokenLine[0];
    string range = brokenLine.back();
    brokenLine = this->split(range, '.');
    string end = this->split(range, '.').back();
    string begin = this->split(range, ',').front();
    string interval = this->split(this->split(range, '.').front(), ',').back();
    returnToken +=
            "int " + variable + " = " + begin + "; " + variable + " != " + end + "; " + variable + " += (" + interval +
            " - " + begin +
            ")";
    returnToken += ") {\n";
    returnToken += this->nodeTrawl(input->child);
    returnToken += "\n}";
    return returnToken;
}

string oceanTransform::metParser(oceanSyntacticNode *input) {
    string returnToken = "";
    vector<string> brokenLine = this->split(input->data, ' ');
    vector<string> brokenKey = this->split(brokenLine[0], ':');
    returnToken +=
            brokenKey[1] + " (" +
            this->format(
                    this->replaceAll(this->trim(input->data.substr(input->data.find_first_of(' ', 0), string::npos)),
                                     " ",
                                     ",")) +
            ") {\n";
    returnToken += this->nodeTrawl(input->child);

    string type = "";
    if (returnToken.find("return ") != string::npos) {
        string returnVal = returnToken.substr(returnToken.find("return "), string::npos);
        while (returnVal.find("\n", 0) != string::npos) {
            returnVal = this->replaceAll(returnVal, "\n", "");
        }
        while (returnVal.find(";", 0) != string::npos) {
            returnVal = this->replaceAll(returnVal, ";", "");
        }
        while (returnVal.find("return ", 0) != string::npos) {
            returnVal = this->replaceAll(returnVal, "return ", "");
        }
        while (returnVal.find(" ", 0) != string::npos) {
            returnVal = this->replaceAll(returnVal, " ", "");
        }
        returnVal = " " + returnVal + " ";
        int end = returnToken.find(returnVal, 0);
        int begin = returnToken.find_last_of(" \n", end - 2);
        type = returnToken.substr(begin + 1, end - begin);
    } else {
        type = "void";
    }

    returnToken += "}\n";
    returnToken = type + " " + returnToken;
    return returnToken;
}

string oceanTransform::rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
    return s;
}

string oceanTransform::ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
    return s;
}

string oceanTransform::trim(string s) {
    return this->rtrim(this->ltrim(s));
}

