#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>

class bitset
{
public:
	explicit
	bitset(size_t size);

	bitset(bitset const & other) = delete;
	auto operator =(bitset const & other)->bitset & = delete;

	bitset(bitset && other) = delete;
	auto operator =(bitset && other)->bitset & = delete;

	auto set(size_t index) -> void;
	auto reset(size_t index) -> void;
	auto test(size_t index) -> bool;

	auto size() const -> size_t;
	auto counter() const -> size_t;

private:
	std::unique_ptr<bool[]>  ptr_;
	size_t size_;
	size_t counter_;
};

bitset::bitset(size_t size) : ptr_(std::make_unique<bool[]>(size)), size_(size), counter_(0) {}

auto bitset::set(size_t index) -> void
{ 
	if (index < size_) 
	{
		ptr_[index] = true; 
		++counter_; 
	}
	else throw("false_index"); 
}

auto bitset::reset(size_t index) -> void
{	if (index < size_)
	{
		ptr_[index] = false;
		--counter_;
	}
	else throw("false_index");
}

auto bitset::test(size_t index) -> bool
{
	if (index < size_)
	{
		return !ptr_[index];
	}
	else throw("false_index");
	
}

auto bitset::size() const -> size_t
{
	return size_;
}

auto bitset::counter() const -> size_t
{
	return counter_;
}

/*=====================================================================================*/

template <typename T1, typename T2>
void construct(T1 * ptr, T2 const & value) {
    new(ptr) T1 (value);
}

template <typename T>
void destroy(T * ptr) noexcept
{
    ptr->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept
{
    for (; first != last; ++first) {
        destroy(&*first);
    }
}

template <typename T>
class allocator
{
public:
	explicit
	allocator( std::size_t size = 0 );
	allocator( allocator const & other );
	auto operator =( allocator const & other ) -> allocator & = delete;
	~allocator();
	auto resize() -> void;
	auto construct(T * ptr, T const & value ) -> void;
	auto destroy( T * ptr ) -> void;
	auto get() -> T *;
	auto get() const -> T const *;
	auto count() const -> size_t;
	auto full() const -> bool;
	auto empty() const -> bool;
private:
	auto destroy(T * first, T * last) -> void;
	auto swap(allocator & other) -> void;
	T * ptr_;
	size_t size_;
	std::unique_ptr<bitset> map_;

};

template <typename T>
allocator<T>::allocator(size_t size) : ptr_((T*)(operator new(size*sizeof(T)))), size_(size), map_(std::make_unique<bitset>(size)) {};


template<typename T>
allocator<T>::allocator(allocator const& other) :
allocator<T>(other.size_)
{
	for (size_t i=0; i < size_; ++i)
		if (other.map_->test(i))
		construct(ptr_ + i, other.ptr_[i]);
}

template<typename T>
allocator<T>::~allocator()
{ 
	destroy(ptr_, ptr_+size_);
	operator delete(ptr_); 
}

template<typename T>
auto allocator<T>::resize() -> void
{
	allocator<T> buff(size_ * 2 + (size_ == 0));
	for (size_t i = 0; i < size_; ++i) construct(buff.ptr_ + i, ptr_[i]);
	this->swap(buff);
}

template<typename T>
auto allocator<T>::construct(T * ptr, T const & value)->void 
{
	if (ptr >= ptr_&&ptr < ptr_ + size_&&map_->test(ptr - ptr_)) 
	{
		new(ptr)T(value);
		map_->set(ptr - ptr_);
	}
	else throw("error");
}

template<typename T>
auto allocator<T>::destroy(T * ptr) -> void 
{
	if(ptr>=ptr_&&ptr<=ptr_+this->count())
	{
		if (!map_->test(ptr-ptr_))
		{
		ptr->~T();
		map_->reset(ptr-ptr_);
		}
	}
	else throw("error");
}

template<typename T>
auto allocator<T>::destroy(T * first, T * last) -> void
{	
	if(first>=ptr_&&last<=ptr_+this->count())
	for (; first != last; ++first) 
	{
		destroy(&*first);
	}
}

template<typename T>
auto allocator<T>::get()-> T* 
{
	return ptr_; 
}

template<typename T>
auto allocator<T>::get() const -> T const * 
{ 
	return ptr_; 
}

template<typename T>
auto allocator<T>::count() const -> size_t
{
	return map_->counter();
}

template<typename T>
auto allocator<T>::full() const -> bool
{
	return map_->counter()==size_;
}

template<typename T>
auto allocator<T>::empty() const -> bool
{
	return map_->counter()==0;
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void 
{
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}

/*=====================================================================================*/

template <typename T>
class stack : private allocator<T>
{
public:
	explicit
	stack(size_t size = 0);								//noexcept
	auto pop() -> void;
	auto top() -> T &;
	auto top() const -> T const &;
	auto push(T const &vaule) -> void;							//strong
	auto operator=(stack const & right)->stack &;					//strong
	auto empty() const -> bool; 							//noexcept
	auto count() const -> size_t;
private:
	allocator<T> allocator_;
	auto throw_is_empty() const -> void;
};

template <typename T>
size_t stack<T>::count() const
{
	return allocator_.count();
}
template <typename T>
stack<T>::stack(size_t size):allocator_(size){}

template <typename T>
void stack<T>::push(T const &item) 
{
	if (allocator_.full()) allocator_.resize(); 
	allocator_.construct(allocator_.get() + this->count(), item);
}
template<typename T>
void stack<T>::pop() 
{
	if (this->count() > 0) 
		allocator_.destroy(allocator_.get() + (this->count()-1));
	else 
		throw_is_empty();
}

template<typename T>
auto stack<T>::top() -> T &
{
	if (allocator_.count() == 0) {
		throw_is_empty();
	}
	return (*(allocator_.get() + allocator_.count() - 1));
}

template<typename T>
auto stack<T>::top() const -> T const &
{
	if (allocator_.count() == 0) 
	{
		throw_is_empty();
	}
	return (*(allocator_.get() + allocator_.count() - 1));
}

template<typename T>
auto stack<T>::operator=(stack const & right)-> stack &
{ 
	if (this != &right) 
	{
		(allocator<T>(right.allocator_)).swap(allocator_);
	}
	return *this;
}

template<typename T>
auto stack<T>::empty() const -> bool 
{
	return allocator_.empty(); 
}

template<typename T>
auto stack<T>::throw_is_empty() const -> void
{
		throw("Stack is empty!");
}
#endif
