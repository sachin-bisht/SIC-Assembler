#include<bits/stdc++.h>
#include<string>

using namespace std;

map <string, string> opcode;
vector < pair <int, pair <string, pair <string, string> > > > assembly_code;
map <string, int> label;
vector <string> objectCode;

string decTohex(int decimal_value)
{
    stringstream ss;
    ss << hex << decimal_value;
    return ss.str();
}

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
	opcode.insert(pair <string , string> ("COMP" , "28"));
	opcode.insert(pair <string , string> ("JSUB" , "48"));
	opcode.insert(pair <string , string> ("JGT" , "34"));
	opcode.insert(pair <string , string> ("MUL" , "20"));
	opcode.insert(pair <string , string> ("OR" , "44"));
	opcode.insert(pair <string , string> ("STSW" , "E8"));
    return;
}

void storeCode()
{
    char ch;
    int i = 0;
    vector <string> info(3);
    string word = "";
    while(1)
    {
        if(scanf("%c" , &ch) == EOF)
        {
            break;
        }

        // If comment are present
        if(ch == '.')
        {
            bool endofFile = false;
            do{
                if(scanf("%c", &ch) == EOF)
                {
                    endofFile = true;
                    break;
                }
            }while(ch != '\n');
            if(endofFile)
                break;
        }

        /* Store the values in structure (i.e.assembly_code) after each line */
        else if(ch == '\n')
        {
            if(word.size() > 0)
            {
                info[i] = word;
                i += 1;
                word = "";
            }

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
                    assembly_code.push_back(make_pair(0, make_pair("-1", make_pair(info[0], info[1]))));
                }
            }
            else
            {
                cout << "Empty Line \n";
            }

            info.clear();
            info.resize(3);
            i = 0;
        }
        /* Store each word after each space*/
        else if((ch == ' ' || ch == '\t') && word.size() > 0)
        {
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

void labelAndAddress()
{
    int PC = stoi(assembly_code[0].second.second.second, nullptr, 16);

    for(int i = 1; i < assembly_code.size() - 1; i++)
    {
        assembly_code[i].first = PC;
        //cout << hex << assembly_code[i].first << "\n";
        string lab = assembly_code[i].second.first;
        if(lab != "-1")
        {
            label[lab] = PC;
            //cout << lab << " : " << hex << PC << "\n";
        }

        string opcode_or_not = assembly_code[i].second.second.first;
        if(opcode.find(opcode_or_not) != opcode.end())
        {
            PC += 3;
        }
        else if(opcode_or_not == "RESW" || opcode_or_not == "RESB")
        {
            string val = assembly_code[i].second.second.second;
            int hexVal = stoi(val);
            //cout << "HEXVAl : " << hex << hexVal << "\n";
            if(opcode_or_not == "RESW")
                hexVal *= 3;
            PC += hexVal;
        }
        else if(opcode_or_not == "WORD")
        {
            PC += 3;
        }
        else if(opcode_or_not == "BYTE")
        {
            string val = assembly_code[i].second.second.second;
            int add = val.size() - 3;
            if(val[0] == 'C')
            {
                PC += add;
            }
            else if(val[0] == 'X')
            {
                add = ceil(add / 2.0);
                PC += add;
            }
        }
        assembly_code[assembly_code.size() - 1].first = PC;
        //cout << hex << assembly_code[assembly_code.size() - 1].first << "\n";
    }

    return;
}

void opCode()
{
    for(int i = 1; i < assembly_code.size() - 1; i++)
    {
        string store = "";
        string opcode_or_not = assembly_code[i].second.second.first;
        string label_or_not = assembly_code[i].second.second.second;
        if(opcode.find(opcode_or_not) != opcode.end())
        {
            store += opcode[opcode_or_not];
            if(label.find(label_or_not) != label.end())
            {
                store += decTohex(label[label_or_not]);
            }
            else if(label_or_not == "-1")
            {
                store += "0000";
            }
            else
            {
                string temp = "";
                int j = 0;
                while(label_or_not[j] != ',')
                {
                    temp += label_or_not[j];
                    j += 1;
                }
                int val = label[temp];
                val += pow(2, 15);
                temp = decTohex(val);
                if(temp.size() != 4)
                {
                    if(store[1] == 'F')
                    {
                        store[1] = '0';
                        if(store[0] == 'F')
                            store[0] = '0';
                        else
                            store[0] = store[0] + 1;
                    }
                    else
                    {
                        store[1] = store[1] + 1;
                    }
                    temp.erase(temp.begin() + 0);
                }
                store += temp;
            }
        }
        else
        {
            if(opcode_or_not == "RESW" || opcode_or_not == "RESB")
            {
                store = "-1";
            }
            else if(opcode_or_not == "BYTE")
            {
                if(label_or_not[0] == 'C')
                {
                    for(int j = 2; j < label_or_not.size() - 1; j++)
                    {
                        int val = label_or_not[j];
                        store += decTohex(val);
                    }
                }
                if(label_or_not[0] == 'X')
                {
                    for(int j = 2; j < label_or_not.size() - 1; j++)
                    {
                        store += label_or_not[j];
                    }
                }
            }
            else if(opcode_or_not == "WORD")
            {
                int val = stoi(label_or_not);
                //cout << "WORD : " << val << "\n";
                string temp = decTohex(val);
                for(int i = temp.size(); i < 6; i++)
                {
                    store += "0";
                }
                store += temp;
            }
        }
        objectCode[i] = store;
    }
    return;
}

void headerFile()
{
    ofstream hFile;
    hFile.open("HeaderRecord", ios :: out | ios :: trunc);
    hFile << "H";
    string filename = assembly_code[0].second.first;
    hFile << filename;
    for(int i = filename.size(); i < 6; i++)
    {
        hFile << " ";
    }
    string stad = assembly_code[0].second.second.second;
    for(int i = stad.size(); i < 6; i++)
    {
        hFile << "0";
    }
    hFile << stad;
    int diff = assembly_code[assembly_code.size() - 1].first - assembly_code[1].first;
    string totLen = decTohex(diff);
    for(int i = totLen.size(); i < 6; i++)
    {
        hFile << "0";
    }
    hFile << totLen;

    void close();

    return;
}

void textFile()
{
    ofstream tFile;
    tFile.open("TextRecordFile", ios :: out | ios :: trunc);
    int i = 1;
    while(i < objectCode.size())
    {
        tFile << "T";
        string stad = decTohex(assembly_code[i].first);
        for(int j = stad.size(); j < 6; j++)
        {
            tFile << "0";
        }
        tFile << stad;
        int ln = 0;
        int tempI = i;
        string codes = "";
        while(tempI < objectCode.size() && objectCode[tempI] != "-1")
        {
            int val = objectCode[tempI].size() / 2;
            if(ln + val <= 30)
            {
                ln += val;
            }
            else
                break;
            codes += objectCode[tempI];
            tempI += 1;
        }
        if(objectCode[tempI] == "-1")
        {
            while(objectCode[tempI] == "-1")
                tempI += 1;
        }
        string temp = decTohex(ln);
        if(temp.size() == 1)
            tFile << "0";
        tFile << temp;
        tFile << codes << "\n";
        i = tempI;
    }

    void close();
    return;
}

void endFile()
{
    ofstream eFile;
    eFile.open("EndRecordFile", ios :: out | ios :: trunc);
    eFile << "E";
    string lab = assembly_code[assembly_code.size() - 1].second.second.second;
    int exad = label[lab];
    lab = decTohex(exad);
    for(int i = lab.size(); i < 6; i++)
        eFile << "0";
    eFile << lab;

    void close();
    return;
}

int main()
{
    assembly_code.clear();
    objectCode.clear();
    opcode_generator();

    storeCode();

    //cout << "size : " << assembly_code.size() << "\n";
    /*for(int i = 0 ; i < assembly_code.size(); i++)
    {
        cout << assembly_code[i].first << " ";
        cout << assembly_code[i].second.first << " ";
        cout << assembly_code[i].second.second.first << " ";
        cout << assembly_code[i].second.second.second << "\n";
    }*/

    labelAndAddress();
    objectCode.resize(assembly_code.size() - 1, "-1");
    opCode();
    /*for(int i = 1; i < objectCode.size(); i++)
        cout << objectCode[i] << "\n";*/

    headerFile();
    textFile();
    endFile();

    return 0;
}
