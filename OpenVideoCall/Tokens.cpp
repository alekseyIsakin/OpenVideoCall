#include "stdafx.h"
#include "Tokens.h"

#include <afxinet.h>
#include "nlohmann/json.hpp"
#include <fstream>

Tokens::Tokens()
{
	_hostName = "";
	_hostToken = "";
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
				//setLang(j["ROOMS_NAMES_TARGET"]);
			}
		}
	}
	return j["status"] == "ok";
}

std::string Tokens::GetHostToken()
{
	CT2CA convert(_hostToken);
	return convert;
}
CString Tokens::GetHostName()
{
	return _hostName;
}

BOOL Tokens::isEmptyToken()
{
	return _hostToken == EMPTY_TOKEN;
}
