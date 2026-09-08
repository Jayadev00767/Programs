#include<iostream>
#include<string>
#include<cctype>  
using namespace std;
int main()
{
    string s;
    cout<<"Enter  a Word";
    cin>>s;
    cout<<"Length:"<<s.length()<<endl;
    cout<<"Upper:";
    for(char &c:s)
    cout<<(char)toupper(c);
    cout<<endl;
    bool pal=true;
    for(size_t i=0,j=s.size()-1;i<j;++i,--j)
    if(s[i]!=s[j])
    {
        pal=false;
        break;

    }
    cout<<s<<(pal?"IS":"is NOT")<<"a palindrome\n"<<endl;
    size_t pos=s.find("AN");
    if(pos!=string::npos)
    {
        cout<<"AN found at index"<<pos<<endl;
    }
    else
    {
        cout<<"AN not found\n";
    }
    return 0;
}