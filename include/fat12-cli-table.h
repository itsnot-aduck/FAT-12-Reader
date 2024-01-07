#ifndef __FAT12_CLI_TABLE_H__
#define __FAT12_CLI_TABLE_H__

#include <stdint.h>
#include "fat12-err.h"
#define CLI_TABLE_ENTRY 7

typedef FAT12_Status_t (*cli_cmd_func_t)(uint8_t argc, char **argv);

typedef struct{
    cli_cmd_func_t function;
    char *help;
} cli_cmd_info_t;

typedef struct{
    const char *name;
    const cli_cmd_info_t *cmd_info;
} cli_cmd_entry_t;


FAT12_Status_t cli_handler(char *input);
/* Entries */
FAT12_Status_t change_dir(uint8_t argc, char **argv);
FAT12_Status_t cat_file(int8_t argc, char **argv);
FAT12_Status_t make_dir(int8_t argc, char **argv);
FAT12_Status_t remove_item(int8_t argc, char **argv);
FAT12_Status_t unmount(int8_t argc, char **argv);
FAT12_Status_t list_item(int8_t argc, char **argv);
FAT12_Status_t list_help(int8_t argc, char **argv);



#endif
/* __FAT12_CLI_TABLE_H__ */