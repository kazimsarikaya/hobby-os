/**
 * @file test_bigint.c
 * @brief Big integer arithmetic
 *
 * This work is licensed under TURNSTONE OS Public License.
 * Please read and understand latest version of Licence.
 */
#define RAMSIZE 0x800000
#include "setup.h"
#include <bigint.h>
#include <strings.h>

int32_t main(void);

static int32_t bigint_test_set_int(void) {
    bigint_t* bigint = bigint_create();

    if (bigint) {
        bigint_destroy(bigint);
    } else {
        print_error("bigint_create failed");
        return -1;
    }

    bigint = bigint_create();

    if(bigint) {
        bigint_set_int64(bigint, 0x1234567890ABCDEFLL);

        const char_t* str = bigint_to_str(bigint);

        if (str) {
            printf("bigint_to_string: %s\n", str);

            if(strncmp(str, "1234567890ABCDEF", 16) != 0) {
                print_error("bigint_to_string failed");
            } else {
                print_success("bigint_to_string passed");
            }


            memory_free((void*)str);
        } else {
            bigint_destroy(bigint);
            print_error("bigint_to_string failed");
            return -1;
        }

        bigint_set_int64(bigint, -0x1234567890ABCDEFLL);

        str = bigint_to_str(bigint);

        if (str) {
            printf("negative bigint_to_string: %s\n", str);

            if(strncmp(str, "-1234567890ABCDEF", 17) != 0) {
                print_error("negative bigint_to_string failed");
            } else {
                print_success("negative bigint_to_string passed");
            }


            memory_free((void*)str);
            bigint_destroy(bigint);
        } else {
            bigint_destroy(bigint);
            print_error("negative bigint_to_string failed");
            return -1;
        }
    } else {
        print_error("bigint_create failed");
        return -1;
    }

    return 0;
}

static int32_t bigint_test_set_str(void) {
    bigint_t* bigint = bigint_create();

    if (bigint) {
        bigint_destroy(bigint);
    } else {
        print_error("bigint_create failed");
        return -1;
    }

    bigint = bigint_create();

    if(bigint) {
        bigint_set_str(bigint, "1234567890ABCDEF1234567890ABCDEF");

        const char_t* str = bigint_to_str(bigint);

        if (str) {
            printf("bigint_to_string: %s\n", str);

            if(strncmp(str, "1234567890ABCDEF1234567890ABCDEF", 32) != 0) {
                print_error("bigint_to_string failed");
            } else {
                print_success("bigint_to_string passed");
            }

            memory_free((void*)str);

        } else {
            bigint_destroy(bigint);
            print_error("bigint_to_string failed");
            return -1;
        }

        bigint_set_str(bigint, "-1234567890ABCDEF1234567890ABCDEF");

        str = bigint_to_str(bigint);

        if (str) {
            printf("negative bigint_to_string: %s\n", str);

            if(strncmp(str, "-1234567890ABCDEF1234567890ABCDEF", 33) != 0) {
                print_error("negative bigint_to_string failed");
            } else {
                print_success("negative bigint_to_string passed");
            }

            memory_free((void*)str);
            bigint_destroy(bigint);
        } else {
            bigint_destroy(bigint);
            print_error("negative bigint_to_string failed");
            return -1;
        }

    } else {
        print_error("bigint_create failed");
        return -1;
    }

    return 0;
}

