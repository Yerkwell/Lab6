#include <iostream>
#include <string>
#include <list>
#include <conio.h>
using namespace std;
class rib;						///��������� ����� "�����", �� ���� �� ���������
class node						///����� "����" (��� ����)
{
public:
	string mark;				///����� (������)
	list <rib> ribs;			///и���, �� ���� ���������
	node()						///����������� "��-���������"
	{
		mark="";
	}
	node(string mark):mark(mark){}	///����������� �� �������
};
class rib						///����� "�����"
{
public:
	char mark;					///����� ����� (������, �������� ��� �������������)
	node* end;					///��������� �� ����� �����
	rib(char c, node* end): mark(c), end(end){}		///����������� �� ����� � �����
};
class Bor							///����� "���" (����������� ��� �������� ���� �� �������)
{
	node root;						///������
	list <node> nodes;				///��� ����
	list <string> words;			///��� ����� � ���� �����
public:
	friend class Aho;				///������ ����� "Aho" �������������
};
class Aho							///����������, �����, ����������� �������� ���-�������
{
	Bor bor;						///���
	node* state;					///��������� ��������
public:
	void addTree(string a);			///������� ���������� ����� (������) � ���
	bool go(char a);				///������� �������� �� �����
	void fail();					///������� ��� ��������, ���� ��� ����������� �����
	void find(string a);			///������� ������ ��������
	void out(string toout);			///������� ������
};
void Aho::addTree(string a)
{
	bor.words.push_back(a);			///������� � ������ ����� �� �������
	state=&bor.root;				///��������� ������� � ������
	for (int i=0; i<a.length(); i++)		///��������� ������ �����������
	{
		if (!go(a[i]))						///���� ��� ��� ��������������� ����� � ����, ������ ��
		{
			string newmark=state->mark;
			newmark.push_back(a[i]);
			node newnod=node(newmark);
			bor.nodes.push_back(newnod);
			node* newnode=&bor.nodes.back();
			state->ribs.push_back(rib(a[i], newnode));
			state=newnode;
		}
	}
	state=&bor.root;					///�� ������ ������ ����� ��������� ������� � ������
}
bool Aho::go(char a)
{
	for (list<rib>::iterator i = state->ribs.begin(); i!=state->ribs.end(); ++i)		///�������� �� ������ ����, ��������� �� ������ �������
	{
		if (i->mark==a)					///���� ���� ����� � ������ ��� ������
		{
			state=i->end;				///��������� �� ����
			return 1;
		}
	}
	return 0;							///����� �������
}
void Aho::fail()
{
	string substr=state->mark;
	while (substr!="")					///���� �����
	{
		substr=substr.substr(1);		///���������� ���������� �� ������ �������� ��������� �� ������ �������
		for (list<node>::iterator i=bor.nodes.begin(); i!=bor.nodes.end(); ++i)			///��������� �� ������ ������
		{
			if (i->mark==substr)		///���� ���� ������ ��� �������
			{
				state=&*i;				///��������� � ��
				return;
			}
		}
	}
	state=&bor.root;					///����� � ������
}
void Aho::find(string a)
{
	state=&bor.root;					///�������� � �����
	for (int i=0; i<a.length(); i++)	///��������� ������� ������ �����������
	{
		while(!go(a[i]))				///���� ��� ����� � ������, ��������������� ������� �������
			if (state!=&bor.root)		///� �� �� � �����
				fail();					///���� ������� � ����������
			else 
				break;					///���� �� ����, �� ��
			out(state->mark);			///���������, ���� ���-�� ��� �����, ������� ���
	}
}
void Aho::out(string toout)
{
	while(toout!="")					///������������� ������ �� �������� ��������� � � ���������
	{
		for (list<string>::iterator j=bor.words.begin(); j!=bor.words.end(); ++j)			///��������� �� ������ ����� �� ����
		{
			if (*j==toout)				///���� ���� ����� (��� ��� � ���������) ��������� � ���� �� ���� �� �������
				cout<<"Substring \""<<toout<<"\" was found\n";			///�������� �������� �� ���� ������������
		}
		toout=toout.substr(1);			///���������� �� ������ 1 ������ �����
	}
}
void main()								///������������
{
	Aho a1;
	cout<<"Vvedite kolichestvo slov v slovare: ";
	int n;
	cin>>n;
	string str;
	for (int i=0; i<n; i++)
	{
		cout<<"Vvedite slovo: ";
		cin>>str;
		a1.addTree(str);
	}
	cout<<"Vvedite stroku dlya poiska:\n";
	_flushall();
	getline(cin,str);
	a1.find(str);
	getch();
}