#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>

template <typename T>
class allocator
{
protected:
    allocator(size_t size = 0);
    ~allocator();
    auto swap(allocator & other) -> void;
    
    allocator(allocator const &) = delete;
    auto operator =(allocator const &) -> allocator & = delete;
    
    T * ptr_;
    size_t size_;
    size_t count_;
};

template <typename T>
class stack
{
public:
	stack();									//noexcept
	stack(stack const &);								//strong
	~stack();									//noexcept
	size_t count() const;								//noexcept
	auto push(T const &) -> void;							//strong
	void pop();									//basic
	const T& top();									//strong
	auto operator=(stack const & right)->stack &;					//strong
	auto empty() const -> bool; 							//noexcept
private:
	T *array_;											
	size_t array_size_;									
	size_t count_;										
};
template<typename T>
auto newcopy(const T * item, size_t size, size_t count) -> T* 				//strong
{
	T * buff = new T[size]; 
	try 
	{
		std::copy(item, item + count, buff);
	}
	catch (...) 
	{
		delete[] buff;
		throw;
	}
	return buff;
}
template <typename T>
size_t stack<T>::count() const
{
	return count_;
}
template <typename T>
stack<T>::stack()
{
	array_size_ = 0;
	array_ = new T[array_size_];
	count_ = 0;
}

template<typename T>
stack<T>::stack(stack const & other) :array_size_(other.array_size_), count_(other.count_), array_(newcopy(other.array_, other.array_size_, other.count_))
{

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
		size_t size = array_size_ * 2 + (array_size_ == 0);
		delete[] array_;
		array_ = newcopy(array_, size, array_size_);
		array_size_ = size;
	}
	array_[count_] = item;
	++count_;
}
template<typename T>
void stack<T>::pop() 
{
	if (count_ == 0) 
	{
		throw std::logic_error("Stack is empty!");
	}
	else 
	{
		count_--;
	}
}

template<typename T>
const T& stack<T>::top()
{
	if (count_ == 0) 
	{
		throw ("Stack is empty!");
	}
	return array_[count_ - 1];
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & 
{
	if (this != &right) 
	{
		delete[] array_;
		newcopy(right.array_, right.array_size_, right.count_);
	}
	return *this;
}

template<typename T>
auto stack<T>::empty() const -> bool
{
	if (count_ == 0)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
#endif
