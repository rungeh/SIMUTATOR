#include "mutator.h"

bool has_suffix(const string& s, const string& suffix)
{
	return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

int putInList(string dPath, list <string> &dList) {
	string frmt = ".txt";
	DIR *dir = opendir(dPath.c_str());
	if (!dir) {
		return 1;
	}

	dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (has_suffix(entry->d_name, frmt)) {
			dList.push_back(dPath + "\\" + entry->d_name);
		}
	}
	closedir(dir);
	return 0;
}

int findInFile(string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	ifstream readFile(dFile);
	bool correct, crctName; 
	correct = 0; crctName = 0;
	int instances, start, end;
	instances = 0; start = 0; end = 0;
	string tmpRead, tmpRead2, subString;
	regex blckStrt("\\s*?Block\\s+?\\{");
	regex endSign("\\s*?\\}\\s*?");
	while (getline(readFile, readout)) {
		tmpRead = readout;
		tmpRead2 = readout;
		if (regex_match(readout, blckStrt)) {
			getline(readFile, tmpRead);
			if (tmpRead == "") getline(readFile, tmpRead);
			if (regex_match(tmpRead, regxs.getBlock())) {
				getline(readFile, tmpRead);
				if (tmpRead == "") {
					getline(readFile, tmpRead);
				}
				if (regex_match(tmpRead, regxs.getName())) {
					if (blck != "Sum2Product" && blck != "Product2Sum" && blck != "Remove Abs") {
						while (!(regex_match(tmpRead2, regxs.getSearch()))) {
							getline(readFile, tmpRead2);
						}
					}
					instances++;
					while (!(regex_match(tmpRead2, endSign))) {
						getline(readFile, tmpRead2);
					}
				}
			}
		}
	}
	return instances;
}

list <sumBox> findSumBlock(string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	list <sumBox> sumBoxList;
	ifstream readFile(dFile);
	regex signRegx("[+-]{2,4}");
	regex endSign("\\s*?\\}\\s*?");
	regex blckStrt("\\s*?Block\\s+?\\{");
	string lenStr, tmpRead, tmpRead2, subString;
	smatch sm;
	int instCount, lenInt;
	instCount = 0;
	bool correct, crctName = 0;
	while (getline(readFile, readout)) {
		tmpRead = readout;
		tmpRead2 = readout;
		if (regex_match(readout, blckStrt)) {
			getline(readFile, tmpRead);
			if (tmpRead == "") getline(readFile, tmpRead);
			if (regex_match(tmpRead, regxs.getBlock())) {
				getline(readFile, tmpRead);
				if (tmpRead == "") {
					getline(readFile, tmpRead);
				}
				if (regex_match(tmpRead, regxs.getName())) {
					while (!(regex_match(tmpRead2, regxs.getSearch()))) {
						getline(readFile, tmpRead2);
					}
					if ((blck == "Sum")) {
						regex_search(tmpRead2, sm, signRegx);
						lenStr = sm[0];
						lenInt = lenStr.length();
						instCount += lenInt;
						sumBoxList.push_back(sumBox(lenStr, lenInt));
					}
					while (!(regex_match(tmpRead2, endSign))) {
						getline(readFile, tmpRead2);
					}
				}
			}
		}
	}
	return sumBoxList;
}

list <sumBox> findRelBlock(string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	list <sumBox> sumBoxList;
	ifstream readFile(dFile);
	regex signRegx("[=~<>]{1,2}");
	regex endSign("\\s*?\\}\\s*?");
	regex blckStrt("\\s*?Block\\s+?\\{");
	string lenStr, tmpRead, tmpRead2, subString;
	smatch sm;
	int instCount, lenInt;
	lenInt = 5;
	instCount = 0;
	bool correct, crctName = 0;
	while (getline(readFile, readout)) {
		tmpRead = readout;
		tmpRead2 = readout;
		if (regex_match(readout, blckStrt)) {
			getline(readFile, tmpRead);
			if (tmpRead == "") getline(readFile, tmpRead);
			if (regex_match(tmpRead, regxs.getBlock())) {
				getline(readFile, tmpRead);
				if (tmpRead == "") {
					getline(readFile, tmpRead);
				}
				if (regex_match(tmpRead, regxs.getName())) {
					while (!(regex_match(tmpRead2, regxs.getSearch()))) {
						getline(readFile, tmpRead2);
					}
					if ((blck == "RelationalOperator")) {
						regex_search(tmpRead2, sm, signRegx);
						lenStr = sm[0];
						lenInt = lenStr.length();
						instCount += lenInt;
						sumBoxList.push_back(sumBox(lenStr, lenInt, 5));
					}
					while (!(regex_match(tmpRead2, endSign))) {
						getline(readFile, tmpRead2);
					}
				}
			}
		}
	}
	return sumBoxList;
}

string intToString(int intChange) {
	string out;
	ostringstream toStr;
	toStr.str("");
	toStr.clear();
	toStr << intChange;
	out = toStr.str();
	return out;
}

