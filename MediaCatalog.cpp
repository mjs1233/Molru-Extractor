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


/*
BASIC FILE STRUCTURE OF 'MediaCatalog.txt'

HEADER
<02> <resource count>

BODY
[8{A}] [ident] <1,2,3{B}> <8{C}>[name] <1,2,3> [7{D}]

FOOTER
some extra zeros.

{A} : As far as I know {A} is always paired with {C}.

PB} : determine the file location

0x1 is BlueArchive_Data/StreamingAssets/PUB/Resource/GameData/MediaResources/
0x2 is BlueArchive_Data/StreamingAssets/PUB/Resource/Preload/MediaResources/
0x3 is BlueArchive_Data/StreamingAssets/

{C} : determine type of file
only 4th byte of {C} is useful. every other bytes are always 0x0

if 4th bytes is 1, the resource is .ogg file
if 4th bytes is 2, the resource is .mp4 file
if 4th bytes is 3, the resource is .jpg file

*/
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

void MediaCatalog::save(std::filesystem::path dir_, bool print_extra = false) {

    std::ofstream catalog_file(dir_/"MediaCatalog.txt");
    catalog_file.clear();
    for (const auto& data : m_media_catalog_data) {

        std::print(catalog_file,"ident: {}\nname: {}\n", data.ident, data.name);
        std::print(catalog_file, "b0: {}\n", convert_bytes_to_string(data.byte0));
        std::print(catalog_file, "b1: {}\n", convert_bytes_to_string(data.byte1));
        std::print(catalog_file, "b2: {}\n", convert_bytes_to_string(data.byte2));
        std::print(catalog_file, "=======================\n");
    }
    catalog_file.close();
    std::print("MediaCatalog.txt Written!");

    if (!print_extra)
        return;

    std::ofstream byte_data_file(dir_/"Byte0.txt");
    for (const auto& [bytes, idxs] : m_byte0_table) {
        std::print(byte_data_file, "{} *************************\n*************************\n", convert_bytes_to_string(m_media_catalog_data[idxs.front()].byte0));

        for (const auto catalog_idx : idxs) {
            auto data = m_media_catalog_data[catalog_idx];
            std::print(byte_data_file,"ident: {}\nname: {}\n", data.ident, data.name);
            std::print(byte_data_file, "b0: {}\n", convert_bytes_to_string(data.byte0));
            std::print(byte_data_file, "b1: {}\n", convert_bytes_to_string(data.byte1));
            std::print(byte_data_file, "b2: {}\n", convert_bytes_to_string(data.byte2));
            std::print(byte_data_file, "=======================\n");
        }
    }
    byte_data_file.close();
    std::print("Byte0.txt Written!");

    byte_data_file.open(dir_/"Byte1.txt");
    for (const auto& [bytes, idxs] : m_byte1_table) {
        std::print(byte_data_file, "{} *************************\n*************************\n", convert_bytes_to_string(m_media_catalog_data[idxs.front()].byte1));

        for (const auto catalog_idx : idxs) {
            auto data = m_media_catalog_data[catalog_idx];
            std::print(byte_data_file,"ident: {}\nname: {}\n", data.ident, data.name);
            std::print(byte_data_file, "b0: {}\n", convert_bytes_to_string(data.byte0));
            std::print(byte_data_file, "b1: {}\n", convert_bytes_to_string(data.byte1));
            std::print(byte_data_file, "b2: {}\n", convert_bytes_to_string(data.byte2));
            std::print(byte_data_file, "=======================\n");
        }
    }
    byte_data_file.close();
    std::print("Byte1.txt Written!");

    byte_data_file.open(dir_/"Byte2.txt");
    for (const auto& [bytes, idxs] : m_byte2_table) {
        std::print(byte_data_file, "{} *************************\n*************************\n", convert_bytes_to_string(m_media_catalog_data[idxs.front()].byte2));

        for (const auto catalog_idx : idxs) {
            auto data = m_media_catalog_data[catalog_idx];
            std::print(byte_data_file,"ident: {}\nname: {}\n", data.ident, data.name);
            std::print(byte_data_file, "b0: {}\n", convert_bytes_to_string(data.byte0));
            std::print(byte_data_file, "b1: {}\n", convert_bytes_to_string(data.byte1));
            std::print(byte_data_file, "b2: {}\n", convert_bytes_to_string(data.byte2));
            std::print(byte_data_file, "=======================\n");
        }
    }
    byte_data_file.close();
    std::print("Byte2.txt Written!");

}

void MediaCatalog::analyze() {

    for (size_t idx = 0; idx < m_media_catalog_data.size(); idx++) {
        auto& data = m_media_catalog_data[idx];

        m_byte0_table[convert_bytes_to_uint64(data.byte0)].push_back(idx);
        m_byte1_table[convert_bytes_to_uint64(data.byte1)].push_back(idx);
        m_byte2_table[convert_bytes_to_uint64(data.byte2)].push_back(idx);
    }
}

bool MediaCatalog::valid() const {
    return m_valid;
}


std::string MediaCatalog::convert_bytes_to_string(std::span<const char> data) {

    std::string result;
    for (const auto c : data) {
        result += std::format("{:X} ", c);
    }
    return result;
}

uint64_t MediaCatalog::convert_bytes_to_uint64(std::span<const char> data) {

    if (data.size() > 8) {
        throw std::logic_error("invalid size of std::span");
    }

    uint64_t result = 0;
    uint64_t shift = 0;
    for (const auto c : data) {
        result |= static_cast<uint64_t>(c) << shift;
        shift += 8;
    }
    return result;

}

bool MediaCatalog::is_termination_char(char ch) {
    return ch == 1 || ch == 2 || ch == 3;
}