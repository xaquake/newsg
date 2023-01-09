#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <curl/curl.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::string;
using std::stringstream;
using std::vector;
using namespace boost::property_tree;

struct NewObj
{
    const string title;
    const string description;
    const string url;
    const string urlToImage;
    const string publishedAt;
    const string content;
};

struct NewsObj
{
    const string status;
    const short totalResults;
    const vector<NewObj> articles;
};

class NewsApi
{
  public:
    const string Api = "https://newsapi.org/v2/everything?q=";
    CURL *curl = curl_easy_init();
    CURLcode resp;
    struct curl_slist *header_to = NULL;
    explicit NewsApi(string key = "NULL") : _key(key)
    {
        assert(_key != "NULL");
        header_to = curl_slist_append(header_to, "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.73 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
    };
    NewsObj GetNews(const string quote, const string sortBy = "popularity", const string language = "ru")
    {
        string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, (Api + quote + string("&sortBy=") + sortBy + string("&language=") + language + string("&apiKey=") + _key).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WC);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        resp = curl_easy_perform(curl);
        curl_easy_reset(curl);
        string status = ParseJson<string>(buffer, "status");
        if (status != "ok")
        {
            std::cout << "invalid key";
            exit(0);
        }
        return NewsObj{
            status,
            ParseJson<short>(buffer, "totalResults"),
            AppendObj(buffer)};
    };

  private:
    string _key;
    vector<NewObj> AppendObj(const string &data)
    {
        vector<NewObj> return_data;
        ptree json_parse;
        stringstream sdata(data);
        read_json(sdata, json_parse);
        BOOST_FOREACH (const ptree::value_type &item, json_parse.get_child("articles"))
            return_data.push_back(NewObj{
                item.second.get<string>("title"),
                item.second.get<string>("description"),
                item.second.get<string>("url"),
                item.second.get<string>("urlToImage"),
                item.second.get<string>("publishedAt"),
                item.second.get<string>("content")});
        return return_data;
    }
    template <typename T>
    T ParseJson(const string &data, const string &&key)
    {
        ptree json_parse;
        stringstream sdata(data);
        read_json(sdata, json_parse);
        return json_parse.get<T>(key);
    };
    static size_t WC(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    };
};
