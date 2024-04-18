#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

using namespace std;


const int n = 1000, lines_at_once = 10;
bool block_comment = 0;


string changed_line(string line);


int main(int argc, char *argv[])
{
    string input_filename = "";
    string output_filename = "";
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-I"))
        {
            i++;
            input_filename = argv[i];
        }
        if (!strcmp(argv[i], "-O"))
        {
            i++;
            output_filename = argv[i];
        }
    }
    if (input_filename == "")
    {
        cout << "Enter input filename: ";
        getline(cin, input_filename);
    }
    if (output_filename == "")
    {
        cout << "Enter output filename: ";
        getline(cin, output_filename);
    }

    ifstream output_file_exists(output_filename);
    string temp;
    if (output_file_exists)
    {
        cout << "Output file already exists. Rewrite? [y/n] ";
        getline(cin, temp);
        if (temp != "y")
        {
            return 0;
        }
    }
    output_file_exists.close();

    ifstream input_file(input_filename);
    ofstream output_file(output_filename);
    string line, line2;
    int current_line_count = 0;
    bool print_info = 1;

    if (input_file)
    {
        getline(input_file, line);
    }

    while (input_file)
    {
        line2 = changed_line(line);
        if (print_info)
        {
            cout << "[in]:  " << line << endl;
            cout << "[out]: " << line2 << endl;
            cout << endl;
        }
        current_line_count += 2;
        if (current_line_count >= lines_at_once)
        {
            current_line_count = 0;
            if (print_info)
            {
                getline(cin, temp);
            }
            if (temp == "exit")
            {
                print_info = 0;
            }
        }
        output_file << line2 << endl;
        getline(input_file, line);
    }
    input_file.close();
    output_file.close();

    return 0;
}


string changed_line(string line)
{

    int b[n]{};
    bool quote = 0;
    char prev = '\0';
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

    int insert_indexes[n], current_index = 0;
    prev = '\0';
    for (int i = line.find("cout"); i < line.length(); i++)
    {
        char ch = line[i];
        if (ch == '<' && prev == '<')
        {
            insert_indexes[current_index + 1] = i - 1;
            insert_indexes[current_index] = i;
            current_index += 2;
            b[i - 1] = 1;
            b[i] = 1;
        }
        else if (ch == ';')
        {
            insert_indexes[current_index] = i;
            current_index++;
            b[i] = 1;
        }
        
        prev = ch;
    }

    int insert_indexes_len = current_index;
    for (int i = insert_indexes_len - 1; i > 0; i--)
    {
        int i1 = -1, i2 = -1;
        for (int j = insert_indexes[i] - 1; j > insert_indexes[i - 1]; j--)
        {
            if (!isspace(line[j]))
            {
                if (!b[j])
                {
                    if (i2 == -1)
                    {
                        i2 = j + 1;
                    }
                    i1 = j;
                }
            }    
        }
        if (i1 != -1 && i2 != -1)
        {
            line.insert(i2, "\"");
            line.insert(i1, "\"");
        }
    }

    return line;
}