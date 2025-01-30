#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <cstring>
#include "csvfile.h"
#include "version.h"
#include "crc32.h"


struct file_prop_t{
    std::string path;
    std::string name;
    std::string date;
    size_t size{};
    std::string crc32;
    };

std::time_t getFileWriteTime ( const std::filesystem::path& filename )
{
#if defined ( _WIN32 )
    {
        struct _stat64 fileInfo{};
        if ( _wstati64 ( filename.wstring ().c_str (), &fileInfo ) != 0 )
        {
            throw std::runtime_error ( "Failed to get last write time." );
        }
        return fileInfo.st_mtime;
    }
#else
    {
        auto fsTime = std::filesystem::last_write_time ( filename );
        return decltype ( fsTime )::clock::to_time_t ( fsTime );
    }
#endif
}

std::string getFormattedFileTime(const std::filesystem::path& filename, const char* format){
    std::time_t tt = getFileWriteTime(filename);
    std::tm *gmt = std::localtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt, format);
    return buffer.str();
}

void showHelp(){
    std::cout   << "\nUsage: fp <directory or file_name> \n"
                << "\nIf specified directory the utility will go \n"
                << "through all the files and display information about them.\n"
                << "\nExample: \n"
                << "  fp test\\src \n"
                << "  fp file.txt"<< std::endl;
}

void parseFile(const std::filesystem::path &path, std::vector<file_prop_t> &file_prop){
    if (is_regular_file(path)){

        file_prop_t fp;

        std::ifstream f;
        f.open(path.string(), std::ios::binary);

        if(!f.is_open())
            std::cout << "Fail! File " << path.string() << " not exist!" << std::endl;

        size_t size_file = file_size(path);

        uint32_t crc{};
        uint8_t buf[1];
        for(int i = 0; i < size_file; i++)
        {
            f.read(reinterpret_cast<char *>(&buf), sizeof(buf));
            crc = crcCycle(crc, buf, sizeof(buf));
        }

        fp.path = path.parent_path().string();
        fp.name = path.filename().string();
        fp.date = getFormattedFileTime(path,"%d.%m.%Y %H:%M:%S");
        fp.size = file_size(path);

        std::stringstream s;
        s << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << crc;
        fp.crc32 = s.str();

        std::cout   << "path = "   << fp.path
                    << " name = "  << fp.name
                    << " date = "  << fp.date
                    << " size = "  << fp.size
                    << " crc32 = " << fp.crc32
                    << '\n';

        file_prop.push_back(fp);
    }
}

int main(int argc, char** argv)
{
    std::cout   << "File Property utility" << " " << PROGRAM_VERSION << " " << __DATE__ << std::endl;

    std::filesystem::path path{};

    if((argc == 2 && strcmp(argv[1],"-h") == 0) || argc == 1){
        showHelp();
        return 0;
    }
    else{
        path = argv[1];
    }

    if (!exists(path)){
        std::cerr << "FAIL! File or directory " << path << " no exist!" << std::endl;
        return 0;
    }

    std::vector<file_prop_t> file_prop;

    uint64_t size_sum{};
    crcInit();

    if(is_directory(path)){
        for(auto& p: std::filesystem::recursive_directory_iterator(path)){
            parseFile(p.path(),file_prop);
        }
    }
    else{
        parseFile(path,file_prop);
    }

    try
    {
        CsvFile csv("report.csv");
        csv << "path" << "name" << "date" << "size" << "crc32" << endrow;
        for(auto &p : file_prop){
            csv << p.path << p.name << p.date << p.size << p.crc32 << endrow;
            size_sum += p.size;
        }
        csv << "----" << "----" << "----" << "----" << "----" << endrow;
        csv << "Summary size" << "" << "" << size_sum << endrow;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception was thrown: " << ex.what() << std::endl;
    }
    return 0;
}
