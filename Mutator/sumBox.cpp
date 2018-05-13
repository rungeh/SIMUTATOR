#include "sumBox.h"

sumBox::sumBox(string s, int sum) {
	this->signInputs = s;
	this->sumFound = sum;
};
sumBox::sumBox(string s, int sum, int cpys) {
	this->signInputs = s;
	this->sumFound = sum;
	this->sumCopys = cpys;
};
sumBox::~sumBox() {};
int sumBox::getFound() {
	return this->sumFound;
}
string sumBox::getSigns() {
	return this->signInputs;
}
int sumBox::getCopys() {
	return this->signInputs.length();
}
int sumBox::getCopys2() {
	return this->sumCopys;
}
void sumBox::setFound(int sf) {
	this->sumFound = sf;
}
void sumBox::setSigns(string s) {
	this->signInputs = s;
}
void sumBox::setCopys(int cps) {
	this->sumCopys = cps;
}