#include "..\..\OpenLive-Windows-MFC-broadcast\OpenLive\Tokens.h"
#include "stdafx.h"
#include "Tokens.h"

#include <afxinet.h>
#include <fstream>


void Tokens::setLang(nlohmann::json langsJson)
{
	_listTargetLang.clear();

	for (auto val = langsJson.begin(); val != langsJson.end(); val++)
	{
		std::string langNm, langToken;
		int indSep = val.key().find_first_of("_");
		langNm = val.key().substr(0, indSep);
		langToken = val.value();

		langHolder lh =
		{
				langNm,			// shortName lang channel
				val.key(),		// FullName lang channel
				langToken		// token
		};

		if (langNm == "HOST")
		{
			lh.langShort = "Floor";
			_listTargetLang.insert(_listTargetLang.begin(), lh);
		}
		else
			_listTargetLang.push_back(lh);

	}
}

Tokens::Tokens()
{
	_hostName = "";
	_hostToken = "";
	_listTargetLang.clear();
}

BOOL Tokens::GetCloudToken(CString roomNumber)
{
	CHttpFile* pHttpFile = NULL;
	CInternetSession session;

	_hostName = "";
	_hostToken = "";

	try
	{
		// 52430201
		pHttpFile = (CHttpFile*)session.OpenURL
		(RSI_URL + roomNumber);
	}
	catch (...)
	{ /*Handle exception */
	}

	nlohmann::json j;

	if (pHttpFile != NULL)
	{
		CByteArray data;
		data.SetSize(4096);
		int ptr = 0;
		ptr = pHttpFile->Read(data.GetData(), data.GetSize());

		j = j.parse(data.GetData());
		j = j.is_array() ? j[0] : j;

		if (j["status"] == "ok") {
			{
				_hostToken = std::string(j["ROOMS_NAMES_HOST"].begin().value()).c_str();
				_hostName = std::string(j["ROOMS_NAMES_HOST"].begin().key()).c_str();
				setLang(j["ROOMS_NAMES_TARGET"]);
			}
		}
	}
	return j["status"] == "ok";
}
std::string Tokens::GetToken(int id)
{
	if (id < _listTargetLang.size())
		return _listTargetLang[id].token;
	return "";
}
CString Tokens::GetName(int id)
{

	if (id < _listTargetLang.size())
		return CString(_listTargetLang[id].langFull.c_str());
	return CString();
}
std::string Tokens::GetHostToken()
{
	CT2A conv(_hostToken);
	return conv;
}

CString Tokens::GetHostName()
{
	return _hostName;
}

BOOL Tokens::isEmptyToken()
{
	return _hostToken == EMPTY_TOKEN;
}

std::vector<langHolder>::iterator Tokens::GetLngBgnIterator()
{
	if (_listTargetLang.size() > 0)
		return _listTargetLang.begin();
	return _listTargetLang.end();
}

std::vector<langHolder>::iterator Tokens::GetLngEndIterator()
{
	if (_listTargetLang.size() > 0)
		return _listTargetLang.end();
	return _listTargetLang.end();
}

