#ifndef RECORD_HTTPWRAPPER_H
#define RECORD_HTTPWRAPPER_H

#include <string>

class HttpWrapper
{
public:
    HttpWrapper();
    ~HttpWrapper();
    HttpWrapper(const HttpWrapper&) = delete;
    HttpWrapper& operator=(const HttpWrapper&) = delete;

    static HttpWrapper& getInstance() {
        static HttpWrapper instance;
        return instance;
    }

public:
    std::string GetCheckSum(const std::string &appkey, const std::string &cname, uint64_t uid, const std::string& token_server);

 
};

#endif

