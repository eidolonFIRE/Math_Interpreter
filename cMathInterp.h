/*
  o------------------------------o
  | · Caleb Johnson ·            |
  | 7 / 5 / 2012                 |
  | version  1.0.01              |
  o------------------------------o
*/



using namespace std;


class cMathInterp
{

	private:
	float *Aux1, *Aux2, *Output;					// pointers to external vars. This is how the mathInter class is "connected" to the outside world
	
	
	struct word_s
	{
		float value;
		char op;
		int type;
	};
	
	cWordlist words;
	
	word_s stack_script[32];					// the script stored in executable stack form
	int index;							// "top" index of the script
	int index_max;
	
	void clearstack();
	void push(float number);					// push a number onto the script stack
	void push(char op);						// push an operator onto the script stack
	bool isnum(char c);						// returns true if the char is part of a number
	bool isop(char c);						// returns true if the char is part of an operator
	bool isop2(char c);						// same as other minus letters
	char getop(string s);						// a string is passed in and an op char is given out
	int getmode(char c);						// returns the type of char... aka op, number, var, bracket????
	int precede(char c);
	
	public:	
	cMathInterp();
	void SetAuxPointers(float *A, float *B, float *OUTPUT);		// setup the pointers to external vars
	int SetScript(string script);
	int Calculate();						// This function reads from the main stack(s) by poping to the "stack" and computing
	
	
};