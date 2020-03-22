#include <Platform.h>

#include <StringConv.h>

static LPCSTR gs_szNullText = "";
static LPCWSTR gs_wszNullText = L"";

FStringConv::FStringConv()
	: m_pszString((LPSTR)gs_szNullText)
	, m_pwszString((LPWSTR)gs_wszNullText)
	, m_dwLength(0)
{
}

FStringConv::FStringConv(LPCSTR pszString)
	: m_pszString((LPSTR)gs_szNullText)
	, m_pwszString((LPWSTR)gs_wszNullText)
	, m_dwLength(0)
{
	*this = pszString;
}

FStringConv::FStringConv(LPCWSTR pwszString)
	: m_pszString((LPSTR)gs_szNullText)
	, m_pwszString((LPWSTR)gs_wszNullText)
	, m_dwLength(0)
{
	*this = pwszString;
}

FStringConv::FStringConv(const FStringConv& String)
	: m_pszString((LPSTR)gs_szNullText)
	, m_pwszString((LPWSTR)gs_wszNullText)
	, m_dwLength(0)
{
	if(String.m_pwszString != gs_wszNullText)
	{
		*this = String.m_pwszString;
	}
}

FStringConv::FStringConv(FStringConv&& String)
	: m_pszString((LPSTR)gs_szNullText)
	, m_pwszString((LPWSTR)gs_wszNullText)
	, m_dwLength(0)
{
	MoveFrom(String);
}

FStringConv::~FStringConv()
{
	this->Clear();
}

void FStringConv::Clear()
{
	if(this->m_pszString != gs_szNullText)
	{
		if(this->m_pszString != nullptr)
		{
			delete[] this->m_pszString;
			this->m_pszString = (LPSTR)gs_szNullText;
		}
	}

	if(this->m_pwszString != gs_wszNullText)
	{
		if(this->m_pwszString != nullptr)
		{
			delete[] this->m_pwszString;
			this->m_pwszString = (LPWSTR)gs_wszNullText;
		}
	}

	this->m_dwLength = 0;
}

DWORD FStringConv::GetLength() const
{
	return this->m_dwLength;
}

FStringConv& FStringConv::operator=(const FStringConv& String)
{
	if(String.m_pwszString != gs_wszNullText)
	{
		*this = String.m_pwszString;
	}

	return *this;
}

FStringConv& FStringConv::operator=(FStringConv&& String)
{
	this->MoveFrom(String);
	return *this;
}

FStringConv& FStringConv::operator=(LPCSTR pszString)
{
	this->Clear();

	if(pszString != nullptr)
	{
		DWORD dwLength = PFString::GetWideCharLength(pszString);

		this->m_pwszString = new WCHAR[dwLength];
		PFString::MultiByteToWideChar(this->m_pwszString, dwLength, pszString);

		this->m_dwLength = dwLength;
	}

	return *this;
}

FStringConv& FStringConv::operator=(LPCWSTR pwszString)
{
	this->Clear();

	if(pwszString != nullptr)
	{
		DWORD dwLength = (DWORD)PFStringW::Length(pwszString) + 1;

		this->m_pwszString = new WCHAR[dwLength];
		PFStringW::Copy(this->m_pwszString, dwLength, pwszString);

		this->m_dwLength = dwLength;
	}

	return *this;
}

FStringConv::operator LPCSTR()
{
	DWORD dwLength;

	if(this->m_pszString != gs_szNullText)
	{
		if(this->m_pszString != nullptr)
		{
			delete[] this->m_pszString;
			this->m_pszString = (LPSTR)gs_szNullText;
		}
	}

	dwLength = PFString::GetMultiByteLength(this->m_pwszString);

	this->m_pszString = new CHAR[dwLength];
	PFString::WideCharToMultiByte(this->m_pszString, dwLength, this->m_pwszString);

	this->m_dwLength = dwLength;

	return this->m_pszString;
}

FStringConv::operator LPCWSTR()
{
	return this->m_pwszString;
}

void FStringConv::MoveFrom(FStringConv& String)
{
	if(this != &String)
	{
		this->Clear();

		this->m_pwszString = String.m_pwszString;
		this->m_pszString = String.m_pszString;
		this->m_dwLength = String.m_dwLength;

		String.m_pwszString = (LPWSTR)gs_wszNullText;
		String.m_pszString = (LPSTR)gs_szNullText;
		String.m_dwLength = 0;
	}
}
