#include "parser.h"

char *load_all(const char *path, size_t *out_sz)
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

bool char_is_any(char ch, const char *any)
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

bool is_open_comment(buf_it *it)
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

bool is_close_comment(buf_it *it)
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

char *parse_ident(buf_it *it, char *buf)
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

void trim_buf(buf_it *it)
{
    while (it->start != it->end && isspace(*it->start))
    {
        ++it->start;
    }
}

char *parse_string(buf_it *it, char *buf)
{
    char *pbuf = buf;

    while (it->start != it->end)
    {
        if (*it->start == '"')
        {
            ++it->start;
            break;
        }

        *buf++ = *it->start;
        ++it->start;
    }

    *buf = '\0';
    return pbuf;
}

void parse(buf_it *it, std::vector<HTMLToken> &tokens)
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
            tok.autoclose = 0;

            tokens.push_back(tok);

            while (it->start != it->end)
            {
                trim_buf(it);
                ident_buf[0] = '\0';
                data_buf[0] = '\0';

                if (*it->start == '/')
                {
                    ++it->start;

                    tokens.push_back(HTMLToken(HT_CloseTag));
                }

                if (*it->start == '>')
                {
                    ++it->start;
                    break;
                }

                parse_ident(it, ident_buf);

                if (*it->start == '=')
                {
                    ++it->start;
                    trim_buf(it);

                    if (*it->start == '"')
                    {
                        ++it->start;

                        parse_string(it, data_buf);
                    }
                    else
                    {
                        // TODO use a better function
                        parse_ident(it, data_buf);
                    }
                }

                HTMLToken attrTok(HT_Attribute);
                strcpy(attrTok.data, ident_buf);
                strcpy(attrTok.data2, data_buf);

                tokens.push_back(attrTok);                
            }
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