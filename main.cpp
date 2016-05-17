#include <iostream>

using namespace std;

struct Node
{
private:
	int value;
	Node* nextNode;

public:
	Node(const int& num) : value(num), nextNode() {}
	Node(const int& num, Node* aNode) : value(num), nextNode(aNode) {}

	void setValue(const int& num) { value = num     ; }
	void setNextNode(Node* aNode) { nextNode = aNode; }

	int getValue()       { return value   ; }
	Node* getNextNode () { return nextNode; }
};

struct List
{
private:
	Node* head;

public:
	List() : head() {}
	
	void addValue(const int& val)
	{
		Node* newNode = new Node(val);
		Node* temp = head;
		if (temp != nullptr)
		{
			while (temp->getNextNode() != nullptr)
				temp = temp->getNextNode();
			temp->setNextNode(newNode);
		}
		else
			head = newNode;
	}

	void deleteValue(const int& val)
	{
		if (head == nullptr)
			return;
		else if (head->getNextNode() == nullptr)
			head = nullptr;
		else if (head->getValue() == val)
		{
			Node* temp = head;
			head = head->getNextNode();
			delete temp;
		}
		else
		{
			Node* temp = head->getNextNode();
			Node* prev = head;
			while (temp->getValue() != val)
			{
				prev = temp;
				temp = temp->getNextNode();
			}
			prev->setNextNode(temp->getNextNode());
			delete temp;
		}
	}

	void printList()
	{
		if (head == nullptr) 
			return;	
		else
		{
			Node* temp = head;
			while (temp != nullptr)
			{
				std::cout << temp->getValue() << "  " << std::endl;
				temp = temp->getNextNode();
			}
		}
		std::cout << "\n\n" << std::endl;
	}
	 
};


int main()
{
	List myList;
	
	myList.addValue(1);
	myList.addValue(2);
	myList.addValue(3);
	myList.addValue(4);
	myList.addValue(5);
	myList.printList();
	
	myList.deleteValue(3);
	myList.printList();

	myList.addValue(100);
	myList.printList();

	myList.deleteValue(1);
	myList.printList();

	myList.addValue(10000);
	myList.addValue(50000);
	myList.printList();

	return 0;
}