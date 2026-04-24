/**
 *  Music Library Indexer (mli)
 *  @author Sjoerd de Jonge
 *  2026-03-28
 *
 *  Method:
 *  1. Read path to a directory [main.cpp]
 *  2. Loop through all files in directory [library_scanner.cpp]
 *  3. Open ifstream for each file with a supported format (.aiff) [aiff_reader.cpp]
 *  4. Read ifstream to start of ID3 tag [aiff_reader.cpp]
 *  5. Read each frame in ID3 tag, parse to string and add to JSON song [id3_parser.cpp]
 *  6. Add JSON song to JSON library [library_scanner.cpp]
 *  7. Write JSON library to (console/file) [main.cpp]
 *
 */


#include <iostream>
#include <filesystem>
#include <fstream>
#include "aiff_reader.h"
#include "id3_parser.h"
#include "nlohmann/json.hpp"
#include "library_scanner.h"
#include "options.h"

void mliIndex(const std::filesystem::path& directory_path, const IndexOptions& options);
void mliHelp();

// TODO: Add arg '-a' for APIC
// TODO: Add arg '<PATH>' for changing target directory
// TODO: Add arg '-v' or '--verbose'
// TODO: Add arg '--shallow' for non-recursive
// TODO: Fix to-dos in UTF-16 converter
// TODO: Look into warnings given at build
// TODO: Rewrite README
// TODO: Tests:
//      TODO: Add UTF-16 surrogate characters to tags of example songs in /music
//      TODO: Add corrupted tags to example songs in /music

int main(const int argc, char *argv[]) {

    #ifdef NDEBUG
        /* BUILD */
        /* Use this directory_path when building the project for release: */
        const std::filesystem::path directory_path = std::filesystem::current_path();
    #else
        /* DEVELOP */
        /* Use this directory_path when running app from the project: */
        const std::string project_root = PROJECT_ROOT;
        const std::filesystem::path directory_path = project_root + "/music";
    #endif

    if (argc > 1) {
        if (strcmp(argv[1], "index") == 0) {
            const IndexOptions options = {
                .verbose = false,
                .subdirectories = true,
                .include_apic = false,
                .output_type = Output::FILE,
            };
            mliIndex(directory_path, options);
        }
        else if (strcmp(argv[1], "help") == 0) {
            mliHelp();
        }
        else {
            // Unknown command
            std::cerr << "mli: '" << argv[1] << "' is not a valid mli command. See 'mli help'.\n";
            return 1;
        }
    } else {
        mliHelp();
    }

    return 0;
}

void mliIndex(const std::filesystem::path& directory_path, const IndexOptions& options) {
    // Directory scanning:
    const nlohmann::json library = libraryToJson(directory_path, options);

    // Output in console:
    if (options.output_type == Output::CONSOLE) std::cout << "Output JSON: \n" << library.dump(4) << std::endl;

    // Output as file:
    else if (options.output_type == Output::FILE) {
        // Create a unique filename for directory 'directory_path':
        std::filesystem::path output_path = makeUniqueFilePath(options.output_filename, directory_path);
        std::cout << "Scanned " << library.size() << " files.\n";
        std::cout << "File written to: " + output_path.string() << std::endl;
        std::ofstream outfile(output_path, std::ios::out);
        if (outfile.is_open()) {
            outfile << library.dump(4) << std::endl;
        }
        if (!outfile.good()) {
            std::cerr << "Failed to write to file: " << output_path.string() << std::endl;
        }
        outfile.close();
    }
}

void mliHelp() {
                //  ================================================================================
    std::cout   << "Music Library Indexer\n"
                   "\n"
                   "  usage: mli <command> [<args>]\n"
                << "\n"
                << "Supported commands:\n"
                << "index       Runs the Music Library Indexer in the current directory. Scans all\n"
                << "            (sub)directories for .aiff files, extracts id3 tag metadata, and\n"
                << "            writes to a JSON in the current/target directory.\n"
                << "            args:\n"
                << "            [<path>]    Full path to a directory to scan\n"
                << "            [-a]        Include APIC (attached picture) frame in JSON, with \n"
                   "                        base64 encoding\n"
                << "            [-v]        Verbose console output\n"
                << "            [--shallow] Single directory only (no subdirectories)\n"
                   "\n"
                   "help        Prints this text.\n"
    ;           //  ================================================================================
}
