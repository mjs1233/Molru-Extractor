#include <iostream>
#include <fstream>
#include <filesystem>

#include "include/MediaCatalog.hpp"

int main() {

    MediaCatalog media_catalog("");
    media_catalog.load();
    media_catalog.save("");
    return 0;
}