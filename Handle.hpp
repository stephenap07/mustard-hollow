#include <cstdint>
#include <cstddef>

template<size_t N1, size_t N2>
struct Handle {
    uint32_t index : N1;
    uint32_t generation : N2;
};
