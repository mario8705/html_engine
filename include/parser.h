#pragma once
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>

// 1 Mb
#define MAX_FILE_BUFFER (1 * 1024 * 1024)

struct buf_it
{
    char *start;
    char *end;
};

char *load_all(const char *path, size_t *out_sz);
bool char_is_any(char ch, const char *any);
char *parse_ident(buf_it *it, char* buf);
void trim_buf(buf_it *it);

// Checks if the buffer contains "<!--" at the current position
bool is_open_comment(buf_it *it);

bool is_close_comment(buf_it *it);

#define HT_OpenTag 1
#define HT_CloseTag 2
#define HT_Data 3
#define HT_Attribute 4

struct HTMLToken
{
    int type;
    int autoclose;
    char data[128];
    char data2[128];

    HTMLToken(int type)
        : type(type)
    {
        data[0] = '\0';
        data2[0] = '\0';
    }
};

char *parse_string(buf_it *it, char *buf);
void parse(buf_it *it, std::vector<HTMLToken> &tokens);
