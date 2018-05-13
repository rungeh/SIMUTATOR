#include "MyForm.h"

list <sumBox> findSumBlock (string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	string::size_type m, n;
	list <sumBox> sumBoxList;
	ifstream read(dFile);
	regex signRegx("[+-]{2,4}");
	regex endSign("\\s+?\\}\\s*?");
	string lenStr;
	smatch sm;
	int instCount, start, end, lenInt;
	instCount = 0;
	bool correct, crctName = 0;
	while (getline(read, readout)) {
		if (regex_match(readout, endSign)) { correct = 0; crctName = 0; }
		if (regex_match(readout, std::regex(regxs.getBlock()))) correct = 1;
		if (regex_match(readout, std::regex(regxs.getName()))) crctName = 1;
		if ((regex_match(readout, std::regex(regxs.getSearch())) && (correct == 1) && (crctName == 1))) { //\s+?Gain\s+?\".+?\"
				regex_search(readout, sm, signRegx);
				lenStr = sm[0];
				lenInt = lenStr.length();
				instCount += lenInt;
				sumBoxList.push_back(sumBox(lenStr, lenInt));
				correct = 0;
			}
	}
	return sumBoxList;
}

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
	string::size_type m, n;
	ifstream read(dFile);
	string subString;
	regex endSign("\\s+?\\}\\s*?");
	int instances, start, end;
	instances = 0;
	bool correct, crctName = 0;
	while (getline(read, readout)) {
		if (regex_match(readout, endSign)) { correct = 0; crctName = 0; }
		if (regex_match(readout, std::regex(regxs.getBlock()))) correct = 1;
		if (regex_match(readout, std::regex(regxs.getName()))) crctName = 1;
		if ((regex_match(readout, std::regex(regxs.getSearch())) && (correct == 1) && (crctName == 1))) { //\s+?Gain\s+?\".+?\"
			subString = readout.substr(start, end - start - 1);
			instances++;
			correct = 0;
			crctName = 0;
		}
	}
	return instances;
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

//void cpyFiles(int mFound, string path, string mPath) {
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

