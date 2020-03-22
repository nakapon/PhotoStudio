#pragma once

#include <algorithm>

namespace PFStringCaseSens
{
	enum Enum
	{
		Sensitive = 0,
		Ignore,
	};
}

template <typename T>
struct TGenericPlatformString
{
	static UInt32 Length(const T* pszString);
	static UInt32 Length(const T* pszString, SIZE_T MaxLength);

	template <SIZE_T MaxLength>
	static inline bool Copy(T (&pszDst)[MaxLength], const T* pszSrc, Int32 CopyLength = -1);
	static bool Copy(T* pszDst, SIZE_T MaxLength, const T* pszSrc, Int32 CopyLength = -1);

	template <SIZE_T MaxLength>
	static inline bool Append(T (&pszDst)[MaxLength], const T* pszSrc, Int32 AppendLength = -1);
	static bool Append(T* pszDst, SIZE_T MaxLength, const T* pszSrc, Int32 AppendLength = -1);

	static Int32 Compare(const T* pszString1, const T* pszString2, PFStringCaseSens::Enum CaseSens = PFStringCaseSens::Sensitive, Int32 CompareLength = -1);

	static Int32 Find(const T* pszString, const T* pszFind, PFStringCaseSens::Enum CaseSens = PFStringCaseSens::Sensitive, Int32 StartIndex = -1);
	static Int32 FindLast(const T* pszString, const T* pszFind, PFStringCaseSens::Enum CaseSens = PFStringCaseSens::Sensitive, Int32 StartIndex = -1);

	static Int32 FindChar(const T* pszString, T Find, Int32 StartIndex = -1);
	static Int32 FindLastChar(const T* pszString, T Find, Int32 StartIndex = -1);

	// C-Like
	static UInt32 Strlen(const T* pszString);
	static UInt32 Strnlen(const T* pszString, SIZE_T MaxLength);

	static T* Strcpy(T* pszDst, const T* pszSrc);
	static T* Strcpy(T* pszDst, SIZE_T MaxLength, const T* pszSrc);
	static T* Strncpy(T* pszDst, const T* pszSrc, UInt32 CopyLength);
	static T* Strncpy(T* pszDst, SIZE_T MaxLength, const T* pszSrc, UInt32 CopyLength);

	static T* Strcat(T* pszDst, const T* pszSrc);
	static T* Strcat(T* pszDst, SIZE_T MaxLength, const T* pszSrc);
	static T* Strncat(T* pszDst, const T* pszSrc, UInt32 AppendLength);
	static T* Strncat(T* pszDst, SIZE_T MaxLength, const T* pszSrc, UInt32 AppendLength);

	static Int32 Strcmp(const T* pszString1, const T* pszString2);
	static Int32 Strncmp(const T* pszString1, const T* pszString2, UInt32 CompareLength);
	static Int32 Stricmp(const T* pszString1, const T* pszString2);
	static Int32 Strnicmp(const T* pszString1, const T* pszString2, UInt32 CompareLength);

	static const T* Strstr(const T* pszString, const T* pszFind);
	static       T* Strstr(T* pszString, const T* pszFind);
	static const T* Strrstr(const T* pszString, const T* pszFind);
	static       T* Strrstr(T* pszString, const T* pszFind);

	static const T* Strchr(const T* pszString, T Find);
	static       T* Strchr(T* pszString, T Find);
	static const T* Strrchr(const T* pszString, T Find);
	static       T* Strrchr(T* pszString, T Find);
};

// Implementation
template <typename T>
UInt32 TGenericPlatformString<T>::Length(const T* pszString)
{
	if(pszString == nullptr)
		return 0;

	for(UInt32 i = 0; i < (UInt32)-1; i++)
	{
		if(pszString[i] == (T)'\0')
			return i;
	}

	return 0;
}

template <typename T>
UInt32 TGenericPlatformString<T>::Length(const T* pszString, SIZE_T MaxLength)
{
	if(pszString == nullptr)
		return 0;

	for(UInt32 i = 0; i < (UInt32)MaxLength; i++)
	{
		if(pszString[i] == (T)'\0')
			return i;
	}

	return (UInt32)MaxLength;
}

