//
// Created by Sjoerd de Jonge on 02/04/2026.
//

#include "aiff_reader.h"
#include "util/helpers.h"

#include <fstream>
#include <ios>

void read(const std::string& filename) {
    std::ifstream inp{ filename, std::ios_base::binary }; // Create an if-stream to open the AIFF.
}