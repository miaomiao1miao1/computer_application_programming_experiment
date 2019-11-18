#include "../../utils/utils.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>

#define debug

#ifdef debug
const char *patterns_filename = "../test/pattern_simple.txt";
const char *string_filename = "../test/string_simple.txt";
#else
const char *patterns_filename = "../test/patterns-127w.txt";
const char *words_filename = "../test/words-98w.txt";
#endif
const char *output_filename = "./result.txt";

extern long long compare_number;
extern int       _avl_tree_node_number;

#define max_pattern_number (2256700)
#define max_string_length (256)
#define memory_length (max_pattern_number * max_string_length)

char **patterns;
int *  count;
int *  pos;
char * string;

void rollback(int index) {
    // pos[index] = 0;
    // return;
    int     temp = 2;
    boolean end = F;
    while (!end) {
        for (int i = pos[index] - temp, p = 0;; --i, ++p) {
            if (i <= 0) {
                // match
                pos[index] = compare_char(string[i], patterns[index][p]) == 0
                                 ? p + 1
                                 : 0;
                end = T;
                break;
            }
            if (compare_char(string[i], patterns[index][p]) != 0) {
                ++temp;
                break;
            }
        }
    }
}

void match(char c, int index) {
    if (compare_char(c, patterns[index][pos[index]++]) == 0) {
        if (compare_char('\0', patterns[index][pos[index]]) == 0) {
            ++count[index];
            rollback(index);
        }
    } else {
        rollback(index);
    }
}

int compare(void *args, int i, int j) {
    int *args_count = (int *)(((void **)args)[1]);
    return args_count[j] - args_count[i];
}
void swap(void *args, int i, int j) {
    char **args_patterns = (char **)(((void **)args)[0]);
    int *  args_count = (int *)(((void **)args)[1]);

    char *temp_parrtern = args_patterns[i];
    args_patterns[i] = args_patterns[j];
    args_patterns[j] = temp_parrtern;

    int temp_count = args_count[i];
    args_count[i] = args_count[j];
    args_count[j] = temp_count;
}

int main() {
    clock_start();

    mp_init(memory_length, mp_exit);
    char *temp = mp_new(max_string_length * sizeof(char));

    int pattern_number = 0;
    int l;
    patterns = mp_new(max_pattern_number);

    FILE *output = open_file(output_filename, "w");
    FILE *f = open_file(patterns_filename, "r");

    while ((l = read(f, temp)) != 0) {
        patterns[pattern_number] = mp_new(l + 1);
        strcpy(patterns[pattern_number++], temp);
    }
    fclose(f);

    count = mp_new(pattern_number * sizeof(int));
    pos = mp_new(pattern_number * sizeof(int));
    string = mp_new(max_string_length);
    memset(string, 0, max_string_length);
    for (int i = 0; i < pattern_number; ++i) {
        count[i] = pos[i] = 0;
    }

    compare_init();

    f = open_file(string_filename, "r");
    char c;
    while (1) {
        c = fgetc(f);
        if (c == EOF) {
            break;
        }

        for (int i = max_string_length - 1; i > 0; --i) {
            string[i] = string[i - 1];
        }
        string[0] = c;

        for (int i = 0; i < pattern_number; ++i) {
            match(c, i);
        }
    }
    fclose(f);

    void *args[] = {patterns, count};
    sort(0, pattern_number - 1, args, compare, swap);

    for (int i = 0; i < pattern_number; ++i) {
        fprintf(output, "%s\t%d\n", patterns[i], count[i]);
    }

    printf("%f s\n", clock_duration());
    fclose(output);
    return 0;
}