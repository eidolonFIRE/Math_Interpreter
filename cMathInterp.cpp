/*
  o------------------------------o
  | · Caleb Johnson ·            |
  | 7 / 31 / 2012                |
  | version  1.1.06              |
  o------------------------------o
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>


#include "cWordlist.cpp"
#include "cMathInterp.h"


using namespace std;


cMathInterp::cMathInterp()
{
	clearstack();
}


void cMathInterp::clearstack()
{
	index_max = 30;
	for (int t = 0; t < 32; t++)
	{
		// clear script stack
		stack_script[t].type = 0;
		stack_script[t].value = 0;
		stack_script[t].op = 0;
	}
	index = 0;
}

void cMathInterp::SetAuxPointers(float *A, float *B, float *OUTPUT)
{
	Aux1 = A;
	Aux2 = B;
	Output = OUTPUT;
}


void cMathInterp::push(float number)
{
	if (stack_script[index].type != 0) index++;			// if there is already a number stored at this index, move to the next
	stack_script[index].value = number;				// record the number at [index]
	stack_script[index].type = 1;
}

void cMathInterp::push(char op)
{
	if (stack_script[index].type != 0) index++;			// if there is already a number stored at this index, move to the next
	stack_script[index].op = op;					// record the op at [index]
	stack_script[index].type = 2;
}

bool cMathInterp::isnum(char c)
{
	bool r = false;
	if ((c >= 48)		//  0
	 && (c <= 57)		//  9
	 || (c == '.')) r = true;
	return r;
}

bool cMathInterp::isop(char c)
{
	bool r = false;
	if ((c >= 97)		//  a
	 && (c <= 122)		//  z 
	 || (c >= 42)		//  *
	 && (c <= 45)		//  -
	 || (c == 47)		//  /
	 || (c == 40)		//  (
	 || (c == 41)		//  )
	 || (c == 94))		//  ^
	 r = true;
	return r;
}

bool cMathInterp::isop2(char c)
{
	bool r = false;
	if ((c >= 42)		//  *
	 && (c <= 45)		//  -
	 || (c == 47)		//  /
	 || (c == 40)		//  (
	 || (c == 41)		//  )
	 || (c == 94))		//  ^
	 r = true;
	return r;
}


char cMathInterp::getop(string s)
{
	char r = 0;
	if (s == "abs") r = 'A';	
	if (s == "max") r = 'M';
	if (s == "min") r = 'N';
	if (s == "sin") r = 'S';
	if (s == "cos") r = 'C';
	if (s == "tan") r = 'T';
	if (s == "invsin") r = 's';
	if (s == "invcos") r = 'c';
	if (s == "invtan") r = 't';
	if (s == "log") r = 'L';
	if (s == "ln") r = 'l';
	if (s == "x") r = 'x';
	if (s == "y") r = 'y';
	
	if (s == ")") r = ')';
	if (s == "(") r = '(';
	
	if (s == "+") r = '+';
	if (s == "-") r = '-';
	if (s == "*") r = '*';
	if (s == "/") r = '/';
	if (s == "^") r = '^';
	return r;
}

int cMathInterp::getmode(char c)					// returns the type of char... aka op, number, var, bracket????
{
	int r = 0;
	if (isnum(c)) r = 1;
	if (isop(c)) r = 2;
	if (c == 40 || c == 41) r = 3;				// parenthesis
	if (c == 120 || c == 121) r = 4;			// vars seperated out
	if ((c >= 97) && (c <= 122)) r = 5;			// a-z
	//printf("\n        - getmode(%c) = %d", c, r);
	return r;
}

int cMathInterp::precede(char c)
{
	int r = 0;
	switch (c)
	{
		case 'L': r = 5; break;
		case 'l': r = 5; break;
		case 'A': r = 5; break;
		case 'M': r = 5; break;
		case 'N': r = 5; break;
		case 'S': r = 5; break;
		case 'C': r = 5; break;
		case 'T': r = 5; break;
		case 's': r = 5; break;
		case 'c': r = 5; break;
		case 't': r = 5; break;
		
		case 'x': r = 2; break;
		case 'y': r = 2; break;
		
		case '^': r = 4; break;
		case '*': r = 3; break;
		case '/': r = 3; break;
		case '+': r = 2; break;
		case '-': r = 2; break;
	}
	return r;
}

int cMathInterp::SetScript(string script)				// Sets the input script.. Error Codes return
{									// 0) All ok
									// 1) Formula has bad formating (missing parenthesis etc)
									// 2) Formula is too long (stack overflow)
									// 3) Formula contains unrecognised data (check operators)
	/////////////////// Tokenize script ///////////////////
	//printf("\n\n===[ Begin Tokenizing ]===");
	words.clear();
	int mode = 0;							// this "mode" keeps track if cursor is a number, var, op etc
	for (int s = 0; s < script.size(); s++)
	{
		if (script[s] == ' ') continue;				// skip white spaces
		if (isop2(script[s]))
		{
			// a single char op is found
			words.Add(script[s]);
			//printf("\n    -  op %c ", w.op);
			continue;
		}
		mode = getmode(script[s]);				// get the type value of the first char
		if (mode != 0)						// if there is a mode, scan until it changes
		{
			int start = s, finish;
			for (finish = s + 1; finish < script.size(); finish++) if (getmode(script[finish]) != mode) break;
			//printf("\n    - Got a mode (%d)-(%d)", start, finish);
			word w;
			string str = script.substr(start, finish - start);
			if (mode == 1)
			{
				w.value = float(atof(str.c_str()));
				//printf("\n    - num %.2f ", w.value);
			}
			if (mode > 1)
			{
				w.op = getop(str);
				//printf("\n    -  op %c ", w.op);
				if (w.op == 0) return 3;
			}
			w.type = mode;
			if (w.type > 2) w.type = 2;
			if (w.type == 1) words.Add(w.value);
				else words.Add(w.op);			// add the word to the output word list
			s = finish - 1;					// jump the scan to the end of the found word so it's not counted twice
		}
		else
		return 3;						// return error: unknown character
	}
	
	
	for (int s = 0; s < words.size() - 1; s++)			// check for double negative, replace with +
	{
		if (words[s] == '-' && words[s+1] == '-')
		{
			words.Erase(s);
			words[s] == '+';
		}
	}

	for (int s = 0; s < words.size() - 1; s++) if (words[s] == '+' && words[s+1] == '+') words.Erase(s);	// check for double positive
	
	for (int s = 0; s < words.size() - 1; s++)			// "+ -" ==> "-"
	{
		if (words[s] == '+' && words[s+1] == '-') words.Erase(s);
		if (words[s] == '-' && words[s+1] == '+') words.Erase(s+1);
	}
	
	for (int s = 0; s < words.size() - 1; s++)
	{
		if (words[s] == '(' && words[s+1] == '-') words.Insert(s + 1, 0.0f);
	}
	
	
	for (int s = 0; s < words.size(); s++)				// correct "minus" into "negative"
	{
		if (words[s] == '-' && words.isop(s-1) && isop2(words[s-1].op) && words[s-1] != ')')
		{
			words.Insert(s, 0.0f);
			words.Insert(s, '(');
			
			s += 2;
			
			// insert ')' between number and lower or equal preceding operator
			int skip = 0;
			for (int ss = s; ss < words.size() - 1; ss++)
			{
				if (words[ss] == '(') skip++;					// skip over brackets
				
				if (words[ss] == ')' && skip > 0) skip--;

				if (skip == 0)
				{
				
					if (words.isnum(ss) && words.isop(ss+1)) 		// first check for number in front of operator
					if (precede(words[ss+1].op) == 2)			// then check for low precedence operator
					{
						words.Insert(ss + 1, ')');
						s = 0;
						break;
					}
					
					if (words[ss] == ')' && words.isop(ss+1) && precede(words[ss + 1].op) == 2)
					{
						words.Insert(ss, ')');
						s = 0;
						break;
					}
				}	
				if (ss == words.size() - 2)					// if fail... put at end
				{
					words.Insert(words.size(), ')');
					s = 0;
					break;
				}
				
			}
		}
	}
	
	printf("\n\n--[ Word list ]--\n");
	for (int t = 0; t < words.size(); t++) 
	{
		if (words.isnum(t)) printf(" %.1f", words[t].value);
		if (words.isop(t)) printf(" %c", words[t].op);
	}
	
	clearstack();
	char optemp[32];						// temporary op stack used in the shunting yard algorithm
	int optemp_index = 0;						// the index or "top" of the optemp stack
	for (int t = 0; t < 32; t++) optemp[t] = 0;			// clear the optemp array to zero
	
	
	
	/////////////// Shunting-Yard Algorithm /////////////////
	//printf("\n\n===[ Begin Shunting-Yard ]===");
	for (int t = 0; t < words.size(); t++)
	{
		//-- if number, push to stack
		if (words.isnum(t))
		{
			push(words[t].value);
			//printf("\n    -Push(%.1f)", words[t].value);
			if (index > index_max) return 2;
		}
		
		if (words.isop(t))
		{
			//printf("\n    -precede, %d, %d", precede(words[t].op), precede(optemp[optemp_index]));
			if (words[t].op == '(')
			{
				if (optemp[optemp_index] != 0) optemp_index++;
				optemp[optemp_index] = words.words[t].op;
				//printf("   ...Push bracket %c", optemp[optemp_index]);
			}
			else
			if (words.words[t].op == ')')
			{
				bool flag = false;
				for (int p = optemp_index; p >= 0; p--)
				{
					if (optemp[optemp_index] == '(')
					{
						//-- found the other bracket, delete it, continue
						//printf("   ...Found (");
						flag = true;
						optemp[optemp_index] = 0;
						if (optemp_index > 0) optemp_index--;
						//-- check for function, if so, pop to stack
						if (precede(optemp[optemp_index]) == 5)
						{
							//printf("   ...Push function op(%c)", optemp[optemp_index]);
							push(optemp[optemp_index]);
							if (index > index_max) return 2;
							optemp[optemp_index] = 0;
							if (optemp_index > 0) optemp_index--;
						}
						break;
					}
					else
					{
						//printf("   ...Pop to stack(%c)", optemp[optemp_index]);
						push(optemp[optemp_index]);
						if (index > index_max) return 2;
						optemp[optemp_index] = 0;
						if (optemp_index > 0) optemp_index--;
					}
				}
				if (flag == false) return 1;
			}
			else
			if (precede(words[t].op) > precede(optemp[optemp_index]))
			{
				//-- if a higher precedence than what is there, push to temp stack
				if (optemp[optemp_index] != 0) optemp_index++;
				optemp[optemp_index] = words[t].op;
				//printf("   ...Push to optemp(%c)", optemp[optemp_index]);

			}
			else
			if (precede(words[t].op) <= precede(optemp[optemp_index]))
			{
				//-- they are equal, pop then push
				//printf("   ...Pop(%c), Push(%c)", optemp[optemp_index], words[t].op);
				push(optemp[optemp_index]);
				optemp[optemp_index] = words[t].op;
				if (index > index_max) return 2;
			}
		}
	
	}
	
	//--- Push/Pop the optemp stack to the script stack
	for (int t = optemp_index; t >= 0; t--)
	{
		//printf("\n    -Pop optemp to stack(%c)", optemp[t]);
		if (optemp[t] == '(') return 1;
		push(optemp[t]);
		if (index > index_max) return 2;
	}
	
	printf("\n\n--[ RPN ]--\n");
	for (int t = 0; t <= index; t++) 
	{
		if (stack_script[t].type == 1) printf(" %.1f", stack_script[t].value);
		if (stack_script[t].type == 2) printf(" %c", stack_script[t].op);
	}
	
	return 0;
}







int cMathInterp::Calculate()
{
	float stack[20];						// this stack is for the number crunching when it solves. (it will read from the other stacks)
	int read = 0;							// read index of the main script stack(s)
	int i = 0;							// the index or "top" of the stack
	// note: index .:. the "top" of the other stacks
	
	for (read = 0; read <= index; read++)				// scan the entire stored stack
	{
		//check for number
		if (stack_script[read].type == 1)
		{
			//if number, pop to stack
			i++;
			if (i > 19) return 1;				// STACK OVERFLOW!!!!!
			stack[i] = stack_script[read].value;
			//printf("\n    - Pop %.2f to stack(%d)", stack[i], i);
		}
		
		//check for operator
		if (stack_script[read].type == 2)
		{
			switch (stack_script[read].op)
			{
			case 'x':	// aux ( x )
				i++;
				if (i > 19) return 1;
				stack[i] = *Aux1;
				break;
			case 'y':	// aux ( y )
				i++;
				if (i > 19) return 1;
				stack[i] = *Aux2;
				break;
			case '+':	// add previous 2 numbers
				//printf("\n    - Add(%.2f, %.2f)", stack[i-1], stack[i]);
				stack[i - 1] = stack[i - 1] + stack[i];
				i--;
				break;
			case '-':	// subtract previous 2 numbers
				stack[i - 1] = stack[i - 1] - stack[i];
				i--;
				break;
			case '*':	// multiply previous 2 numbers
				stack[i - 1] = stack[i - 1] * stack[i];
				i--;
				break;
			case '/':	// divide previous 2 numbers
				stack[i - 1] = stack[i - 1] / stack[i];
				i--;
				break;
			case '^':	// power
				stack[i - 1] = pow(stack[i - 1], stack[i]);
				i--;
				break;
			case 'A':	// abs value of current number
				stack[i] = abs(stack[i]);
				break;
			case 'M':	// returns larger of previous 2 numbers
				if (stack[i] > stack[i - 1]) stack[i - 1] = stack[i];
				i--;
				break;
			case 'N':	// returns smaller of previous 2 numbers
				if (stack[i] < stack[i - 1]) stack[i - 1] = stack[i];
				i--;
				break;
			case 'S':	// SIN
				stack[i] = sin(stack[i]);
				break;
			case 'C':	// COS
				stack[i] = cos(stack[i]);
				break;
			case 'T':	// TAN
				stack[i] = tan(stack[i]);
				break;
			case 's':	// inverse SIN
				stack[i] = asin(stack[i]);
				break;
			case 'c':	// inverse COS
				stack[i] = acos(stack[i]);
				break;
			case 't':	// inverse TAN  (using previous 2 numbers to get full directional angle)
				stack[i] = atan2(stack[i - 1], stack[i]);
				i--;
				break;
			case 'L':	// Log10
				stack[i] = log10(stack[i]);
				break;
			case 'l':	// Log10
				stack[i] = log(stack[i]);
				break;
			}
		}
		if (i < 0) return 1;
	}
	
	*Output = stack[i];						// write the output
	return 0;							// return no errors
}