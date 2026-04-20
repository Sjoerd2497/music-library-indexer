//
// Created by Sjoerd de Jonge on 20/04/2026.
//

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "library_scanner.h"
#include "util/json.hpp"
#include "aiff_reader.h"
#include "id3_parser.h"

// Scans all supported music files in the directory (and optionally subdirectories) and writes their ID3 tag data to a JSON.
//
// Arguments:
// - directory_path: The path to the directory to scan for music files.
// - sub_directories: A bool whether to also include subdirectories, default = true.
nlohmann::json libraryToJson(const std::string& directory_path, const bool sub_directories) {
    // TODO: Verify directory_path

    // Create a path:
    const auto path = std::filesystem::path(directory_path);

    nlohmann::json library;

    // Lambda for scanning the directories (recursive or not):
    auto scan = [&](const auto& iterator) {
        for (auto const& dir_entry : iterator) {
            if (dir_entry.path().extension() == ".aiff" || dir_entry.path().extension() == ".aif") {
                // Create an if-stream to open the file.
                std::ifstream fin{ dir_entry.path(), std::ios_base::binary };

                // Skip ifstream to the start of the ID3 tag.
                locateId3(fin);
                const nlohmann::json song = id3ToJson(fin);
                library.push_back(song);
            }
        }
    };

    if (sub_directories) {
        const std::filesystem::recursive_directory_iterator iterator{path};
        scan(iterator);
    } else {
        const std::filesystem::directory_iterator iterator{path};
        scan(iterator);
    }

    return library;
}
