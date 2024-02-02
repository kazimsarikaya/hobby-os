/**
 * @file compiler_string.64.c
 * @brief Implementation of the string constant.
 *
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */

#include <compiler/compiler.h>
#include <logging.h>
#include <strings.h>
#include <utils.h>

MODULE("turnstone.compiler.codegen");

int8_t compiler_execute_string_const(compiler_t* compiler, compiler_ast_node_t* node, int64_t* result) {
    UNUSED(result);

    compiler_symbol_t * symbol = memory_malloc(sizeof(compiler_symbol_t));

    if (symbol == NULL) {
        return -1;
    }

    symbol->name = sprintf(".L%i", compiler->next_label_id++);
    symbol->type = COMPILER_SYMBOL_TYPE_STRING;
    symbol->size = strlen(node->token->text) + 1;
    symbol->is_const = true;
    symbol->string_value = node->token->text;

    node->symbol = symbol;

    buffer_printf(compiler->rodata_buffer, ".section .rodata.%s\n", symbol->name);
    buffer_printf(compiler->rodata_buffer, ".align 8\n");
    buffer_printf(compiler->rodata_buffer, ".local %s\n", symbol->name);
    buffer_printf(compiler->rodata_buffer, ".type %s, @object\n", symbol->name);
    buffer_printf(compiler->rodata_buffer, ".size %s, %lli\n", symbol->name, symbol->size);
    buffer_printf(compiler->rodata_buffer, "%s:\n", symbol->name);
    buffer_printf(compiler->rodata_buffer, "\t.string \"%s\"\n", symbol->string_value);
    buffer_printf(compiler->rodata_buffer, "\n\n\n");

    hashmap_put(compiler->main_symbol_table->symbols, symbol->name, symbol);

    return 0;
}
