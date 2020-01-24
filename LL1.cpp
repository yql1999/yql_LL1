//LL(1������ 
#include<iostream>
#include<algorithm>
#include<cstring>
#include<string>
#include<stdio.h>
#include<vector>
#include<map>
#include<set>
#include<cmath>
#include<stack>
#include<queue>
#include<fstream>
#include<iomanip>
#include<iterator>
using namespace std;


vector<char> finish_sign;
vector<char> nofinish_sign;
vector<pair<char, string> > grammer;//�ķ�
map<char, set<char> > first_set;
map<char, set<char> > follow_set;
map<char, vector<string> > table;
int n;	//�ķ��� 

bool isNofinish_sign(char c)
{
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}

//���� 
void input() {
	int i,j,t;
	string s;
	ifstream in("input.txt");
	if (!in) {
		cout << "Error:������input.txt�ļ���" << endl;
		system("Pause");
		exit(1);
	}
	in >> n;
	for (i = 0; i < n; i++) {
		in >> s;
		pair<char, string> tmp;
		tmp.second = "";
		tmp.first = s[0];
		//�ӵ������ַ���ʼ����->���棩 
		for (j = 3; j < s.length(); j++) {
			tmp.second = tmp.second + s[j];
		}
		grammer.push_back(tmp);
		for (j = 0; j < s.length(); j++) {
			if (s[j] != '-'&&s[j] != '>') {
				//������ս�� 
				if (isNofinish_sign(s[j])) {
					t = 0;
					for (; t < nofinish_sign.size(); t++) {
						if (nofinish_sign[t] == s[j]) {
							break;
						}
					}
					if (t == nofinish_sign.size()) 
						nofinish_sign.push_back(s[j]);
				}
				else{
					t = 0;
					for (; t < finish_sign.size(); t++) {
						if (finish_sign[t] == s[j]) {
							break;
						}
					}
					if (t == finish_sign.size()) 
						finish_sign.push_back(s[j]);
				}
			}

		}
	}
	//NOTE: # Ҳ���ս�� 
	finish_sign.push_back('#');
}

//First���� 
void getfirst(char t) {
	int tag = 0;
	for (int i = 0; i < n; i++) {
		if (grammer[i].first == t) {
			//��һ�����ս���Ŀɼ��뼯�� 
			if (!isNofinish_sign(grammer[i].second[0])) 
				first_set[t].insert(grammer[i].second[0]);
			//��һ�������ս����������ҵ����ս������������Ҹ��ַ�����First�� 
			else{
				for (int j = 0; j < grammer[i].second.length(); j++) {
					if (!isNofinish_sign(grammer[i].second[j])){
						first_set[t].insert(grammer[i].second[j]);
						break;
					}
					//�ݹ�
					getfirst(grammer[i].second[j]);
					bool flag = 0;
					set<char>::iterator it;
					for (it = first_set[grammer[i].second[j]].begin(); it != first_set[grammer[i].second[j]].end(); it++) {
						if (*it == '$') {
							flag = 1;
						}
						else 
							first_set[t].insert(*it);//���ǿյĶ�����
					}
					//����ǿ�
					if (!flag) 
						break;
					else 
						tag++;
				}
				//�������Ҳ���Ϊ�գ��Ͱѿռ���First�� 
				if (tag == grammer[i].second.length())
					first_set[t].insert('$');
			}
		}
	}
}

//Follow���� 
void getfollow(char t) {
	for (int i = 0; i < n; i++) {
		int index = -1;
		int len = grammer[i].second.length();
		for (int j = 0; j < len; j++)
		{
			if (grammer[i].second[j] == t)
			{
				index = j;
				break;
			}
		}
		if (index != -1 && index < len - 1) {
			char x = grammer[i].second[index + 1];
			//����ֱ�Ӹ��ս����ֱ�Ӽ���
			if (!isNofinish_sign(x)) {
				follow_set[t].insert(x);
			}
			else{
				bool flag = 0;
				set<char>::iterator it;
				for (it = first_set[x].begin(); it != first_set[x].end(); it++) {
					if (*it == '$') {
						flag = 1;
					}
					else
						follow_set[t].insert(*it);
				}
				if (flag && grammer[i].first != t) {
					getfollow(grammer[i].first);
					char tmp = grammer[i].first;
					//��������follow�� 
					set<char>::iterator it;
					for (it = follow_set[tmp].begin(); it != follow_set[tmp].end(); it++) {
						follow_set[t].insert(*it);
					}
				}
			}
		}
		else if (index != -1 && index == len - 1 && t != grammer[i].first) {//�����һ��Ҳֱ�Ӱ�followȫ������
			getfollow(grammer[i].first);
			char tmp = grammer[i].first;
			//ȫ��follow(X)����
			set<char>::iterator it;
			for (it = follow_set[tmp].begin(); it != follow_set[tmp].end(); it++) {
				follow_set[t].insert(*it);
			}

		}
	}
}

