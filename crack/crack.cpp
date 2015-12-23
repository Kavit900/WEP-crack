#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int simulate_res(unsigned char *IV, unsigned char *keys, int index_byte, unsigned char out)
{
	unsigned char S[256];
	unsigned char temp_keys[8];
	unsigned char a,s1,k;
	int i,j,result;
	memcpy(temp_keys, IV, 3);
	memcpy(&temp_keys[3], keys, 5);
	for(i=0; i<256; i++)
	{
		S[i]=i;
	}
	j=k=0;
	for(i=0;i<index_byte+3;i++)
	{
		j=(unsigned char)(j+S[i]+temp_keys[k]);
		j=j%256;
		a=S[i];
		S[i]=S[j];
		S[j]=a;
		if(++k>=8)
		{
			k=0;
		}
	}
	for(i=0; i<256; i++)
	{
		if(S[i]==out)
		{
			s1=i;
		}
	}
	if((S[0] != index_byte+3) || (S[1] != 0))
	{
		return -1;
	}
	else
	{
		result = (s1-j-S[index_byte+3]);
		result=result%256;
		if(result<0)
		{
			result=result+256;
		}
		return result;
	}
}


int main(int argc, char* argv[])
{
	int i,j,k, temp, read, max, index_max, index_byte;
	unsigned char count[256];
	unsigned char keys[5];
	FILE *file_log;
	unsigned char temp_line[256];
	unsigned char plain = 'C';
	unsigned char out;
	memset(keys, 0, 5);
	for(index_byte = 0; index_byte<5; index_byte++)
	{
		memset(count, 0, 256); // all values of the array count are set to zero
		file_log = fopen(argv[1], "r");
		read = fread(temp_line, sizeof(unsigned char), 143, file_log);
		while(read!=0)
		{
			if(temp_line[24]==index_byte+3 && (temp_line[25]==255))
			{
				out = plain ^ temp_line[27];
				temp = simulate_res(&temp_line[24],keys,index_byte,out);
				if((temp>=0)&&(temp<=255))
				{
					count[temp]++;
				}
			}
			read=fread(temp_line, sizeof(unsigned char), 143, file_log);
		}
		max=0;
		index_max=0;
		for(i=0;i<256;i++)
		{
			if(count[i]>max)
			{
				index_max=i;
				max=count[i];
			}
		}
		cout<<max<<endl;
		keys[index_byte]=(unsigned char)index_max;
	}
	for(int i=0; i<5; i++)
	{
		cout<<keys[i];
	}
	cout<<endl;
}