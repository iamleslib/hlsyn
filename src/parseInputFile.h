//
//  parseInputFile.hpp
//  Assignment3
//
//  Created by Dario on 12/12/19.
//  Copyright © 2019 Dario. All rights reserved.
//

#ifndef parseInputFile_h
#define parseInputFile_h
#include <string>
#include <stdio.h>
#include <vector>
#include <unordered_map>
using namespace std;
void parseInputFile(string fileName, unordered_map<string, unordered_map<string, vector<string>>> & varTypeToLengthToNamesMap, vector<vector<string>> &operationLines);
bool existence(vector <string> outputVars, string tempVars);
#endif /* parseInputFile_hpp */
