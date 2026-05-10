//
// Created by tgian on 26. 5. 10..
//
#include "include/MediaCatalog.hpp"
#include <print>
#include <fstream>
#include <bit>
#include <iostream>
#include <ranges>
#include <array>


//<02> <resource count>
//[8] [ident] <1,2,3> [name] <1,2,3> [7]
//[8] [ident] <1,2,3> [name] <1,2,3> [7]
//......

MediaCatalog::MediaCatalog(std::filesystem::path dir) : m_dir(dir), m_valid(false) {

}

void MediaCatalog::load() {

    std::ifstream file(m_dir / "MediaCatalog.bytes", std::ios::binary);
    if (!file) {
        return;
    }

    uintmax_t file_size = std::filesystem::file_size(m_dir / "MediaCatalog.bytes");
    std::print("{}", file_size);

    bytes.resize(file_size);
    file.read(reinterpret_cast<char*>(bytes.data()), static_cast<long long>(file_size));
    file.close();

    std::array<char, 4> resource_count_bytes{};

    uint64_t ptr = 0;

    ptr++; //drop first byte

    //get content count
    for (int idx = 0; idx < 4; idx++,ptr++) {
        resource_count_bytes[idx] = bytes[idx];
    }
    uint32_t resources_count = std::bit_cast<uint32_t>(resource_count_bytes);
    while (ptr + 8 + 8 + 7 < file_size) {

        MediaCatalogData data;

        for (int idx = 0; idx < 8; idx++, ptr++) {
            data.byte0[idx] = bytes[ptr];
        }

        while (!is_termination_char(bytes[ptr])) {
            data.ident += bytes[ptr];
            ptr++;
        }
        data.load_byte = bytes[ptr];
        ptr++;

        for (int idx = 0; idx < 8; idx++, ptr++) {
            data.byte1[idx] = bytes[ptr];
        }

        while (!is_termination_char(bytes[ptr])) {
            data.name += bytes[ptr];
            ptr++;
        }
        //drop termination char
        ptr++;

        for (int idx = 0; idx < 7; idx++, ptr++) {
            data.byte2[idx] = bytes[ptr];
        }
        m_media_catalog_data.push_back(std::move(data));
    }
    std::cout << "size : " << m_media_catalog_data.size() << "\n";
}

void MediaCatalog::save(std::filesystem::path dir_) {

    std::ofstream file(dir_/"MediaCatalog.txt");
    file.clear();
    for (const auto& data : m_media_catalog_data) {
        file << data.ident << " // " << data.name << "\n";
    }
    file.close();

}

bool MediaCatalog::valid() const {
    return m_valid;
}


bool MediaCatalog::is_termination_char(char ch) {
    return ch == 1 || ch == 2 || ch == 3;
}