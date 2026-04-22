//
// Created by Sjoerd de Jonge on 22/04/2026.
//

#ifndef MLI_CONFIG_H
#define MLI_CONFIG_H

// Struct containing options for the index command.
//
// OPTION           RESULT
// verbose:         More console output during scanning.
// subdirectories:  Include files in subdirectories in scan.
// include_apic:    Include attached picture ID3 frame as base64 string (image stored in text format).
// output_path:     String containing file location and name.
struct IndexOptions {
    bool verbose = false;
    bool subdirectories = true;
    bool include_apic = false;
    std::string output_path = "library_snapshot.json";
};

#endif //MLI_CONFIG_H
