/* Generates output verilogFile */

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "OpGraph.h"

using namespace std;

string generateVerilogString(unordered_map<string, unordered_map<string, vector<string>>> varTypeToLengthToNamesMap, unordered_map<int, vector<OpNode>> startTimeToNodesMap, int latency)
{
	string verilogString;
	//---Standard module header---
	verilogString += "`timescale 1ns / 1ps \n\n\n";

	//---Standard module declaration---
	verilogString += "module HLSM(Clk, Rst, Start, Done,  ";

	//---> OTHER inputs and outputs for port declaration
	for (auto varTypeToSubMapPair : varTypeToLengthToNamesMap)
	{
		string varType = varTypeToSubMapPair.first;
		unordered_map<string, vector<string>> varSizeToNameMap = varTypeToSubMapPair.second;

		int loopCount = 0;
		for (auto varSizeToNamesMapPair : varSizeToNameMap)
		{
			string varSize = varSizeToNamesMapPair.first;
			vector<string> varNames = varSizeToNamesMapPair.second;

			//---> Listing OTHER inputs ports
			if (varType == "input")
			{
				for (unsigned i = 0; i < varNames.size(); i++)
				{
					verilogString += varNames[i];

					if (!((i == varNames.size() - 1) && (loopCount == varSizeToNameMap.size() - 1)))
					{
						verilogString += ", ";
					}

				}
			}
			loopCount++;

		}
		//---> Listing OTHER outputs ports
		
		for (auto varSizeToNamesMapPair : varSizeToNameMap)
		{
			string varSize = varSizeToNamesMapPair.first;
			vector<string> varNames = varSizeToNamesMapPair.second;

			//---> Listing inputs
			if (varType == "output")
			{
				for (unsigned i = 0; i < varNames.size(); i++)
				{
					verilogString += varNames[i] + ", ";

				}
			}
			
		}
	}

	verilogString += ");\n";

	//---Standard inputs---
	verilogString += "input Clk, Rst, Start;\n";
	
	//---Standard outputs---
	verilogString += "output reg Done;\n\n";

	//---> OTHER inputs, outputs, reg variables
	for (auto varTypeToSubMapPair : varTypeToLengthToNamesMap)
	{
		string varType = varTypeToSubMapPair.first;

		unordered_map<string, vector<string>> varSizeToNameMap = varTypeToSubMapPair.second;

		for (auto varSizeToNamesMapPair : varSizeToNameMap)
		{
			string varSize = varSizeToNamesMapPair.first;
			vector<string> varNames = varSizeToNamesMapPair.second;

			//Checking for input variables
			if (varType == "input")
			{
				//Checking if signed or unsigned (REUSABLE)
				verilogString += "input ";
				if (varSize[0] == 'I')
				{
					verilogString += "signed ";
				}
				//Finding bitwidth of variable (REUSABLE)
				size_t found = varSize.find_last_of("t");
				string bitWidth = varSize.substr(found + 1);

				verilogString += "[" + to_string(stoi(bitWidth) - 1) + ":0]";

				for (unsigned i = 0; i < varNames.size(); i++)
				{
					verilogString += varNames[i];

					if (!(i == varNames.size() - 1))
					{
						verilogString += ", ";
					}
					else {
						verilogString += ";\n";
					}
				}

				verilogString += "\n";


			}
			//Checking for output variables
			else if (varType == "output")
			{
				//Checking if signed or unsigned (REUSABLE)
				verilogString += "output reg ";
				if (varSize[0] == 'I')
				{
					verilogString += "signed ";
				}
				//Finding bitwidth of variable (REUSABLE)
				size_t found = varSize.find_last_of("t");
				string bitWidth = varSize.substr(found + 1);

				verilogString += "[" + to_string(stoi(bitWidth) - 1) + ":0]";
				for (unsigned i = 0; i < varNames.size(); i++)
				{
					verilogString += varNames[i];

					if (!(i == varNames.size() - 1))
					{
						verilogString += ", ";
					}
					else {
						verilogString += ";\n";
					}

				}
				verilogString += "\n";
			}
			//Checking for reg variables
			else if (varType == "variable")
			{
				//Checking if signed or unsigned (REUSABLE)
				verilogString += "reg ";
				if (varSize[0] == 'I')
				{
					verilogString += "signed ";
				}
				//Finding bitwidth of variable (REUSABLE)
				size_t found = varSize.find_last_of("t");
				string bitWidth = varSize.substr(found + 1);

				verilogString += "[" + to_string(stoi(bitWidth) - 1) + ":0]";
				for (unsigned i = 0; i < varNames.size(); i++)
				{
					verilogString += varNames[i];

					if (!(i == varNames.size() - 1))
					{
						verilogString += ", ";
					}
					else {
						verilogString += ";\n";
					}

				}
				verilogString += "\n";
			}

		}
	}

	verilogString += "\n";
	//--->END DECLARING INPUTS, OUTPUTS, REGS<---//


	//---> Declaring State Parameters <---//

	verilogString += "parameter S_WAIT = 0, ";

	int i = 1;
	while (i <= startTimeToNodesMap.size() + 1)
	{
		verilogString += "S_T" + to_string(i) + " = " + to_string(i) + ",";
		//cout << i << endl;
		i++;
	}
	verilogString += " S_FINAL = " + to_string(startTimeToNodesMap.size() + 2) + ";\n";


	//Determining min number of bits needed for State variable
	int minBits = ceil(log2(startTimeToNodesMap.size() + 2));

	verilogString += "reg [" + to_string(minBits - 1) + ":0] State;\n\n";

	

	//---Standard Always block and initial wait state
	verilogString += "always @(posedge Clk) begin\nif(Rst == 1) begin\nState <= S_WAIT;\nDone <= 0;\nend\nelse begin\n";
	verilogString += "case(State)\n";
	verilogString += "S_WAIT : begin\nDone <= 0;\nif(Start == 1) begin\nState <= S_T1;\nend\nend\n";
	

	//Looping through State CASES for operations
	for (auto q : startTimeToNodesMap) 
	{

		verilogString += "S_T" + to_string(q.first) + " : begin\n";
		
		for (int j = 0; j < q.second.size(); j++) 
		{
			
			//Checking # of input variables
			if (q.second[j].inputVars.size() == 3)
			{
				verilogString += q.second[j].outputVar + " <= " + q.second[j].inputVars[0] + " " + q.second[j].operatorToken + " " + q.second[j].inputVars[1] + " : " + q.second[j].inputVars[2] + ";\n";
			}
			else
			{
				verilogString += q.second[j].outputVar + " <= " + q.second[j].inputVars[0] + " " + q.second[j].operatorToken + " " + q.second[j].inputVars[1] + ";\n";


			}
		}
		if (q.first != latency) 
		{
			int nextState = q.first + 1;
			verilogString += "State <= S_T" + to_string(nextState) + ";\n";
			verilogString += "end\n";
		}
		else 
		{
			verilogString += "State <= S_FINAL;\nend\n";
		}

	}
	
	//---> Standard Ending module
	verilogString += "S_FINAL : begin\nDone <= 1;\nend\nendcase\nend\nend\nendmodule";



	return verilogString;
}