#ifndef FREEBLOCKSALLOCATOR_H
#define FREEBLOCKSALLOCATOR_H

#include "allocator.h"

class FreeBlocksAllocator : public Allocator {
public:

    pointer allocate(size_t size) override {
        if (size <= 0) {
            std::cerr << "size must be bigger than 0\n";
            return nullptr;
        }
        if (size > size_total - size_used) { 
            return nullptr; 
        }
        auto *header = find(size);
        if (header == nullptr) { 
            return nullptr;
        }
        block_separation(header, size);
        return header + 1;
    };

    void deallocate(pointer ptr) override {
        if (!check_address(ptr)) {
            return;
        }
        auto *header = static_cast<Header *>(ptr) - 1;
        header->available = true;
        size_used -= header->size;
        defragmentation(header);
    };

    explicit FreeBlocksAllocator(size_t size) {
        if ((pointer_start = malloc(size)) == nullptr) {
            std::cerr << "failed to allocate memory\n";
            return;
        }
        size_total = size;
        pointer_end = static_cast<void *>(static_cast<char *>(pointer_start) + size_total);
        auto *header = (Header *) pointer_start;
        header->size = (size_total - size_header);
        header->size_previous = 0;
        header->available = true;
        size_used = size_header;
    };

private:

    void defragmentation(Header *header) {
        if (previous_free(header)) {
            auto *previous = header->previous();
            if (header->next() != pointer_end) {
                header->next()->size_previous += previous->size + size_header;
            }
            previous->size += header->size + size_header;
            size_used -= size_header;
            header = previous;
        }
        if (next_free(header)) {
            header->size += size_header + header->next()->size;
            size_used -= size_header;
            auto *next = header->next();
            if (next != pointer_end) { next->size_previous = header->size; }
        }
    }

    bool previous_free(Header *header) {
        auto *previous = header->previous();
        return header != pointer_start && previous->available;
    }

    bool next_free(Header *header) {
        auto *next = header->next();
        return header != pointer_end && next->available;
    }

};

#endif //FREEBLOCKSALLOCATOR_H