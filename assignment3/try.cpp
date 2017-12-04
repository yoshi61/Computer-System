// ifstream constructor.
#include <sys/stat.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
using namespace std;
bool fileExists(const string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
int main () 
{
	string filename = "Bat.jack";
	if(fileExists(filename))
	{
		cout<< "faaaaa" << endl;
	}

  return 0;
}