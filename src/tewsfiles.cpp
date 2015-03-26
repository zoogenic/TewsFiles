#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/adaptors.hpp>

namespace bpt = boost::property_tree;

namespace Private
{
    static const char outfilename[FILENAME_MAX] = "/tmp/tews_file";

    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        size_t written;
        written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    CURLcode downloadUrl(const std::string &url)
    {
        CURL *curl;
        FILE *fp;
        CURLcode res = CURLE_FAILED_INIT;
        curl = curl_easy_init();
        if (curl)
        {
            fp = fopen(outfilename,"wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
        return res;
    }

    namespace Predicate
    {
        struct CheckTag
        {
            CheckTag(const char *val) : val_(val)
            {
            }

            bool operator ()(const bpt::ptree::value_type &tag)
            {
                return tag.first == val_;
            }
        private:
            const std::string val_;
        };
    } // namespace Predicate
} //namespace Private

int main()
{
    namespace bpxml = boost::property_tree::xml_parser;
    namespace ba = boost::adaptors;

    static const std::string rss_url = "http://downloads.bbc.co.uk/podcasts/worldservice/tae/rss.xml";
    Private::downloadUrl(rss_url);

    bpt::ptree pt;
    bpt::read_xml(Private::outfilename, pt, bpxml::trim_whitespace | bpxml::no_comments);

    for (auto item: pt.get_child("rss.channel") | ba::filtered(Private::Predicate::CheckTag("item")))
    {
        for (auto prop: item.second | ba::filtered(Private::Predicate::CheckTag("link")))
        {
            std::cout << prop.second.get_value<std::string>() << std::endl;
        }
    }
}
