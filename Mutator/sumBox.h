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
class sumBox
{
	int sumFound;
	int sumCopys;
	string signInputs;
public:
	sumBox(string s, int sum);
	sumBox(string s, int sum, int cpys);
	~sumBox();
	int getFound();
	string getSigns();
	int getCopys();
	int getCopys2();
	void setFound(int sf);
	void setSigns(string s);
	void setCopys(int cps);
};

