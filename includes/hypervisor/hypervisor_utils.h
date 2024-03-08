/**
 * @file hypervisor_utils.h
 * @brief defines hypervisor related utility functions
 *
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */



#ifndef ___HYPERVISOR_UTILS_H
#define ___HYPERVISOR_UTILS_H 0

#include <types.h>
#include <memory/paging.h>

uint64_t hypervisor_allocate_region(frame_t** frame, uint64_t size);
uint64_t hypervisor_create_stack(uint64_t stack_size);
int8_t   vmx_validate_capability(uint64_t target, uint32_t allowed0, uint32_t allowed1);
uint32_t vmx_fix_reserved_1_bits(uint32_t target, uint32_t allowed0);
uint32_t vmx_fix_reserved_0_bits(uint32_t target, uint32_t allowed1);

int8_t hypevisor_deploy_program(const char_t* entry_point_name);

#endif