template <typename T>
template <SIZE_T MaxLength>
inline bool TGenericPlatformString<T>::Copy(T (&pszDst)[MaxLength], const T* pszSrc, Int32 CopyLength)
{
	return TGenericPlatformString<T>::Copy(pszDst, MaxLength, pszSrc, CopyLength);
}

template <typename T>
bool TGenericPlatformString<T>::Copy(T* pszDst, SIZE_T MaxLength, const T* pszSrc, Int32 CopyLength)
{
	if(pszDst == nullptr || MaxLength == 0 || pszSrc == nullptr)
		return false;

	pszDst[0] = (T)'\0';

	return TGenericPlatformString<T>::Append(pszDst, MaxLength, pszSrc, CopyLength);
}

template <typename T>
template <SIZE_T MaxLength>
inline bool TGenericPlatformString<T>::Append(T (&pszDst)[MaxLength], const T* pszSrc, Int32 AppendLength)
{
	return TGenericPlatformString<T>::Append(pszDst, MaxLength, pszSrc, AppendLength);
}

template <typename T>
bool TGenericPlatformString<T>::Append(T* pszDst, SIZE_T MaxLength, const T* pszSrc, Int32 AppendLength)
{
	UInt32 i, j;
	UInt32 MaxIndex;

	if(pszDst == nullptr || MaxLength == 0 || pszSrc == nullptr)
		return false;

	MaxIndex = (UInt32)MaxLength - 1;
	pszDst[MaxIndex] = (T)'\0';

	for(i = 0; i < MaxIndex; i++)
	{
		if(pszDst[i] == (T)'\0')
			break;
	}

	if(i == MaxIndex)
		return false;

	for(j = 0; (i + j) < MaxIndex; j++)
	{
		pszDst[i + j] = pszSrc[j];
		if(pszSrc[j] == (T)'\0' || (UInt32)AppendLength <= j)
		{
			pszDst[i + j] = (T)'\0';
			return true;
		}
	}

	return false;
}

template <typename T>
Int32 TGenericPlatformString<T>::Compare(const T* pszString1, const T* pszString2, PFStringCaseSens::Enum CaseSens, Int32 CompareLength)
{
	if(pszString1 == nullptr || pszString2 == nullptr)
		return -1;

	UInt32 Length = (UInt32)-1;

	if(0 < CompareLength)
	{
		Length = (UInt32)CompareLength;
	}

	for(UInt32 i = 0; i < Length; i++)
	{
		T Char1 = pszString1[i];
		T Char2 = pszString2[i];

		if(Char1 == (T)'\0' && Char2 == (T)'\0')
			return 0;

		if(CaseSens == PFStringCaseSens::Ignore)
		{
			Char1 = tolower(Char1);
			Char2 = tolower(Char2);
		}

		if(Char1 < Char2)
			return -1;

		if(Char2 < Char1)
			return 1;
	}

	return 0;
}

template <typename T>
Int32 TGenericPlatformString<T>::Find(const T* pszString, const T* pszFind, PFStringCaseSens::Enum CaseSens, Int32 StartIndex)
{
	UInt32 StringLength, FindLength;
	UInt32 StringOffset = 0;
	UInt32 SearchLength;

	if(pszString == nullptr || pszFind == nullptr)
		return -1;

	StringLength = TGenericPlatformString<T>::Length(pszString);
	if(0 <= StartIndex && StartIndex < (Int32)StringLength)
	{
		StringOffset = StartIndex;
		pszString = &pszString[StringOffset];
		StringLength = StringLength - StringOffset;
	}

	FindLength = TGenericPlatformString<T>::Length(pszFind);
	if(StringLength == 0 || FindLength == 0 || StringLength < FindLength)
		return -1;

	SearchLength = StringLength - FindLength + 1;

	for(UInt32 i = 0; i < SearchLength; i++)
	{
		bool bNotEqual = false;

		for(UInt32 j = 0; j < FindLength; j++)
		{
			T Char1 = pszString[i + j];
			T Char2 = pszFind[j];

			if(CaseSens == PFStringCaseSens::Ignore)
			{
				Char1 = tolower(Char1);
				Char2 = tolower(Char2);
			}

			if(Char1 != Char2)
			{
				bNotEqual = true;
				break;
			}
		}

		if(!bNotEqual)
			return (Int32)(i + StringOffset);
	}

	return -1;
}

