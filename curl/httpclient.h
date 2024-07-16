
#ifndef NRTC_HTTP_CLIENT_H
#define NRTC_HTTP_CLIENT_H

#include <string>
#include <strings.h>
#include <curl/curl.h>

class CurlHttpPost
{
public:
	CurlHttpPost(std::string url)
		:url_(url),
		header_list_(nullptr),
		easy_handle_(nullptr)
	{

	}
	~CurlHttpPost()
	{
		if (header_list_ != nullptr) {
			curl_slist_free_all(header_list_);
			header_list_ = nullptr;
		}
		if (easy_handle_)
		{
			curl_easy_cleanup(easy_handle_);
			easy_handle_ = nullptr;
		}
	}
	void SetPostData(std::string data)
	{
		post_data_ = data;
	}
	bool Create(int ms_timeout)
	{
		easy_handle_ = curl_easy_init();
		if (easy_handle_ == nullptr) {
			return false;
		}
		curl_easy_setopt(easy_handle_, CURLOPT_LOW_SPEED_TIME, 3L);
		curl_easy_setopt(easy_handle_, CURLOPT_LOW_SPEED_LIMIT, 10L);

		if (ms_timeout > 0)
		{
			curl_easy_setopt(easy_handle_, CURLOPT_TIMEOUT_MS, ms_timeout);
		}

		curl_easy_setopt(easy_handle_, CURLOPT_URL, url_.c_str());
		curl_easy_setopt(easy_handle_, CURLOPT_HTTPHEADER, header_list_);
		curl_easy_setopt(easy_handle_, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(easy_handle_, CURLOPT_COOKIEFILE, ""); // enable cookie
		curl_easy_setopt(easy_handle_, CURLOPT_USERAGENT, "NEngine/1.0 (compatible; MSIE 6.0; Windows NT 5.1)");


		curl_easy_setopt(easy_handle_, CURLOPT_POST, 1);
		curl_easy_setopt(easy_handle_, CURLOPT_POSTFIELDS, post_data_.data());
		curl_easy_setopt(easy_handle_, CURLOPT_POSTFIELDSIZE, post_data_.size());
			
		if (strncasecmp(url_.c_str(), "https://", 8) == 0) {
			curl_easy_setopt(easy_handle_, CURLOPT_SSL_VERIFYHOST, 2);
			curl_easy_setopt(easy_handle_, CURLOPT_SSL_VERIFYPEER, 0);
		}
		curl_easy_setopt(easy_handle_, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(easy_handle_, CURLOPT_WRITEFUNCTION, WriteMemory);

		return true;
	}
	void ConfigProxy(int type, std::string host, unsigned short port, std::string user, std::string pass)
	{
		// "Daniel Stenberg"<daniel@haxx.se> tells me by mail that we are using
		// Proxy Tunnel, so we should turn on this flag. This helps me a lot.
		curl_easy_setopt(easy_handle_, CURLOPT_HTTPPROXYTUNNEL, 1L);
		//curl_easy_setopt(easy_handle, CURLOPT_PROXYAUTH, CURLAUTH_ANYSAFE);
		curl_easy_setopt(easy_handle_,
			CURLOPT_PROXYTYPE,
			type);
		curl_easy_setopt(easy_handle_, CURLOPT_PROXY, host.c_str());
		curl_easy_setopt(easy_handle_, CURLOPT_PROXYPORT, port);

		if (!user.empty()) {
			curl_easy_setopt(easy_handle_,
				CURLOPT_PROXYUSERNAME,
				user.c_str());
			if (!pass.empty())
				curl_easy_setopt(easy_handle_,
				CURLOPT_PROXYPASSWORD,
				pass.c_str());
		}
	}
	void addHeader(const std::string &name, const std::string &value)
	{
		std::string header_field;
		header_field.append(name);
		header_field.append(": ");
		header_field.append(value);
		header_list_ = curl_slist_append(header_list_, header_field.c_str());
	}
	CURLcode DoEasy()
	{
		CURLcode code = CURL_LAST;
		if (easy_handle_)
		{
			code = curl_easy_perform(easy_handle_);
		}
		return code;
	}

	static size_t WriteMemory(void *ptr, size_t size, size_t count, void *param)
	{
		size_t bytes_to_store = size * count;
		CurlHttpPost *request = static_cast<CurlHttpPost *>(param);
		request->content_.append(static_cast<char *>(ptr), bytes_to_store);
		return bytes_to_store;
	}

	std::string GetRetContent()
	{
		return content_;
	}

private:
	std::string url_;
	std::string post_data_;
	std::string content_;
	CURL *easy_handle_;
	curl_slist *header_list_;
};

#endif //NRTC_HTTP_CLIENT_H