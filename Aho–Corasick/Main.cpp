#include <iostream>
#include <string>
#include <list>
#include <conio.h>
using namespace std;
class rib;						///Объявляем класс "ребро", но пока не описываем
class node						///Класс "узел" (для бора)
{
public:
	string mark;				///Метка (строка)
	list <rib> ribs;			///Рёбра, из него выходящие
	node()						///Конструктор "по-умолчанию"
	{
		mark="";
	}
	node(string mark):mark(mark){}	///Конструктор со строкой
};
class rib						///Класс "ребро"
{
public:
	char mark;					///Метка ребра (символ, которому оно соответствует)
	node* end;					///Указатель на конец ребра
	rib(char c, node* end): mark(c), end(end){}		///Конструктор по метке и концу
};
class Bor							///Класс "бор" (конструкция для хранения слов из словаря)
{
	node root;						///Корень
	list <node> nodes;				///Все узлы
	list <string> words;			///Все слова в виде строк
public:
	friend class Aho;				///Делаем класс "Aho" дружественным
};
class Aho							///Собственно, класс, реализующий алгоритм Ахо-Корасик
{
	Bor bor;						///Бор
	node* state;					///Состояние автомата
public:
	void addTree(string a);			///Функция добавления слова (дерева) в бор
	bool go(char a);				///Функция перехода по ребру
	void fail();					///Функция для действий, если нет подходящего ребра
	void find(string a);			///Функция поиска подстрок
	void out(string toout);			///Функция вывода
};
void Aho::addTree(string a)
{
	bor.words.push_back(a);			///Заносим в список слово из словаря
	state=&bor.root;				///Переводим автомат в корень
	for (int i=0; i<a.length(); i++)		///Разбираем строку посимвольно
	{
		if (!go(a[i]))						///Если ещё нет соответствующих ребра и узла, создаём их
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
	state=&bor.root;					///На всякий случай снова переводим автомат в корень
}
bool Aho::go(char a)
{
	for (list<rib>::iterator i = state->ribs.begin(); i!=state->ribs.end(); ++i)		///Проходим по списку рёбер, выходящих из данной вершины
	{
		if (i->mark==a)					///Если есть ребро с нужной нам меткой
		{
			state=i->end;				///Переходим по нему
			return 1;
		}
	}
	return 0;							///Иначе неудача
}
void Aho::fail()
{
	string substr=state->mark;
	while (substr!="")					///Пока можно
	{
		substr=substr.substr(1);		///Потихоньку отщипываем от строки текущего состояния по одному символу
		for (list<node>::iterator i=bor.nodes.begin(); i!=bor.nodes.end(); ++i)			///Пробегаем по списку вершин
		{
			if (i->mark==substr)		///Если есть нужная нам вершина
			{
				state=&*i;				///Переходим в неё
				return;
			}
		}
	}
	state=&bor.root;					///Иначе в корень
}
void Aho::find(string a)
{
	state=&bor.root;					///Начинаем с корня
	for (int i=0; i<a.length(); i++)	///Разбираем входную строку посимвольно
	{
		while(!go(a[i]))				///Если нет ребра с меткой, соответствующей данному символу
			if (state!=&bor.root)		///И мы не в корне
				fail();					///Ищем вершину с подстрокой
			else 
				break;					///Если же есть, то ок
			out(state->mark);			///Проверяем, если что-то уже нашли, выводим это
	}
}
void Aho::out(string toout)
{
	while(toout!="")					///Рассматриваем строку из текущего состояния и её подстроки
	{
		for (list<string>::iterator j=bor.words.begin(); j!=bor.words.end(); ++j)			///Пробегаем по списку слова из бора
		{
			if (*j==toout)				///Если наша метка (или уже её подстрока) совпадает с один из слов из словаря
				cout<<"Substring \""<<toout<<"\" was found\n";			///Радостно сообщаем об этом пользователю
		}
		toout=toout.substr(1);			///Откусываем от строки 1 символ слева
	}
}
void main()								///Тестирование
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