#ifndef __ZPOOL_H__
#define __ZPOOL_H__

#include "ztools.h"

namespace lib {

    template <typename __T, s32 __list_nums__ = 1, s32 __chunk_nums__ = 64>
    class zpool {

        enum class CHUNK_STATE {
            USING,
            FREEING
        };

        struct ChunkList;
        struct Chunk {
            s8 buffer[sizeof(__T)];
            ChunkList *parent;
            Chunk *prev;
            Chunk *next;
            s32 len;
            CHUNK_STATE state;
        };

        struct ChunkList {
            s32 count;
            ChunkList *prev;
            ChunkList *next;
            Chunk chunks[__chunk_nums__];
        };

    public:

        zpool() : head_(nullptr), list_head_(nullptr), list_count_(0) { init_pool(__list_nums__); }

        ~zpool() {
            while (list_head_ != nullptr) { 
                auto tmp = list_head_;
                list_head_ = list_head_->next;
                free(tmp);
            }
        }

        __T * Create() {
            Chunk * chunk = create();
            __T *t = new (chunk->buffer) __T();
            return t;
        }

        // template <typename... Args>
        // __T * Create(Args... args) {
        //     Chunk *chunk = create();
        //     __T *t = new (chunk->buffer) __T(args...);
        //     return t;
        // }

        template <typename... Args>
        __T * Create(Args&&... args) {
            Chunk *chunk = create();
            __T *t = new (chunk->buffer) __T(std::forward<Args>(args)...);
            return t;
        }

        void Recover(__T *t) {
            t->~__T();
            recover((struct Chunk *)t);
        }

        s32 Count() { return list_count_ * __chunk_nums__; }

    private:

        void init_pool(s32 __list_nums) {
            for (int i = 0; i < __list_nums; ++i) {
                ChunkList *list = (ChunkList *)malloc(sizeof(ChunkList));
                zassert(list != nullptr, "malloc for new ChunkList failed");
                init_chunk_list(list);

                list->next = list_head_;
                if (list_head_ != nullptr) { list_head_->prev = list; }
                list_head_ = list;
            }
            list_count_++;
        }

        inline void init_chunk_list(ChunkList *__list) {
            __list->prev = nullptr;
            __list->next = nullptr;
            __list->count = 0;
            for (int i = 0; i < __chunk_nums__; ++i) {
                __list->chunks[i].parent = __list;
                __list->chunks[i].state = CHUNK_STATE::FREEING;
                __list->chunks[i].len = sizeof(struct Chunk);
                __list->chunks[i].prev = nullptr;
                __list->chunks[i].next = nullptr;
                add(&(__list->chunks[i]));
            }
        }

        inline void free_chunk_list(ChunkList *__list) {
            for (int i = 0; i < __chunk_nums__; ++i) {
                zassert(__list->chunks[i].state == CHUNK_STATE::FREEING, "freed ChunkList but its Chunk is not free");
                remove(&(__list->chunks[i]));
            }
            if (__list->next != nullptr) { __list->next->prev = __list->prev; }
            if (__list->prev != nullptr) { __list->prev->next = __list->next; }
            if (__list == list_head_) { list_head_ = __list->next; }
            free(__list);
            --list_count_;
        }


    private:

        Chunk * create() {
            Chunk *ret = nullptr;
            if (head_ == nullptr) { init_pool(1); }
            ret = head_;
            remove(head_);
            zassert(ret->state == CHUNK_STATE::FREEING && ret->len == sizeof(struct Chunk), "Chunk invalid");
            ++ret->parent->count;
            ret->state = CHUNK_STATE::USING;
            return ret;
        }

        void recover(Chunk *__chunk) {
            zassert(__chunk->state == CHUNK_STATE::USING && __chunk->len == sizeof(struct Chunk), "Chunk invalid");
            zassert(__chunk->parent->count > 0, "ChunkList error");
            --__chunk->parent->count;
            __chunk->state = CHUNK_STATE::FREEING;
            if (__chunk->parent->count == 0 && list_count_ > __list_nums__) { free_chunk_list(__chunk->parent); }
            else { add(__chunk); }
        }

        inline void add(Chunk *__chunk) {
            __chunk->next = head_;
            if (head_ != nullptr) { head_->prev = __chunk; }
            head_ = __chunk;
        }

        inline void remove(Chunk *__chunk) {
            if (__chunk->next != nullptr) { __chunk->next->prev = __chunk->prev; }
            if (__chunk->prev != nullptr) { __chunk->prev->next = __chunk->next; }
            if (__chunk == head_) { head_ = __chunk->next; }
            __chunk->next = nullptr;
            __chunk->prev = nullptr;
        }


    private:
        Chunk *head_;
        ChunkList *list_head_;
        s32 list_count_;
    };
}


#define create_from_pool(pool, ...) pool.Create(__VA_ARGS__)
#define recover_to_pool(pool, p) pool.Recover(p)



#endif // __ZPOOL_H__