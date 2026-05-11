//
// Created by tgian on 26. 5. 10..
//

#ifndef MOLLUEXP_MEDIACATALOG_HPP
#define MOLLUEXP_MEDIACATALOG_HPP
#include <filesystem>
#include <span>
#include <vector>
#include <array>
#include <string_view>
#include <unordered_map>

//<12byte>
//<file ident??(0x3 terminated)> <8byte> <file name (0x3 terminated)> <15byte>
//<file ident??(0x3 terminated)> <8byte> <file name (0x3 terminated)> <15byte>

struct MediaCatalogData {
    std::string ident;
    std::string name;
    std::array<char,8> byte0;
    std::array<char,8> byte1;
    char load_byte;
    std::array<char,7> byte2;
};

class MediaCatalog {
public:
    explicit MediaCatalog(std::filesystem::path dir);
    void load();
    void save(std::filesystem::path dir, bool print_extra = false);
    bool valid() const;
    void analyze();

private:
    bool is_termination_char(char ch);
    std::string convert_bytes_to_string(std::span<const char> data);
    uint64_t convert_bytes_to_uint64(std::span<const char> data);
    std::vector<char> bytes;
    std::vector<MediaCatalogData> m_media_catalog_data;
    bool m_valid;
    std::filesystem::path m_dir;

    std::unordered_map<uint64_t,std::vector<size_t>> m_byte0_table;
    std::unordered_map<uint64_t,std::vector<size_t>> m_byte1_table;
    std::unordered_map<uint64_t,std::vector<size_t>> m_byte2_table;
};


#endif //MOLLUEXP_MEDIACATALOG_HPP