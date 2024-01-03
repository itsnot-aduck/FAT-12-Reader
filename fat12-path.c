#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat12-config.h"
#include <string.h>

const char *root = "G5_FPT:~";

typedef struct path_list{
    char name[12];
    struct path_list* pNext;
} PathNode;

PathNode* path_list = NULL;
uint8_t depth = 0;

void PATH_move_to_child(char* node){
    PathNode *tmp = (PathNode*) malloc(sizeof(PathNode));
    tmp -> pNext = NULL;
    if (tmp != NULL){
        if(depth == 0){
           path_list = tmp;
           path_list -> pNext = NULL;
        }
        else{
            PathNode *pCursor = path_list;
            while (pCursor -> pNext != NULL){
                pCursor = pCursor->pNext;
            }
            pCursor -> pNext = tmp;
        }
    strcpy(tmp->name, node);
    depth += 1; 
    }
}

void PATH_move_to_father(){
    if( depth >= 2){
        PathNode *pCursor = path_list;
        while (pCursor -> pNext -> pNext != NULL){
            pCursor = pCursor->pNext;
        }
        free(pCursor -> pNext);
        pCursor -> pNext = NULL;
    }
    else if (depth == 1){
        free(path_list);
        path_list->pNext = NULL;
    }
    depth--;
}

void Path_Get_Current(){
    printf("%s", root);
    PathNode *pCursor = path_list;
    for (int i = 0; i < depth; i++){
        printf("/%s", pCursor->name);
        pCursor = pCursor -> pNext;
    }
    printf("$");
}

int main(){
    PATH_move_to_child("Hi0");
    PATH_move_to_child("Hi");
    PATH_move_to_child("Hi2");
    PATH_move_to_father();
    PATH_move_to_father();
    Path_Get_Current();
    return 0;
}