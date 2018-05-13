#include "strListv2.h"

strListv2::strListv2(string str, string file, int i, int m) {
	this->strL = str;
	this->strN = file;
	this->inst = i;
	this->maxInst = m;
};
strListv2::~strListv2() {};
string strListv2::getPath() {
	return this->strL;
}
string strListv2::getName() {
	return this->strN;
}
void strListv2::setPath(string s) {
	this->strL = s;
}
void strListv2::setName(string s) {
	this->strN = s;
}
int strListv2::getInst() {
	return this->inst;
}
int strListv2::getMaxInst() {
	return this->maxInst;
}