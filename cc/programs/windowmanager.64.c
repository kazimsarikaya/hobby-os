/**
 * @file windowmanager.64.c
 * @brief Window Manager implementation
 *
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */

#include <windowmanager.h>
#include <logging.h>
#include <memory.h>
#include <utils.h>
#include <hashmap.h>
#include <cpu.h>
#include <cpu/task.h>
#include <utils.h>
#include <device/mouse.h>
#include <device/kbd.h>
#include <device/kbd_scancodes.h>
#include <strings.h>
#include <graphics/screen.h>
#include <graphics/text_cursor.h>
#include <graphics/font.h>

MODULE("turnstone.user.programs.windowmanager");

void video_text_print(const char_t* text);

extern boolean_t windowmanager_initialized;

extern window_t* windowmanager_current_window;
extern hashmap_t* windowmanager_windows;

extern buffer_t* shell_buffer;
extern buffer_t* mouse_buffer;

static int8_t windowmanager_main(void) {
    uint32_t font_width = 0, font_height = 0;

    font_get_font_dimension(&font_width, &font_height);

    screen_info_t screen_info = screen_get_info();

    windowmanager_current_window = windowmanager_create_greater_window();
    windowmanager_windows = hashmap_integer(16);

    list_t* mq = list_create_queue();

    task_add_message_queue(mq);

    hashmap_put(windowmanager_windows, (void*)windowmanager_current_window->id, windowmanager_current_window);

    task_set_interruptible();

    shell_buffer = buffer_new_with_capacity(NULL, 4100);
    mouse_buffer = buffer_new_with_capacity(NULL, 4096);

    windowmanager_clear_screen(windowmanager_current_window);
    SCREEN_FLUSH(0, 0, 0, 0, screen_info.width, screen_info.height);

    windowmanager_initialized = true;

    while(true) {
        boolean_t flush_needed = windowmanager_draw_window(windowmanager_current_window);


        if(flush_needed) {
            text_cursor_show();
            SCREEN_FLUSH(0, 0, 0, 0, screen_info.width, screen_info.height);
        }

        while(list_size(mq) == 0) {
            task_set_message_waiting();
            task_yield();

            if(buffer_get_length(shell_buffer) == 0 && buffer_get_length(mouse_buffer) == 0) {
                continue;
            } else {
                break;
            }
        }

        uint64_t kbd_length = 0;
        uint32_t kbd_ev_cnt = 0;
        uint64_t mouse_length = 0;
        uint32_t mouse_ev_cnt = 0;

        kbd_report_t* kbd_data = (kbd_report_t*)buffer_get_all_bytes_and_reset(shell_buffer, &kbd_length);
        mouse_report_t* mouse_data = (mouse_report_t*)buffer_get_all_bytes_and_reset(mouse_buffer, &mouse_length);

        if(kbd_length == 0 && mouse_length == 0) {
            memory_free(kbd_data);
            memory_free(mouse_data);

            continue;
        }

        if(mouse_length) {
            mouse_ev_cnt = mouse_length / sizeof(mouse_report_t);

            mouse_report_t* last = &mouse_data[mouse_ev_cnt - 1];

            if(last->buttons & MOUSE_BUTTON_LEFT) {
                text_cursor_hide();
                text_cursor_move(last->x / font_width, last->y / font_height);
                text_cursor_show();
            }
        }

        memory_free(mouse_data);

        if(kbd_length == 0) {
            memory_free(kbd_data);

            continue;
        }

        char_t data[4096] = {0};
        uint32_t data_idx = 0;

        kbd_ev_cnt = kbd_length / sizeof(kbd_report_t);

        for(uint32_t i = 0; i < kbd_ev_cnt; i++) {
            if(kbd_data[i].is_pressed) {
                if(kbd_data[i].is_printable) {
                    if(kbd_data[i].key == '\n' && windowmanager_current_window->on_enter) {
                        windowmanager_current_window->on_enter(windowmanager_current_window);
                    } else if(kbd_data[i].key == '\t'){
                        windowmanager_move_cursor_to_next_input(windowmanager_current_window);
                    }else {
                        data_idx = windowmanager_append_wchar_to_buffer(kbd_data[i].key, data, data_idx);
                    }
                } else {
                    if(kbd_data[i].key == KBD_SCANCODE_BACKSPACE) {
                        data[data_idx++] = '\b';
                        data[data_idx++] = ' ';
                        data[data_idx++] = '\b';
                    } else if(kbd_data[i].key == KBD_SCANCODE_F2) {
                        window_t* options_window = windowmanager_create_primary_options_window();

                        if(options_window != NULL) {
                            windowmanager_insert_and_set_current_window(options_window);
                        }
                    } else if(kbd_data[i].key == KBD_SCANCODE_F3) {
                        windowmanager_remove_and_set_current_window(windowmanager_current_window);
                    } else if(kbd_data[i].key == KBD_SCANCODE_UP) {
                        text_cursor_hide();
                        text_cursor_move_relative(0, -1);
                        text_cursor_show();
                    } else if(kbd_data[i].key == KBD_SCANCODE_DOWN) {
                        text_cursor_hide();
                        text_cursor_move_relative(0, 1);
                        text_cursor_show();
                    } else if(kbd_data[i].key == KBD_SCANCODE_LEFT) {
                        text_cursor_hide();
                        text_cursor_move_relative(-1, 0);
                        text_cursor_show();
                    } else if(kbd_data[i].key == KBD_SCANCODE_RIGHT) {
                        text_cursor_hide();
                        text_cursor_move_relative(1, 0);
                        text_cursor_show();
                    }
                }
            }
        }

        data[data_idx] = NULL;

        memory_free(kbd_data);

        window_t* edit_area = NULL;

        if(windowmanager_find_window_by_text_cursor(windowmanager_current_window, &edit_area)) {
            if(edit_area != NULL) {
                windowmanager_set_window_text(edit_area, data);
            }
        }
    }

    return 0;
}

uint64_t windowmanager_task_id = 0;

int8_t windowmanager_init(void) {
    memory_heap_t* heap = memory_get_default_heap();

    windowmanager_task_id = task_create_task(heap, 32 << 20, 64 << 10, windowmanager_main, 0, NULL, "windowmanager");

    while(!windowmanager_initialized) {
        cpu_sti();
        cpu_idle();
    }

    return windowmanager_task_id == -1ULL ? -1 : 0;
}
