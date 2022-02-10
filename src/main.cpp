#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "generateVerilogString.h"
#include "OpGraph.h"
#include "parseInputFile.h"



using namespace std;

int main(int argc, char* argv[])
{
	//Command Arguments
	void CommandArgCheck(int argc, char* argv[]);

	unordered_map<string, unordered_map<string, vector<string>>> varTypeToLengthToNamesMap;
	vector<vector<string>> operationLines;

	//Dario
	parseInputFile(argv[1], varTypeToLengthToNamesMap, operationLines);

	//Sean
	OpGraph *opG = new OpGraph(operationLines, stoi(argv[2]));
	opG->scheduleAlap();
	opG->scheduleOperations();
	unordered_map<int, vector<OpNode>> startTimeToNodesMap = opG->getListRTimesToNodesMap();
	
	//Leslie
	string verilogString;
	verilogString += generateVerilogString(varTypeToLengthToNamesMap, startTimeToNodesMap, stoi(argv[2]));

	//cout << verilogString << endl;

	ofstream outFileStream;
	outFileStream.open(argv[3]);
	outFileStream << verilogString;
	outFileStream.close();


	return 0;
}
