#pragma once

template<typename TFrom, typename TTor>
class TIterator;

template<typename TFrom, typename TTo, typename TIter = TIterator<TFrom, TTo>>
class TContainer
{
public:
	TContainer(TFrom& from);
	TIter begin();
	TIter end();

private:
	TFrom& From;
	typename TTo::TUserData UserData;
};

template<typename TFrom, typename TTo>
class TIterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = TTo;
	using difference_type = int;
	using pointer = TTo*;
	using reference = TTo&;

public:
	TIterator(TFrom* from, typename TTo::TUserData* userData);
	TIterator& operator++();

	bool operator==(const TIterator& rhs) const;
	bool operator!=(const TIterator& rhs) const; 
	TTo operator*();

public:
	TFrom* From = nullptr;
	typename TTo::TUserData* UserData = nullptr;

	unsigned Index = 0;
	unsigned Count;
};

template<typename TFrom, typename TTo, typename TIter>
TContainer<TFrom, TTo, TIter>::TContainer(TFrom& from)
	: From(from)
{
	From.Init(&UserData);
}

template<typename TFrom, typename TTo, typename TIter>
TIter TContainer<TFrom, TTo, TIter>::begin()
{
	return TIter(&From, &UserData);
}

template<typename TFrom, typename TTo, typename TIter>
TIter TContainer<TFrom, TTo, TIter>::end()
{
	auto it = TIter(&From, &UserData);
	it.Index = it.Count;
	return it;
}

template<typename TFrom, typename TTo>
TIterator<TFrom, TTo>::TIterator(TFrom* from, typename TTo::TUserData* userData)
	: From(from)
	, UserData(userData)
	, Count(from->Count(UserData))
{
}

template<typename TFrom, typename TTo>
TIterator<TFrom, TTo>& TIterator<TFrom, TTo>::operator++()
{
	++this->Index;
	return *this;
}

template<typename TFrom, typename TTo>
bool TIterator<TFrom, TTo>::operator==(const TIterator& rhs) const
{
	return Index == rhs.Index;
}

template<typename TFrom, typename TTo>
bool TIterator<TFrom, TTo>::operator!=(const TIterator& rhs) const
{
	return !(*this == rhs);
}

template<typename TFrom, typename TTo>
TTo TIterator<TFrom, TTo>::operator*()
{
	return From->Get(UserData, Index);
}
