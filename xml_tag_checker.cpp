#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class xmldata;
template <class T>
class stack {
	class node {
	public:
		T data;
		node* next;
		node() {
			next = nullptr;
		}
	};
	node* top;
public:
	stack() {
		top = nullptr;
	}
	int size() {
		int count = 0;
		stack <T>* s2 = new stack <T>;
		while (isEmpty() != true) {
			s2->push(returnTop());
			pop();
			count++;
		}
		while (s2->isEmpty() != true) {
			push(s2->returnTop());
			s2->pop();
		}
		return count;
	}

	//2. isEmpty
	bool isEmpty() {
		if (top == NULL) return true;
		else return false;

	}

	//3. Return top
	T returnTop() {
		if (top != NULL) {
			return top->data;
		}
		/*else
			return NULL;*/
	}

	// 4.Pop
	void pop() {
		if (top == NULL) return;
		else {
			node* temp = new node;
			temp = top;
			top = top->next;
			delete temp;
		}
	}

	// 5.Push
	void push(T const e) {
		node* temp = new node;
		if (temp != NULL) {
			temp->data = e;
			if (top != NULL) {
				temp->next = top;
			}
			top = temp;
		}
	}

	// 6. Clear
	void clear() {
		while (isEmpty() != true) {
			pop();
		}
	}

	// 7. Print
	void print() {
		stack <T>* s2 = new stack <T>;
		while (isEmpty() != true) {
			s2->push(returnTop());
			cout << returnTop() << " ";
			pop();

		}
		while (s2->isEmpty() != true) {
			push(s2->returnTop());
			s2->pop();
		}
		cout << endl;
	}
	
	bool search(T& s1) {
		if (!isEmpty()) {
			stack s2;
			bool flag = false;
			while (!isEmpty()) {
				if (s1 == returnTop()) {
					pop();
					flag = true;
				}
				else {
					s2.push(returnTop());
					pop();
				}
			}
			while (!s2.isEmpty()) {
				push(s2.returnTop());
				s2.pop();
			}
			return flag;
		}
		else 
			return false;
	}
};


int checkLength(string s) {
	int i = 0;
	int count = 0;
	while (s[i] != '\0') {
		count++;
		i++;
	}
	return count;
}

class xmldata : stack <xmldata> {
	string str;
	int linenum;
public:
	xmldata() {
		str = '\0';
		linenum = 0;
	}

	xmldata(string s, int n) {
		str = s;
		linenum = n;
	}

	string returnData() {
		return str;
	}

	int returnline() {
		return linenum;
	}

	bool operator == (xmldata obj) {
		if (str == obj.str) {
			return true;
		}
		else return false;
	}

	bool checkAttribute() {
		int equalCount = 0;
		int singleCount = 0;
		int doubleCount = 0;
		int scheck = 0;
		int dcheck = 0;
		int echeck = 0;
		for (int i = 0; i < checkLength(str); i++) {
			if (str[i] == '=') {
				equalCount++;
			}
			if (str[i] == 39) {
				singleCount++;
			}
			if (str[i] == '"') {
				doubleCount++;
			}
		}
		if (equalCount * 2 != singleCount + doubleCount) {
			return false;
		}
		else {
			int x = 0;
			for (int i = 0; i < checkLength(str); i++) {
				if (str[i] == '=') {
					if (str[i + 1] == 39) {
						x = i + 2;
						while (str[x] != '=' && x < checkLength(str)) {
							if (str[x] == 39) {
								scheck = 1;
							}
							else if (str[x] == '"') {
								dcheck = 1;
							}
							x++;
						}
						if (dcheck == 1 || scheck==0) {
							return false;
						}

						dcheck = 0;
						scheck = 0;
						i = x - 1;
					}
					else if (str[i + 1] == '"') {
						x = i + 2;
						while (str[x] != '=' && x < checkLength(str)) {
							if (str[x] == 39) {
								scheck = 1;
							}
							else if (str[x] == '"') {
								dcheck = 1;
							}
							x++;
						}
						if (dcheck == 0 || scheck == 1) {
							return false;
						}

						dcheck = 0;
						scheck = 0;
						i = x - 1;
					}
				}
			}
		}
		return true;
	}
};



