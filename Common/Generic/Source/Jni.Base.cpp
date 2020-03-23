#include <Platform.h>

#include <Jni.Base.h>

bool Jni::Base::SetNativeHandle(JNIEnv* pEnv, jobject Object, LPCSTR pszFieldName, HANDLE hHandle)
{
	jclass Class;
	jfieldID Field;

	if(pEnv == nullptr || Object == nullptr || pszFieldName == nullptr)
		return false;

	Class = pEnv->GetObjectClass(Object);
	if(Class == nullptr)
		return false;

	Field = pEnv->GetFieldID(Class, pszFieldName, "J");
	if(Field == nullptr)
		return false;

	pEnv->SetLongField(Object, Field, (jlong)hHandle);

	return true;
}

HANDLE Jni::Base::GetNativeHandle(JNIEnv* pEnv, jobject Object, LPCSTR pszFieldName)
{
	jclass Class;
	jfieldID Field;

	if(pEnv == nullptr || Object == nullptr || pszFieldName == nullptr)
		return nullptr;

	Class = pEnv->GetObjectClass(Object);
	if(Class == nullptr)
		return nullptr;

	Field = pEnv->GetFieldID(Class, pszFieldName, "J");
	if(Field == nullptr)
		return nullptr;

	return (HANDLE)pEnv->GetLongField(Object, Field);
}

jobject Jni::Base::NewObject(JNIEnv* pEnv, LPCSTR pszClassName, LPCSTR pszCnsSig, ...)
{
	jclass Class;
	jmethodID CnsMethod;
	jobject Object;

	if(pEnv == nullptr || pszClassName == nullptr)
		return nullptr;

	Class = pEnv->FindClass(pszClassName);
	if(Class == nullptr)
		return nullptr;

	CnsMethod = pEnv->GetMethodID(Class, "<init>", pszCnsSig);
	if(CnsMethod == nullptr)
		return nullptr;

	{
		va_list args;

		va_start(args, pszCnsSig);

		Object = pEnv->NewObjectV(Class, CnsMethod, args);

		va_end(args);
	}

	return Object;
}