string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void cpyFiles(int mFound, string path, string halfpath, string currMut) {
	string conv, newPath, mPath;
	mPath = halfpath + "\\" + "M";
	for (int i = 1; i < mFound + 1; i++) {
		conv = intToString(i);
		newPath = mPath + conv;
		if (!fs::exists(halfpath)) { // Check if src folder exists
			fs::create_directory(halfpath); // create src folder
			fs::copy(path, newPath, fs::copy_options::recursive);
		}
		else
			fs::copy(path, newPath, fs::copy_options::recursive);
	}
}

int mutateGain(list <string> &AbsList, ofstream &readMeOut, int counter, string rplcStr, smatch sm, int rplcInt, ofstream &outFile, string readout, regex numRegx, string instStr, int inst, string maxInstStr, int maxInstances, string fileN, string searchStr, string blck, string replaceStr) {
	string::size_type o;
	string replaceStr2;
	switch (counter) {
		case 1: {
			AbsList.pop_back();
			replaceStr2 = std::regex_replace(readout, numRegx, replaceStr);
			AbsList.push_back(replaceStr2);
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
			break;
		}
		case 2: {
			AbsList.pop_back();
			rplcStr = sm[0];
			rplcInt = stoi(rplcStr, &o);
			replaceStr = intToString(rplcInt - 1);
			replaceStr2 = std::regex_replace(readout, numRegx, replaceStr);
			AbsList.push_back(replaceStr2);
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
			break;
		}
		case 3: {
			AbsList.pop_back();
			rplcStr = sm[0];
			rplcInt = stoi(rplcStr, &o);
			replaceStr = intToString(rplcInt + 1);
			replaceStr2 = std::regex_replace(readout, numRegx, replaceStr);
			AbsList.push_back(replaceStr2);
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
			counter = 0;
			break;
		}
		case 4: {
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "No mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " had no valid value(s) at instance " + instStr + "/" + maxInstStr + "." << endl;
			counter = 0;
			break;
		}
	}
	return counter;
}

string changeSign (string orgStr, int indx) {
	string modStr = orgStr;
	if (orgStr[indx] == '+') {
		modStr[indx] = '-';
		return modStr;
	}
	else if (orgStr[indx] == '-') {
		modStr[indx] = '+';
		return modStr;
	}
}

vector <string> signListGen(string orgStr) {
	vector <string> swpLst;
	string tmp;
	for (unsigned int i = 0; i < orgStr.length(); i++) {
		tmp = changeSign(orgStr, i);
		swpLst.push_back(tmp);
	}
	return swpLst;
}

vector <string> relListGen(string orgStr) {
	vector <string> swpLst;
	string tmp;
	if (orgStr == "==") {
		swpLst.insert(swpLst.end(), { "~=", "<", "<=", ">=", ">" });
	}
	else if (orgStr == "~=") {
		swpLst.insert(swpLst.end(), { "==", "<", "<=", ">=", ">" });
	}
	else if (orgStr == "<") {
		swpLst.insert(swpLst.end(), { "==", "~=", "<=", ">=", ">" });
	}
	else if (orgStr == "<=") {
		swpLst.insert(swpLst.end(), { "==", "~=", "<", ">=", ">" });
	}
	else if (orgStr == ">=") {
		swpLst.insert(swpLst.end(), { "==", "~=", "<", "<=", ">" });
	}
	else if (orgStr == ">") {
		swpLst.insert(swpLst.end(), { "==", "~=", "<", "<=", ">=" });
	}
	return swpLst;
}

int findInFileSum(string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	ifstream read(dFile);
	regex signRegx("[+-]{2,4}");
	string lenStr;
	smatch sm;
	int instCount, lenInt;
	instCount = 0;
	bool correct, crctName = 0;
	while (getline(read, readout)) {
		if (regex_match(readout, std::regex(regxs.getBlock()))) correct = 1;
		if (regex_match(readout, std::regex(regxs.getName()))) crctName = 1;
			if ((regex_match(readout, std::regex(regxs.getSearch())) && (correct == 1) && (crctName == 1))) { //\s+?Gain\s+?\".+?\"
				regex_search(readout, sm, signRegx);
				lenStr = sm[0];
				lenInt = lenStr.length();
				instCount += lenInt;
				correct = 0;
			}
	}
	return instCount;
}

string addQuotesToStr(string s) {
	return "\"" + s + "\"";
}

int mutateSum(list <string> &AbsList, vector <string> &signs, ofstream &readMeOut, string rplcStr, smatch sm, int rplcInt, ofstream &outFile, string readout, regex signRegx, string instStr, int inst,
	string maxInstStr, int maxInstances, string fileN, string searchStr, string blck) {
	string replaceStr2, vecString, newStr, rplcQuote;
	if (!(signs.empty()))
		vecString = signs.back();
	else
		return 1;
	signs.pop_back();

	AbsList.pop_back();
	newStr = std::regex_replace(readout, signRegx, vecString);
	AbsList.push_back(newStr);
	instStr = intToString(inst);
	maxInstStr = intToString(maxInstances);
	readMeOut << "A mutation of the file " + fileN + " was made." << endl;
	readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + vecString + " at instance " + instStr + "/" + maxInstStr + "." << endl;
}

