//
// Created by Sjoerd de Jonge on 02/04/2026.
//

#ifndef MUSIC_LIBRARY_INDEXER_AIFF_READER_H
#define MUSIC_LIBRARY_INDEXER_AIFF_READER_H
#include <array>
#include <cstdint>

// Audio IFF uses big-endian order, meaning the most significant byte is stored at the smallest memory address

void read(const std::string& filename);

// The FORM chunk as defined by AIFF spec
struct formChunk {
    std::array<char, 4> ckID;       // Always 'FORM'
    int32_t ckSize;                 // The size of the data portion
    std::array<char, 4> formType;   // What is in the FORM chunk, always 'AIFF'
    char chunks [];                 // The chunks contained within the FORM.
};
#endif //MUSIC_LIBRARY_INDEXER_AIFF_READER_H
