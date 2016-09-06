/*
  o------------------------------o
  | · Caleb Johnson ·            |
  | 7 / 5 / 2012                 |
  | version  1.0.01              |
  o------------------------------o
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>



using namespace std;

class word 
{
	public:
	float value;
	char op;
	int type;

};

	bool operator==(word& x, word& y);
	bool operator==(word& x, char c);	
	bool operator!=(word& x, char c);

class cWordlist
{
	public:
		vector< word > words;
	
		void Add(float v);
		void Add(char c);
		
		void Insert(int i, float v);
		void Insert(int i, char c);
		void Erase(int i);
		
		int size();
		void clear();
		
		bool isnum(int i);
		bool isop(int i);
		
		word& operator[](int i);

};




void cWordlist::Add(float v)
{
	word w;
	w.type = 1;
	w.value = v;
	words.push_back(w);
}

void cWordlist::Add(char c)
{
	word w;
	w.type = 2;
	w.op = c;
	words.push_back(w);
}

void cWordlist::Insert(int i, float v)
{
	word w;
	w.type = 1;
	w.value = v;
	words.insert(words.begin() + i, w);
}

void cWordlist::Insert(int i, char c)
{
	word w;
	w.type = 2;
	w.op = c;
	words.insert(words.begin() + i, w);
}
void cWordlist::Erase(int i)
{
	words.erase(words.begin() + i);
}
int cWordlist::size()
{
	return words.size();
}

void cWordlist::clear()
{
	words.clear();
}


bool cWordlist::isnum(int i)
{
	if (words[i].type == 1) return true;
	else return false;
}

bool cWordlist::isop(int i)
{
	if (words[i].type == 2) return true;
	else return false;
}

word& cWordlist::operator[](int i)
{
	return words[i];
}

bool operator==(word& x, word& y)
{
	if (x.type == y.type && x.type == 1 && x.value == y.value
	 || x.type == y.type && x.type == 2 && x.op == y.op) return true;
	 else return false;
}

bool operator==(word& x, char c)
{
	if (x.type == 2 && x.op == c) return true;
	else return false;
}

bool operator!=(word& x, char c)
{
	if (x.type == 2 && x.op != c) return true;
	else return false;
}