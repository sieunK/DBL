#include <iostream>

using namespace std;

template <typename T>
struct Node {
	T data;
	Node* prev, * next;
public:
	Node(T _data)
		:data(_data), prev(nullptr), next(nullptr) {}	
};

template <typename T>
class LRU {
	Node<T>* head, * tail;
	size_t num;
public:
	LRU(void):head(nullptr), tail(nullptr), num(0) {}
	void push_front(T data)
	{
		Node<T>* p = new Node<T>(data);
		
		num++;
		
		if(head == nullptr) 
		{
			head = p;
			tail = p;
			return ;	
		}
		
		head->prev = p;
		p->next = head;
		head = p;
	}
	void push_back(T data)
	{
		Node<T>* p = new Node<T>(data);
		
		num++;
		
		if(head == nullptr)
		{
			head = p;
			tail = p;
			return ;
		}
		
		tail->next = p;
		p->prev = tail;
		tail = p;
	}
	void pop_front(void)
	{
		num--;
		if(head == nullptr)
			return ;
			
		if(head == tail)
		{
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		
		Node<T>* p = head;	
		head = head->next;
		
		delete p;
	}
	void pop_back(void)
	{
		num--;
		if(head == nullptr)
			return ;
		
		if(head == tail)
		{
			delete head;
			head = nullptr;
			tail = nullptr;
		}	
		
		Node<T>* p = tail;
		tail = tail->prev;
		
		delete p;
	}
	bool remove(T data)
	{
		Node<T>* p = head;
		for(; p!=nullptr; p=p->next)
		{
			if(p->data == data) 
			{
				if (p == head && p == tail)
				{
					head = nullptr;
					tail = nullptr;
				}
				else if(p == head)
				{
					head = p->next;
				}
				else if(p == tail)
				{
					tail = p->prev;
				}
				else {
					p->next->prev = p->prev;
					p->prev->next = p->next;
				}
				delete p;
				num--;
				return true;
			}
		}
		return false;
	}
	size_t size(void) const 
	{
		return num;
	}
	friend ostream& operator << (ostream& os, const LRU& lru) 
	{
		Node<T>* p = lru.head;
		for( ; p!=nullptr; p=p->next)
			os << p->data << ' ';
		return os;	
	}
};

template <typename T>
class DBL {
	LRU<T> L1, L2;
	size_t c;
public:
	DBL(size_t _c):c(_c) {}
	bool find(T data) 
	{
		/* hit in L1 */
		if(L1.remove(data)) 
		{
			L2.push_back(data);
			return true;
		}
		
		/* hit in L2 */
		if(L2.remove(data))
		{
			L2.push_back(data);
			return true;
		}		
		
		/* miss */
		if(L1.size() == c)
		{
			L1.pop_front();
		}
		else if(L1.size() + L2.size() == 2*c)
		{
			L2.pop_front();
		}

		L1.push_back(data);
		return false;
	}	
	friend ostream& operator << (ostream& os, const DBL& dbl)
	{
		os << "L1: " << dbl.L1 << endl;
		os << "L2: " << dbl.L2 << endl;
		return os;
	}
};

int main(void)
{
	size_t c;
	
	cout << "Enter the c: ";
	cin >> c;
	
	DBL<int> dbl(c);

	while(1)
	{
		int data;
		cout << "Enter the data: ";
		cin >> data;
		
		dbl.find(data);
		cout << dbl << endl;
	}
	return 0;
}
