#pragma once

#include "nlohmann/json.hpp"

#define EMPTY_TOKEN ""
#define RSI_URL _T("https://secure.rsi.exchange:3222/get_tokens_by_uuid_keyvalue?uuid_=")

#define JSON_HOST "ROOMS_NAMES_HOST"
#define JSON_TARGET_LANGS "ROOMS_NAMES_TARGET"
#define JSON_RELAY_LANGS  "ROOMS_NAMES_CHAT"

class langHolder {
public:
	std::string langShort;
	std::string langFull;
	std::string token;
};

class Tokens
{
protected:
	CString _hostToken;
	CString _hostName;

	std::vector<langHolder> _listTargetLang;
	std::vector<langHolder> _listRelayLang;

	// from ROOMS_NAMES_TARGET
	std::vector<langHolder> setLang(nlohmann::json langsJson);

	u_int _countLang;
public:
	Tokens();
	BOOL GetCloudToken(CString roomNumber);

	std::string GetToken(int id);
	CString GetName(int id);

	std::string GetHostToken();
	CString GetHostName();

	BOOL isEmptyToken();

	u_int GetCountLang();

	std::vector<langHolder>::iterator GetTargetLngBgnItr();
	std::vector<langHolder>::iterator GetTargetLngEndItr();

	std::vector<langHolder>::iterator GetRelayLngBgnItr();
	std::vector<langHolder>::iterator GetRelayLngEndItr();
};

