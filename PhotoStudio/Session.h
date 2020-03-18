#pragma once

class Session
{
public:

	static bool StoreSession(LPCTSTR pszFilePath);

	static bool RestoreSession(LPTSTR pszFilePath, UInt32 MaxLength);

private:
	Session() = delete;
	~Session() = delete;
	Session(const Session&) = delete;
	Session& operator=(const Session&) = delete;
};
