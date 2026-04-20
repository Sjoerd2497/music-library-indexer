//
// Created by Sjoerd de Jonge on 03/04/2026.
//

#include <bitset>
#include <fstream>
#include <iostream>
#include "id3_parser.h"
#include "util/base64.h"
#include "util/helpers.h"
#include "util/json.hpp"


// Append the ID3 tag to the JSON as item it.
//
// Arguments:
// - fin: Reference to the ifstream at the start of the ID3 tag.
// - verbose: Optional bool to enable console output, default false.
nlohmann::json id3ToJson(std::ifstream& fin, const bool verbose) {
    nlohmann::json song;
    // Extract ID3 header.
    const ID3Header id3_header = parseId3Header(fin);
    // Extract ID3 frames and add to JSON.
    extractId3Frames(fin, fromSynchsafe32(id3_header.size), song, verbose);
    return song;
}



// Accepts an fstream at the start of an ID3 tag and parses the header(s).
//
// Arguments:
// - fin: Reference to the ifstream at the start of the ID3 tag.
// - verbose: Optional bool to enable console output, default false.
ID3Header parseId3Header(std::ifstream& fin, const bool verbose) {
    ID3Header id3_tag_header{};
    fin.read(reinterpret_cast<char*>(&id3_tag_header), sizeof(id3_tag_header));
    if (verbose) {
        std::cout << "=== ID3 Tag Header ===" << "\n";
        std::cout << "file_identifier: " << charsToStr(id3_tag_header.file_identifier) << "\n";
        std::cout << "version: " << static_cast<int>(id3_tag_header.version[0]) << "." << static_cast<int>(id3_tag_header.version[1]) << "\n";
    }
    const std::bitset<8> flags{id3_tag_header.flags};
    if (verbose){
        std::cout << "flags: " << flags << "\n";
        std::cout << "size: " << fromSynchsafe32(id3_tag_header.size) << "\n";
    }

    // If file has an extended header, skip past it.                                bit: 76543210
    // If flag b is set, which is bit 6, it has an extended header. (considering flags = abcd0000)
    if (id3_tag_header.flags & (1 << 6 )) {
        std::array<uint8_t, 4> extended_header_size{};
        fin.read(reinterpret_cast<char*>(&extended_header_size), sizeof(extended_header_size));
        // Skip ahead extended_header_size bytes from current position:
        fin.seekg(fromSynchsafe32(extended_header_size), std::ios_base::cur);
    }

    return id3_tag_header;
}

// Accepts an ifstream at past the ID3 header, scans ID3 frames and writes them to JSON.
//
// Arguments:
// - fin: Reference to the ifstream at the start of the first ID3 frame.
// - id3_size: The size of the ID3 tag, including the header.
// - song: The JSON to store the song's ID3 data in.
// - verbose: Optional bool to enable console output, default false.
void extractId3Frames(std::ifstream& fin, const uint32_t id3_size, nlohmann::json& song, const bool verbose) {
    const int curr = fin.tellg(); // Current pos on the ifstream, just past the ID3 header

    // For every frame:
    while (fin.good() && fin.tellg() < curr + id3_size) {
        // Create the frame header
        ID3FrameHeader id3_frame_header{};
        fin.read(reinterpret_cast<char*>(&id3_frame_header), sizeof(id3_frame_header));

        // Optional? Maybe this can be deleted after removing the couts in this function.
        // Exit when reaching padding bytes ( \0 = 00000000 )
        if (id3_frame_header.frame_id[0] == '\0') break;

        if (verbose) {
            std::cout << "frame: " << charsToStr(id3_frame_header.frame_id);
            std::cout << ", size: " << fromSynchsafe32(id3_frame_header.size) << "\n";
        }

        // Read the frame data:
        const uint32_t size = fromSynchsafe32(id3_frame_header.size);
        std::vector<uint8_t> frame_data(size);
        fin.read(reinterpret_cast<char*>(frame_data.data()), fromSynchsafe32(id3_frame_header.size));

        if (const auto frame = makeFrame(id3_frame_header, frame_data)) {
            frame->toJson(song);
        }
    }
}

//
std::unique_ptr<ID3Frame> makeFrame(ID3FrameHeader header, const std::vector<uint8_t>& data) {
    const std::string id = header.frameIdToStr();
    if (id == "TXXX") return std::make_unique<TXXX>(header, data);
    if (id == "COMM") return std::make_unique<COMM>(header, data);
    if (id == "APIC") return std::make_unique<APIC>(header, data);
    if (id[0] == 'T') return std::make_unique<TextInformationFrame>(header, data);
    return nullptr;
}