int mutateGain(ofstream &readMeOut, int counter, string rplcStr, smatch sm, int rplcInt, ofstream &outFile, string readout, regex numRegx, string instStr, int inst, string maxInstStr, int maxInstances, string fileN, string searchStr, string blck, string replaceStr) {
	string::size_type o;
	string replaceStr2;
	switch (counter) { //for Gain, perhaps Sum
		case 1: {
			outFile << std::regex_replace(readout, numRegx, replaceStr) << endl;
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr + " at instance " + instStr + "/" + maxInstStr + "." << endl;
			break;
		}
		case 2: {
			rplcStr = sm[0];
			rplcInt = stoi(rplcStr, &o);
			replaceStr2 = intToString(rplcInt - 1);
			outFile << std::regex_replace(readout, numRegx, replaceStr2) << endl;
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr2 + " at instance " + instStr + "/" + maxInstStr + "." << endl;
			break;
		}
		case 3: {
			rplcStr = sm[0];
			rplcInt = stoi(rplcStr, &o);
			replaceStr2 = intToString(rplcInt + 1);
			outFile << std::regex_replace(readout, numRegx, replaceStr2) << endl;
			instStr = intToString(inst);
			maxInstStr = intToString(maxInstances);
			readMeOut << "A mutation of the file " + fileN + " was made." << endl;
			readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + replaceStr2 + " at instance " + instStr + "/" + maxInstStr + "." << endl;
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
	for (int i = 0; i < orgStr.length(); i++) {
		tmp = changeSign(orgStr, i);
		swpLst.push_back(tmp);
	}
	return swpLst;
}

int findInFileSum(string readout, string dFile, string searchStr, string blck, rgxList regxs) {
	string::size_type m, n;
	ifstream read(dFile);
	regex signRegx("[+-]{2,4}");
	string lenStr;
	smatch sm;
	int instCount, start, end, lenInt;
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

int mutateSum(vector <string> &signs, ofstream &readMeOut, string rplcStr, smatch sm, int rplcInt, ofstream &outFile, string readout, regex signRegx, string instStr, int inst, string maxInstStr, int maxInstances, string fileN, string searchStr, string blck) {
	string::size_type o;
	int signNum;
	string replaceStr2, vecString;
	if (!(signs.empty()))
		vecString = signs.back();
	else
		return 1;
	signs.pop_back();

	outFile << std::regex_replace(readout, signRegx, vecString) << endl;
	instStr = intToString(inst);
	maxInstStr = intToString(maxInstances);
	readMeOut << "A mutation of the file " + fileN + " was made." << endl;
	readMeOut << "The property " + searchStr + " in the block " + blck + " was changed to " + vecString + " at instance " + instStr + "/" + maxInstStr + "." << endl;
}

int mutateFiles(string filePathFrm, string filePathTo, string currMut) {
	//send in which block is to be mutated
	int desired, instances, start, end, maxInstances, appendName, chk, found, mFound, inst, counter, 
		rplcInt, mutMult, currNum, errChk, signRmvInt, sumNum;

	list <strList> paths;
	list <strListv2> readMeList;
	list <string> files, files2, extra, cmrd;
	string readout, searchStr, replaceStr, blck, subString, srch, rplc, fileN, instStr, maxInstStr, conv, replaceStr2, rplcStr, signRmvStr, mutStr;
	string::size_type m, n, o;

	list <sumBox> sumList;

	regex numRegx("\\d+");
	regex signRegx("[+-]{2,4}");
	regex blckRegx, srchRegx, nameRegx;
	smatch sm;
	vector <string> signs, signs3, signs4;
	rgxList Rgx;

	string ending = ".txt";
	//string ending = ".mdl";

	start, end = 0;
	instances = 0;
	desired = 1;
	appendName = 1;
	chk = 0;
	counter = 0;

	bool changed = 0;
	bool correct, crctName = 0;
	//blck = "Gain";
	//blck = "Sum";
	mutStr = currMut;
	//searchStr = "Gain";
	replaceStr = "1";

	if (mutStr == "Gain") {
		searchStr = "Gain";
		blckRegx = "\\s+?BlockType\\s+?Gain";
		srchRegx = "\\s+?Gain\\s+?\".+?\"";
		nameRegx = "\\s+?Name\\s+?\"\\w+\"";
		//nameRegx = "\\s+?Name\\s+?\"\\w+\"";
		//nameRegx = "Name";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
		mutMult = 3;
	}
	if (mutStr == "Sum") {
		searchStr = "Inputs";
		blckRegx = "\\s+?BlockType\\s+?Sum";
		srchRegx = "\\s+?Inputs\\s+?\".+?\"";
		//nameRegx = "\\s+?Name\\s+?\"\\w+\"";
		nameRegx = "\\s+?Name\\s+?\"\\w+\"";
		//nameRegx = "Name";
		Rgx.setBlock(blckRegx);
		Rgx.setSearch(srchRegx);
		Rgx.setName(nameRegx);
		mutMult = 3; //fixa för 3,4
		//signs.erase(std::remove(signs.begin(), signs.end(), "++"), signs.end());
		//unclear. ++, +-, -+, --
		//[+-]+
		//[+-]{2}
		//if --, replace with one of others etc
		//kolla i mutationtest
		//räknar eventuellt tex "++++" som två instanser
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

	dirent *entry; //ändra till findInFileSum för sum
	while ((entry = readdir(dir)) != NULL) {
		if (has_suffix(entry->d_name, ending)) {
			files.push_back(path + "\\" + entry->d_name);
			found = 0;
			if (mutStr == "Sum") {
				//sumNum = findInFileSum(readout, files.back(), searchStr, blck, Rgx);
				found = findInFile(readout, files.back(), searchStr, mutStr, Rgx);
				sumList = findSumBlock(readout, files.back(), searchStr, mutStr, Rgx);
				for (int i = 0; i < sumList.size(); i++) {
					if (sumList.back().getFound() >= 1) {
						for (int j = 0; j < (sumList.back().getCopys())-1; j++) { files.push_back(files.back()); }
						for (int l = 1; l < (sumList.back().getCopys())+1; l++) {
							if (l == 1) currNum = l;
							//if (i%sumNum == 0) currNum++;
							mFound++; //create object
							rplc = intToString(mFound);
							paths.push_back(strList(mPath + rplc + "\\" + entry->d_name, currNum));
							readMeList.push_back(strListv2(mPath + rplc + "\\readmeM" + rplc + ".txt", entry->d_name, currNum, found));
							//currNum++;
							if (l%sumList.back().getCopys() == 0) currNum++;
						}
					}
					else
						files.pop_back();
				}
			}
			else if (mutStr == "Gain") {
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
		}
	}
	closedir(dir);

	//cpyFiles(mFound, path, mPath);
	cpyFiles(mFound, path, halfpath, currMut);

	while (!(paths.empty()) && (!(files.empty()))) {
		changed = 0;
		instances = 0;
		correct = 0;
		crctName = 0;
		inst = 0;
		maxInstances = 0;

		ifstream readFile(files.front());
		files.pop_front();

		ofstream outFile(paths.front().getString());
		desired = paths.front().getInst();
		paths.pop_front();

		ofstream readMeOut(readMeList.front().getPath());
		inst = readMeList.front().getInst();
		fileN = readMeList.front().getName();
		maxInstances = readMeList.front().getMaxInst();
		readMeList.pop_front();

		while (getline(readFile, readout)) {
			//m = readout.find(blck);
			if (regex_match(readout, blckRegx)) correct = 1;
			if (regex_match(readout, nameRegx)) crctName = 1;
			if ((regex_match(readout, srchRegx)) && (correct == 1) && (crctName == 1)) instances++;
				if ((changed != 1) && (correct == 1) && (instances == desired)) {
					//outFile << readout.replace(start + 1, subString.length(), replaceStr) << endl;
					counter++;
					//regex_search(readout, sm, numRegx);
					if (mutStr == "Gain") {
						regex_search(readout, sm, numRegx);
						counter = mutateGain(readMeOut, counter, rplcStr, sm, rplcInt, outFile, readout, numRegx, instStr, inst, maxInstStr, 
							maxInstances, fileN, searchStr, mutStr, replaceStr);
						changed = 1;
						correct = 0;
					}
					else if (mutStr == "Sum") {
						regex_search(readout, sm, signRegx);
						signRmvStr = sm[0];
						if (signs.empty()) signs = signListGen(signRmvStr);
						errChk = mutateSum(signs, readMeOut, rplcStr, sm, rplcInt, outFile, readout, signRegx, instStr, inst, maxInstStr,
							maxInstances, fileN, searchStr, mutStr);
						
						if (errChk == 1) break;
						changed = 1;
						correct = 0;
					}
					//outFile << std::regex_replace(readout, numRegx, replaceStr) << endl;
				}
				else if (readout != "")
					outFile << readout << endl;
				else 
					outFile << readout;
			}
		}
	}
	//cout << "No blocks " + blck + " to mutate!" << endl;

[STAThread]
int main(void) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Mutator::MyForm form;
	Application::Run(%form);
}