void mutateBlockType(ofstream &readMeOut, string rplcStr, smatch sm, ofstream &outFile, string readout, regex typeRegx, string instStr, int inst, string maxInstStr, int maxInstances, string fileN, string searchStr) {
	outFile << std::regex_replace(readout, typeRegx, rplcStr) << endl;
	instStr = intToString(inst);
	maxInstStr = intToString(maxInstances);
	readMeOut << "A mutation of the file " + fileN + " was made." << endl;
	readMeOut << "The property " + searchStr + " in the block " + searchStr + " was changed to " + rplcStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
}

void mutateLine(list <string> &AbsList, ofstream &readMeOut, string rplcStr, ofstream &outFile, string readout, regex wordRgx, string instStr, int inst, string maxInstStr, int maxInstances, string fileN, string searchStr, string srcRplc, string dstRplc, string lineStr) {
	string newStr, srcRplcQuote;
	srcRplcQuote = addQuotesToStr(srcRplc);
	newStr = std::regex_replace(readout, wordRgx, srcRplcQuote);
	AbsList.push_back(newStr);
	instStr = intToString(inst);
	maxInstStr = intToString(maxInstances);
	readMeOut << "A mutation of the file " + fileN + " was made." << endl;
	readMeOut << "The property " + searchStr + " in the block " + "Line" + " was changed to " + srcRplc + " at instance " + instStr + "/" + maxInstStr + "." << endl;
}
string mutateFromOld(list <string> &AbsList, ofstream &readMeOut, ofstream &outFile, regex wordRgx, string instStr, int inst, string maxInstStr,
	int maxInstances, string fileN, string searchStr2, ifstream &readFile, int desired, string prvStr) {

	string readout, newStr, rplcQuote, tmpRead, tmpRead2, rplcStr;

	regex blckStrt("\\s*?Block\\s+?\\{");
	regex endSign("\\s+?\\}\\s*?");
	regex nameRegx("\\s*Name\\s*\"\\s*\\w.+?\\s*\"");
	regex blckRegx("\\s+?BlockType\\s+?Goto");
	regex gotoTagRegx("\\s*GotoTag\\s*\"\\s*\\w.+?\\s*\"");
	regex frmBlckRegx("\\s+?BlockType\\s+?From");

	bool changed = 0;
	int instances = 0; int frmInst = 0;
	smatch sm;


	while (getline(readFile, readout)) {
		tmpRead = readout;
		tmpRead2 = readout;
		if (regex_match(readout, blckStrt)) {
			AbsList.push_back(readout);
			getline(readFile, tmpRead);
			if (tmpRead == "") getline(readFile, tmpRead);
			AbsList.push_back(tmpRead);
			if (regex_match(tmpRead, blckRegx) && (!changed)) {
				getline(readFile, tmpRead);
				if (tmpRead == "") {
					getline(readFile, tmpRead);
				}
				AbsList.push_back(tmpRead);
				if (regex_match(tmpRead, nameRegx)) {
					instances++;
					if (instances == desired) {//!=
						if (instances > maxInstances) instances = 1;
						while (!(regex_match(tmpRead2, gotoTagRegx))) {
							getline(readFile, tmpRead2);
							if (tmpRead2 != "") AbsList.push_back(tmpRead2);
						}
						regex_search(tmpRead2, sm, wordRgx);
						rplcStr = sm[1];
					}
					while (!(regex_match(tmpRead2, endSign))) {
						getline(readFile, tmpRead2);
						if (tmpRead2 != "") AbsList.push_back(tmpRead2);
					}
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
				}
				else {
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
				}
			}
			else if (regex_match(tmpRead, frmBlckRegx) && (!changed)) {
				getline(readFile, tmpRead);
				if (tmpRead == "") getline(readFile, tmpRead);
				AbsList.push_back(tmpRead);
				if (regex_match(tmpRead, nameRegx)) {
					frmInst++;
					if (frmInst > maxInstances) frmInst = 1;
					if (!changed) {
						while (!(regex_match(tmpRead2, gotoTagRegx))) {
							getline(readFile, tmpRead2);
							if (tmpRead2 != "") AbsList.push_back(tmpRead2);
						}
						regex_search(tmpRead2, sm, wordRgx);
						if (sm[1] != rplcStr && sm[1] != prvStr) {
							prvStr = sm[1];
							AbsList.pop_back();
							rplcQuote = addQuotesToStr(rplcStr);
							newStr = std::regex_replace(tmpRead2, wordRgx, rplcQuote);
							AbsList.push_back(newStr);
							instStr = intToString(inst);
							maxInstStr = intToString(maxInstances);
							readMeOut << "A mutation of the file " + fileN + " was made." << endl;
							readMeOut << "The property GotoTag in the block " + searchStr2 + " was changed to " + rplcStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
							changed = 1;
						}
						while (!(regex_match(readout, endSign))) {
							getline(readFile, readout);
							if (readout != "") AbsList.push_back(readout);
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front(); //fixa ändringar i rätt line block
						}
					}
					else {
						while (!(regex_match(tmpRead2, endSign))) {
							getline(readFile, tmpRead2);
							if (tmpRead2 != "") AbsList.push_back(tmpRead2);
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front(); //fixa ändringar i rätt line block
						}
					}
				}
				else
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
			}
			else
				while (!(AbsList.empty())) {
					outFile << AbsList.front() << endl;
					AbsList.pop_front(); //fixa ändringar i rätt line block
				}
		}
		else if (readout != "")
			outFile << readout << endl;
		else
			outFile << readout;
	}
	return prvStr;
}

