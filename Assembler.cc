#include<bits/stdc++.h>

using namespace std;

map <string, string> opcode;
vector < pair <int, pair <string, pair <string, string> > > > assembly_code;

void opcode_generator()
{
	opcode.insert(pair <string , string> ("LDA" , "00"));
	opcode.insert(pair <string , string> ("AND" , "40"));
	opcode.insert(pair <string , string> ("DIV" , "24"));
	opcode.insert(pair <string , string> ("SUB" , "1C"));
	opcode.insert(pair <string , string> ("ADD" , "18"));
	opcode.insert(pair <string , string> ("LDL" , "08"));
	opcode.insert(pair <string , string> ("RD" , "D8"));
	opcode.insert(pair <string , string> ("WD" , "DC"));
	opcode.insert(pair <string , string> ("LDCH" , "50"));
	opcode.insert(pair <string , string> ("STX" , "10"));
	opcode.insert(pair <string , string> ("JLT" , "38"));
	opcode.insert(pair <string , string> ("TIX" , "2C"));
	opcode.insert(pair <string , string> ("TD" , "E0"));
	opcode.insert(pair <string , string> ("STCH" , "54"));
	opcode.insert(pair <string , string> ("STL" , "14"));
	opcode.insert(pair <string , string> ("LDX" , "04"));
	opcode.insert(pair <string , string> ("RSUB" , "4C"));
	opcode.insert(pair <string , string> ("STA" , "0C"));
	opcode.insert(pair <string , string> ("J" , "3C"));
	opcode.insert(pair <string , string> ("JEQ" , "30"));
	opcode.insert(pair <string , string> ("COMP" , "26"));
	opcode.insert(pair <string , string> ("COMP" , "14"));
	opcode.insert(pair <string , string> ("JSUB" , "48"));
	opcode.insert(pair <string , string> ("JGT" , "34"));
	opcode.insert(pair <string , string> ("MUL" , "20"));
	opcode.insert(pair <string , string> ("OR" , "44"));
	opcode.insert(pair <string , string> ("STSW" , "E8"));
    return;
}

void labelAndAddress()
{
    char ch;
    int i = 0;
    vector <string> info(3);
    string word = "";
    while(1)
    {
        if(scanf("%c" , &ch) == EOF)
            break;

        /* Store the values in structure (i.e.assembly_code) after each line */
        if(ch == '\n')
        {
            if(word.size() > 0)
            {
                //cout << "Word : " << word << "\n";
                info[i] = word;
                i += 1;
                word = "";
            }
            //cout << i << endl;
            /*for(int j = 0; j < i; j++)
                cout << info[j] << " ";
            cout << endl;*/

            if(i == 3)
            {
                assembly_code.push_back(make_pair(0, make_pair(info[0], make_pair(info[1], info[2]))));
            }
            else if(i == 1)
            {
                assembly_code.push_back(make_pair(0, make_pair("-1", make_pair(info[0], "-1"))));
            }
            else if(i == 2)
            {
                if(opcode.find(info[0]) != opcode.end())
                {
                    assembly_code.push_back(make_pair(0, make_pair("-1", make_pair(info[0], info[1]))));
                }
                else if(info[0] == "BYTE" || info[0] == "WORD" || info[0] == "RESW" || info[0] == "RESB")
                {
                    assembly_code.push_back(make_pair(0, make_pair("-1", make_pair(info[0], info[1]))));
                }
                else if(opcode.find(info[1]) != opcode.end())
                {
                    assembly_code.push_back(make_pair(0, make_pair(info[0], make_pair(info[1], "-1"))));
                }
                else if(info[1] == "BYTE" || info[1] == "WORD" || info[1] == "RESW" || info[1] == "RESB")
                {
                    assembly_code.push_back(make_pair(0, make_pair(info[0], make_pair(info[1], "-1"))));
                }
                else
                {
                    printf ("Cannot detect any opcode or byte or word or reserved (byte or word) instruction\n");
                }
            }
            else
            {
                cout << "Empty Line \n";
            }

            info.clear();
            info.resize(3);
            i = 0;
            //break;

        }
        /* Store each word after each space*/
        else if((ch == ' ' || ch == '\t') && word.size() > 0)
        {
            //cout << "Word : " << word;
            info[i] = word;
            i += 1;
            word = "";
        }
        else if(ch == ' ' || ch == '\t')
            continue;
        else
        {
            word += ch;
        }
    }


    return;
}

int main()
{
    opcode_generator();

    labelAndAddress();

    for(int i = 0 ; i < assembly_code.size(); i++)
    {
        cout << assembly_code[i].first << " ";
        cout << assembly_code[i].second.first << " ";
        cout << assembly_code[i].second.second.first << " ";
        cout << assembly_code[i].second.second.second << "\n";
    }

    return 0;
}
