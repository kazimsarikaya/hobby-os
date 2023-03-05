/*
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */

#ifndef ___SETUP_H
#define ___SETUP_H 0

#include <types.h>
#include <logging.h>
#include <memory.h>
#include "os_io.h"

#ifndef RAMSIZE
#define RAMSIZE 0x100000
#endif

#define REDCOLOR "\033[1;31m"
#define GREENCOLOR "\033[1;32m"
#define RESETCOLOR "\033[0m"

FILE* mem_backend = NULL;
int32_t mem_backend_fd = 0;
uint64_t mmmap_address = 4ULL << 30;
uint64_t mmap_size = RAMSIZE;

int printf(const char* format, ...);
int vprintf ( const char* format, va_list arg );

size_t video_printf(char_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = vprintf(fmt, args);
    va_end(args);

    return res;
}

void print_success(const char* msg){
    printf("%s%s%s%s", GREENCOLOR, msg, RESETCOLOR, "\r\n");
}

void print_error(const char* msg){
    printf("%s%s%s%s", REDCOLOR, msg, RESETCOLOR, "\r\n");
}

void cpu_hlt(){
}

#define PRINTLOG(M, L, msg, ...)  if(LOG_NEED_LOG(M, L)) { \
            if(LOG_LOCATION) { video_printf("%s:%i:%s:%s: " msg "\n", __FILE__, __LINE__, logging_module_names[M], logging_level_names[L], ## __VA_ARGS__); } \
            else {video_printf("%s:%s: " msg "\n", logging_module_names[M], logging_level_names[L], ## __VA_ARGS__); } }

memory_heap_t* heap = NULL;

int8_t setup_ram2() {

    if(mmap_size < 4096) {
        print_error("invalid ram size min should be 4k");
        return -1;
    }

    mem_backend = tmpfile();

    if(mem_backend == NULL) {
        print_error("cannot create ram tmpfile");
        return -2;
    }

    fseek(mem_backend, mmap_size - 1, SEEK_SET);

    int8_t zero = 0;

    fwrite(&zero, 1, 1, mem_backend);
    fseek(mem_backend, 0, SEEK_SET);

    mem_backend_fd  = fileno(mem_backend);

    void* mmap_res = mmap((void*)mmmap_address, mmap_size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE, mem_backend_fd, 0);

    printf("mmap res %p\n", mmap_res);

    if(mmap_res != (void*)mmmap_address) {
        print_error("cannot mmap ram tmpfile");
        fclose(mem_backend);
        mem_backend = NULL;
        return -3;
    }

    heap = memory_create_heap_simple(mmmap_address, mmmap_address + mmap_size);

    if(heap == NULL) {
        print_error("cannot setup heap");
        return -4;
    }

    memory_set_default_heap(heap);

    return 0;
}

void remove_ram2() {
    if(heap) {
        memory_heap_stat_t stat;
        memory_get_heap_stat(&stat);
        printf("mem stats:\n\tmalloc count: 0x%lx\n\tfree count: 0x%lx\n\ttotal space: 0x%lx\n\tfree space: 0x%lx\n\tdiff: 0x%lx\n", stat.malloc_count, stat.free_count, stat.total_size, stat.free_size, stat.total_size - stat.free_size);

        if(stat.malloc_count != stat.free_count) {
            print_error("memory leak detected");
        } else {
            print_success("no memeory leak detected");
        }
    }

    if(mem_backend) {
        munmap((void*)mmmap_address, mmap_size);
        fclose(mem_backend);
    }
}

void __attribute__((constructor)) start_ram() {
    int8_t res = setup_ram2();

    if(res) {
        exit(res);
    }
}

void __attribute__((destructor)) stop_ram() {
    remove_ram2();
}


#ifdef ___TESTMODE

uint8_t mem_area[RAMSIZE] = {0};
uint64_t __kheap_bottom = 0;

void* SYSTEM_INFO;

void* KERNEL_FRAME_ALLOCATOR = NULL;

typedef void   * frame_t;
typedef int8_t memory_paging_page_type_t;
typedef void   * memory_page_table_t;


int8_t memory_paging_add_va_for_frame_ext(memory_page_table_t* p4, uint64_t va_start, frame_t* frm, memory_paging_page_type_t type){
    UNUSED(p4);
    UNUSED(va_start);
    UNUSED(frm);
    UNUSED(type);
    return 0;
}

void dump_ram(char_t* fname){
    FILE* fp = fopen( fname, "w" );
    fwrite(mem_area, 1, RAMSIZE, fp );

    fclose(fp);
}

void* task_get_current_task(){
    return NULL;
}

void* lock_create_with_heap(memory_heap_t* heap){
    UNUSED(heap);
    return NULL;
}

void* lock_create_with_heap_for_future(memory_heap_t* heap, boolean_t for_future){
    UNUSED(heap);
    UNUSED(for_future);
    return NULL;
}

int8_t lock_destroy(void* lock){
    UNUSED(lock);
    return 0;
}

void lock_acquire(void* lock){
    UNUSED(lock);
}

void lock_release(void* lock){
    UNUSED(lock);
}

#endif

#endif
