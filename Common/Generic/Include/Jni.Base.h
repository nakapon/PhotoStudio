#pragma once

#include <jni.h>

namespace Jni
{
	class Base
	{
	public:
		static bool SetNativeHandle(JNIEnv* pEnv, jobject Object, LPCSTR pszFieldName, HANDLE hHandle);
		static HANDLE GetNativeHandle(JNIEnv* pEnv, jobject Object, LPCSTR pszFieldName);

		static jobject NewObject(JNIEnv* pEnv, LPCSTR pszClassName, LPCSTR pszCnsSig, ...);

	private:
		Base() = delete;
		~Base() = delete;
		Base(const Base&) = delete;
		Base& operator=(const Base&) = delete;
	};
}
