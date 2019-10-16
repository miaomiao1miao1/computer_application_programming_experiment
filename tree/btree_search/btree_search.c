#include "../../utils/utils.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>

// #define debug

#ifdef debug
const char *patterns_filename = "../test/patterns.txt";
const char *words_filename = "../test/words.txt";
#else
const char *patterns_filename = "../test/patterns-127w.txt";
const char *words_filename = "../test/words-98w.txt";
#endif
const char *output_filename = "./result.txt";

extern long long compare_number;

const int    max_pattern_number = 1280000;
const int    max_string_length = 256;
const size_t memory_length = 1 << 30;

int compare(void *a, void *b) { return compare_string(a, b); }

tree_node *str_init(char *s) {
    char *str = mp_new(strlen(s)+1);
    strcpy(str, s);
    return tn_init(str);
}

int main() {
    clock_start();

    mp_init(memory_length, mp_exit);
    char *temp = mp_new(max_string_length * sizeof(char));
    int   word_number = 0;
    int   word_exist_number = 0;

    avl_tree * at = at_init(compare);

    FILE *output = open_file(output_filename, "w");
    FILE *f = open_file(patterns_filename, "r");

    while (read(f, temp)) {
        at_add(at, str_init(temp));
    }
    fclose(f);

    // at_print(at);

    f = open_file(words_filename, "r");
    while (read(f, temp)) {
        ++word_number;

        if (at_search(at, temp)) {
            ++word_exist_number;
            fprintf(output, "%s yes\n", temp);
        } else {
            fprintf(output, "%s no\n", temp);
        }
    }
    fclose(f);

    fprintf(output, "%10.2f %10lld %7d %7d\n", (double)mp_get_length() / 1024,
            compare_number, word_number, word_exist_number);
    printf("%f s\n", clock_duration());
    fclose(output);
    return 0;
}