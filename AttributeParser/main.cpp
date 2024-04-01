#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <bits/stdc++.h>
using namespace std;

#define DEBUG 0

/******************Helper Functions******************/
template<typename T>
void print(const T& t)
{
    if(DEBUG == 1)
    {
        std::cout << t << std::endl;
    }
}

string replaceAll(string str, const string &from, const string &to)
{
    if(str.empty() || str.length() == 0)
        return "";
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::vector<std::string> mysplit(const std::string& input, char keyword)
{
    std::vector<std::string> ret = {};
    if(input.empty() || input.length() == 0)
        return ret;
    std::string segment;
    std::stringstream ss(input);
    while(std::getline(ss, segment, keyword))
    {
        ret.push_back(segment);
    }
    return ret;
}

struct Tag
{
    std::string tagName = "";
    bool isClose = false;
    std::map<std::string,std::string> attributeMap = {};
    std::vector<std::string> childrenNames = {};

    std::string toString() const {
        std::string result = "<" ;
        if(isClose) {
            result += "/";
        }
        
        result += tagName;
        
        // Add attributes
        for(const auto& pair : attributeMap) {
            result += " " + pair.first + "=" + pair.second + "";
        }

        result += ">";
        
        return result;
    }
};

std::map<std::string,std::vector<Tag>> tagsMap;
std::vector<Tag> tagList;

std::vector<Tag> getAllTags(const std::string& tagName)
{
    if(DEBUG == 1)
        std::cout << "\n getAllTags: " << tagList.size();
    if(tagList.empty() || tagName.empty() || tagName.length() == 0)
        return {};
    std::vector<Tag> ret;
    auto iter = tagList.begin();    
    while(iter != tagList.end())
    {
        if(iter->tagName != tagName)
        {
            iter++;
            continue;
        }
        auto iter2  = std::find_if(iter,tagList.end(),
                    [&](const Tag& tag_)
                    {
                        return (tag_.tagName == tagName && tag_.isClose);
                    });
        if(iter2 == tagList.end())
        {
            iter++;
            continue;
        }
        else
        {
            // First element
            ret.push_back(*iter);
            for(auto i = iter; i != iter2+1; i++ )
            {
                if(i != tagList.end() && !(i->isClose))
                {
                    auto retIter = std::find_if(ret.begin(),ret.end(),
                                    [&](const Tag& t_)
                                    {
                                        return i->tagName == t_.tagName && !t_.isClose;
                                    });
                    if(retIter == ret.end() && tagName != i->tagName)
                    {
                        ret.push_back(*i);
                    }
                }
                else if(i == tagList.end())
                {
                    break;
                }
            }
            break;
        }
    }
    return ret;
}

bool getTags(const std::vector<std::string>& tagNames, std::vector<Tag>& ret)
{
    bool inValidTagName = false;
    for(const auto& tagName : tagNames)
    {
        auto iter = std::find_if(tagList.begin(), tagList.end(),
        [&](const Tag& t_)
        {
            return !t_.isClose && t_.tagName == tagName;
        });
        if(iter != tagList.end())
        {
            ret.push_back(*iter);
        }
        else{
            inValidTagName = true;
        }
    }
    return inValidTagName;
}

std::string getValue(const std::vector<Tag>& tags, const std::string& value)
{
    if(tags.empty())
    {
        return "Not Found!";
    }
    auto iter = tagsMap.find(tags[0].tagName);
    if(iter == tagsMap.end())
    {
        return "Not Found!";
    }
    std::vector<Tag> givenTags = iter->second;
    if(DEBUG == 1)
    {
        std::cout << "\n-----------------------------------";
        std::cout << "\ntags :";
        for(const auto& a : tags)
        {
            std::cout << a.tagName << ",";
        }
        std::cout << "\n-----------------------------------";
        std::cout << "\ngivenTags :";
        for(const auto& a : givenTags)
        {
            std::cout << a.tagName << ",";
        }
        std::cout << "\n-----------------------------------\n";
    }

    for(int i = 1; i < tags.size() ; i++)
    {
        Tag prevTag = tags[i-1];
        Tag t = tags[i];
        auto childIter = std::find_if(prevTag.childrenNames.begin(),
                                    prevTag.childrenNames.end(),
                                    [&](const std::string& s)
                                    {
                                        return s == t.tagName;
                                    });
        if(childIter == prevTag.childrenNames.end())
        {
            return "Not Found!";
        }
    }

    Tag lastTag = tags[tags.size()-1];
    auto attrMap = lastTag.attributeMap;
    auto iter3 = attrMap.find(value);
    if(iter3 != attrMap.end())
    {
        return iter3->second;
    }
    return "Not Found!";
}

void fillChildren(Tag& tag)
{
    auto tagIter = std::find_if(tagList.begin(),
        tagList.end(),
        [&](const Tag& t_)
        {
            return !t_.isClose && t_.tagName == tag.tagName;
        });
    if(tagIter == tagList.end())
    {
        return;
    }
    auto closeTagIter = std::find_if(tagIter,
        tagList.end(),
        [&](const Tag& t_)
        {
            return t_.isClose && t_.tagName == tag.tagName;
        });
    if(closeTagIter == tagList.end())
    {
        return;
    }
    std::vector<std::string> ret;
    // start from next of tagIter to avoid duplicate same tag name
    for(auto i = tagIter+1; i != closeTagIter ; i++)
    {
        if(!i->isClose)
        {
            ret.push_back(i->tagName);
        }
    }
    tag.childrenNames = ret;
}

Tag getTag(const std::string& name_)
{
    auto tagIter = std::find_if(tagList.begin(),tagList.end(),
    [&](const Tag& t)
    {
        return (!t.isClose && t.tagName == name_);
    });
    if(tagIter != tagList.end())
    {
        return *tagIter;
    }
    return Tag();
}

bool isAlreadyChildOfAnotherTag(const std::string& currentTagName, const std::string& tName)
{
    for(const auto &tag : tagList)
    {
        if(tag.isClose || tag.tagName == currentTagName)
        {
            continue;
        }
        auto iter = std::find(tag.childrenNames.begin(),tag.childrenNames.end(),tName);
        if(iter != tag.childrenNames.end())
        {
            return true;
        }
    }
    return false;
}

void fillChildrenTags()
{
    // modifies TagList
    for(auto &tag : tagList)
    {
        if(tag.isClose)
        {
            continue;
        }
        fillChildren(tag);
    }

    if(DEBUG == 1)
    {
        std::cout << "\n------------Children before--------------------------\n";
        for(const auto& a : tagList)
        {
            std::stringstream ss;
            ss << "\n" << a.tagName << ": [";
            for(auto s : a.childrenNames)
            {
                ss << s << ",";
            }
            ss << "]";
            std::cout << ss.str();
        }
        std::cout << "\n--------------------------------------\n";
    }

    // modifies TagList
    for(auto &tag : tagList)
    {
        if(tag.isClose)
        {
            continue;
        }
        std::vector<std::string> ret;
        for(auto iter = tag.childrenNames.begin(); iter != tag.childrenNames.end(); iter++)
        {
            if(!isAlreadyChildOfAnotherTag(tag.tagName,*iter))
            {
                ret.push_back(*iter);
            }
        }
        tag.childrenNames = ret;
    }
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int numberOfInputTagLines = 0;
    int numberOfQueryLines = 0;

    cin >> numberOfInputTagLines;
    cin >> numberOfQueryLines;

    if(DEBUG == 1)
    {
        std::cout << numberOfInputTagLines << ", " << numberOfQueryLines;
    }

    std::vector<std::string> inputLines, inputQueryLines;

    for(int i=0; i<=numberOfInputTagLines; i++)
    {
        std::string input;
        std::getline(std::cin,input);
        inputLines.push_back(input);
    }

    for(int i=0; i<=numberOfQueryLines; i++)
    {
        std::string input;
        std::getline(std::cin,input);
        inputQueryLines.push_back(input);
    }

    if(DEBUG == 1)
    {
        std::cout << "\n--------inputLines--------------------\n";
        for(auto a : inputLines)
        {
            std::cout << a << std::endl;
        }
        std::cout << "\n--------inputQueryLines--------------------\n";
        for(auto a : inputQueryLines)
        {
            std::cout << a << std::endl;
        }
        std::cout << "\n----------------------------";
    }

    // parsing inputlines
    for(const auto& line : inputLines)
    {
        print(line);
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        ss >> token;
        if(token.length() >= 2)
        {
            Tag tag;
            bool isClosingTag = token[1] == '/';
            if (isClosingTag)
            {
                tag.isClose = true;
                token.replace(token.begin(),token.begin()+2,"");
                token = replaceAll(token, ">", "");
                tag.tagName = token;
            }
            else
            {
                tag.isClose = false;
                token.replace(token.begin(),token.begin()+1,"");
                token = replaceAll(token, ">", "");
                tag.tagName = token;
                auto pos = line.find(' ');
                if(pos != std::string::npos)
                {
                    std::string allAttributes = line.substr(pos);
                    allAttributes = replaceAll(allAttributes, " = ", "=");
                    allAttributes = replaceAll(allAttributes, ">", "");
                    std::vector<std::string> ret = mysplit(allAttributes,' ');
                    std::map<std::string,std::string> atMap;
                    for(const std::string& a : ret)
                    {
                        auto c = mysplit(a, '=');
                        if(c.size() > 1)
                        {
                            std::string dd = replaceAll(c[1],"\"","");
                            atMap.emplace(c[0],dd);
                        }
                        
                    }
                    tag.attributeMap = atMap;
                }
            }
            tagList.push_back(tag);
            if(DEBUG == 1)
            {
                std::cout << "\ngggggg : " << tag.tagName;
            }
        }
    }
    if(DEBUG == 1)
        std::cout << "\nhhhhhhhh";
    
    fillChildrenTags();

    if(DEBUG == 1)
    {
        std::cout << "\n------------Children after--------------------------\n";
        for(const auto& a : tagList)
        {
            std::stringstream ss;
            ss << "\n" << a.tagName << ": [";
            for(auto s : a.childrenNames)
            {
                ss << s << ",";
            }
            ss << "]";

            std::cout << ss.str();
        }
        std::cout << "\n--------------------------------------\n";
    }

    auto tagIter = tagList.begin();
    while(tagIter != tagList.end())
    {
        auto a  = *tagIter;
        if(DEBUG == 1)
        {
            std::cout << "\niiiiiiiiiiiiii";
            std::cout << "\n" <<  a.toString();
        }
        auto v = getAllTags(a.tagName);
        tagsMap.emplace(a.tagName,v);
        auto tagFIter = std::find_if(tagIter,tagList.end(),
        [&](const Tag& t_)
        {
            return t_.isClose && t_.tagName == a.tagName;
        });

        if(tagFIter != tagList.end())
        {
            tagIter = tagFIter + 1;
            continue;
        }
        tagIter++;
    }

    if(DEBUG == 1)
    {
        std::cout << "\\nkkkkkkkkkkkkkk";
        for(const auto& a : tagsMap)
        {
            std::stringstream ss;
            ss << "\n" << a.first << " : " << "[ ";

            for(auto v : a.second)
            {
                ss << v.tagName << ",";
            }
            ss << "]\n";
            std::cout << ss.str();
        }
        std::cout << "\\nllllllllllllll";
    }

    // Final Output
    for(const auto& line : inputQueryLines)
    {
        if(DEBUG == 1)
            std::cout << "\n line : " << line;
        std::vector<std::string> v = mysplit(line,'~');
        if(!(v.size() > 1))
        {
            continue;
        }
        std::string tNames = v[0];
        std::string valueName = v[1];
        std::vector<std::string> tagNames = mysplit(tNames,'.');
        if(DEBUG ==1)
        {
            std::cout << "----tagNames---";
            std::cout << "\n" <<  valueName << "[";
            for(auto s : tagNames)
            {
                std::cout << s << ",";
            }
            std::cout << "]";
        }
        std::vector<Tag> tags;
        if(!getTags(tagNames,tags))
        {
            std::string value = valueName;
            std::string output = getValue(tags,value);
            std::cout << output << std::endl; 
        }
        else{
            std::cout << "Not Found!" << std::endl;
        }
    }
    return 0;
}
