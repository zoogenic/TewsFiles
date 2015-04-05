#define CURL_STATICLIB

// STD
#include <iostream>

// BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>

#include <curl/curl.h>

namespace bpt = boost::property_tree;
namespace bfs = boost::filesystem;

namespace Private
{
    static const char outfilename[] = "/tmp/tews_file";

    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        size_t written;
        written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    CURLcode downloadUrl(const std::string &url, const char *out)
    {
        CURL *curl;
        FILE *fp;
        CURLcode res = CURLE_FAILED_INIT;
        curl = curl_easy_init(); // TODO: try multi interface instead of easy interface
        if (curl)                // TODO: or maybe do not close connection
        {
            fp = fopen(out,"wb");
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

    int processCommandLine(int argc, char *argv[], bfs::path &output_dir)
    {
        namespace bpo = boost::program_options;

        bpo::options_description desc("Program options:");
        desc.add_options()
            ("help,h", "Show help")
            ("output,o", bpo::value<bfs::path>(&output_dir), "Output folder")
            ;
        bpo::variables_map vm;

        try
        {
            const bpo::parsed_options parsed = bpo::command_line_parser(argc, argv).options(desc).run();
            bpo::store(parsed, vm);
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            return 1;
        }

        bpo::notify(vm);
        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 2;
        }

        if (false == bfs::exists(output_dir))
        {
            std::cout << "Path does not exist" << std::endl;
            return 3;
        }

        return 0;
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

int main(int argc, char *argv[])
{
    namespace bpxml = boost::property_tree::xml_parser;
    namespace ba = boost::adaptors;

    // processing command line arguments
    bfs::path output_dir;
    int parsed = Private::processCommandLine(argc, argv, output_dir);
    if (0 != parsed)
    {
        return parsed;
    }

//    static const std::string rss_url = "http://downloads.bbc.co.uk/podcasts/worldservice/tae/rss.xml";
    static const std::string new_url = "http://www.bbc.co.uk/learningenglish/english/features/the-english-we-speak/ep-150324";
    Private::downloadUrl(new_url, Private::outfilename);

    {
        std::cout << "* trying to open and read: " << Private::outfilename << std::endl;
        std::ifstream outfile_stream(Private::outfilename);
        if (false == outfile_stream.is_open())
        {
            perror((std::string("error while openning file ") + Private::outfilename).c_str());
        }
        for (std::string line; std::getline(outfile_stream, line); )
        {
            std::cout << line << std::endl;
        }
        if (true == outfile_stream.bad())
        {
            perror((std::string("error while reading file: ") + Private::outfilename).c_str());
        }
        outfile_stream.close();
    }

//    bpt::ptree pt;
//    bpt::read_xml(Private::outfilename, pt, bpxml::trim_whitespace | bpxml::no_comments);
//
//    const bfs::path pages_dir = output_dir / "pages";
//    bfs::create_directories(pages_dir);
//
//    static const std::string podcast_head = "http://www.bbc.co.uk/learningenglish/english/features/the-english-we-speak/ep-";
//    for (auto item: pt.get_child("rss.channel") | ba::filtered(Private::Predicate::CheckTag("item")))
//    {
//        for (auto prop: item.second | ba::filtered(Private::Predicate::CheckTag("link")))
//        {
//            const std::string suburl = prop.second.get_value<std::string>().substr(59, 6);
//            const bfs::path page_file = pages_dir / (suburl + ".html");
//            if (true == bfs::exists(page_file))
//            {
//                std::cout << "File: " << page_file << " is downloaded already." << std::endl;
//            }
//            else
//            {
//                Private::downloadUrl(podcast_head + suburl, page_file.string().c_str());
//            }
//        }
//    }

//    for (auto file: boost::make_iterator_range(bfs::directory_iterator(pages_dir), bfs::directory_iterator())
//            | ba::filtered(static_cast<bool (*)(const bfs::path &)>(&bfs::is_regular_file)))
//    {
////http://downloads.bbc.co.uk/learningenglish/features/tews/150317_TEWS_stab_in_the_dark_v1.pdf
////http://downloads.bbc.co.uk/learningenglish/features/tews/150317_tews_a_stab_in_the_dark_download.mp3
//        bfs::ifstream strm(file, std::ios_base::in);
//        if (!strm)
//        {
//            std::cout << "Cannot open file \"" << file << "\"" << std::endl;
//            continue;
//        }
//
//        boost::regex re("(http:([\\-\\w\\d\\.\\/]+)\\.mp3)", boost::regex_constants::perl);
//        for (std::string line; std::getline(strm, line); )
//        {
//            for (boost::sregex_token_iterator i(line.begin(), line.end(), re, 0), ie; i != ie; ++i)
//            {
//                std::cout << *i << std::endl;
//            }
//        }
//    }
}
