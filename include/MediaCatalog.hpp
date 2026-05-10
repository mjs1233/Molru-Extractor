//
// Created by tgian on 26. 5. 10..
//

#ifndef MOLLUEXP_MEDIACATALOG_HPP
#define MOLLUEXP_MEDIACATALOG_HPP
#include <filesystem>
#include <vector>
#include <string_view>

//<12byte>
//<file ident??(0x3 terminated)> <8byte> <file name (0x3 terminated)> <15byte>
//<file ident??(0x3 terminated)> <8byte> <file name (0x3 terminated)> <15byte>

struct MediaCatalogData {
    std::string ident;
    std::string name;
    char byte0[8];
    char byte1[8];
    char load_byte;
    char byte2[7];
};

class MediaCatalog {
public:
    explicit MediaCatalog(std::filesystem::path dir);
    void load();
    void save(std::filesystem::path dir);
    bool valid() const;

private:
    bool is_termination_char(char ch);

    std::vector<char> bytes;
    std::vector<MediaCatalogData> m_media_catalog_data;
    bool m_valid;
    std::filesystem::path m_dir;
};


#endif //MOLLUEXP_MEDIACATALOG_HPP