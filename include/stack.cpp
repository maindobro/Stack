#include <stdlib.h>
#include <iostream>
template <typename T>
class stack
{
private:
	T *array_;											// óêàçàòåëü íà ñòåê
	size_t array_size_;									// êîëè÷åñòâî ýëåìåíòîâ â ñòåêå
	size_t count_;										// íîìåð òåêóùåãî ýëåìåíòà ñòåêà
	auto swap(stack & right) -> void;					// ìåíÿåò ñîäåðæèìîå ñòåêîâ	
public:
	stack();											// êîíñòðóêòîð
	~stack();											// äåñòðóêòîð
	size_t count() const;
	auto push(T const &) -> void;						// ïîìåñòèòü ýëåìåíò â âåðøèíó ñòåêà
	T pop();											// óäàëèòü ýëåìåíò èç âåðøèíû ñòåêà è âåðíóòü åãî
	auto operator=(stack const & right)->stack &;		// ïåðåîïðåäåëåíèå îïåðàòîðà, ïðèñâàèâàåò çíà÷åíèå êîíòåéíåðó"
};

int main()
{
	stack<int> a;
	a.push(1);
	a.push(2);
	a.push(3);
	stack<int> b;
	b.push(2);
	system("pause");
}

template <typename T>
size_t stack<T>::count() const
{
	std::cout << count_;
	return count_;
}
template <typename T>
stack<T>::stack()
{
	array_size_ = 0;
	array_ = new T[array_size_];
	count_ = 0;
}

template <typename T>
stack<T>::~stack()
{
	delete[] array_;
}

template<typename T>
void stack<T>::push(T const &item) 
{
	if (count_ == array_size_) 
	{
		count_++;
		size_t size = array_size_ * 2 + (array_size_ == 0);
		T *buff = new T[size];
		for (int i = 0; i < count_; i++) 
		{
			buff[i] = array_[i];
		}
		delete[] array_;
		array_ = buff;
		array_size_ = size;
		array_[count_ - 1] = item;
	}
}
template<typename T>
T stack<T>::pop() 
{
	if (count_ == 0) 
	{
		throw std::logic_error("Stack is empty!");
	}
	return array_[--count_];
}
template<typename T>
auto stack<T>::swap(stack & right) -> void 
{
	array_size_ = right.array_size_;
	count_ = right.count_;
	T *buff = new T[array_size_];
	std::swap(buff, array_);
	std::swap(buff, right.array_);
	std::swap(buff, array_);
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & 
{
	if (this != &right) 
	{
		(stack(right)).swap(*this);
	}
	return *this;
}
