#include <iostream>
#include <curl/curl.h>
#include "newsg.hpp"

using namespace std;

int main()
{
    NewsApi NA(""); //your key from https://newsapi.org/
    auto news = NA.GetNews("russia");
    cout << "Results: " << news.totalResults << endl;
    cout << "Status:" << news.status << endl;
    cout << "News:" << endl
         << endl;
    for (auto item : news.articles)
    {
        cout << item.url << endl;
        cout << item.title << endl;
        cout << item.description << endl;
        cout << item.urlToImage << endl;
        cout << item.publishedAt << endl
             << endl;
        /*cout << item.content << endl << endl; 
        too much char literals */
    }
}