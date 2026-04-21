/**
 *  Music Library Indexer
 *  @author Sjoerd de Jonge
 *  2026-03-28
 *
 *  Method:
 *  1. Read path to a directory [main.cpp]
 *  2. Loop through all files in directory [library_scanner.cpp]
 *  3. Open ifstream for each file with a supported format (.aiff) [aiff_reader.cpp]
 *  4. Read ifstream to start of ID3 tag [aiff_reader.cpp]
 *  5. Read each frame in ID3 tag, parse to string and add to JSON song
 *  6. Add JSON song to JSON library
 *
 */


#include <iostream>
#include <filesystem>
#include <fstream>
#include "aiff_reader.h"
#include "id3_parser.h"
#include "util/json.hpp"
#include "library_scanner.h"

int main() {
    const std::string project_root = PROJECT_ROOT;
    const std::string directory_path = project_root + "/music";

    // Recursive directory scanning:
    const nlohmann::json library = libraryToJson(directory_path);
    std::cout << "Recursive library JSON: \n" << library.dump(4) << std::endl;
    // Non-recursive directory scanning:
    const nlohmann::json library2 = libraryToJson(directory_path, false);
    std::cout << "Non recursive library JSON: \n" << library2.dump(4) << std::endl;

    return 0;
}
