#include <Platform.h>

#include <gtest/gtest.h>

namespace
{

TEST(String, Length)
{
	EXPECT_EQ(7, PFString::Length(TEXT("test123")));
	EXPECT_EQ(7, PFString::Strlen(TEXT("test123")));

	EXPECT_EQ(4, PFString::Length(TEXT("test123"), 4));
	EXPECT_EQ(4, PFString::Strnlen(TEXT("test123"), 4));
}

TEST(String, Copy)
{
	TCHAR szString[256] = { 0 };

	EXPECT_EQ(true, PFString::Copy(szString, TEXT("test123")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123")));

	EXPECT_EQ(true, PFString::Copy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123")));

	EXPECT_EQ(false, PFString::Copy(szString, 4, TEXT("123test")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("123")));

	EXPECT_EQ(true, PFString::Copy(szString, PF_ARRAY_LENGTH(szString), TEXT("123test"), 3));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("123")));

	PFString::Strcpy(szString, TEXT("test123"));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123")));

	PFString::Strcpy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123"));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123")));

	PFString::Strncpy(szString, TEXT("123test"), 3);
	EXPECT_EQ(0, _tcscmp(szString, TEXT("123")));

	PFString::Strncpy(szString, PF_ARRAY_LENGTH(szString), TEXT("123test"), 3);
	EXPECT_EQ(0, _tcscmp(szString, TEXT("123")));
}

