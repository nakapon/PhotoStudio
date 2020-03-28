#include <Platform.h>

#include <FString.h>

template <typename T>
FStringT<T>::FStringT(UInt32 MaxLength)
	: pszData(nullptr), MaxLength(0)
{
	Construct(nullptr, MaxLength);
}

template <typename T>
FStringT<T>::FStringT(const T* pszString, UInt32 MaxLength)
	: pszData(nullptr), MaxLength(0)
{
	MaxLength = PFMath::Max(PFStringT<T>::Length(pszString) + 1, MaxLength);

	Construct(pszString, MaxLength);
}

template <typename T>
FStringT<T>::FStringT(T Char)
	: pszData(nullptr), MaxLength(0)
{
	Construct(nullptr, 2);
	pszData[0] = Char;
}

template <typename T>
FStringT<T>::FStringT(T Char, UInt32 CharCount)
	: pszData(nullptr), MaxLength(0)
{
	Construct(nullptr, CharCount + 1);
	for(UInt32 i = 0; i < CharCount; i++)
	{
		pszData[i] = Char;
	}
}

template <typename T>
FStringT<T>::FStringT(const FStringT& String)
	: pszData(nullptr), MaxLength(0)
{
	Construct(String.pszData, String.MaxLength);
}

template <typename T>
FStringT<T>::FStringT(FStringT&& String)
	: pszData(nullptr), MaxLength(0)
{
	MoveFrom(String);
}

template <typename T>
FStringT<T>::~FStringT()
{
	Destruct();
}

template <typename T>
T* FStringT<T>::GetData()
{
	return pszData;
}

template <typename T>
const T* FStringT<T>::GetData() const
{
	return pszData;
}

template <typename T>
UInt32 FStringT<T>::GetLength() const
{
	return PFStringT<T>::Length(pszData);
}

template <typename T>
UInt32 FStringT<T>::GetMaxLength() const
{
	return MaxLength;
}

template <typename T>
void FStringT<T>::Clear()
{
	Construct(nullptr, 0);
}

template <typename T>
bool FStringT<T>::Resize(UInt32 NewMaxLength)
{
	NewMaxLength = PFMath::Max(1u, NewMaxLength);

	if(MaxLength == NewMaxLength)
		return false;

	T* pszOld = pszData;

	pszData = new T[NewMaxLength];
	PFMemory::Zero(pszData, sizeof(T) * NewMaxLength);

	MaxLength = NewMaxLength;

	if(pszOld != nullptr)
	{
		PFStringT<T>::Copy(pszData, MaxLength, pszOld);

		delete[] pszOld;
		pszOld = nullptr;
	}

	return true;
}

template <typename T>
void FStringT<T>::Empty()
{
	if(pszData != nullptr)
	{
		pszData[0] = (T)'\0';
	}
}

template <typename T>
bool FStringT<T>::IsEmpty() const
{
	return (pszData == nullptr || pszData[0] == (T)'\0');
}

template <typename T>
bool FStringT<T>::IsValidIndex(UInt32 uiIndex) const
{
	return uiIndex < GetLength();
}

template <typename T>
bool FStringT<T>::Set(const FStringT& String, Int32 SetLength)
{
	return Set(String.pszData, SetLength);
}

template <typename T>
bool FStringT<T>::Set(const T* pszString, Int32 SetLength)
{
	UInt32 Length = PFStringT<T>::Length(pszString);
	if(0 < Length)
	{
		Length = PFMath::Min(Length, (UInt32)SetLength);
	}

	return Construct(pszString, Length + 1);
}

template <typename T>
FStringT<T>& FStringT<T>::Append(const FStringT& String, Int32 AppendLength)
{
	Append(String.pszData, AppendLength);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::Append(const T* pszString, Int32 AppendLength)
{
	if(pszString != nullptr)
	{
		UInt32 Length = PFStringT<T>::Length(pszString);
		if(0 < AppendLength)
		{
			Length = PFMath::Min(Length, (UInt32)AppendLength);
		}
		Resize(GetLength() + Length + 1);
		PFStringT<T>::Append(pszData, MaxLength, pszString);
	}
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::AppendChar(T Char)
{
	Resize(GetLength() + 1 + 1);
	PFStringT<T>::AppendChar(pszData, MaxLength, Char);
	return *this;
}

template <typename T>
Int32 FStringT<T>::Compare(const FStringT& String, PFStringCaseSens::Enum CaseSens, Int32 CompareLength) const
{
	return PFStringT<T>::Compare(pszData, String.pszData, CaseSens, CompareLength);
}

template <typename T>
Int32 FStringT<T>::Compare(const T* pszString, PFStringCaseSens::Enum CaseSens, Int32 CompareLength) const
{
	return PFStringT<T>::Compare(pszData, pszString, CaseSens, CompareLength);
}

template <typename T>
FStringT<T>& FStringT<T>::operator=(const FStringT& String)
{
	Set(String.pszData);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator=(FStringT&& String)
{
	MoveFrom(String);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator=(const T* pszString)
{
	Set(pszString);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator=(T Char)
{
	Construct(nullptr, 2);
	pszData[0] = Char;
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator+=(const FStringT& String)
{
	Append(String.pszData);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator+=(const T* pszString)
{
	Append(pszString);
	return *this;
}

template <typename T>
FStringT<T>& FStringT<T>::operator+=(T Char)
{
	AppendChar(Char);
	return *this;
}

template <typename T>
T& FStringT<T>::operator[](Int32 nIndex)
{
	return this->pszData[nIndex];
}

template <typename T>
const T& FStringT<T>::operator[](Int32 nIndex) const
{
	return this->pszData[nIndex];
}

template <typename T>
FStringT<T>::operator T*()
{
	if(pszData == nullptr)
		return nullptr;

	return &pszData[0];
}

template <typename T>
FStringT<T>::operator const T*() const
{
	if(pszData == nullptr)
		return nullptr;

	return &pszData[0];
}

template <typename T>
typename FStringT<T>::iterator FStringT<T>::begin()
{
	return pszData;
}

template <typename T>
typename FStringT<T>::iterator FStringT<T>::end()
{
	return pszData + GetLength();
}

template <typename T>
typename FStringT<T>::const_iterator FStringT<T>::begin() const
{
	return pszData;
}

template <typename T>
typename FStringT<T>::const_iterator FStringT<T>::end() const
{
	return pszData + GetLength();
}

template <typename T>
bool FStringT<T>::Construct(const T* pszString, UInt32 NewLength)
{
	NewLength = PFMath::Max(1u, NewLength);

	if(NewLength != MaxLength)
	{
		PF_SAFE_DELETE_ARRAY(pszData);

		pszData = new T[NewLength];
		MaxLength = NewLength;
	}

	PFMemory::Zero(pszData, sizeof(T) * NewLength);

	if(pszString != nullptr)
	{
		PFStringT<T>::Copy(pszData, MaxLength, pszString);
	}

	return true;
}

template <typename T>
void FStringT<T>::Destruct()
{
	PF_SAFE_DELETE_ARRAY(pszData);
	MaxLength = 0;
}

template <typename T>
void FStringT<T>::MoveFrom(FStringT& String)
{
	if(this != &String)
	{
		Destruct();

		pszData = String.pszData;
		MaxLength = String.MaxLength;

		String.pszData = nullptr;
		String.MaxLength = 0;
	}
}

// instantiation
template class FStringT< CHAR>;
template class FStringT<WCHAR>;
