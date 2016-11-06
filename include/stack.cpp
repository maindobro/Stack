#include <iostream>

template <typename T>
class stack
{
public:
	stack();							//noexcept
	stack(stack const &);						//strong
	~stack();							//noexcept
	size_t count() const;						//noexcept
	auto push(T const &) -> void;					//strong
	T pop();							//basic
	auto operator=(stack const & right)->stack &;			//strong
private:
	T *array_;							
	size_t array_size_;							
	size_t count_;							
};
template<typename T>
auto newcopy(const T * item, size_t size, size_t count) -> T*		//strong
{
	T * buff = new T[size];
	std::copy(item, item + count, buff);
	return buff;
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
T stack<T>::pop()
{
	if (count_ == 0)
	{
		throw std::logic_error("Stack is empty!");
	}
	return array_[--count_];
}
template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & {
	if (this != &right) {
		delete[] array_;
		newcopy(right.array_, right.array_size_, right.count_);
	}
	return *this;
}
