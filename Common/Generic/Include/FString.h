#pragma once

/*
 * Fundamental String Class
 */

template <typename T>
class FStringT
{
public:
	typedef T ElementType;
	typedef T CharType;

public:
	FStringT(UInt32 MaxLength = 0);
	FStringT(const T* pszString, UInt32 MaxLength = 0);
	FStringT(T Char);
	FStringT(T Char, UInt32 CharCount);

	FStringT(const FStringT& String);

	FStringT(FStringT&& String);

	~FStringT();

	void MoveFrom(FStringT& String);

public:
	      T* GetData();
	const T* GetData() const;

	// WITHOUT the terminating null character
	UInt32 GetLength() const;

	// including the terminating null character
	UInt32 GetMaxLength() const;

	void Clear();

	bool Resize(UInt32 NewMaxLength);

	void Empty();
	bool IsEmpty() const;

	bool IsValidIndex(UInt32 Index) const;

public:
	bool Set(const FStringT& String, Int32 SetLength = -1);
	bool Set(const T* pszString, Int32 SetLength = -1);

	FStringT<T>& Append(const FStringT& String, Int32 AppendLength = -1);
	FStringT<T>& Append(const T* pszString, Int32 AppendLength = -1);

	FStringT<T>& AppendChar(T Char);

	Int32 Compare(const FStringT& String, PFStringCaseSens::Enum CaseSens = PFStringCaseSens::Sensitive, Int32 CompareLength = -1) const;
	Int32 Compare(const T* pszString, PFStringCaseSens::Enum CaseSens = PFStringCaseSens::Sensitive, Int32 CompareLength = -1) const;

public:
	// operator=
	FStringT<T>& operator=(const FStringT& String);
	FStringT<T>& operator=(FStringT&& String);
	FStringT<T>& operator=(const T* pszString);
	FStringT<T>& operator=(T Char);

	// operator+=
	FStringT<T>& operator+=(const FStringT& String);
	FStringT<T>& operator+=(const T* pszString);
	FStringT<T>& operator+=(T Char);

	// operator+
	friend FStringT<T> operator+(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		FStringT<T> Result;
		Result.Construct(Lhs.pszData, Lhs.GetLength() + Rhs.GetLength() + 1);
		PFStringT<T>::Append(Result.pszData, Result.MaxLength, Rhs.pszData);
		return Result;
	}

	friend FStringT<T> operator+(const FStringT<T>& Lhs, const T* pszRhs)
	{
		FStringT<T> Result;
		Result.Construct(Lhs.pszData, Lhs.GetLength() + PFStringT<T>::Length(pszRhs) + 1);
		PFStringT<T>::Append(Result.pszData, Result.MaxLength, pszRhs);
		return Result;
	}

	friend FStringT<T> operator+(const T* pszLhs, const FStringT<T>& Rhs)
	{
		FStringT<T> Result;
		Result.Construct(pszLhs, PFStringT<T>::Length(pszLhs) + Rhs.GetLength() + 1);
		PFStringT<T>::Append(Result.pszData, Result.MaxLength, Rhs.pszData);
		return Result;
	}

	// operator==
	friend bool operator==(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) == 0;
	}

	friend bool operator==(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) == 0;
	}

	friend bool operator==(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) == 0;
	}

	// operator!=
	friend bool operator!=(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) != 0;
	}

	friend bool operator!=(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) != 0;
	}

	friend bool operator!=(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) != 0;
	}

	// operator<
	friend bool operator<(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) < 0;
	}

	friend bool operator<(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) < 0;
	}

	friend bool operator<(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) < 0;
	}

	// operator<=
	friend bool operator<=(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) <= 0;
	}

	friend bool operator<=(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) <= 0;
	}

	friend bool operator<=(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) <= 0;
	}

	// operator>
	friend bool operator>(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) > 0;
	}

	friend bool operator>(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) > 0;
	}

	friend bool operator>(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) > 0;
	}

	// operator>=
	friend bool operator>=(const FStringT<T>& Lhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, Rhs.pszData) >= 0;
	}

	friend bool operator>=(const FStringT<T>& Lhs, const T* pszRhs)
	{
		return PFStringT<T>::Compare(Lhs.pszData, pszRhs) >= 0;
	}

	friend bool operator>=(const T* pszLhs, const FStringT<T>& Rhs)
	{
		return PFStringT<T>::Compare(pszLhs, Rhs.pszData) >= 0;
	}

	// operator[]
	T& operator[](Int32 nIndex);
	const T& operator[](Int32 nIndex) const;

	operator T*();
	operator const T*() const;

public:
	// ranged for support
	using iterator = T*;
	using const_iterator = const T*;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

private:
	T* pszData;
	UInt32 MaxLength;

private:
	bool Construct(const T* pszString, UInt32 NewLength);
	void Destruct();
};

typedef FStringT< CHAR> FStringA;
typedef FStringT<WCHAR> FStringW;
typedef FStringT<TCHAR> FString;
