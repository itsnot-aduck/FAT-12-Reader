#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat12-config.h"
#include "fat12-parser.h"
#include "fat12-path.h"
#include <string.h>

const char *root = "G5_FPT:~";

typedef struct path_list{
    char name[12];
    struct path_list* pNext;
} PathNode;

PathNode* path_list = NULL;
uint8_t depth = 0;

void PATH_move_to_child(char* node){
    if (strstr(node, ".."))
    {   
        if (depth > 0)
            PATH_back_to_father();
    }
    else{
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

}

void PATH_back_to_father(){
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

void PATH_back_to_root(){
    PathNode *pCursor = path_list, *pTemp;
    while(pCursor -> pNext != NULL){
        pTemp = pCursor;
        pCursor = pCursor -> pNext;
        free(pTemp);
    }
    path_list = NULL;
    depth = 0;
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
