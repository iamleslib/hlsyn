
#ifndef HLSYN_H
#define HLSYN_H

#include <string>
#include <vector>
#include <unordered_map>
#include "OpGraph.h"

using namespace std;

string generateVerilogString(unordered_map<string, unordered_map<string, vector<string>>> varTypeToLengthToNamesMap, unordered_map<int, vector<OpNode>> startTimeToNodesMap, int latency);


#endif