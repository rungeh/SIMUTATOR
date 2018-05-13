#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <regex>
#include <iterator>
#include <sstream> 
#include <experimental/filesystem>
//#include <filesystem>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

#pragma once
class strListv2
{
	int inst;
	int maxInst;
	string strL;
	string strN;
public:
	strListv2(string str, string file, int i, int m);
	~strListv2();
	string getPath();
	string getName();
	void setPath(string s);
	void setName(string s);
	int getInst();
	int getMaxInst();
};