template <typename T>
Int32 TGenericPlatformString<T>::FindLast(const T* pszString, const T* pszFind, PFStringCaseSens::Enum CaseSens, Int32 StartIndex)
{
	UInt32 StringLength, FindLength;
	UInt32 SearchLength;

	if(pszString == nullptr || pszFind == nullptr)
		return -1;

	StringLength = TGenericPlatformString<T>::Length(pszString);
	if(0 <= StartIndex && StartIndex < (Int32)StringLength)
	{
		StringLength = StringLength - (UInt32)StartIndex;
	}

	FindLength = TGenericPlatformString<T>::Length(pszFind);
	if(StringLength == 0 || FindLength == 0 || StringLength < FindLength)
		return -1;

	SearchLength = StringLength - FindLength + 1;

	for(UInt32 i = 0; i < SearchLength; i++)
	{
		UInt32 Index = StringLength - FindLength - i;

		bool bNotEqual = false;

		for(UInt32  j = 0; j < FindLength; j++)
		{
			T Char1 = pszString[Index + j];
			T Char2 = pszFind[j];

			if(CaseSens == PFStringCaseSens::Ignore)
			{
				Char1 = tolower(Char1);
				Char2 = tolower(Char2);
			}

			if(Char1 != Char2)
			{
				bNotEqual = true;
				break;
			}
		}

		if(!bNotEqual)
			return (Int32)Index;
	}

	return -1;
}

template <typename T>
Int32 TGenericPlatformString<T>::FindChar(const T* pszString, T Find, Int32 StartIndex)
{
	if(pszString == nullptr)
		return -1;

	UInt32 Length = TGenericPlatformString<T>::Length(pszString);
	if((Int32)Length <= StartIndex)
		return -1;

	StartIndex = std::max(0, StartIndex);

	for(Int32 i = StartIndex; ; i++)
	{
		T Char = pszString[i];
		if(Char == (T)'\0')
			return -1;

		if(Char == Find)
			return i;
	}
}

template <typename T>
Int32 TGenericPlatformString<T>::FindLastChar(const T* pszString, T Find, Int32 StartIndex)
{
	if(pszString == nullptr)
		return -1;

	UInt32 Length = TGenericPlatformString<T>::Length(pszString);
	if((Int32)Length <= StartIndex)
		return -1;

	if(0 < StartIndex)
	{
		Length -= StartIndex;
	}

	for(UInt32 i = 0; i < Length; i++)
	{
		UInt32 Index = Length - 1 - i;

		T Char = pszString[Index];
		if(Char == Find)
			return (Int32)Index;
	}

	return -1;
}

template <typename T>
UInt32 TGenericPlatformString<T>::Strlen(const T* pszString)
{
	return TGenericPlatformString<T>::Length(pszString);
}

template <typename T>
UInt32 TGenericPlatformString<T>::Strnlen(const T* pszString, SIZE_T MaxLength)
{
	return TGenericPlatformString<T>::Length(pszString, MaxLength);
}

