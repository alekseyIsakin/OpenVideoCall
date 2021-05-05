#pragma once

#include "nlohmann/json.hpp"

#define EMPTY_TOKEN ""
#define RSI_URL _T("https://secure.rsi.exchange:3222/get_tokens_by_uuid_keyvalue?uuid_=")

class Tokens
{
protected:
	CString _hostToken;
	CString _hostName;

public:
	Tokens();
	BOOL GetCloudToken(CString roomNumber);
	std::string GetHostToken();
	CString GetHostName();

	BOOL isEmptyToken();
};