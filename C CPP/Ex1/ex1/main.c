#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINE 1024
#define VALID_ARGS_NUM  5
#define VALID_ARG_NUM 2
#define ENCODE 99
#define DECODE 98
#define DIVIDER_BASIS 10
#define STR_TEST "test"
#define STR_ENCODE "encode"
#define STR_DECODE "decode"
#define MINUS '-'



 /**
  * A function that checks if k string is valid k- if it has non digit
  * chars, if its a float.
  * @param k_str a pointer to the k string we get from the cmd.
  * @return 0 if its not valid and 1 if valid.
  */
 int is_k_valid (char* k_str){
  if ((k_str[0] != MINUS) && (!isdigit(k_str[0]))){
    return 0;
  }
  for(size_t i = 1; i < strlen(k_str); i++){
    if (!isdigit(k_str[i])){
      return 0;
    }
    else{
      continue;
    }
  }
    float flo_k = strtof(k_str, NULL);
    long int int_k = strtol(k_str, NULL,DIVIDER_BASIS );
    char zero[] = "0";
    int equ =  strcmp(k_str,zero);
    if (!equ){
        return 1;
    }
    if (!flo_k){
        return 0;
    }
    else if (int_k != 0 && flo_k > int_k){
        return 0;
    }
    else{
        return 1;
    }
}

/**
 * A function that checks if two strings are equal.
 * @param str_1 pointer to string.
 * @param str_2 pointer to string.
 * @return 1 if equal, 0 else.
 */
int checking_equality(char* str_1, char* str_2){
    int equ =  strcmp(str_1, str_2);
    if (!equ){
        return 1;
    }
    else{
        return 0;
    }

}

/**
 * A function that checks if the command is a valid command string.
 * @param command pointer to string.
 * @return 1 if valid 0 else.
 */
int is_command_valid(char* command) {
    if (checking_equality(command, STR_ENCODE)){
        return ENCODE;
    }
    else if(checking_equality(command, STR_DECODE)){
        return DECODE;
    }
    else{
        return 0;
    }
}

/**
 * A function that gets command,k,and an in and out path and encode/decodes
 * the file.
 * @param command encode or decode.
 * @param k the shift.
 * @param in_path path - could be invalid path or empty file.
 * @param out_path path - could be not existed or invalid.
 * @return 0 if writing or reading failed and 1 in success.
 */
int writing(int command,int k, char* in_path, char* out_path) {
    char line[MAX_LINE+1];
    FILE *in_file = fopen(in_path, "r");
    FILE *out_file = fopen(out_path, "w");
    if (in_file == NULL){
        return 0;
    }
    if (out_file==NULL){
      return 0;
    }
    if(command == ENCODE) {
        while (fgets(line, MAX_LINE, in_file)) {
            encode(line, k);
            fputs(line, out_file);
        }
    }
    if(command == DECODE) {
        while (fgets(line, MAX_LINE, in_file)) {
            decode(line, k);
            fputs(line, out_file);
        }
    }
    fclose(in_file);
    fclose(out_file);
    return 1;
}

/**
 * A function that tests the encode/decode functions i wrote in cipher.c.
 * @return 0 if one or more tests has failed,else 1.
 */
int testing(){
    int passed_all_tests = 1;
    if (test_encode_non_cyclic_lower_case_positive_k ()){
        passed_all_tests = 0;
    }
    if (test_encode_cyclic_lower_case_special_char_positive_k ()){
        passed_all_tests = 0;
    }
    if (test_encode_non_cyclic_lower_case_special_char_negative_k ()){
        passed_all_tests = 0;
    }
    if (test_encode_cyclic_lower_case_negative_k ()){
        passed_all_tests = 0;
    }
    if (test_encode_cyclic_upper_case_positive_k ()){
        passed_all_tests = 0;
    }
    if (test_decode_non_cyclic_lower_case_positive_k ()){
        passed_all_tests = 0;
    }
    if (test_decode_cyclic_lower_case_special_char_positive_k ()){
        passed_all_tests = 0;
    }
    if (test_decode_non_cyclic_lower_case_special_char_negative_k ()){
        passed_all_tests = 0;
    }
    if (test_decode_cyclic_lower_case_negative_k ()){
        passed_all_tests = 0;
    }
    if (test_decode_cyclic_upper_case_positive_k ()){
        passed_all_tests = 0;
    }
    if (passed_all_tests){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * A function that runs the program due to the ex requirements.
 * @param argc the number of arguments gotten in cmd.
 * @param argv the arguments gotten from cmd. stored in an arry.
 * @return EXIT_FAILURE if the program failed else EXIT_SUCCESS.
 */

int main (int argc, char *argv[])
{
    if ((argc != VALID_ARGS_NUM) && (argc != VALID_ARG_NUM)){
        //printf("%d",argc);
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
        return EXIT_FAILURE;
    }
    else if(argc == VALID_ARG_NUM) {
        if (!checking_equality(STR_TEST, argv[1])) {
            fprintf(stderr, "Usage: cipher test\n");
            return EXIT_FAILURE;
        }
        else {
            if(testing()){
                return EXIT_SUCCESS;
            }
            else{
                return EXIT_FAILURE;
            }
        }
    }
    else{
        if(!is_command_valid(argv[1])){
            fprintf(stderr, "The given command is invalid.\n");
            return EXIT_FAILURE;
        }
        else{
            if(!is_k_valid(argv[2])) {
                fprintf(stderr, "The given shift value is invalid.\n");
                return EXIT_FAILURE;
            }
            else{
                int command = is_command_valid(argv[1]);
                int k_int = strtol(argv[2], NULL,DIVIDER_BASIS);
                if(!writing(command,k_int,argv[3],argv[4])){
                    fprintf(stderr, "The given file is invalid.\n");
                    return EXIT_FAILURE;
                }
                else{
                    return EXIT_SUCCESS;
                }

            }

        }

    }
}