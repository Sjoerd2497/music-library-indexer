//
// Created by Sjoerd de Jonge on 24/04/2026.
//

#ifndef MLI_COMMANDS_H
#define MLI_COMMANDS_H
#include <filesystem>

#include "options.h"


void mliIndex(const std::filesystem::path& directory_path, const IndexOptions& options);
void mliHelp();

#endif //MLI_COMMANDS_H
