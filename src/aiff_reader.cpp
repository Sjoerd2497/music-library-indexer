//
// Created by Sjoerd de Jonge on 02/04/2026.
//

#include "aiff_reader.hpp"

#include <format>

#include "id3_parser.hpp"
#include "util/helpers.hpp"
#include <fstream>
#include <ios>
#include <iostream>

// Reads the ifstream of an AIFF to the point start of the ID3 tag.
//
// Arguments:
// fin: The ifstream of an .aiff file.
// verbose: (Optional) bool to toggle console output.
aiffData scanFile(std::ifstream& fin, const bool verbose) {
    aiffData output;
    output.id3_pos = std::nullopt;
    if (fin) {
        formChunk form_chunk{};
        fin.read(reinterpret_cast<char*>(&form_chunk), sizeof(form_chunk));
        form_chunk.ckSize = fromBigEndianInt(form_chunk.ckSize); // Byte swap from big endian to native endian

        if (verbose) {
            std::cout << "=== FORM chunk ===" << "\n";
            std::cout << "ckID: " << charsToStr(form_chunk.ckID) << "\n";
            std::cout << "ckSize: " << form_chunk.ckSize << "\n";
            std::cout << "formType: " << charsToStr(form_chunk.formType) << "\n";
        }

        // Loop through the file, extracting the ckID and ckSize of each chunk
        while (fin.good() && fin.tellg() < form_chunk.ckSize + 8) {
            chunkHeader chunk_header{};
            fin.read(reinterpret_cast<char*>(&chunk_header), sizeof(chunk_header));
            chunk_header.ckSize = fromBigEndianInt(chunk_header.ckSize);

            const std::string ckID = charsToStr(chunk_header.ckID);

            if (verbose) {
                std::cout << std::format("=== {} chunk ===", ckID)  << "\n";
                std::cout << "ckID: " << ckID << "\n";
                std::cout << "ckSize: " << chunk_header.ckSize << "\n";
            }

            if (ckID == "ID3 ") {
                if (verbose) std::cout << "Found ID3 tag." << "\n";
                output.id3_pos = fin.tellg();
            }

            if (ckID == "NAME") {
                output.name.resize(chunk_header.ckSize); // Resize container
                fin.read(reinterpret_cast<std::istream::char_type *>(output.name.data()), chunk_header.ckSize);
                if (chunk_header.ckSize % 2 != 0) fin.seekg(1, std::ios_base::cur);
                continue;
            }

            if (ckID == "AUTH") {
                output.auth.resize(chunk_header.ckSize); // Resize container
                fin.read(reinterpret_cast<std::istream::char_type *>(output.auth.data()), chunk_header.ckSize);
                if (chunk_header.ckSize % 2 != 0) fin.seekg(1, std::ios_base::cur);
                continue;
            }

            if (ckID == "(c) ") {
                output.copyright.resize(chunk_header.ckSize); // Resize container
                fin.read(reinterpret_cast<std::istream::char_type *>(output.copyright.data()), chunk_header.ckSize);
                if (chunk_header.ckSize % 2 != 0) fin.seekg(1, std::ios_base::cur);
                continue;
            }

            if (ckID == "ANNO") {
                output.anno.resize(chunk_header.ckSize); // Resize container
                fin.read(reinterpret_cast<std::istream::char_type *>(output.anno.data()), chunk_header.ckSize);
                if (chunk_header.ckSize % 2 != 0) fin.seekg(1, std::ios_base::cur);
                continue;
            }

            // Determine how far we skip ahead, which is equal to the size of data in the chunk.
            // If the ckSize is odd, there is a padding byte at the end not counted in ckSize.
            const int32_t skip = (chunk_header.ckSize % 2 == 0) ? chunk_header.ckSize : chunk_header.ckSize + 1;
            fin.seekg(skip, std::ios_base::cur);
            if (verbose) std::cout << "current position: " << fin.tellg() << "\n";
        }
    }
    return output;
}