static int32_t bigint_test_shl_shr(void) {
    bigint_t* bigint_1 = bigint_create();
    bigint_t* bigint_2 = bigint_create();

    if(bigint_1 && bigint_2) {
        bigint_set_str(bigint_1, "1234");

        if(bigint_shl(bigint_2, bigint_1, 4) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        const char_t* str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shl: %s\n", str);

            if(strncmp(str, "12340", 5) != 0) {
                print_error("bigint_shl failed");
            } else {
                print_success("bigint_shl passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        if(bigint_shr(bigint_2, bigint_1, 4) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shr: %s\n", str);

            if(strncmp(str, "123", 3) != 0) {
                print_error("bigint_shr failed");
            } else {
                print_success("bigint_shr passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        if(bigint_shl(bigint_2, bigint_1, 0) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shl: %s\n", str);

            if(strncmp(str, "1234", 4) != 0) {
                print_error("bigint_shl failed");
            } else {
                print_success("bigint_shl passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        if(bigint_shr(bigint_2, bigint_1, 0) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shr: %s\n", str);

            if(strncmp(str, "1234", 4) != 0) {
                print_error("bigint_shr failed");
            } else {
                print_success("bigint_shr passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        if(bigint_shl(bigint_2, bigint_1, 64) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shl: %s\n", str);

            if(strncmp(str, "12340000000000000000", 20) != 0) {
                print_error("bigint_shl failed");
            } else {
                print_success("bigint_shl passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        if(bigint_shr(bigint_2, bigint_1, 64) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shr: %s\n", str);

            if(strncmp(str, "0", 1) != 0) {
                print_error("bigint_shr failed");
            } else {
                print_success("bigint_shr passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");

        if(bigint_shr(bigint_2, bigint_1, 64) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shl failed");
            return -1;
        }

        str = bigint_to_str(bigint_2);

        if (str) {
            printf("bigint_shr: %s\n", str);

            if(strncmp(str, "1234567890ABCDEF", 16) != 0 || strlen(str) != 16) {
                print_error("bigint_shr failed");
            } else {
                print_success("bigint_shr passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_shr failed");
            return -1;
        }

    } else {
        bigint_destroy(bigint_1);
        bigint_destroy(bigint_2);
        print_error("bigint_create failed");
        return -1;
    }

    bigint_destroy(bigint_1);
    bigint_destroy(bigint_2);

    return 0;
}

static int32_t bigint_test_cmp(void) {
    bigint_t* bigint_1 = bigint_create();
    bigint_t* bigint_2 = bigint_create();

    if(bigint_1 && bigint_2) {
        bigint_set_str(bigint_1, "1234");
        bigint_set_str(bigint_2, "5678");

        if(bigint_cmp(bigint_1, bigint_2) != -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234");
        bigint_set_str(bigint_2, "1234");

        if(bigint_cmp(bigint_1, bigint_2) != 0) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "5678");
        bigint_set_str(bigint_2, "1234");

        if(bigint_cmp(bigint_1, bigint_2) != 1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "-5678");
        bigint_set_str(bigint_2, "-1234");

        if(bigint_cmp(bigint_1, bigint_2) != -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "-1234");
        bigint_set_str(bigint_2, "-5678");

        if(bigint_cmp(bigint_1, bigint_2) != 1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "-1234");
        bigint_set_str(bigint_2, "5678");

        if(bigint_cmp(bigint_1, bigint_2) != -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234");
        bigint_set_str(bigint_2, "-5678");

        if(bigint_cmp(bigint_1, bigint_2) != 1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "5678");

        if(bigint_cmp(bigint_1, bigint_2) != 1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        bigint_set_str(bigint_1, "-1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "-5678");

        if(bigint_cmp(bigint_1, bigint_2) != -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            print_error("bigint_cmp failed");
            return -1;
        }

        print_success("bigint_cmp passed");
    } else {
        bigint_destroy(bigint_1);
        bigint_destroy(bigint_2);
        print_error("bigint_create failed");
        return -1;
    }

    bigint_destroy(bigint_1);
    bigint_destroy(bigint_2);

    return 0;
}

static int32_t bigint_test_add_sub(void) {
    bigint_t* bigint_1 = bigint_create();
    bigint_t* bigint_2 = bigint_create();
    bigint_t* bigint_3 = bigint_create();

    if(bigint_1 && bigint_2 && bigint_3) {
        bigint_set_str(bigint_1, "1234");
        bigint_set_str(bigint_2, "5678");

        if(bigint_add(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        const char_t* str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_add: %s\n", str);

            if(strncmp(str, "68AC", 4) != 0) {
                print_error("bigint_add failed");
            } else {
                print_success("bigint_add passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        if(bigint_sub(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_sub: %s\n", str);

            if(strncmp(str, "-4444", 5) != 0) {
                print_error("bigint_sub failed");
            } else {
                print_success("bigint_sub passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "1234567890ABCDEF1234567890ABCDEF");

        if(bigint_add(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_add: %s\n", str);

            if(strncmp(str, "2468ACF121579BDE2468ACF121579BDE", 32) != 0) {
                print_error("bigint_add failed");
            } else {
                print_success("bigint_add passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        if(bigint_sub(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_sub: %s\n", str);

            if(strncmp(str, "0", 1) != 0) {
                print_error("bigint_sub failed");
            } else {
                print_success("bigint_sub passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "1234567891ABCDEF1234567890ABCDEF");

        if(bigint_sub(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_sub: %s\n", str);

            if(strncmp(str, "-10000000000000000000000", 24) != 0) {
                print_error("bigint_sub failed");
            } else {
                print_success("bigint_sub passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "-1234567891ABCDEF1234567890ABCDEF");

        if(bigint_add(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_add: %s\n", str);

            if(strncmp(str, "-10000000000000000000000", 24) != 0) {
                print_error("bigint_add failed");
            } else {
                print_success("bigint_add passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        bigint_set_str(bigint_1, "-1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "-1234567891ABCDEF123456");

        if(bigint_add(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_add: %s\n", str);

            if(strncmp(str, "-1234567890BE02458AC602467FBE0245", 33) != 0) {
                print_error("bigint_add failed");
            } else {
                print_success("bigint_add passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_add failed");
            return -1;
        }

        bigint_set_str(bigint_1, "91A000000000");
        bigint_set_str(bigint_2, "1234567890ABCDEF1234567890ABCDEF");

        if(bigint_sub(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_sub: %s\n", str);

            if(strncmp(str, "-1234567890ABCDEF1233C4D890ABCDEF", 24) != 0) {
                print_error("bigint_sub failed");
            } else {
                print_success("bigint_sub passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_sub failed");
            return -1;
        }

    } else {
        bigint_destroy(bigint_1);
        bigint_destroy(bigint_2);
        bigint_destroy(bigint_3);
        print_error("bigint_create failed");
        return -1;
    }

    bigint_destroy(bigint_1);
    bigint_destroy(bigint_2);
    bigint_destroy(bigint_3);

    return 0;
}

static int32_t bigint_test_mul(void) {
    bigint_t* bigint_1 = bigint_create();
    bigint_t* bigint_2 = bigint_create();
    bigint_t* bigint_3 = bigint_create();

    if(bigint_1 && bigint_2 && bigint_3) {
        bigint_set_str(bigint_1, "1234");
        bigint_set_str(bigint_2, "5678");

        if(bigint_mul(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;
        }

        const char_t* str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_mul: %s\n", str);

            if(strncmp(str, "6260060", 7) != 0) {
                print_error("bigint_mul failed");
            } else {
                print_success("bigint_mul passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;
        }

        bigint_set_str(bigint_1, "1234567890ABCDEF1234567890ABCDEF");
        bigint_set_str(bigint_2, "1234");

        if(bigint_mul(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_mul: %s\n", str);

            if(strncmp(str, "14B60B60AA97760A3D760B60AA97760A28C", 35) != 0) {
                print_error("bigint_mul failed");
            } else {
                print_success("bigint_mul passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;

        }

        bigint_set_str(bigint_2, "FEDCBA9876543210");

        if(bigint_mul(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_mul: %s\n", str);

            if(strncmp(str, "121FA00ACD77D742358D290922D96432236D88FE55618CF0", 48) != 0) {
                print_error("bigint_mul failed");
            } else {
                print_success("bigint_mul passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;

        }

        bigint_set_str(bigint_2, "-FEDCBA9876543210");

        if(bigint_mul(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_mul: %s\n", str);

            if(strncmp(str, "-121FA00ACD77D742358D290922D96432236D88FE55618CF0", 48) != 0) {
                print_error("bigint_mul failed");
            } else {
                print_success("bigint_mul passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_mul failed");
            return -1;

        }

    } else {
        bigint_destroy(bigint_1);
        bigint_destroy(bigint_2);
        bigint_destroy(bigint_3);
        print_error("bigint_create failed");
        return -1;
    }

    bigint_destroy(bigint_1);
    bigint_destroy(bigint_2);
    bigint_destroy(bigint_3);

    return 0;
}

static int32_t bigint_test_pow(void) {
    bigint_t* bigint_1 = bigint_create();
    bigint_t* bigint_2 = bigint_create();
    bigint_t* bigint_3 = bigint_create();

    if(bigint_1 && bigint_2 && bigint_3) {
        bigint_set_str(bigint_1, "3");
        bigint_set_str(bigint_2, "2");

        if(bigint_pow(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_pow failed");
            return -1;
        }

        const char_t* str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_pow: %s\n", str);

            if(strncmp(str, "9", 1) != 0) {
                print_error("bigint_pow failed");
            } else {
                print_success("bigint_pow passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_pow failed");
            return -1;
        }

        bigint_set_str(bigint_1, "12");
        bigint_set_str(bigint_2, "34");

        if(bigint_pow(bigint_3, bigint_1, bigint_2) == -1) {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_pow failed");
            return -1;
        }

        str = bigint_to_str(bigint_3);

        if (str) {
            printf("bigint_pow: %s\n", str);

            if(strncmp(str, "1C9040830AA8880352DFDF4C48E4FBA82690BE45210000000000000", 55) != 0) {
                print_error("bigint_pow failed");
            } else {
                print_success("bigint_pow passed");
            }

            memory_free((void*)str);
        } else {
            bigint_destroy(bigint_1);
            bigint_destroy(bigint_2);
            bigint_destroy(bigint_3);
            print_error("bigint_pow failed");
            return -1;

        }

    } else {
        bigint_destroy(bigint_1);
        bigint_destroy(bigint_2);
        bigint_destroy(bigint_3);
        print_error("bigint_create failed");
        return -1;
    }

    bigint_destroy(bigint_1);
    bigint_destroy(bigint_2);
    bigint_destroy(bigint_3);

    return 0;
}

int32_t main(void) {
    int32_t result = 0;

    result = bigint_test_set_int();

    if(result != 0) {
        return result;
    }

    result = bigint_test_set_str();

    if(result != 0) {
        return result;
    }

    result = bigint_test_shl_shr();

    if(result != 0) {
        return result;
    }

    result = bigint_test_cmp();

    if(result != 0) {
        return result;
    }

    result = bigint_test_add_sub();

    if(result != 0) {
        return result;
    }

    result = bigint_test_mul();

    if(result != 0) {
        return result;
    }

    result = bigint_test_pow();

    if(result != 0) {
        return result;
    }

    if (result == 0) {
        print_success("bigint test passed");
    } else {
        print_error("bigint test failed");
    }

    return result;
}