list <pair<string,string>> mutateFrom(list <string> &AbsList, ofstream &readMeOut, ofstream &outFile, regex wordRgx, string instStr, int inst, string maxInstStr,
	int maxInstances, string fileN, string searchStr2, ifstream &readFile, int desired, bool put, list <pair<string, string>> &chgList) {

	string readout, newStr, rplcQuote, tmpRead, tmpRead2, rplcStr, rplcStr2, prvDsr, nxtChg, tmpStr;

	regex blckStrt("\\s*?Block\\s+?\\{");
	regex endSign("\\s+?\\}\\s*?");
	regex nameRegx("\\s*Name\\s*\"\\s*\\w.+?\\s*\"");
	regex blckRegx("\\s+?BlockType\\s+?Goto");
	regex gotoTagRegx("\\s*GotoTag\\s*\"\\s*\\w.+?\\s*\"");
	regex frmBlckRegx("\\s+?BlockType\\s+?From");

	list <string> nameList;

	bool chng1, chng2;
	chng1 = 0; chng2 = 0;
	int instances = 0; int frmInst = 0; int counter = 0;
	smatch sm;

	if (!(chgList.empty())) {
		rplcStr = chgList.front().first;
		rplcStr2 = chgList.front().second;
		chgList.pop_front();
	}


	while (getline(readFile, readout)) {
		tmpRead = readout;
		tmpRead2 = readout;
		if (regex_match(readout, blckStrt)) {
			AbsList.push_back(readout);
			getline(readFile, tmpRead);
			if (tmpRead == "") getline(readFile, tmpRead);
			AbsList.push_back(tmpRead);
			if (regex_match(tmpRead, frmBlckRegx) && (chgList.empty())) {
				getline(readFile, tmpRead);
				if (tmpRead == "") {
					getline(readFile, tmpRead);
				}
				AbsList.push_back(tmpRead);
				if (regex_match(tmpRead, nameRegx)) {
					while (!(regex_match(tmpRead2, gotoTagRegx))) {
						getline(readFile, tmpRead2);
						if (tmpRead2 != "") AbsList.push_back(tmpRead2);
					}
					regex_search(tmpRead2, sm, wordRgx);
					nameList.push_back(sm[1]);
					while (!(regex_match(tmpRead2, endSign))) {
						getline(readFile, tmpRead2);
						if (tmpRead2 != "") AbsList.push_back(tmpRead2);
					}
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
				}
				else {
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
				}
			}
			else if (regex_match(tmpRead, blckRegx) && ((!chng1) || (!chng2))) {
				getline(readFile, tmpRead);
				if (tmpRead == "") getline(readFile, tmpRead);
				AbsList.push_back(tmpRead);
				if (regex_match(tmpRead, nameRegx)) { //kommer in med tom nameList i.e. försöker ta från tom chgList
					if (!(put)) {
						for (list <string>::const_iterator iter1 = nameList.begin(); iter1 != nameList.end(); ++iter1) {
							for (list <string>::const_iterator iter2 = iter1; ++iter2 != nameList.end();) {
								{
									chgList.push_back(make_pair(*iter1, *iter2));
								}
							}
						}
						put = 1;
						rplcStr = chgList.front().first;
						rplcStr2 = chgList.front().second;
						chgList.pop_front();
					}
					frmInst++;
					if (frmInst > maxInstances) frmInst = 1;
					if ((rplcStr != "") && (rplcStr2 != "")) {
						while (!(regex_match(tmpRead2, gotoTagRegx))) {
							getline(readFile, tmpRead2);
							if (tmpRead2 != "") AbsList.push_back(tmpRead2);
						}
						regex_search(tmpRead2, sm, wordRgx);
						if ((sm[1] != rplcStr && sm[1] == rplcStr2)) {
							AbsList.pop_back();
							rplcQuote = addQuotesToStr(rplcStr);
							newStr = std::regex_replace(tmpRead2, wordRgx, rplcQuote);
							AbsList.push_back(newStr);
							instStr = intToString(inst);
							maxInstStr = intToString(maxInstances);
							readMeOut << "A mutation of the file " + fileN + " was made." << endl;
							readMeOut << "The property GotoTag in the block " + searchStr2 + " was changed to " + rplcStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
							chng1 = 1;
						}
						else if ((sm[1] == rplcStr && sm[1] != rplcStr2)) {
							AbsList.pop_back();
							rplcQuote = addQuotesToStr(rplcStr2);
							newStr = std::regex_replace(tmpRead2, wordRgx, rplcQuote);
							AbsList.push_back(newStr);
							instStr = intToString(inst);
							maxInstStr = intToString(maxInstances);
							readMeOut << "A mutation of the file " + fileN + " was made." << endl;
							readMeOut << "The property GotoTag in the block " + searchStr2 + " was changed to " + rplcStr2 + " at instance " + instStr + "/" + maxInstStr + "." << endl;
							chng2 = 1;
						}
						while (!(regex_match(readout, endSign))) {
							getline(readFile, readout);
							if (readout != "") AbsList.push_back(readout);
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front(); //fixa ändringar i rätt line block
						}
					}
					else {
						while (!(regex_match(tmpRead2, endSign))) {
							getline(readFile, tmpRead2);
							if (tmpRead2 != "") AbsList.push_back(tmpRead2);
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front(); //fixa ändringar i rätt line block
						}
					}
				}
				else
					while (!(AbsList.empty())) {
						outFile << AbsList.front() << endl;
						AbsList.pop_front(); //fixa ändringar i rätt line block
					}
			}
			else
				while (!(AbsList.empty())) {
					outFile << AbsList.front() << endl;
					AbsList.pop_front(); //fixa ändringar i rätt line block
				}
		}
		else if (readout != "")
			outFile << readout << endl;
		else
			outFile << readout;
	}
	return chgList;
}