int main() {
	ifstream file("input.xml");
	if (!file) {
		cout << "File not found." << endl;
	}
	else {
		string s1;
		stack<xmldata> st1;
		bool finalcheckkk = true;
		int lineNumber = 1;
		while (file.eof() != true) {
			getline(file, s1);

			//header check
			if (s1[0] == '<' && s1[1] == '?') {
				stack <char> st;
				st.push('>');
				st.push('?');
				int size = s1.length();
				for (int i = 2; i < size; i++) {
					if (s1[i] == st.returnTop()) {
						st.pop();
					}
				}
				if (st.isEmpty() == false) {
					cout << "Error in Header on Line: " << lineNumber << " Header: " << s1 << endl;
				}
				xmldata xml(s1, lineNumber);
				bool flagggg = xml.checkAttribute();
				if (!flagggg) {
					cout << "Error in Attributes on line " << lineNumber << ": " << s1 << endl;
					finalcheckkk = false;
				}
				lineNumber++;
			}

			//comment check
			else if (s1[0] == '<' && s1[1] == '!' && s1[2] == '-' && s1[3] == '-') {
				stack <char> st;
				st.push('>');
				st.push('-');
				st.push('-');
				int size = s1.length();
				for (int i = 4; i < size; i++) {
					if (s1[i] == st.returnTop()) {
						st.pop();
					}
				}
				if (st.isEmpty() == false) {
					cout << "Error in comment on Line: " << lineNumber << " comment: " << s1 << endl;
				}
				xmldata xml(s1, lineNumber);
				bool flagggg = xml.checkAttribute();
				if (!flagggg) {
					cout << "Error in Attributes on line " << lineNumber << ": " << s1 << endl;
					finalcheckkk = false;
				}
				lineNumber++;
			}

			//tag check
			else if (s1[0] == '<' && s1[1] != '/') {
				int z = 0;
				int w = 0;
				int size = s1.length();
				bool check1 = false;
				bool check2 = false;
				for (int i = 2; i < size; i++) {
					if (s1[i] == '<') {
						w = i;
						check1 = true;
					}
					if (s1[i] == '=') {
						check2 = true;
						z = i;
					}
				}

				// no opening bracket and no equal sign
				if (check1 == false && check2 == false) {
					int x = 0, y = 1;
					string s2 = s1;
					while (x < size) {
						if (s2[x] == '>') {
							break;
						}
						x++;
					}
					x++;
					s2.erase(x);
					s2.insert(1, "/");
					xmldata obj(s2, lineNumber);
					st1.push(obj);
					lineNumber++;
				}

				//equal sign only
				else if (check1 == false && check2 == true) {
					int x = 0, y = 0;
					string s2 = s1;
					s2.insert(1, "/");
					while (y < z) {
						if (s2[y] == ' ') {
							break;
						}
						x++;
						y++;
					}
					s2.erase(x);
					s2.insert(x, ">");
					xmldata obj(s2, lineNumber);
					xmldata xml(s1, lineNumber);
					bool flagggg = xml.checkAttribute();
					if (!flagggg) {
						cout << "Error in Attributes on line " << lineNumber << ": " << s1 << endl;
						finalcheckkk = false;
					}
					st1.push(obj);
					lineNumber++;
				}

				//only opening bracket
				else if (check1 == true && check2 == false) {
					int x = 0, y = 0;
					string s2 = s1;
					s2.insert(1, "/");

					while (y < w) {
						if (s2[y] == '>') break;
						y++;
						x++;
					}
					x++;
					s2.erase(x);
					xmldata obj(s2, lineNumber);
					st1.push(obj);
					string s3 = s1;
					x = w;
					y = 0;
					s3.erase(0, w);
					xmldata temp = st1.returnTop();
					if (s3 == temp.returnData() && lineNumber == temp.returnline()) {
						st1.pop();
					}
					else {
						cout << "Mismatched tags Error on Line: " << lineNumber << ": " << s1 << endl;
					}
					lineNumber++;
				}

				//opening bracket and equal sign exists
				else if (check1 == true && check2 == true) {
					int x = 0, y = 0;
					string s2 = s1;
					s2.insert(1, "/");
					while (y < z) {
						if (s2[y] == ' ') {
							break;
						}
						x++;
						y++;
					}
					s2.erase(x);
					s2.insert(x, ">");
					xmldata obj(s2, lineNumber);
					xmldata xml(s1, lineNumber);
					bool flagggg = xml.checkAttribute();
					if (!flagggg) {
						cout << "Error in Attributes on line " << lineNumber << ": " << s1 << endl;
						finalcheckkk = false;
					}
					st1.push(obj);
					string s3 = s1;
					x = w;
					y = 0;
					s3.erase(0, w);
					xmldata temp = st1.returnTop();
					if (s3 == temp.returnData() && lineNumber == temp.returnline()) {
						st1.pop();
					}
					else {
						cout << "Mismatched tags Error on Line: " << lineNumber << ": " << s1 << endl;
					}
					lineNumber++;
				}
			}

			// closing tag
			else if (s1[0] == '<' && s1[1] == '/') {
				xmldata temp = st1.returnTop();
				if (s1 == temp.returnData()) {
					st1.pop();
				}
				else {
					xmldata temp2(s1, lineNumber);
					bool flag = st1.search(temp2);
					if (flag == false) {
						cout << "Mismatched tags Error on Line: " << lineNumber << ": " << s1 << endl;

					}
				}
				lineNumber++;
			}

			// not sure about this
			else if (s1[0] != '<') {
				bool flag = false;
				string s2 = s1;
				int x = 0;
				int size = s1.length();
				while (x < size) {
					if (s1[x] == '<') {
						flag = true;
						break;
					}
					x++;
				}
				if (flag == true) {
					s2.erase(0, x);
				}
				xmldata xml(s1, lineNumber);
				bool flagggg = xml.checkAttribute();
				if (!flagggg) {
					cout << "Error in Attributes on line " << lineNumber << ": " << s1 << endl;
					finalcheckkk = false;
				}
				if (s2[0] == '<' && s2[1] == '/') {
					xmldata temp = st1.returnTop();
					if (s2 == temp.returnData()) {
						st1.pop();
					}
					else {
						xmldata temp2(s1, lineNumber);
						bool searchflag = st1.search(temp2);
						if (searchflag == false) {
							cout << "Mismatched tags Error on Line: " << lineNumber << ": " << s1 << endl;
						}
					}
				}
				else cout << "No tag on line: " << lineNumber << endl;
				lineNumber++;
			}
		}

		cout << endl;

		// for mismatched tags
		if (st1.isEmpty() == false) {
			cout << "Following are the missing tags:" << endl;
			while (st1.isEmpty() != true) {
				xmldata temp = st1.returnTop();
				string string1 = temp.returnData();

				//string1.erase(1, 1);
				cout << "On line " << temp.returnline() << ": " << string1 << endl;
				st1.pop();
			}
		}
		else if (finalcheckkk){
			cout << "All tags and quotes matched." << endl;
		}
	}
	return 0;
}