template <typename T>
T* TGenericPlatformString<T>::Strcpy(T* pszDst, const T* pszSrc)
{
	TGenericPlatformString<T>::Copy(pszDst, TGenericPlatformString<T>::Length(pszSrc) + 1, pszSrc);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strcpy(T* pszDst, SIZE_T MaxLength, const T* pszSrc)
{
	TGenericPlatformString<T>::Copy(pszDst, MaxLength, pszSrc);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strncpy(T* pszDst, const T* pszSrc, UInt32 CopyLength)
{
	TGenericPlatformString<T>::Copy(pszDst, CopyLength + 1, pszSrc, CopyLength);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strncpy(T* pszDst, SIZE_T MaxLength, const T* pszSrc, UInt32 CopyLength)
{
	TGenericPlatformString<T>::Copy(pszDst, MaxLength, pszSrc, CopyLength);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strcat(T* pszDst, const T* pszSrc)
{
	TGenericPlatformString<T>::Append(pszDst, TGenericPlatformString<T>::Length(pszDst) + TGenericPlatformString<T>::Length(pszSrc) + 1, pszSrc);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strcat(T* pszDst, SIZE_T MaxLength, const T* pszSrc)
{
	TGenericPlatformString<T>::Append(pszDst, MaxLength, pszSrc);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strncat(T* pszDst, const T* pszSrc, UInt32 AppendLength)
{
	TGenericPlatformString<T>::Append(pszDst, TGenericPlatformString<T>::Length(pszDst) + AppendLength + 1, pszSrc, AppendLength);
	return pszDst;
}

template <typename T>
T* TGenericPlatformString<T>::Strncat(T* pszDst, SIZE_T MaxLength, const T* pszSrc, UInt32 AppendLength)
{
	TGenericPlatformString<T>::Append(pszDst, MaxLength, pszSrc, AppendLength);
	return pszDst;
}

template <typename T>
Int32 TGenericPlatformString<T>::Strcmp(const T* pszString1, const T* pszString2)
{
	return TGenericPlatformString<T>::Compare(pszString1, pszString2, PFStringCaseSens::Sensitive);
}

template <typename T>
Int32 TGenericPlatformString<T>::Strncmp(const T* pszString1, const T* pszString2, UInt32 CompareLength)
{
	return TGenericPlatformString<T>::Compare(pszString1, pszString2, PFStringCaseSens::Sensitive, CompareLength);
}

template <typename T>
Int32 TGenericPlatformString<T>::Stricmp(const T* pszString1, const T* pszString2)
{
	return TGenericPlatformString<T>::Compare(pszString1, pszString2, PFStringCaseSens::Ignore);
}

template <typename T>
Int32 TGenericPlatformString<T>::Strnicmp(const T* pszString1, const T* pszString2, UInt32 CompareLength)
{
	return TGenericPlatformString<T>::Compare(pszString1, pszString2, PFStringCaseSens::Ignore, CompareLength);
}

template <typename T>
const T* TGenericPlatformString<T>::Strstr(const T* pszString, const T* pszFind)
{
	Int32 Ptr = TGenericPlatformString<T>::Find(pszString, pszFind);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
T* TGenericPlatformString<T>::Strstr(T* pszString, const T* pszFind)
{
	Int32 Ptr = TGenericPlatformString<T>::Find(pszString, pszFind);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
const T* TGenericPlatformString<T>::Strrstr(const T* pszString, const T* pszFind)
{
	Int32 Ptr = TGenericPlatformString<T>::FindLast(pszString, pszFind);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
T* TGenericPlatformString<T>::Strrstr(T* pszString, const T* pszFind)
{
	Int32 Ptr = TGenericPlatformString<T>::FindLast(pszString, pszFind);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
const T* TGenericPlatformString<T>::Strchr(const T* pszString, T Find)
{
	Int32 Ptr = TGenericPlatformString<T>::FindChar(pszString, Find);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
T* TGenericPlatformString<T>::Strchr(T* pszString, T Find)
{
	Int32 Ptr = TGenericPlatformString<T>::FindChar(pszString, Find);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
const T* TGenericPlatformString<T>::Strrchr(const T* pszString, T Find)
{
	Int32 Ptr = TGenericPlatformString<T>::FindLastChar(pszString, Find);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}

template <typename T>
T* TGenericPlatformString<T>::Strrchr(T* pszString, T Find)
{
	Int32 Ptr = TGenericPlatformString<T>::FindLastChar(pszString, Find);
	if(Ptr < 0)
		return nullptr;

	return &pszString[Ptr];
}
