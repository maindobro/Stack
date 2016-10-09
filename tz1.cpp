#include <stdlib.h>
#include <iostream>
template <typename T>
class stack
{
private:
	T *array_;											// указатель на стек
	size_t array_size_;									// количество элементов в стеке
	size_t count_;										// номер текущего элемента стека
	auto swap(stack & right) -> void;					// меняет содержимое стеков	
public:
	stack();											// конструктор
	~stack();											// деструктор
	size_t count() const;
	auto push(T const &) -> void;						// поместить элемент в вершину стека
	T pop();											// удалить элемент из вершины стека и вернуть его
	auto operator=(stack const & right)->stack &;		// переопределение оператора, присваивает значение контейнеру"
};

int main()
{
	stack<int> a;
	a.push(1);
	a.push(2);
	a.push(3);
	a.push(4);
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
		for (int i = 0; i < count_; i++) 
		{
			std::cout << array_[i] << "[" << i << "]" << " ";
		}
		std::cout << std::endl;
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
