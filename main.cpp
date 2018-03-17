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

static char *load_all(const char *path, size_t *out_sz)
{
    FILE* fp;
    char *buf;
    unsigned long long sz;

    if ((fp = fopen(path, "rb")) == NULL)
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (sz >= MAX_FILE_BUFFER)
    {
        fprintf(stderr, "File size exceeds max buffer size\n");
        fclose(fp);
        return NULL;
    }

    buf = (char *)malloc(sz);
    if (!buf)
    {
        fprintf(stderr, "Could not alloc the required memory\n");
        fclose(fp);
        return NULL;
    }

    if (fread(buf, 1, sz, fp) != sz)
    {
        fprintf(stderr, "Could not read the file\n");
        free(buf);
        fclose(fp);
        return NULL;
    }

    *out_sz = sz;

    fclose(fp);
    return buf;
}

static bool char_is_any(char ch, const char *any)
{
    while (*any)
    {
        if (ch == *any)
        {
            return true;
        }

        ++any;
    }

    return false;
}

static char *parse_ident(buf_it *it, char* buf)
{
    char *pbuf = buf, ch;

    while (it->start != it->end)
    {
        ch = *it->start;

        if (isalpha(ch) || (pbuf != buf && (isalnum(ch) || char_is_any(ch, "-"))))
        {
            *buf++ = ch;
            ++it->start;
        }
        else
        {
            break;
        }
    }

    if (buf == pbuf)
    {
        return NULL;
    }

    *buf = '\0';
    return pbuf;
}

static void trim_buf(buf_it *it)
{
    while (it->start != it->end && isspace(*it->start))
    {
        ++it->start;
    }
}

// Checks if the buffer contains "<!--" at the current position
static bool is_open_comment(buf_it *it)
{
    if (it->end - it->start >= 4)
    {
        if (strcmp(it->start, "<!--") == 0)
        {
            return true;
        }
    }

    return false;
}

static bool is_close_comment(buf_it *it)
{
    if (it->end - it->start >= 3)
    {
        if (strcmp(it->start, "-->") == 0)
        {
            return true;
        }
    }

    return false;
}

#define HT_OpenTag 1
#define HT_CloseTag 2
#define HT_Data 3

struct HTMLToken
{
    int type;
    char data[128];

    HTMLToken(int type)
        : type(type)
    {
    }
};

static void parse(buf_it *it, std::vector<HTMLToken> &tokens)
{
    char ident_buf[64];
    bool close_tag;
    char data_buf[128];
    char *pdata = data_buf;

    while (it->start != it->end)
    {
        if (*it->start == '<')
        {
            ++it->start;

            trim_buf(it);

            if (*it->start == '/')
            {
                close_tag = true;
                ++it->start;
                trim_buf(it);
            }
            else
            {
                close_tag = false;
            }

            HTMLToken tok(close_tag ? HT_CloseTag : HT_OpenTag);
            parse_ident(it, tok.data);

            tokens.push_back(tok);

            while (it->start != it->end && *it->start != '>')
            {
                ++it->start;
            }

            ++it->start;
        }

        pdata = data_buf;
        trim_buf(it);
        
        while (it->start != it->end)
        {
            /*
            if (is_open_comment(it))
            {
                // Trim the comment

                while (it->start != it->end)
                {
                    if (is_close_comment(it))
                    {
                        it->start += 3;
                        break;
                    }

                    ++it->start;
                }
            }*/

            if (*it->start == '<')
            {
                break;
            }

            if (*it->start == '\n')
            {
                *pdata++ = ' ';
            }
            else
            {
                *pdata++ = *it->start;
            }

            ++it->start;
        }
        *pdata = '\0';

        if (strlen(data_buf) != 0)
        {
            HTMLToken tok(HT_Data);
            strcpy(tok.data, data_buf);
// TODO avoid copying the data buffer
            tokens.push_back(tok);
        }
    }
}

static void parse_and_run(const char *path)
{
    size_t sz;
    char *buf = load_all(path, &sz);
    buf_it it;
    std::vector<HTMLToken> tokens;

    if (!buf)
    {
        return; // TODO
    }

    it.start = buf;
    it.end = buf + sz;

    parse(&it, tokens);

    free(buf);

    // Ok, we have all the tokens so lets create a dom now

    

    for (HTMLToken &tok : tokens)
    {
        printf("Token : %d, data: %s\n", tok.type, tok.data);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    parse_and_run(argv[1]);

    return 0;
}