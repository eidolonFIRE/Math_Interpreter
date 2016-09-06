/*
  o------------------------------o
  | · Caleb Johnson ·            |
  | 7 / 3 / 2012                 |
  | version  1.0                 |
  o------------------------------o
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "cMathInterp.cpp"

using namespace std;




int main()
{
	cMathInterp interp;
	float a, b, output = 0;
	
	interp.SetAuxPointers(&a, &b, &output);
	
	//system("CLS");
	string str = "";
	
	a = 3.14;
	b = 2.75;
	
	
	do
	{
		printf("\n\n\n  Enter script: ");
		//scanf("%s", &cstr);
		//cin >> str;
		getline(cin, str);
		//str = "3 + 4";
		int er = interp.SetScript(str);
		
		if (er != 0) printf("\n\n ERROR - %d\n", er);
		else
		{		
			interp.Calculate();
			printf("\n\n\t=====  Output: %.2f\n\n", output);
		}

	} while (str != "quit");
	return 0;
};