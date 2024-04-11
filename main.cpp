#include <iostream>
#include <string>
#include <cctype>

using namespace std;


int main()
{
    string line = "";
    getline(cin, line);
    bool block_comment = 0;

    int b[256]{};
    bool quote = 0;
    char prev = '\0';
    cout << line << endl;
    for (int i = 0; i < line.length(); i++)
    {
        char ch = line[i];
        if (ch == '"' && prev != '\\')
        {
            quote = !quote;
            b[i] = 1;
        }
        if (quote)
        {
            b[i] = quote;
        }
        prev = ch;
    }

    bool inline_comment = 0;
    prev = '\0';
    for (int i = 0; i < line.length(); i++)
    {
        char ch = line[i];
        if (ch == '/' && prev == '/' && b[i] == 0)
        {
            inline_comment = 1;
            b[i - 1] = 1;
        }
        if (inline_comment)
        {
            b[i] = 1;
        }
        prev = ch;
    }

    
    prev = '\0';
    for (int i = 0; i < line.length(); i++)
    {
        char ch = line[i];
        if (ch == '*' && prev == '/' && b[i] == 0)
        {
            block_comment = 1;
            b[i - 1] = 1;
        }
        if (ch == '/' && prev == '*' && b[i] == 0)
        {
            block_comment = 0;
            b[i] = 1;
        }
        if (block_comment)
        {
            b[i] = 1;
        }
        prev = ch;
    }

    for (int i = 0; i < line.length(); i++)
    {
        cout << b[i];
    }

    return 0;
}