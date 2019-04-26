#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "memory_block.h"
#include "direct_mapped.h"

direct_mapped_cache* dmc_init(main_memory* mm)
{
    // TODO
    direct_mapped_cache* result = malloc(sizeof(direct_mapped_cache));
    result->mm = mm;
    result->cs = cs_init();
    // set dirty bits to 0
    for(int i = 0; i < DIRECT_MAPPED_NUM_SETS; i++){ 
        result->dirty_bits[i] = false;
    }
    return result;
}

static int addr_to_set(void* addr)
{
    uint16_t result = ((int)addr >> MAIN_MEMORY_BLOCK_SIZE_LN);
    result = result & DIRECT_MAPPED_NUM_SETS_LN-1;
    return result;
}


void dmc_store_word(direct_mapped_cache* dmc, void* addr, unsigned int val)
{
    // Precompute start address of memory block
    size_t addr_offt = (size_t) (addr - MAIN_MEMORY_START_ADDR) % MAIN_MEMORY_BLOCK_SIZE;
    void* mb_start_addr = addr - addr_offt;

    // get cache index for block
    int index = addr_to_set(mb_start_addr); 

    // get cacheline from cashe
    memory_block* cacheline = dmc->cache[index];

    // Check addresses are equal
    if(cacheline != NULL && cacheline->start_addr == mb_start_addr){
        // if equal, update cache line, set dirty bit to true
        unsigned int* mb_addr = cacheline->data + addr_offt;
        *mb_addr = val;
        dmc->dirty_bits[index] = true;
    }
    else {
        //miss
        ++dmc->cs.w_misses;
        // if not equal, need to evcit, check dirty bit
        if(dmc->dirty_bits[index] == true){
            // if dirty bit true, write to memory and overrwite
            mm_write(dmc->mm, mb_start_addr, cacheline);
            unsigned int* mb_addr = cacheline->data + addr_offt;
            *mb_addr = val;
        }
        else {
            // if dirty bit false, overrwite
            unsigned int* mb_addr = cacheline->data + addr_offt;
            *mb_addr = val;
        }
    }

    // Update statistics
    ++dmc->cs.w_queries;
}

unsigned int dmc_load_word(direct_mapped_cache* dmc, void* addr)
{   
    // declare result and mb_addr
    unsigned int result;
    unsigned int* mb_addr;

    // Precompute start address of memory block
    size_t addr_offt = (size_t) (addr - MAIN_MEMORY_START_ADDR) % MAIN_MEMORY_BLOCK_SIZE;
    void* mb_start_addr = addr - addr_offt;
    
    // get cache index for block
    int index = addr_to_set(mb_start_addr);

    // get cacheline from cashe
    memory_block* cacheline = dmc->cache[index];    

    // Check addresses are equal
    // checxk for nul
    if(cacheline != NULL && cacheline->start_addr == mb_start_addr){
        // hit if they are
        mb_addr = cacheline->data + addr_offt;
        result = *mb_addr;
    }
    else {
        // miss if not equal
        ++dmc->cs.w_misses;
        // check dirty bit
        if (dmc->dirty_bits[index] == true){
            // if dirty, writeback
            mm_write(dmc->mm, mb_start_addr, cacheline);
            dmc->dirty_bits[index] = false;
        }

        // evect via overrwite
        mb_free(cacheline);
        cacheline = mm_read(dmc->mm, mb_start_addr);
        mb_addr = cacheline->data + addr_offt;
        result = *mb_addr;
    }
    return result;
}

void dmc_free(direct_mapped_cache* dmc)
{
    for(int i = 0; i < DIRECT_MAPPED_NUM_SETS; i++){
        mb_free(dmc->cache[i]);
    }
    free(dmc);
    printf("Freed\n");
}