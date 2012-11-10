/*

Copyright (c) 2012, BDE NE666
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice, this 
   list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or 
   other materials provided with the distribution.
 - Neither the name of the BDE NE666 nor the names of its contributors may be used 
   to endorse or promote products derived from this software without specific prior 
   written permission.
   
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "string_crypt.h"


static char* crypt_str = NULL;
static unsigned int str_len = 0;
static unsigned int crypt_state = 0;


char* strappend(char* out,const char* app)
{
	unsigned int len = 0, lenapp = strlen(app),i=0;
	if (out != NULL)
		len = strlen(out);
	out = (char*) realloc((void*) out, (len + lenapp + 1)*sizeof(char));
	for (i=len;i < len + lenapp + 1;i++)
		out[i] = app[i-len];
	out[len + lenapp] = '\0';
	return out;
}


void first_init_str_crypt()
{
	crypt_str = NULL;
	str_len = 0;
	crypt_state = 0;
}


void begin_str_crypt(const char* key)
{
	free_str_crypt();
	str_len = strlen(key);
	crypt_str = malloc((strlen(key) + 1)* sizeof(char));
	strcpy(crypt_str,key);
}

char get_current_crypt_char()
{	
	return crypt_str[crypt_state];
}

char next_crypt_char()
{
	crypt_state++;
	if (crypt_state > str_len)
		crypt_state = 0;
	return crypt_str[crypt_state];
}

void free_str_crypt()
{
	if (crypt_str != NULL)
		free(crypt_str);
		
	crypt_str = NULL;
	str_len = 0;
	crypt_state = 0;
}


char* itoa(int value, char* result, int base) { 
		if (base < 2 || base > 36) { *result = '\0'; return result; }
	
		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;
	
		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );
	
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--= *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;
}
