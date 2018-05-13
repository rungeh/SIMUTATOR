#include "strList.h"

strList::strList(string str, int i) {
	this->strL = str;
	this->inst = i;
}
strList::~strList() {};
string strList::getString() {
	return this->strL;
}
void strList::setString(string s) {
	this->strL = s;
}
int strList::getInst() {
	return this->inst;
}