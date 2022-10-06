//
// Created by quentin on 06/10/22.
//
#include "../include/utils.h"

float truncate(float value, int val1, int val2)
{
    if(value < val1)
        return val1;
    if(value > val2)
        return val2;
    return value;
}

char* stradd(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
}