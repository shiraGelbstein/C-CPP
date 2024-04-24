#include "cipher.h"
#include <ctype.h>
#define NUM_OF_LETTERS 26
#define HEAD_RANGE_OF_SMALL 97
#define TAIL_RANGE_OF_SMALL 122
#define HEAD_RANGE_OF_BIG 65
#define TAIL_RANGE_OF_BIG 90

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.



void encode (char s[], int k) //assuming char has '/0' in the end.
{int i = 0;
    while (s[i] != '\0') {
        if (!isalpha(s[i]))
        {
            i++;
            continue;
        }
        else {
            k = k % NUM_OF_LETTERS;
            int org_char_num = s[i], num_k = org_char_num + k;
            if ((HEAD_RANGE_OF_BIG <= num_k && num_k <= TAIL_RANGE_OF_BIG) ||
            (HEAD_RANGE_OF_SMALL <= num_k && num_k <= TAIL_RANGE_OF_SMALL))
            {
                s[i] = num_k;
            }
            if (org_char_num <= TAIL_RANGE_OF_BIG)
            {
                if (num_k < HEAD_RANGE_OF_BIG)
                {
                    int meow = HEAD_RANGE_OF_BIG - num_k;
                    char new_char = TAIL_RANGE_OF_BIG - (meow-1);
                    s[i] = new_char;
                }
                else if (num_k > TAIL_RANGE_OF_BIG) {
                    int meow = num_k % TAIL_RANGE_OF_BIG;
                    char new_char = HEAD_RANGE_OF_BIG + (meow -1);
                    s[i] = new_char;
                }
                else {
                    char new_char = num_k;
                    s[i] = new_char;
                }
            }
            else {
                if (num_k < HEAD_RANGE_OF_SMALL) {
                    int meow = HEAD_RANGE_OF_SMALL - num_k;
                    char new_char = TAIL_RANGE_OF_SMALL - (meow-1);;
                    s[i] = new_char;
                }
                else if (num_k > TAIL_RANGE_OF_SMALL) {
                    int meow = num_k % TAIL_RANGE_OF_SMALL;
                    char new_char = HEAD_RANGE_OF_SMALL + (meow-1);;
                    s[i] = new_char;
                }
            }
        }
        i++;
}
}


void decode (char s[], int k)
{encode(s, -k);}

