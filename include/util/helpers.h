//
// Created by Sjoerd de Jonge on 02/04/2026.
//

#pragma once
#include <bit>
#include <concepts>

#ifndef MUSIC_LIBRARY_INDEXER_HELPERS_H
#define MUSIC_LIBRARY_INDEXER_HELPERS_H

template <std::integral T>
T fromBigEndianInt(T value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

inline float fromBigEndianFloat(const float value) {
    if constexpr (std::endian::native == std::endian::big) {
        return value;
    } else {
        auto asInt = std::bit_cast<uint32_t>(value);
        asInt = std::byteswap(asInt);
        return std::bit_cast<float>(asInt);
    }
}

#endif //MUSIC_LIBRARY_INDEXER_HELPERS_H
