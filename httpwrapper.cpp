#include "httpclient.h"
#include "httpwrapper.h"
#include "./fjson/json_builder/fjson_builder.h"
#include "./fjson/json_parser/fjson_parser.h"
#include "./fjson/json_helper.h"
#include <regex>
#include <stdarg.h>
#include <algorithm>
#include <array>

namespace {
unsigned char ToHex(unsigned char x) {
  return x > 9 ? x + 55 : x + 48;
}
} 
std::string GetURLEncode(const std::string& origin) {
  std::string result;

  size_t length = origin.length();
  for (size_t i = 0; i < length; i++) {
    if (isalnum((unsigned char)origin[i]) || (origin[i] == '-') || (origin[i] == '_') || (origin[i] == '.') ||
        (origin[i] == '~')) {
      result += origin[i];
    } else if (origin[i] == ' ') {
      result += "+";
    } else {
      result += '%';
      result += ToHex((unsigned char)origin[i] >> 4);
      result += ToHex((unsigned char)origin[i] % 16);
    }
  }
  return result;
}

HttpWrapper::HttpWrapper()
{
}

HttpWrapper::~HttpWrapper()
{
}

std::string HttpWrapper::GetCheckSum(const std::string &appkey, const std::string &cname, uint64_t uid, const std::string& token_server)
{
  std::string checksum;
  std::string url = token_server + "?appkey=" + appkey + "&channelName=" + GetURLEncode(cname) + "&type=1&uid=" + std::to_string(uid);
	CurlHttpPost httpPost(url);

	if (!httpPost.Create(2000))
	{
		return checksum;
	}

#ifdef USE_PROXY
	httpPost.ConfigProxy(CURLPROXY_SOCKS5, SOCKS5_HOST, SOCKS5_PORT, SOCKS5_USER, SOCKS5_PASS);
#endif

    int code;
    std::string content;

	code = httpPost.DoEasy();
	if (code == CURLE_OK)
	{
        content = httpPost.GetRetContent();
        json_value *jv = json_parse(content.c_str(), content.size());
        if (nullptr != jv)
        {
            checksum = json_value_find_as_string(jv, "checksum");
            if (checksum.empty())
            {
                printf("Fauilre to request token: %s\n", content.c_str());
            } else {
                printf("Request Token: %s\n", checksum.c_str());
            }
        }
	}

	return checksum;
}
