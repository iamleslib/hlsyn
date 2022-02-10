//
//  parseInputFile.cpp
//  Assignment3
//
//  Created by Dario on 12/12/19.
//  Copyright © 2019 Dario. All rights reserved.
//

#include "parseInputFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;
bool existence(vector <string> outputVars, string tempVars){
    if (std::find(outputVars.begin(), outputVars.end(), tempVars) != outputVars.end())
        return true;
    else
        return false;
}


void parseInputFile(string fileName, unordered_map<string, unordered_map<string, vector<string>>> & varTypeToLengthToNamesMap, vector<vector<string>> &operationLines){
string inFileName = fileName;
string outFileName;
vector<string> tempVector;
//vector<vector<string>> operationLines;
vector<string> inputVars;
vector<string> outputVars;
vector<string> variableVars;

    
    //------------UINT16--<a,b,c>
    unordered_map<string, vector<string>> inputvarSizeToNamesMap;
    unordered_map<string, vector<string>> outputvarSizeToNamesMap;
    unordered_map<string, vector<string>> variablesvarSizeToNamesMap;
    
    //------------input-----varSizeToNamesMap
    unordered_map<string, unordered_map<string, vector<string>>> varTypeToSizeToNamesMap;
    
    
//unordered_map<string, unordered_map<string, vector<string>>>& varTypeToLengthToNamesMap
  //  inFileName = "hls_test1.c";
    
    ifstream file(inFileName);
    if (!file)
    {
        cout << "Unable to open the file.\n";
        std::exit(0);
    }
   else if (file.peek() == std::ifstream::traits_type::eof()) // Check if file is empty.
    {
        cout << "The input file was empty. Uh-oh!!!\n";
        std::exit(0);
    
    }
    
    string str;
    
    while (std::getline(file, str)) {
        //std::cout << str << "\n";
        istringstream iss(str);
        int stop = 0;
        ///seperate individual words
        do
        {
            string subs;
            iss >> subs;
            if ((subs.size() >= 2) && ((subs[0] == '/') && (subs[1] == '/'))){
                
                stop = 1;
                subs = "";
            }
            else{
                
                if (!subs.empty() && (stop == 0)){
                    for (int i = 0; i<subs.length(); i++)
                        if (subs[i] == ','){
                            subs.resize(i);
                        }
                    tempVector.push_back(subs);
                    //cout << "Substring: " << subs << endl;
                }
            }
        } while (iss);
        ///// END seperate individual words
        
        
        ////START seperate into input, output, wire, register, vector of string vectors
        if (tempVector.empty()){ // Check if tempVector is null
            //Do Nothing, go to next line.
        }
        /*
        //------------UINT16--<a,b,c>
        unordered_map<string, vector<string>> varSizeToNamesMap;
        
        //------------input-----varSizeToNamesMap
        unordered_map<string, unordered_map<string, vector<string>>> varTypeToSizeToNamesMap;
         */
        else if (tempVector[0].compare("input") == 0){
            vector<string> subVector;
            //cout<< tempVector[0];
            for (int i = 2; i < tempVector.size(); i++){
                subVector.push_back(tempVector[i]);
                inputVars.push_back(tempVector[i]);
            }
            inputvarSizeToNamesMap.insert({tempVector[1], subVector});
            
            subVector.clear();
        }
        //// output map insert
        else if (tempVector[0].compare("output") == 0){
            vector<string> subVector;
            //cout<< tempVector[0];
            for (int i = 2; i < tempVector.size(); i++){
                subVector.push_back(tempVector[i]);
                inputVars.push_back(tempVector[i]);

            }
            outputvarSizeToNamesMap.insert({tempVector[1], subVector});
            
            subVector.clear();
        }
        else if (tempVector[0].compare("variable") == 0){
            vector<string> subVector;
            //cout<< tempVector[0];
            for (int i = 2; i < tempVector.size(); i++){
                subVector.push_back(tempVector[i]);
                inputVars.push_back(tempVector[i]);

            }
            variablesvarSizeToNamesMap.insert({tempVector[1], subVector});
            
            subVector.clear();
        }
        //// variable map insert
        
        else{
            operationLines.push_back(tempVector);
        }
        
        tempVector.clear();
        
    }
    varTypeToSizeToNamesMap.insert({"output", outputvarSizeToNamesMap});
    varTypeToSizeToNamesMap.insert({"variable", variablesvarSizeToNamesMap});
    varTypeToSizeToNamesMap.insert({"input", inputvarSizeToNamesMap});
    /////// START Check if variables exist
    ///// check left side of equal sign if it is an output, wire, register variable
    for (int i = 0; i < operationLines.size(); i++){
        if (existence(inputVars, operationLines[i][0]) == false){
            cout << "Error in input file";
            std::exit(0);
        }
    }
    for (int i = 0; i < operationLines.size(); i++){
        
        if (operationLines[i].size() == (3)){
            if (!((existence(inputVars, operationLines[i][2])))){
                cout << "Error in Input File" << endl;
                std::exit(0);
                
            }
        }
            if (operationLines[i].size() == (5)){
                if (!((existence(inputVars, operationLines[i][4])))){
                    cout << "Error in Input File" << endl;
                    std::exit(0);
                    
                }
                
            }
            if (operationLines[i].size() == (7)){
                if (!((existence(inputVars, operationLines[i][6])))){
                    cout << "Error in Input File" << endl;
                    std::exit(0);
                    
                }
            
        }
    
        }
    ////END seperate into input, output, wire, register, vector of string vectors

    
    
    varTypeToLengthToNamesMap = varTypeToSizeToNamesMap;
        
    }///end parseInputFile