long long mutateFiles(string filePathFrm, string filePathTo, string currMut) {
	int desired, instances, start, end, maxInstances, appendName, chk, found, mFound, inst, counter,
		rplcInt, mutMult, currNum, errChk, numPop, numExtra, frmInst, sumSize, numFileMut;

	list <strList> paths;
	list <strListv2> readMeList;
	list <string> files, files2, extra, cmrd;
	string readout, searchStr, replaceStr, blck, subString, srch, rplc, fileN, instStr, maxInstStr, conv, replaceStr2, rplcStr,
		signRmvStr, mutStr, tmpRead, tmpRead2, srcRplc, dstRplc, absChk, lineStr, tmpSrc, searchStr2, prvStr, tmpFile;

	list <sumBox> sumList;
	list <string> AbsList;
	list <pair<string, string>> chgList;

	regex prdRgx("[*\/]{1,2}");
	regex inputRegx("\\s+?Inputs\\s+?\".+?\"");
	regex blckStrt("\\s*?Block\\s+?\\{");
	regex lineStrt("\\s*?Line\\s+?\\{");
	regex brchStrt("\\s*?Branch\\s+?\\{");
	regex endSign("\\s*?\\}\\s*?");
	regex numRegx("\\d+");
	regex signRegx("[+-]{2,4}");
	regex wordRgx("\"(\\w+)\"");
	regex sumRgx("Sum");
	regex prodRgx("Product");
	regex nameRegx("\\s*Name\\s*\"\\s*\\w.+?\\s*\"");
	regex blckRegx, srchRegx, lineSrcRegx, lineDstRegx, gotoTagRegx, frmBlckRegx;
	smatch sm;
	vector <string> signs, signs3, signs4;
	rgxList Rgx;

	//string ending = ".txt";
	string ending = ".mdl";

	start = 0;
	end = 0;
	instances = 0;
	frmInst = 0;
	desired = 1;
	appendName = 1;
	chk = 0;
	counter = 0;
	numPop = 1;
	rplcInt = 0;
	numExtra = 1;

	bool changed = 0; bool put = 0;
	bool correct, crctName, lineCrct, crctBlck, dstChk = 0;
	mutStr = currMut;
	replaceStr = "1";

	if (mutStr == "Gain") {
		searchStr = "Gain";
		blckRegx = "\\s+?BlockType\\s+?Gain";
		srchRegx = "\\s+?Gain\\s+?\".+?\"";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
		mutMult = 3;
	}
	else if (mutStr == "Sum" || mutStr == "Sum2Add" || mutStr == "Sum2Subtract") {
		searchStr = "Inputs";
		blckRegx = "\\s+?BlockType\\s+?Sum";
		srchRegx = "\\s+?Inputs\\s+?\".+?\"";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
		mutMult = 3;
		currNum = 1;
		numFileMut = 1;
	}
	else if (mutStr == "Sum2Product") {
		searchStr = "Sum";
		blckRegx = "\\s+?BlockType\\s+?Sum";
		srchRegx = blckRegx;
		rplcStr = "Product";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
	}
	else if (mutStr == "Product2Sum") {
		searchStr = "Product";
		blckRegx = "\\s+?BlockType\\s+?Product";
		srchRegx = blckRegx;
		rplcStr = "Sum";
		sumRgx = searchStr;
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
	}
	else if (mutStr == "Remove Abs") {
		searchStr = "Abs";
		blckRegx = "\\s+?BlockType\\s+?Abs";
		srchRegx = blckRegx;
		lineSrcRegx = "\\s*SrcBlock\\s*\"\\s*\\w.+?\\s*\"";
		lineDstRegx = "\\s*DstBlock\\s*\"\\s*\\w.+?\\s*\"";
		lineStr = "Line";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
	}
	else if (mutStr == "GotoFrom") {
		searchStr = "Goto";
		searchStr2 = "From";
		blckRegx = "\\s+?BlockType\\s+?Goto";
		frmBlckRegx = "\\s+?BlockType\\s+?From";
		srchRegx = blckRegx;
		gotoTagRegx = "\\s*GotoTag\\s*\"\\s*\\w.+?\\s*\"";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
	}
	else if (mutStr == "RelationalOperator") {
		searchStr = "Operator";
		blckRegx = "\\s+?BlockType\\s+?RelationalOperator";
		srchRegx = "\\s+?Operator\\s+?\".+?\"";
		signRegx = "[=~<>]{1,2}";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
		currNum = 1;
		numFileMut = 1;
	}

	ostringstream convert, cnvrt, convMax;
	string path = filePathFrm;
	string halfpath = filePathTo;
	string mPath = halfpath + "\\" + "M";

	mFound = 0;
	DIR *dir = opendir(path.c_str());
	if (!dir) {
		return 1;
	}

	dirent *entry; //find files in the specified inpath which have the correct suffix
	while ((entry = readdir(dir)) != NULL) {
		if (has_suffix(entry->d_name, ending)) {
			files.push_back(path + "\\" + entry->d_name);
			found = 0;
			if (mutStr == "Gain") { //based on mutation, the adding to the readMe and path-lists work differently
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				if (found >= 1) {
					for (int j = 0; j < (mutMult*found) - 1; j++) { files.push_back(files.back()); }
					for (int i = 1; i < (mutMult*found) + 1; i++) {
						if (i == 1) currNum = i;
						mFound++;
						rplc = intToString(mFound);
						paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, currNum));
						readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, currNum, found));
						if (i%mutMult == 0) currNum++;
					}
				}
				else
					files.pop_back();
			}
			else if (mutStr == "Sum") {
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				tmpFile = files.back();
				files.pop_back();
				if (found >= 1) {
					sumList = findSumBlock(readout, tmpFile, searchStr, mutStr, Rgx);
					sumSize = sumList.size();
					for (unsigned int i = 0; i < sumSize; i++) {
						for (int j = 0; j < (sumList.front().getCopys()); j++) { files.push_back(tmpFile); }
						for (int l = 1; l < (sumList.front().getCopys()) + 1; l++) {
							mFound++;
							rplc = intToString(mFound);
							paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, numFileMut));
							readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, numFileMut, found));
							if (l%sumList.front().getCopys() == 0) {
								numFileMut++;
							}
						}
						sumList.pop_front();
					}
					numFileMut = 1;
				}
			}
			else if (mutStr == "RelationalOperator") {
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				tmpFile = files.back();
				files.pop_back();
				if (found >= 1) {
					sumList = findRelBlock(readout, tmpFile, searchStr, mutStr, Rgx);
					sumSize = sumList.size();
					for (unsigned int i = 0; i < sumSize; i++) {
						for (int j = 0; j < (sumList.front().getCopys2()); j++) { files.push_back(tmpFile); }
						for (int l = 1; l < (sumList.front().getCopys2()) + 1; l++) {
							mFound++;
							rplc = intToString(mFound);
							paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, numFileMut));
							readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, numFileMut, found));
							if (l%sumList.front().getCopys2() == 0) {
								numFileMut++;
							}
						}
						sumList.pop_front();
					}
					numFileMut = 1;
				}
			}
			else if (mutStr == "Sum2Product" || mutStr == "Product2Sum" || mutStr == "Remove Abs" || mutStr == "Sum2Subtract" || mutStr == "Sum2Add") { //for these however, the work the same
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				if (found >= 1) {
					for (int j = 0; j < (found)-1; j++) { files.push_back(files.back()); }
					for (int i = 1; i < (found)+1; i++) {
						if (i == 1) currNum = i;
						mFound++;
						rplc = intToString(mFound);
						paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, currNum));
						readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, currNum, found));
						currNum++;
					}
				}
				else
					files.pop_back();
				found = 0;
			}
			else if (mutStr == "GotoFrom") { //for these however, the work the same
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				//(numOfPairs^2 - numOfPairs)/2
				numExtra = (pow(found,2) - found) / 2;
				if (found >= 1) {
					for (int j = 0; j < (numExtra) - 1; j++) { files.push_back(files.back()); }
					for (int i = 1; i < (numExtra) + 1; i++) {
						if (i == 1) currNum = i;
						else if (currNum > found) currNum = 1;
						mFound++;
						rplc = intToString(mFound);
						paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, currNum));
						readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, currNum, found));
						currNum++;
					}
				}
				else
					files.pop_back();
				found = 0;
			}
		}
	}
	closedir(dir);

	cpyFiles(mFound, path, halfpath, currMut); //copy all files to new folders and subfolders

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	while (!(paths.empty()) && (!(files.empty()))) {
		changed = 0; instances = 0; correct = 0; crctName = 0; inst = 0; maxInstances = 0; lineCrct = 0; //reset values

		ifstream readFile(files.front()); //read from the frontmost file
		files.pop_front();

		ofstream outFile(paths.front().getString()); //write to the frontmost file
		desired = paths.front().getInst();
		paths.pop_front();

		ofstream readMeOut(readMeList.front().getPath()); //setup the readme-file 
		inst = readMeList.front().getInst();
		fileN = readMeList.front().getName();
		maxInstances = readMeList.front().getMaxInst();
		readMeList.pop_front();

		if (mutStr == "Sum2Product" || mutStr == "Product2Sum") {
			while (getline(readFile, readout)) {
				if (!changed && regex_match(readout, blckRegx)) {
					getline(readFile, tmpRead);
					crctName = regex_match(tmpRead, nameRegx);
					changed = ((instances += crctName) == desired);

					if (changed) {
						regex_search(readout, sm, sumRgx);
						mutateBlockType(readMeOut, rplcStr, sm, outFile, readout, sumRgx, instStr, inst, maxInstStr,
							maxInstances, fileN, searchStr);
						outFile << tmpRead << endl;
						if (mutStr == "Product2Sum") {
							while (!(regex_match(tmpRead, endSign))) {
								getline(readFile, tmpRead);
								if (regex_match(tmpRead, inputRegx)) {
									tmpRead = std::regex_replace(tmpRead, prdRgx, "+-");
								}
								outFile << tmpRead << endl;
							}
						}
					}
					else {
						outFile << readout << endl;
						outFile << tmpRead << endl;
					}
				}
				else {
					outFile << readout;
					if (readout != "")
						outFile << endl;
				}
			}
		}
		else if (mutStr == "Gain" || mutStr == "Sum" || mutStr == "Sum2Subtract" || mutStr == "Sum2Add" || mutStr == "RelationalOperator") {
			while (getline(readFile, readout)) {
				tmpRead = readout;
				tmpRead2 = readout;
				if (regex_match(readout, blckStrt) && (!changed)) {
					AbsList.push_back(readout);
					getline(readFile, tmpRead);
					if (tmpRead == "") getline(readFile, tmpRead);
					AbsList.push_back(tmpRead);
					if (regex_match(tmpRead, blckRegx)) {
						getline(readFile, tmpRead);
						if (tmpRead == "") {
							getline(readFile, tmpRead);
						}
						AbsList.push_back(tmpRead);
						if (regex_match(tmpRead, nameRegx)) {
							while (!(regex_match(tmpRead2, srchRegx))) {
								getline(readFile, tmpRead2);
								if (tmpRead2 != "") AbsList.push_back(tmpRead2);
							}
							instances++;
							if (instances == desired) {
								if (mutStr == "Gain") {
									counter++;
									regex_search(tmpRead2, sm, numRegx);
									if (sm[0] != "") {
										counter = mutateGain(AbsList, readMeOut, counter, rplcStr, sm, rplcInt, outFile, tmpRead2, numRegx, instStr, inst, maxInstStr,
											maxInstances, fileN, searchStr, mutStr, replaceStr);
									}
									else {
										counter = 4;
										counter = mutateGain(AbsList, readMeOut, counter, rplcStr, sm, rplcInt, outFile, tmpRead2, numRegx, instStr, inst, maxInstStr,
											maxInstances, fileN, searchStr, mutStr, replaceStr);
									}
									changed = 1;
								}
								else if (mutStr == "Sum") {
									regex_search(tmpRead2, sm, signRegx);
									signRmvStr = sm[0];
									if (signs.empty()) signs = signListGen(signRmvStr);
									errChk = mutateSum(AbsList, signs, readMeOut, rplcStr, sm, rplcInt, outFile, tmpRead2, signRegx, instStr, inst, maxInstStr,
										maxInstances, fileN, searchStr, mutStr);

									if (errChk == 1) break;
									changed = 1;
								}
								else if (mutStr == "RelationalOperator") {
									regex_search(tmpRead2, sm, signRegx);
									signRmvStr = sm[0];
									if (signs.empty()) signs = relListGen(signRmvStr);
									errChk = mutateSum(AbsList, signs, readMeOut, rplcStr, sm, rplcInt, outFile, tmpRead2, signRegx, instStr, inst, maxInstStr,
										maxInstances, fileN, searchStr, mutStr);

									if (errChk == 1) break;
									changed = 1;
								}
								else if (mutStr == "Sum2Subtract") {
									regex_search(tmpRead2, sm, signRegx);
									signRmvStr = sm[0];
									if (signs.empty()) signs.push_back("+-");
									errChk = mutateSum(AbsList, signs, readMeOut, rplcStr, sm, rplcInt, outFile, tmpRead2, signRegx, instStr, inst, maxInstStr,
										maxInstances, fileN, searchStr, mutStr);

									if (errChk == 1) break;
									changed = 1;
								}
								else if (mutStr == "Sum2Add") {
									regex_search(tmpRead2, sm, signRegx);
									signRmvStr = sm[0];
									if (signs.empty()) signs.push_back("++");
									errChk = mutateSum(AbsList, signs, readMeOut, rplcStr, sm, rplcInt, outFile, tmpRead2, signRegx, instStr, inst, maxInstStr,
										maxInstances, fileN, searchStr, mutStr);

									if (errChk == 1) break;
									changed = 1;
								}
							}
							while (!(regex_match(tmpRead2, endSign))) {
								getline(readFile, tmpRead2);
								if (tmpRead2 != "") AbsList.push_back(tmpRead2);
							}
							while (!(AbsList.empty())) {
								outFile << AbsList.front() << endl;
								AbsList.pop_front();
							}
						}
						else {
							while (!(AbsList.empty())) {
								outFile << AbsList.front() << endl;
								AbsList.pop_front();
							}
						}
					}
					else
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front();
						}
				}
				else if (readout != "")
					outFile << readout << endl;
				else
					outFile << readout;
			}
		}
		else if (mutStr == "Remove Abs") {
			while (getline(readFile, readout)) {
				if (regex_match(readout, blckStrt)) {
					getline(readFile, tmpRead);
					if (regex_match(tmpRead, blckRegx)) {
						crctBlck = 1;
						getline(readFile, tmpRead2);
						if (regex_match(tmpRead2, nameRegx)) {
							crctName = 1;
							regex_search(tmpRead2, sm, wordRgx);
							srcRplc = sm[1]; //alt sm[0]
							instances++;
							if ((!changed) && (instances == desired)) {
								while (!(regex_match(readout, endSign))) {
									getline(readFile, readout); //move past/remove the block
								}
							}
						}
						else if (readout != "") {
							outFile << readout << endl;
							if (tmpRead != "") outFile << tmpRead << endl;
							if (tmpRead2 != "") outFile << tmpRead2 << endl;
						}
					}
					else if (readout != "") {
						outFile << readout << endl;
						if (tmpRead != "") outFile << tmpRead << endl;
					}
				}
				else if (regex_match(readout, lineStrt)) {
					tmpRead = readout;
					tmpRead2 = readout;
					while (!(regex_match(tmpRead, lineSrcRegx))) {
						AbsList.push_back(tmpRead);
						getline(readFile, tmpRead);
					}
					AbsList.push_back(tmpRead);
					regex_search(tmpRead, sm, wordRgx);
					if (sm[1] != searchStr) {
						tmpSrc = sm[1];
						while (getline(readFile, tmpRead)) {
							AbsList.push_back(tmpRead);
							if (regex_match(tmpRead, brchStrt)) {
								while (!(regex_match(tmpRead2, lineDstRegx))) {
									getline(readFile, tmpRead2);
									AbsList.push_back(tmpRead2);
								}
								regex_search(tmpRead2, sm, wordRgx);
								if (sm[1] == searchStr) {
									srcRplc = tmpSrc;
									while (!(regex_match(readout, endSign))) {
										getline(readFile, readout);
										AbsList.pop_back();
									}
									break;
								}
								else {
									while (!(regex_match(readout, endSign))) {
										getline(readFile, readout);
										AbsList.push_back(readout);
									}
								}
							}
							else if (regex_match(tmpRead, lineDstRegx)) {
								regex_search(tmpRead, sm, wordRgx);
								if (sm[1] == searchStr) {
									srcRplc = tmpSrc;
									readout = tmpRead;
									while (!(regex_match(readout, endSign))) {
										getline(readFile, readout);
									}
									if ((AbsList.size() == 4) || (AbsList.size() == 5)) numPop = 2;
									for (int i = 0; i < (numPop * 2); i++) {
										AbsList.pop_back();
									}
									break;
								}
								else if (sm[1] != searchStr) {
									while (!(regex_match(readout, endSign))) {
										getline(readFile, readout);
										AbsList.push_back(readout);
									}
									break;
								}
							}
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front();
						}
					}
					else if (sm[1] == searchStr) {
						AbsList.pop_back();
						mutateLine(AbsList, readMeOut, rplcStr, outFile, tmpRead, wordRgx, instStr, inst, maxInstStr,
							maxInstances, fileN, searchStr, srcRplc, dstRplc, lineStr);
						while (!(regex_match(readout, endSign))) {
							getline(readFile, readout);
							AbsList.push_back(readout);
						}
						while (!(AbsList.empty())) {
							outFile << AbsList.front() << endl;
							AbsList.pop_front();
						}
					}
				}
				else if (readout != "")
					outFile << readout << endl;
				else
					outFile << readout;
			}
		}
		else if (mutStr == "GotoFrom") { 
			if (!(chgList.empty())) put = 1;
			chgList = mutateFrom(AbsList, readMeOut, outFile, wordRgx, instStr, inst, maxInstStr,
				maxInstances, fileN, searchStr2, readFile, desired, put, chgList);
			}
		}
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		return duration;
	}

[STAThread]
int main(void) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Mutator::MyForm form;
	Application::Run(%form);
}