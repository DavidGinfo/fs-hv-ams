#pragma once

#include <cstdint>
#include <type_traits>

#define force_inline inline __attribute__((always_inline))

/**
 * Fills an array with a default value.
 *
 * @tparam T the array type
 * @param array the array to be filled
 * @param value the default value that gets filled in
 * @param size the size of the array
 */
template<typename T>
void fillArray(T *array, T value, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        array[i] = value;
    }
}

template<typename From>
std::make_signed_t<From> toSigned(From value);

template<typename T>
T swap(T value);

// TODO implement template for reverse function
constexpr uint8_t reverse(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

constexpr uint16_t reverse16(uint16_t b) {
    b = (b & 0xFF00) >> 8 | (b & 0x00FF) << 8;
    b = (b & 0xF0F0) >> 4 | (b & 0x0F0F) << 4;
    b = (b & 0xCCCC) >> 2 | (b & 0x3333) << 2;
    b = (b & 0xAAAA) >> 1 | (b & 0x5555) << 1;
    return b;
}

template<typename T>
T shiftArithmetic(T value, uint8_t amount);

template <auto V>
static constexpr auto FORCE_CONSTEVAL = V;
