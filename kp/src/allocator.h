#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>

class Allocator {
public:
    typedef void value_type;
    typedef value_type *pointer;

    Allocator() = default;

    ~Allocator() {
        ::free(pointer_start);
    }

    void free() {
        auto *header = static_cast<Header *>(pointer_start);
        header->size = (size_total - size_header);
        header->available = true;
        size_used = size_header;
    };

    virtual pointer allocate(size_t size) = 0;
    virtual void deallocate(pointer ptr) = 0;

protected:

    struct Header {
    public:
        size_t size;
        size_t size_previous;
        bool available;

        inline Header *next() {
            return (Header *) ((char *) (this + 1) + size);
        }

        inline Header *previous() {
            return (Header *) ((char *) this - size_previous) - 1;
        }

    };

    const size_t size_header = sizeof(Header);
    pointer pointer_start = nullptr;
    pointer pointer_end = nullptr;
    size_t size_total = 0;
    size_t size_used = 0;

    Header* find(size_t size) {
        auto *header = static_cast<Header *>(pointer_start);
        while (!header->available || header->size < size) {
            header = header->next();
            if (header >= pointer_end) { 
                return nullptr; 
            }
        }
        return header;
    }

    void block_separation(Header *header, size_t chunk) {
        size_t size_block = header->size;
        header->size = chunk;
        header->available = false;
        if (size_block - chunk >= size_header) {
            auto *next = header->next();
            next->size_previous = chunk;
            next->size = size_block - chunk - size_header;
            next->available = true;
            size_used += chunk + size_header;
            auto *followed = next->next();
            if (followed < pointer_end) {
                followed->size_previous = next->size;
            }
        } else {
            header->size = size_block;
            size_used += size_block;
        }
    }

    bool check_address(void *ptr) {
        auto *header = static_cast<Header *>(pointer_start);
        while (header < pointer_end) {
            if (header + 1 == ptr){ 
                return true; 
            }
            header = header->next();
        }
        return false;
    }

};

#endif //ALLOCATOR_H