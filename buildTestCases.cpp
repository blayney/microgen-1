#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class testStruct{
	public:
		char name;
		vector<char> memberVars;
};

testStruct buildStruct(testStruct *structure){

	char selection;
	cin >> selection;

	switch(selection){
		case 'n':
			char varname;
			cout << "\nplease input a name for this var:   ";
			cin >> varname;
			cin.ignore(1,'\n');
			structure -> memberVars.push_back(varname);
			break;
		case 'l':
			for (int i = 0;
			 i < static_cast<int>(structure -> memberVars.size()); ++i)
			{
				cout << "\n" << structure -> memberVars.at(i);
			}
			break;
		case 's':
			return *structure;
		default:
			cout << "\ncommand not recognised\n";
			break;
	}

	return *structure;
}

testStruct declareStruct(){
	testStruct *structure = new testStruct();
	char selection;
	char structname;

	cout << "\ndefine test struct\n";
	cout << "please enter a name for this struct:  ";

	cin >> structname;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	structure -> name = structname;

	cout << "\ntype 'n' to add new var\n";
	cout << "type 'l to list added vars'\n";
	cout << "type 's' to save test struct\n";

	buildStruct(structure);
	
}

int main(int argc, char* argv[]){

	cout << "\nTest case expression builder for verilator custom testbench\n\n";

	declareStruct();

	return 0;
}