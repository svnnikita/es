#pragma once

#include <cstdint>

constexpr uint8_t SIZE{32};

class Circular_buffer{
    public:
        void put(uint8_t);
        uint8_t get();
        bool empty();
        bool full();
    
        Circular_buffer();        // Circular_buffer b1;
    
    private:
        uint8_t buf[32];    // Противоречит конструктору с параметрами!
        uint8_t wr_idx;
        uint8_t rd_idx;
        bool full_;
    };