#pragma once

#include <iostream>
#include <fstream>

class CsvFile;

inline static CsvFile& endrow(CsvFile& file);
inline static CsvFile& flush(CsvFile& file);

class CsvFile
{
    std::ofstream fs_;
    const std::string separator_;
public:
    explicit CsvFile(const std::string &filename);

    CsvFile(const std::string &filename, std::string separator);

    ~CsvFile();

    void flush();

    void endrow();

    CsvFile& operator << ( CsvFile& (* val)(CsvFile&));

    CsvFile& operator << (const char * val);

    CsvFile& operator << (const std::string & val);


    template<typename T>
    CsvFile& operator << (const T& val)
    {
        fs_ << val << separator_;
        return *this;
    }
};

inline static CsvFile& endrow(CsvFile& file)
{
    file.endrow();
    return file;
}

inline static CsvFile& flush(CsvFile& file)
{
    file.flush();
    return file;
}
