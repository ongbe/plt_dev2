#pragma once
//#include <string.h>
#include <string>
#include <vector>
#include <hash_map>

using namespace std;

#ifdef __GNUC__
using namespace __gnu_cxx;

struct hashstring
{
    long operator()(const string& s) const
    {
        return __stl_hash_string(s.c_str());
    }
};
#endif

#ifdef __GNUC__
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

extern string remove_space(const string & src);
extern string upper_case(const string & src);
extern void split_string(const string & src, const string & delimiter, vector<string> & vstr);
extern string get_today();
extern string get_time();
//��ȡ���������ݵ��ڴ�
extern long load_bfile(const char * filename, void * dst);
//���ڴ����ݴ洢���������ļ���ע��sizeΪ�ڴ���ֽ���
extern void save_bmem(const char * filename, void * src, long size);
