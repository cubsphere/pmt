#include <iostream>
#include <fstream>
#include "boyer-moore.h"
#include "shift-or.h"

const string ab = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

bool verify_algorithm(string algorithm_name, int edit_distance)
{
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for boyer-moore\n";
            return false;
        }
        return true;
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        if (edit_distance != 0)
        {
            cout << "edit distance must be 0 for shift-or\n";
            return false;
        }
        return true;
    }

    cout << "unrecognized algorithm " << algorithm_name << "\n";
    return false;
}

void use_boyer_moore(ifstream &text_file, string pat, bool count_mode)
{
    auto C = bad_char(pat, ab);
    int *S = good_suffix(pat);
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, pat, C, S);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = boyer_moore(txt, pat, C, S);
            if (!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }
    delete[] S;
}

void use_shift_or_64(ifstream &text_file, string pat, bool count_mode)
{
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    auto C = char_mask(pat, ab);
    auto ones = all_ones(pat.length());
    
    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, pat, C, ones);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = shift_or_64(txt, pat, C, ones);
            if (!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }

    free(ones->bits);
    free(ones);
    for (int i = 0; i < ab.length(); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
}

void use_shift_or(ifstream &text_file, string pat, bool count_mode)
{
    string txt;
    vector<int> occ;
    getline(text_file, txt);
    auto C = char_mask(pat, ab);
    auto ones = all_ones(pat.length());

    if (count_mode)
    {
        int occnum = 0;
        while (!text_file.eof())
        {
            occ = shift_or(txt, pat, C, ones);
            occnum += occ.size();
            getline(text_file, txt);
        }
        cout << occnum << '\n';
    }
    else
    {
        while (!text_file.eof())
        {
            occ = shift_or(txt, pat, C, ones);
            if (!occ.empty())
                cout << txt << '\n';
            getline(text_file, txt);
        }
    }

    free(ones->bits);
    free(ones);
    for (int i = 0; i < ab.length(); ++i)
    {
        free(C[ab[i]]->bits);
        free(C[ab[i]]);
    }
}

void process_text(ifstream &text_file, string pat, string algorithm_name, bool count_mode, int edit_distance)
{
    if (algorithm_name.compare("boyer-moore") == 0)
    {
        use_boyer_moore(text_file, pat, count_mode);
    }
    else if (algorithm_name.compare("shift-or") == 0)
    {
        if (pat.length() <= 64)
        {
            use_shift_or_64(text_file, pat, count_mode);
        }
        else
        {
            use_shift_or(text_file, pat, count_mode);
        }
    }
}