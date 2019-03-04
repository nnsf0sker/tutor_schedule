/*
 * schedule.h
 *
 *  Created on: 26 февр. 2019 г.
 *      Author: nnsf0sker
 */

// в fileInput дописать коды ошибок

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>


using namespace std;

string cutFirstWord(string &str) {
	int i;
	int wordEnd=0;

	while (wordEnd<str.length()) {
		if ((str[wordEnd]!=' ')&&(str[wordEnd]!='\n')) {
			wordEnd++;
		} else {
			break;
		}
	}

	string result;
	result.resize(wordEnd);
	for(i=0; i<wordEnd; i++) {
		result[i]=str[i];
	}

	str = str.erase(0, wordEnd+1);

	return result;
}

class Answer {
private:
	static unsigned int n;
	string childName_;
	string timePointId_;

public:
	void fill(const string newChildName, const string newTimePointId) {
		childName_.resize(newChildName.length());
		childName_ = newChildName;
		timePointId_.resize(newTimePointId.length());
		timePointId_ = newTimePointId;
		return;
	}

	void consoleOutput() {
		cout << childName_ << " " << timePointId_ << endl;
		return;
	}


};

class Schedule {

private:
	unsigned int M_; // количество возможных промежутков
	unsigned int N_; // количество детей
	int *a_; // массив, показывающий возможность занятия
	string *childId_;
	string *timePointId_;
	Answer *answers_;
	int nAnswers;

	void changeKLCols(const int k, const int l) {
		int i, tmp1;
		for (i=0; i<N_; i++) {
			tmp1 = a_[i*M_+k];
			a_[i*M_+k] = a_[i*M_+l];
			a_[i*M_+l] = tmp1;
		}

		string tmp2;
		int k_len = 0, l_len = 0;
		k_len = timePointId_[k].length();
		l_len = timePointId_[l].length();
		tmp2.resize(k_len);
		tmp2 = timePointId_[k];
		timePointId_[k].resize(l_len);
		timePointId_[k] = timePointId_[l];
		timePointId_[l].resize(k_len);
		timePointId_[l] = tmp2;

		return;
	}

	void changeKLRows(const int k, const int l) {
		int i, tmp1;
		for (i=0; i<M_; i++) {
			tmp1 = a_[k*M_+i];
			a_[k*M_+i] = a_[l*M_+i];
			a_[l*M_+i] = tmp1;
		}

		string tmp2;
		int k_len = 0, l_len = 0;
		k_len = childId_[k].length();
		l_len = childId_[l].length();
		tmp2.resize(k_len);
		tmp2 = childId_[k];
		childId_[k].resize(l_len);
		childId_[k] = childId_[l];
		childId_[l].resize(k_len);
		childId_[l] = tmp2;

		return;
	}

public:
	Schedule () {
		M_ = 0;
		N_ = 0;
		a_ = 0;
		childId_ = 0;
		timePointId_ = 0;
		answers_ = 0;
		nAnswers = 0;
	}

	~Schedule () {
		delete []a_;
		delete []childId_;
		delete []timePointId_;
		delete []answers_;
	}

	int fileInput(const char *filename) {
		ifstream file(filename);
		string tmp1, tmp2;
		string buff[1024];
		int buffLength = 0;
		int i;

		N_ = 0;
		while(getline(file, tmp1)) {
			cutFirstWord(tmp1);
			while(tmp1.length()>0) {
				tmp2 = cutFirstWord(tmp1);
				for(i=0; i<buffLength; i++) {
					if(buff[i]==tmp2) {
						goto flag;
					}
				}
				buff[buffLength]=tmp2;
				buffLength++;
				flag:
				continue;
			}
			N_++;
		}

		file.clear();
		file.seekg(0, ios::beg);

		M_ = buffLength;
		timePointId_ = new string[M_];
		childId_ = new string[N_];
		a_ = new int[N_*M_];
		answers_ = new Answer[min(M_+1, N_+1)];
		for (i=0; i<N_*M_; i++) {
			a_[i]=0;
		}
		for (i=0; i<M_; i++) {
			timePointId_[i]=buff[i];
		}
		int childTmpCounter = 0;

		while(getline(file, tmp1)) {
			childId_[childTmpCounter] = cutFirstWord(tmp1);
			while(tmp1.length()>0){
				tmp2 = cutFirstWord(tmp1);
				for (i=0; i<M_; i++) {
					if (timePointId_[i]==tmp2) {
						a_[childTmpCounter*M_+i] = 1;
						break;
					}
				}
			}
			childTmpCounter++;
		}

		file.close();

		return 1;
	}

	void computeSchedule(const int M, const int N) {
		int i, j;
		int sum, tmp_max_sum, tmp_minus, i_tmp_max, k, l, j_tmp_max;

		sum=0;
		i=0;
		while (i<N) {
			j=0;
			while(j<M) {
				sum+=a_[i*M_+j];
				j++;
			}
			i++;
		}

		if (sum<1) return;

		i=0;
		while (i<N) {
			j=0;
			while(j<M) {
				if (a_[i*M_+j]==0) {
					j++;
					continue;
				} else {
					tmp_minus = 0;
					for (k=j; (k<(N-1)*M_+M)&&(k!=i*M_+j); k+=M_) {
							tmp_minus+=a_[k];
					}
					for (k=(i-1)*M_; (k<(i-1)*M_+M)&&(k!=i*M_+j); k++) {
							tmp_minus+=a_[k];
					}
					if (tmp_max_sum<=(sum-tmp_minus)) {
						i_tmp_max = i;
						j_tmp_max = j;
						tmp_max_sum = sum-tmp_minus;
					}
				}
				j++;
			}
			i++;
		}

		changeKLCols(j_tmp_max, M-1);
		changeKLRows(i_tmp_max, N-1);

		answers_[nAnswers].fill(childId_[N-1], timePointId_[M-1]);
//		cout << nAnswers << endl;
		nAnswers++;
//		for (k=M-1; (k<(N-1)*M_+M)&&(k!=(N-1)*M_+M-1); k+=M_) {
//			a_[k]=0;
//		}
//		for (k=(N-1)*M_; (k<(N-1)*M_+M)&&(k!=(N-1)*M_+M); k++) {
//			a_[k]=0;
//		}

		if ((M-1>0)&&(N-1>0)) {
			computeSchedule(M-1, N-1);
		}

		return;
	}

	void computeSchedule() {
		computeSchedule(M_, N_);
		return;
	}

	void consoleOutput() {
		int i, j;
		int colWidth[M_+1];

		colWidth[0] = 1;
		for (i=0; i<N_; i++) {
			if (childId_[i].length()>colWidth[0]) {
				colWidth[0] = childId_[i].length();
			}
		}

		cout << setw(colWidth[0]) << " " << " ";

		for (i=0; i<M_; i++) {
			colWidth[i+1] = timePointId_[i].length();
			cout << setw(colWidth[i+1]) << timePointId_[i] << " ";
		}
		cout << endl;

		for (i=0; i<N_; i++) {
			cout << setw(colWidth[0]) << childId_[i] << " ";
			for (j=0; j<M_; j++) {
				cout << setw(colWidth[j+1]) << a_[i*M_+j] << " ";
			}
			cout << endl;
		}

		return;
	}

	void answersOutput() {
		for (int i=0; i<nAnswers; i++) {
			answers_[i].consoleOutput();
		}
	}
};

#endif /* SCHEDULE_H_ */
