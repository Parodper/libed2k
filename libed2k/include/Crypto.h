#include <string>

namespace IsCrypto
{
#ifndef WIN32
    typedef unsigned char byte;
    typedef unsigned int DWORD;
    typedef unsigned int UINT;
#endif

void CreateIV(byte *src, int len_src, byte *iv, int len_iv);
void CreateKey(byte *key, int len_key);
void convert_number(int number, byte *array, int len_array);

std::string EncryptPasswd(const std::string& strPasswd, const std::string& strLogin);
std::string DecryptPasswd(const std::string& strEncpasswd, const std::string& strLogin);

};