TEST(String, Append)
{
	TCHAR szString[256] = { 0 };

	EXPECT_EQ(true, PFString::Copy(szString, TEXT("test123")));
	EXPECT_EQ(true, PFString::Append(szString, TEXT("append")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123append")));

	EXPECT_EQ(true, PFString::Copy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123")));
	EXPECT_EQ(true, PFString::Append(szString, PF_ARRAY_LENGTH(szString), TEXT("append")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123append")));

	EXPECT_EQ(true, PFString::Copy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123")));
	EXPECT_EQ(false, PFString::Append(szString, 10, TEXT("append")));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123ap")));

	EXPECT_EQ(true, PFString::Copy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123")));
	EXPECT_EQ(true, PFString::Append(szString, PF_ARRAY_LENGTH(szString), TEXT("append"), 2));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123ap")));

	PFString::Strcpy(szString, TEXT("test123"));
	PFString::Strcat(szString, TEXT("append"));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123append")));

	PFString::Strcpy(szString, TEXT("test123"));
	PFString::Strncat(szString, TEXT("append"), 2);
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123ap")));

	PFString::Strcpy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123"));
	PFString::Strcat(szString,PF_ARRAY_LENGTH(szString), TEXT("append"));
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123append")));

	PFString::Strcpy(szString, PF_ARRAY_LENGTH(szString), TEXT("test123"));
	PFString::Strncat(szString, PF_ARRAY_LENGTH(szString), TEXT("append"), 2);
	EXPECT_EQ(0, _tcscmp(szString, TEXT("test123ap")));
}

TEST(String, Compare)
{
	EXPECT_EQ(0, PFString::Compare(TEXT("test123"), TEXT("test123")));
	EXPECT_EQ(1, PFString::Compare(TEXT("test132"), TEXT("test123")));
	EXPECT_EQ(-1, PFString::Compare(TEXT("test123"), TEXT("test132")));
	EXPECT_EQ(-1, PFString::Compare(TEXT("test1"), TEXT("test10")));
	EXPECT_EQ(1, PFString::Compare(TEXT("test10"), TEXT("test1")));

	EXPECT_EQ(0, PFString::Compare(TEXT("test123"), TEXT("test321"), PFStringCaseSens::Sensitive, 4));

	EXPECT_EQ(1, PFString::Compare(TEXT("test123‚Ä‚·‚Æ"), TEXT("TEST123‚Ä‚·‚Æ"), PFStringCaseSens::Sensitive));
	EXPECT_EQ(-1, PFString::Compare(TEXT("TEST123‚Ä‚·‚Æ"), TEXT("test123‚Ä‚·‚Æ"), PFStringCaseSens::Sensitive));
	EXPECT_EQ(0, PFString::Compare(TEXT("test123‚Ä‚·‚Æ"), TEXT("TEST123‚Ä‚·‚Æ"), PFStringCaseSens::Ignore));

	EXPECT_EQ(1, PFString::Compare(TEXT("test3"), TEXT("test10")));
	EXPECT_EQ(-1, PFString::Compare(TEXT("test10"), TEXT("test3")));
	EXPECT_EQ(1, PFString::Compare(TEXT("test03"), TEXT("test010")));

	EXPECT_EQ(0, PFString::Strcmp(TEXT("test123"), TEXT("test123")));
	EXPECT_EQ(0, PFString::Strncmp(TEXT("test123"), TEXT("test321"), 4));
	EXPECT_EQ(0, PFString::Stricmp(TEXT("test123‚Ä‚·‚Æ"), TEXT("TEST123‚Ä‚·‚Æ")));
	EXPECT_EQ(0, PFString::Strnicmp(TEXT("test123"), TEXT("TEST321"), 4));
}

TEST(String, Find)
{
	EXPECT_EQ(-1, PFString::Find(TEXT("test123test"), TEXT("45")));
	EXPECT_EQ(-1, PFString::Find(TEXT("45"), TEXT("test123test")));

	EXPECT_EQ(5, PFString::Find(TEXT("test123test"), TEXT("23te")));

	EXPECT_EQ(-1, PFString::Find(TEXT("test123test"), TEXT("23TE")));
	EXPECT_EQ(5, PFString::Find(TEXT("test123test"), TEXT("23TE"), PFStringCaseSens::Ignore));

	EXPECT_EQ(7, PFString::Find(TEXT("test123test"), TEXT("test"), PFStringCaseSens::Sensitive, 4));

	EXPECT_EQ(nullptr, PFString::Strstr(TEXT("test123test"), TEXT("45")));
	EXPECT_EQ(0, _tcscmp(TEXT("23test"), PFString::Strstr(TEXT("test123test"), TEXT("23te"))));
}

TEST(String, FindLast)
{
	EXPECT_EQ(-1, PFString::FindLast(TEXT("test123test"), TEXT("45")));
	EXPECT_EQ(-1, PFString::FindLast(TEXT("45"), TEXT("test123test")));

	EXPECT_EQ(14, PFString::FindLast(TEXT("test123test123test123"), TEXT("test")));

	EXPECT_EQ(-1, PFString::FindLast(TEXT("test123test123test123"), TEXT("TEST")));
	EXPECT_EQ(14, PFString::FindLast(TEXT("test123test123test123"), TEXT("TEST"), PFStringCaseSens::Ignore));

	EXPECT_EQ(18, PFString::FindLast(TEXT("test123test123test123"), TEXT("123")));

	EXPECT_EQ(12, PFString::FindLast(TEXT("‚Ä‚·‚Æ123‚Ä‚·‚Æ123‚Ä‚·‚Æ123"), TEXT("‚Ä‚·‚Æ")));

	EXPECT_EQ(0, PFString::FindLast(TEXT("test123test"), TEXT("test"), PFStringCaseSens::Sensitive, 4));

	EXPECT_EQ(nullptr, PFString::Strrstr(TEXT("test123test"), TEXT("45")));
	EXPECT_EQ(0, _tcscmp(TEXT("test23"), PFString::Strrstr(TEXT("test12test23"), TEXT("test"))));
}

TEST(String, FindChar)
{
	EXPECT_EQ(-1, PFString::FindChar(TEXT("test123test"), _T('l')));
	EXPECT_EQ(1, PFString::FindChar(TEXT("test123test"), _T('e')));

	EXPECT_EQ(8, PFString::FindChar(TEXT("test123test"), _T('e'), 4));
	EXPECT_EQ(-1, PFString::FindChar(TEXT("test123test"), _T('e'), 11));

	EXPECT_EQ(nullptr, PFString::Strchr(TEXT("test123test"), _T('l')));
	EXPECT_EQ(0, _tcscmp(TEXT("est123test"), PFString::Strchr(TEXT("test123test"), _T('e'))));
}

TEST(String, FindLastChar)
{
	EXPECT_EQ(-1, PFString::FindLastChar(TEXT("test123test"), _T('|')));
	EXPECT_EQ(8, PFString::FindLastChar(TEXT("test123test"), _T('e')));

	EXPECT_EQ(1, PFString::FindLastChar(TEXT("test123test"), _T('e'), 4));
	EXPECT_EQ(-1, PFString::FindLastChar(TEXT("test123test"), _T('e'), 11));

	EXPECT_EQ(nullptr, PFString::Strrchr(TEXT("test123test"), _T('|')));
	EXPECT_EQ(0, _tcscmp(TEXT("est"), PFString::Strrchr(TEXT("test123test"), _T('e'))));
}

}
