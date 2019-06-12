#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>
#include "colorformatmanager.h"

using namespace std;

ColorFormatManager::ColorFormatManager()
{
}

void ColorFormatManager::LoadFormats(string formatFile)
{
    cout << "Color Format Manager :: LoadFormats." << endl;
    ifstream input(formatFile);
    for( std::string line; getline( input, line ); )
    {
        if(line.size() == 0)
        {
            cout << "Size 0" << endl;
            continue;
        }
        else if(line.at(0) == '#')
        {
            cout << "Comment: " << line << endl;
            continue;
        }
        else if(line.at(0) != '[')
        {
            cout << "Format doesn't start with [" << line << endl;
            continue;
        }
        else
        {
            cout << line << endl;
            string key = FindFormatKey(line);
            string format = GetFormatString(line);
            formats.insert(make_pair(key, format));
        }
    }
}

string ColorFormatManager::FindFormatKey(string line)
{
    size_t startOfKey = line.find_first_of("[");
    size_t endOfKey = line.find_first_of("]");

    if(startOfKey != string::npos && endOfKey != string::npos)
    {
        return line.substr(startOfKey + 1, endOfKey - 1);
    }

    return "";
}

string ColorFormatManager::GetFormatString(string line)
{
    size_t endOfKey = line.find_first_of("]");

    if(endOfKey != string::npos)
    {
        return line.substr(endOfKey + 1);
    }

    return "";
}

vector<string> ColorFormatManager::GetFormatsKeys()
{
    vector<string> v;
    for(map<string,string>::iterator it = formats.begin(); it != formats.end(); ++it) {
       v.push_back(it->first);
    }

    return v;
}

vector<string> ColorFormatManager::GetVariableKeys()
{
    vector<string> v;
    for(map<string,string>::iterator it = variables.begin(); it != variables.end(); ++it) {
        v.push_back(it->first);
    }

    return v;
}

void ColorFormatManager::SetVariables(map<string, string> vars)
{
    variables = vars;
}

string ColorFormatManager::GetFormat(string key)
{
    string format = formats[key];
    vector<string> variableKeys = GetVariableKeys();
    for(string &value: variableKeys)
    {
        string variable = variables[value];
        format = ReplaceVariable(format, value, variable);
    }

    return format;
}

string ColorFormatManager::ReplaceVariable(string original, string var, string replacement)
{
    auto pos = original.find(var);

    string newString = "";

    if(pos == string::npos)
    {
        return original;
    }
    else
    {
        while (pos != string::npos)
        {
            newString = original.replace(original.find(var), var.length(), replacement);
            pos = original.find(var);

            return newString;
        }
        return newString;
    }
}