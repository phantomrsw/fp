
#include "csvfile.h"

CsvFile::CsvFile(const std::string &filename)
        : fs_()
        , separator_(";")
{
    fs_.exceptions(std::ios::failbit | std::ios::badbit);
    fs_.open(filename);
}

CsvFile::CsvFile(const std::string &filename, std::string separator = ";")
        : fs_()
        , separator_(std::move(separator))
{
    fs_.exceptions(std::ios::failbit | std::ios::badbit);
    fs_.open(filename);
}

CsvFile::~CsvFile()
{
    flush();
    fs_.close();
}

void CsvFile::flush()
{
    fs_.flush();
}

void CsvFile::endrow()
{
    fs_ << std::endl;
}

CsvFile &CsvFile::operator << ( CsvFile& (* val)(CsvFile&))
{
    return val(*this);
}

CsvFile &CsvFile:: operator << (const char * val)
{
    fs_ << '"' << val << '"' << separator_;
    return *this;
}
CsvFile &CsvFile:: operator << (const std::string & val)
{
    fs_ << '"' << val << '"' << separator_;
    return *this;
}