//����Ԥ�������Select���ϣ� 
void settable() {
	for (int i = 0; i < n; i++) {
		bool flag = 1;
		for (int j = 0; j < grammer[i].second.length() && flag; j++) {
			flag = 0;
			char tmp = grammer[i].second[j];
			if (!isNofinish_sign(tmp)) {
				if (tmp != '$') {
					string t = "";
					t=t+grammer[i].first;
					t+="->" + grammer[i].second;
					table[tmp].push_back(t);
				}
				else if (tmp == '$') {
					set<char>::iterator it;
					for (it = follow_set[grammer[i].first].begin(); it != follow_set[grammer[i].first].end(); it++) {
						string t = "";
						t = t + grammer[i].first;
						t += "->" + grammer[i].second;
						table[*it].push_back(t);
					}
				}
			}
			else {
				set<char>::iterator it;
				for (it = first_set[tmp].begin(); it != first_set[tmp].end(); it++) {
					string t = "";
					t = t + grammer[i].first;
					t += "->" + grammer[i].second;
					table[*it].push_back(t);
				}
				if (first_set[tmp].count('$') != 0) {
					flag = 1;
					set<char>::iterator it;
					for (it = follow_set[grammer[i].first].begin(); it != follow_set[grammer[i].first].end(); it++) {
						string t = "";
						t = t + grammer[i].first;
						t += "->" + grammer[i].second;
						table[*it].push_back(t);
					}
				}
			}
		}
	}
}

int main() {
	input();
	for (int i = 0; i < nofinish_sign.size(); i++)
		getfirst(nofinish_sign[i]);
	cout << "***************FIRST��***********************" << endl;
	map<char,set<char> >::iterator it;
	set<char>::iterator itt;
	for (it = first_set.begin(); it != first_set.end(); it++) {
		cout << it->first << ": ";
		for (itt = it->second.begin(); itt != it->second.end(); itt++) {
			cout << *itt << " ";
		}
		cout << endl;
	}
	cout << "****************FOLLOW��**********************" << endl;
	for (int i = 0; i < nofinish_sign.size(); i++) {
		if (i == 0) {
			follow_set[nofinish_sign[i]].insert('#');
		}
		getfollow(nofinish_sign[i]);
	}
	//map<char,set<char> >::iterator it;
	//set<char>::iterator itt;
	for (it = follow_set.begin(); it != follow_set.end(); it++) {
		cout << it->first << ": ";
		for (itt = it->second.begin(); itt != it->second.end(); itt++) {
			cout << *itt << " ";
		}
		cout << endl;
	}
	settable();
	cout << "****************Ԥ�������**********************" << endl;
	for (int i = 0; i < finish_sign.size(); i++){
		if (finish_sign[i] != '$') {
			cout << setw(12) << finish_sign[i];
		}
	}
	cout << endl;
	for (int i = 0; i < nofinish_sign.size(); i++) {
		//printf ( "|%5s" , nofinish_sign[i] );
		cout << "|" << nofinish_sign[i];
		printf ( "%4s" , "|" );
		for (int j = 0; j < finish_sign.size(); j++) {
			if (finish_sign[j] != '$') {
				if (table.find(finish_sign[j]) == table.end()) {
					cout << setw(10) << "/";
				}
				else {
					bool flag = 0;
					vector<string>::iterator it;
					for (it = table[finish_sign[j]].begin(); it != table[finish_sign[j]].end(); it++) {
						if ((*it)[0] == nofinish_sign[i]) {
							flag = 1;
							cout << setw(10) << (*it);
							break;
						}
					}
					if (!flag) {
						cout << setw(10) << "/";
					}
				}
			}
			
		}cout << endl;
	}
	ofstream out("output.txt");
	out << "****************Ԥ�������**********************" << endl;
	for (int i = 0; i < finish_sign.size(); i++){
		if (finish_sign[i] != '$') {
			out << setw(12) << finish_sign[i];
		}
	}
	out << endl;
	for (int i = 0; i < nofinish_sign.size(); i++) {
		out << " " << nofinish_sign[i];
		//printf ( "%4s" , "|" );
		for (int j = 0; j < finish_sign.size(); j++) {
			if (finish_sign[j] != '$') {
				if (table.find(finish_sign[j]) == table.end()) {
					out << setw(10) << "/";
				}
				else {
					bool flag = 0;
					vector<string>::iterator it;
					for (it = table[finish_sign[j]].begin(); it != table[finish_sign[j]].end(); it++) {
						if ((*it)[0] == nofinish_sign[i]) {
							flag = 1;
							out << setw(10) << (*it);
							break;
						}
					}
					if (!flag) {
						out << setw(10) << "/";
					}
				}
			}

		}
		out << endl;
	}
	system("Pause");
}
