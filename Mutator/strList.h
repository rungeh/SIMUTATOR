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
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

#pragma once
class strList
{
	int inst;
	string strL;
public:
	strList(string str, int i);
	~strList();
	string getString();
	void setString(string s);
	int getInst();
};

