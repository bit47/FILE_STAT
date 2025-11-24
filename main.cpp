#include "include\\file_stat.h"

using namespace std;
using strpair = pair<string,int>;//好烂的名字，但想不出来了
using unchar = unsigned char;

class FileInfo
{

private:
    int _wcnt;
    int _lcnt;
    long long _cn_cnt; // 中文字符计数
    ifstream _inFile;
    ofstream _outFile;
    map<string,int> _dict;

public:
//----------------------------------------------------------构造
    FileInfo(string address1,string address2)
    :_wcnt(0)
    ,_lcnt(0)
    ,_cn_cnt(0)
    ,_inFile(address1)
    ,_outFile(address2)
    ,_dict()
    {}

//----------------------------------------------------------析构
    ~FileInfo() 
    {
        if (_inFile.is_open()) _inFile.close();
        if (_outFile.is_open()) _outFile.close();
    }

//----------------------------------------------------------记录前五重复单词
    vector<strpair> topWu()
    {
        string word;
    
        _inFile.clear();
        _inFile.seekg(0,ios::beg);
        if(_inFile.is_open())
        {
            while(_inFile >> word)//逐个单词读
            {
                //处理标点符号(没处理字符串中间的标点 先不管了)
                while(!word.empty() && ispunct((unchar)word.front()))
                {
                    word.erase(word.begin());
                }
                while (!word.empty() && ispunct((unchar)word.back()))
                {
                    word.pop_back();
                }
                if(word.empty())
                {
                    continue;
                }
                //转换大小写
                transform(word.begin(),word.end(),word.begin(),[](char c){return tolower((unsigned char)c);});
                //存储
                _dict[word]++;
                _wcnt++;
            }

            if(_dict.empty())
            {
                cerr<<"empty"<<endl;
            }
            //排序
            vector<strpair> _tmp(_dict.begin(),_dict.end());
            sort(_tmp.begin(),_tmp.end(),[](const strpair& a,const strpair& b){return a.second>b.second;});
            
            vector<strpair> result(_tmp.begin(),_tmp.end());
            result.resize(min(5,(int)result.size()));

            return result;
        }
        else
        {
            cerr<<"open failed from36"<<endl;
            exit(31415926);
        }
    }

//----------------------------------------------------------记录行数
    void countLine()
    {
        string line;
        if(_inFile.is_open())
        {
            _inFile.clear();
            _inFile.seekg(0,ios::beg);
            while(getline(_inFile,line))
            {
                _lcnt++;
            }
        }
        else
        {
            cerr<<"open failed from87"<<endl;
            exit(535897);
        }
    }
//休息一下, 马上回来
//好吧其实是去跑校园跑去了

//----------------------------------------------------------统计中文字符
    bool isChineseChar(uint32_t cp)
    {
        return (cp >= 0x4E00 && cp <= 0x9FFF)
            || (cp >= 0x3400 && cp <= 0x4DBF)
            || (cp >= 0x20000 && cp <= 0x2A6DF)
            || (cp >= 0x2A700 && cp <= 0x2B73F)
            || (cp >= 0x2B740 && cp <= 0x2B81F)
            || (cp >= 0x2B820 && cp <= 0x2CEAF)
            || (cp >= 0xF900 && cp <= 0xFAFF)
            || (cp >= 0x2F800 && cp <= 0x2FA1F);
    }

//-----------------------------------------------------------统计字符串中的 UTF-8 中文字符数
    long long countChToUtf8(const string &s)
    {
        long long cnt = 0;
        size_t i = 0;
        const unsigned char* data = (const unsigned char*)s.c_str();
        size_t n = s.size();
        while(i < n)
        {
            unsigned char c = data[i];
            uint32_t cp = 0;
            size_t seq = 0;
            if(c < 0x80)
            {
                cp = c; seq = 1;
            }
            else if((c >> 5) == 0x6) // 110xxxxx
            {
                if(i + 1 >= n) break;
                cp = ((c & 0x1F) << 6) | (data[i+1] & 0x3F);
                seq = 2;
            }
            else if((c >> 4) == 0xE) // 1110xxxx
            {
                if(i + 2 >= n) break;
                cp = ((c & 0x0F) << 12) | ((data[i+1] & 0x3F) << 6) | (data[i+2] & 0x3F);
                seq = 3;
            }
            else if((c >> 3) == 0x1E) // 11110xxx
            {
                if(i + 3 >= n) break;
                cp = ((c & 0x07) << 18) | ((data[i+1] & 0x3F) << 12) | ((data[i+2] & 0x3F) << 6) | (data[i+3] & 0x3F);
                seq = 4;
            }
            else
            {
                seq = 1;
            }

            if(seq > 0 && isChineseChar(cp)) cnt++;
            i += seq;
        }
        return cnt;
    }

//-----------------------------------------------------------统计文件中的中文字符总数
    void countChinese()
    {
        string line;
        _cn_cnt = 0;
        if(_inFile.is_open())
        {
            _inFile.clear();
            _inFile.seekg(0,ios::beg);
            while(getline(_inFile,line))
            {
                _cn_cnt += countChToUtf8(line);
            }
        }
        else
        {
            cerr<<"open failed from167"<<endl;
            exit(932384);
        }
    }

//----------------------------------------------------------写入result.txt
    void outPut()
    {
        if(_outFile.is_open())
        {
            countLine();//笑死了我就说行数为什么一直是0, 原来忘记调用了
            countChinese();
            vector<strpair> vec = topWu();
            _outFile<<"总行数:"<<_lcnt<<'\n';
            _outFile<<"总字数:"<<_wcnt<<'\n';
            _outFile<<"中文字符总数:"<<_cn_cnt<<'\n';
            _outFile<<"高频单词Top5:"<<'\n';
            for(auto& [key,val]: vec)
            {
                if(val<2)
                {
                    continue;
                }
                _outFile<<key<<':'<<val<<'\n';
            }
            _outFile<<flush;
        }
        else
        {
            cerr<<"open failed from188"<<endl;
            exit(626433);
        }
    }
};

int main(int argc, char* argv[])
{
    // 默认路径
    string inputPath = "..\\test\\input.txt";
    string outputPath = "..\\test\\result.txt";

    //解析
    for(int i = 1; i < argc; ++i)
    {
        string arg = string(argv[i]);
        if(arg == "--output" || arg == "-o")
        {
            if(i + 1 < argc)
            {
                outputPath = string(argv[i+1]);
                ++i;
            }
            else
            {
                cerr<<"缺少输出文件路径或路径多余\n";
                return 83279;
            }
        }
        else
        {
            if(inputPath == "..\\test\\input.txt")
            {
                inputPath = arg;
            }
            else
            {
                cerr<<"多余的参数: "<<arg<<endl;
                return 50288;
            }
        }
    }

    FileInfo file(inputPath, outputPath);
    file.outPut();
    cout<<"success\n";

    return 0;
}
