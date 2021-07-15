/**
 * @file strings.h>
 * @brief main string operations interface
 */
#ifndef ___STRINGS_H
/*! prevent duplicate header error macro */
#define ___STRINGS_H 0

#include <types.h>
#include <memory.h>

/**
 * @brief calculates null ended string's length
 * @param[in]  string string to calculate length
 * @return length
 */
size_t strlen(char_t* string);

/**
 * @brief compares two string
 * @param  string1 first string
 * @param  string2 second string
 * @return <0 if string1>string2, 0 string1=string2, >0 string1<string2
 */
int8_t strcmp(char_t* string1, char_t* string2);

/**
 * @brief compares two string with first n bytes
 * @param  string1 first string
 * @param  string2 second string
 * @param  n first n bytes
 * @return <0 if string1>string2, 0 string1=string2, >0 string1<string2
 */
int8_t strncmp(char_t* string1, char_t* string2, size_t n);

/**
 * @brief checks str starts with prefix
 * @param  str string for look
 * @param  prefix to compare
 * @return 0 if str starts with prefix, else -1
 */
int8_t strstarts(char_t* str, char_t* prefix);

/**
 * @brief checks str ends with suffix
 * @param  str string for look
 * @param  suffix to compare
 * @return 0 if str ends with suffix, else -1
 */
int8_t strends(char_t* str, char_t* suffix);

/**
 * @brief concanates two string and returns new one
 * @param  string1 first string
 * @param  string2 second string
 * @return new string, needs freeing or memory leak
 */
char_t* strcat_at_heap(memory_heap_t* heap, char_t* string1, char_t* string2);

/*! strcat at default heap */
#define strcat(s1, s2) strcat_at_heap(NULL, s1, s2)

/**
 * @brief copies source string to destination string
 * @param  source      source string
 * @param  destination destination string
 * @return 0 on success
 *
 * NULL will not be copied. destination should be equal or greater then source.
 * destination should have space for NULL.
 */
int8_t strcpy(char_t* source, char_t* destination);

/**
 * @brief reverse a string
 * @param[in]  source string to be reversed
 * @return  reversed string
 *
 * allocates new space, hence return value needs be freed, else leak will be happened
 */
char* strrev(char_t* source);

/**
 * @brief converts string into number
 * @param[in]  source string represents number in base
 * @param[in]  base   base of number inside string
 * @return number
 */
number_t ato_base(char_t* source, number_t base);

/*! ato_base macro for base 10 */
#define atoi(number) ato_base(number, 10)

/**
 * @brief converts string into number
 * @param[in]  source string represents number in base
 * @param[in]  base   base of number inside string
 * @return number
 */
unumber_t atou_base(char_t* source, number_t base);


/*! atou_base macro for base 10 */
#define atou(number) atou_base(number, 16)
/*! atou_base macro for base 16 */
#define atoh(number) atou_base(number, 16)

/**
 * @brief convers number to its string representation
 * @param[in]  number number to be converted string
 * @param[in]  base   base value of conversion
 * @return  string represents number
 *
 * return value should be freed or memory leak will be happened
 */
char_t* ito_base(number_t number, number_t base);

/*! ito_base macro for base 10 */
#define itoa(number) ito_base(number, 10)


/**
 * @brief convers unsigned number to its string representation
 * @param[in]  number unsigned number to be converted string
 * @param[in]  base   base value of conversion
 * @return  string represents number
 *
 * return value should be freed or memory leak will be happened
 */
char_t* uto_base(unumber_t number, number_t base);

#define utoa(number) uto_base(number, 10)
#define utoh(number) uto_base(number, 16)

/**
 * @brief duplicate string at heap
 * @param[in] heap destination heap
 * @param[in] src source string
 * @return duplicate at heap
 */
char_t* strdup_at_heap(memory_heap_t* heap, char_t* src);

/*! string duplicate at default heap */
#define strdup(src) strdup_at_heap(NULL, src);
#endif
