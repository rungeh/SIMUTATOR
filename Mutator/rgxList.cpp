#include "rgxList.h"

rgxList::rgxList() {};
rgxList::~rgxList() {};
regex rgxList::getBlock() {
	return this->blckRegx;
}
regex rgxList::getSearch() {
	return this->srchRegx;
}
regex rgxList::getName() {
	return this->nameRegx;
}
void rgxList::setBlock(regex s) {
	this->blckRegx = s;
}
void rgxList::setSearch(regex s) {
	this->srchRegx = s;
}
void rgxList::setName(regex s) {
	this->nameRegx = s;
}