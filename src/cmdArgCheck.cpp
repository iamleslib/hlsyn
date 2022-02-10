#include <string>
#include <iostream>

using namespace std;

void CommandArgCheck(int argc, char* argv[])
{


	//===================> CHECKING COMMAND ARGUMENTS<===================//

	//command-line inputs
	string inFileName;
	//string latencyConstraint;
	string outFileName;


	if (argc == 4)    //Correct # of command arguments
	{
		//Setting parameters
		inFileName = argv[1];
		//latencyConstraint = argv[2];
		outFileName = argv[3];

		//Verifying that input file is .txt and output file is .v
		if (!(inFileName.substr(inFileName.length() - 2) == ".c") || !(outFileName.substr(outFileName.length() - 2) == ".v"))
		{
			cout << "Usage: " << argv[0] << " <netlistFile>  <latency>  <verilogFile> " << endl;
			exit(0);
		}
	}
	else //Incorrect # of command arguments
	{

		cout << "Incorrect number of command arguments entered" << endl;
		cout << "Usage: " << argv[0] << " <netlistFile>  <latency>  <verilogFile> " << endl;
		exit(0);
	}
	//=================> END CHECKING COMMAND ARGUMENTS<=================//
}