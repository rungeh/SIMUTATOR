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
class rgxList
{
	regex blckRegx;
	regex srchRegx;
	regex nameRegx;
public:
	rgxList();
	~rgxList();
	regex getBlock();
	regex getSearch();
	regex getName();
	void setBlock(regex s);
	void setSearch(regex s);
	void setName(regex s);
};

