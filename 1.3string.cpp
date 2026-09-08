#include<iostream>
#include<string>
#include<cctype>
using namespace std;
int main()
{
string s ="Verifiaction";
cout<<"First 4"<<s.substr(0,4)<<endl;
cout<<"From 4"<<s.substr(4)<<endl;
int c=s.compare("Verify");
cout<<"Compare vs Verify:"<<(c<0?"<":c>0?">":"=")<<endl;
int freq[26]={0};
for(char ch:s)if (isalpha((unsigned char)ch))freq[tolower(ch)-'a']++;
cout<<"Letter counts:";
for(int i=0;i<26;i++)if(freq[i])cout<<char('a'+i)<<"?"<<freq[i]<<" ";
cout<<endl;
return 0;
}

 