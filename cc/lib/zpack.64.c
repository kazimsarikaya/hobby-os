/*
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */

#include <zpack.h>

typedef struct zpack_match_t {
    int32_t best_size;
    int64_t best_pos;
} zpack_match_t;

static int32_t zpack_count_similar (buffer_t in, int64_t in_len, int64_t p, int64_t in_p) {
    int32_t similar = 0;

    while (in_p < in_len && buffer_peek_byte_at_position(in, p) == buffer_peek_byte_at_position(in, in_p)) {
        similar++;
        p++;
        in_p++;

        if(similar == 0x7f + 0x40 + 4) {
            break;
        }
    }

    return similar;
}

static zpack_match_t zpack_find_bestmatch(buffer_t in, int64_t in_len, int64_t in_p) {
    int64_t p =  in_p - 1;
    int32_t best_size = 0;
    int64_t best_pos = p;

    while (p > 0 && p >= (in_p - 255)) {
        int32_t size = zpack_count_similar(in, in_len, p, in_p);

        if (size > best_size) {
            best_size = size;
            best_pos = p;
        }

        if(best_size == 0x7f + 0x40 + 4) {
            break;
        }

        p--;
    }

    return (zpack_match_t){.best_size = best_size, .best_pos = best_pos};
}

int64_t zpack_pack (buffer_t in, buffer_t out) {
    buffer_t individuals = buffer_new_with_capacity(NULL, 257);
    int64_t in_len = buffer_get_length(in);

    while (buffer_remaining(in)) {
        int64_t in_p = (int64_t)buffer_get_position(in);

        zpack_match_t zpm = zpack_find_bestmatch(in, in_len, in_p);

        if(zpm.best_size > 3) {
            /* copy */
            if (buffer_get_length(individuals)) {
                out = buffer_append_byte(out, (buffer_get_length(individuals) - 1) | 0xC0);

                out = buffer_append_buffer(out, individuals);

                buffer_reset(individuals);
            }

            if(zpm.best_size > 0x7f + 0x40 + 4) {
                zpm.best_size = 0x7f + 0x40 + 4;
            }

            int32_t offset = zpm.best_pos - in_p;

            if(!buffer_seek(in, zpm.best_size, BUFFER_SEEK_DIRECTION_CURRENT)) {
                return -1;
            }

            zpm.best_size -= 4;

            out = buffer_append_byte(out, zpm.best_size);
            out = buffer_append_byte(out, offset);

        } else {
            /* individual bytes */
            individuals = buffer_append_byte(individuals, buffer_get_byte(in));

            if (buffer_get_length(individuals) == 0x40) {
                out = buffer_append_byte(out, (buffer_get_length(individuals) - 1) | 0xC0);

                out = buffer_append_buffer(out, individuals);

                buffer_reset(individuals);
            }
        }
    }

    if (buffer_get_length(individuals)) {
        out = buffer_append_byte(out, (buffer_get_length(individuals) - 1) | 0xC0);

        out = buffer_append_buffer(out, individuals);

        buffer_reset(individuals);
    }

    buffer_destroy(individuals);

    return buffer_get_length(out);
}

int64_t zpack_unpack(buffer_t in, buffer_t out) {
    while (buffer_remaining(in)) {

        int32_t size = buffer_get_byte(in);

        if ((size & 0xC0) == 0xC0) {
            size &= 0x3f;
            size++;

            uint8_t* data = buffer_get_bytes(in, size);
            out = buffer_append_bytes(out, data, size);

            memory_free(data);

        } else {
            int32_t offset;

            offset = -256 | (int8_t)buffer_get_byte(in);
            size += 3;

            int64_t o_p = buffer_get_position(out);

            while(size-- >= 0 ) {
                out = buffer_append_byte(out, buffer_peek_byte_at_position(out, o_p + offset));
                o_p++;
            }
        }
    }

    return buffer_get_length(out);
}
