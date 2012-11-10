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

#include "rip.h"
#include "string_crypt.h"
	
#define IPS_FORBBIDEN_OFFSET 0x454f46

size_t _get_filesize(FILE* f)
{
	size_t s;
	size_t c = ftell(f);
	fseek(f,0,SEEK_END);
	s = ftell(f);
	fseek(f,c,SEEK_SET);
	return s;
}

const char *_byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, (x & z) ? "1" : "0");
    }

    return b;
}

static const BYTE farbrausch_table[0xE] = {0x4D, 0x5A, 0x42, 0x44, 0x45, 0x20, 0x4E, 0x45, 0x36, 0x36, 0x36, 0x20,0x50,0x45};
static const BYTE basic_mingw_table[0x82] = {
	//0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
	0x4D, 0x5A, 0x42, 0x44, 0x45, 0x20, 0x4E, 0x45, 0x36, 0x36, 0x36, 0x20, 0xff, 0xff, 0x00, 0x00,
0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
0x0e, 0x1f, 0xba, 0x0e, 0x00, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x54, 0x68,
0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f,
0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x44, 0x4f, 0x53, 0x20,
0x6d, 0x6f, 0x64, 0x65, 0x2e, 0x0d, 0x0d, 0x0a, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x50, 0x45 };
static const BYTE ips_table[0x5] = {0x50, 0x41, 0x54, 0x43, 0x48};
static const BYTE mz_table[0x2] = {0x4D, 0x5A};


rip_header create_rip_header(DWORD size,DWORD flags,DWORD width,DWORD height,const char* name, DWORD stub,DWORD param,DWORD offset)
{
	rip_header r;
	r.magic[0] = 0xff;
	r.magic[1] = 0x00;
	r.magic[2] = 0xf8;
	r.magic[3] = 0x4e; 
	r.magic[4] = 0x45;
	r.magic[5] = 0x36;
	r.magic[6] = 0x36;
	r.magic[7] = 0x36;
	r.magic[8] = 0x99;
	
	strncpy((char*) r.name,name,42);
	
	r.flags = flags; 
	r.width = width;  
	r.height = height; 
	r.size = size;   
	r.stub = stub;  
	r.param = param;
	r.offset = offset;
	
	return r;
}

int is_patch(const BYTE* const file,DWORD size)
{
	if (file && detect_know_header(file,size) == RIPH_IPS)
		return 1;
	return 0;
}

int patch(rect_in_picture* r, const char* const target)
{
	if (!r || !r->rgb_pixels || !target || !(r->rip_h.flags & RIP_IS_PATCH) || r->rip_h.size < 3 )
		return RIP_ARG_ERROR;
	
	FILE* f = fopen(target,"rb");
	if (!f)
	{
		char* ips = malloc (strlen(target) + 11);
		strcpy(ips , "patch_");
		strcpy(&ips[6] , target);
		strcpy(&ips[strlen(ips)] , ".ips");
		RIP_PRINTF("%s not found writing %s...\n",target,ips);
		f = fopen(ips,"wb");
		if (!f)
			return RIP_OPEN_FILE_ERROR;
		
		if (r->rip_h.flags & RIP_KNOW_HEADER && r->rip_h.param >> 16 == RIPH_IPS)
			fwrite("PATCH",1,5,f);
		fwrite(&r->rgb_pixels[r->rip_h.offset],1,r->rip_h.size,f);
		fclose(f);
	}
	else
	{
		DWORD size = _get_filesize(f);
		BYTE* p = malloc(size);
		fread(p,1,size,f);
		fclose(f);
		RIP_PRINTF("%s found patching...\n",target);
		int continu = 1;
		unsigned int k = 0, i = r->rip_h.offset;
		DWORD ips_size = r->rip_h.size + r->rip_h.offset;
		DWORD offset, patch_size, RLE_size; 
		if (!(r->rip_h.flags & RIP_KNOW_HEADER && r->rip_h.param >> 16 == RIPH_IPS))
			i+=5;
		while (continu)
		{
			if (i >= ips_size - 5)
			{
				continu = 0;
			}
			else
			{
				offset = (DWORD)(r->rgb_pixels[i]) << 16 | (DWORD)(r->rgb_pixels[i+1] << 8) | (DWORD)(r->rgb_pixels[i+2]);
				patch_size = (DWORD)(r->rgb_pixels[i+3]) << 8 | (DWORD)(r->rgb_pixels[i+4]);
				RIP_PRINTF("Offset : 0x%X, Size : %u\n",offset,patch_size);
				if (offset == IPS_FORBBIDEN_OFFSET) {
					continu =0;
					break;
				}
				else if (patch_size)
				{
					if (ips_size < i + patch_size) {
						continu =0;
						RIP_PRINTF("IPS corrupted\n");
						break;
					}
					if (size < offset + patch_size)
					{
						p = realloc(p,offset + patch_size);
						memset(&p[size],0,offset + patch_size - size);
						size = offset + patch_size;
					}
					memcpy(&p[offset],&r->rgb_pixels[i+5],patch_size);
					i+=patch_size + 5;
				}
				else if (i < ips_size - 8)
				{
					RLE_size = (WORD)(r->rgb_pixels[i+5] << 8) | (WORD)(r->rgb_pixels[i+6]);
					if (size < offset + RLE_size)
					{
						p = realloc(p,offset + RLE_size);
						memset(&p[size],0,offset + RLE_size - size);
						size = offset + RLE_size;
					}
					BYTE value = r->rgb_pixels[i+7];
					RIP_PRINTF("RLE compression : %u, Value : %u\n",RLE_size,value);
					memset(&p[offset],value,RLE_size);
					i+=8;
				}
				else
				{
					RIP_PRINTF("IPS corrupted\n");
					continu = 0;
					break;
				}
			}
		}
		f = fopen(target,"w");
		if (!f) { free(p); return RIP_OPEN_FILE_ERROR; }
		fwrite(p,1,size,f);
		fclose (f);
	}
	return 0;
}

DWORD clear_flags(DWORD flags)
{
	if (flags & RIP_HIDED || flags & RIP_IS_PATCH)
	{
		flags = flags & (~RIP_PARTED_FILE);
	}
	if (flags & RIP_PARTED_FILE)
	{
		flags = flags & (~RIP_FORCE);
	}
	if (flags & RIP_FORCE)
	{
		flags = flags & ((~RIP_CRYPTED) | (~RIP_KNOW_HEADER));
	}
	flags |= RIP_VERSION_1;
	return flags;
}

WORD detect_know_header(const BYTE* const file, DWORD size)
{
	if (size > 0x0E && !memcmp(farbrausch_table,file,0xE))
		return RIPH_FARBRAUSCH;
	else if (size > 0x82 && !memcmp(basic_mingw_table,file,0x82))
		return RIPH_MINGWIN_WIN32;
	else if (size > 0x05 && !memcmp(ips_table,file,0x2))
		return RIPH_IPS;
	else if (size > 0x02 && !memcmp(mz_table,file,0x2))
		return RIPH_MZ_DOS;
	return RIPH_NO;
}

DWORD size_of_know_header(WORD id)
{
	if (id == RIPH_FARBRAUSCH)
	{
		RIP_PRINTF("You use :\n");
		RIP_PRINTF("kkrunchy best compressor ever ;) BDE NE666 Newbies on the scene !\n");
		return 0xE;
	}
	else if (RIPH_MINGWIN_WIN32 == id)
		return 0x82;
	else if (id == RIPH_IPS)
		return 0x5;
	else if (id == RIPH_MZ_DOS)
		return 0x2;
	return 0;
}

unsigned int detect_executable(const BYTE* file, DWORD size)
{
	if (size < 16 || !file)
		return RIPE_NO;
	if (file[0] == 0x7f && file[1] == 'E' && file[2] == 'L' && file[3] == 'F')
		return RIPE_ELF;
	else if (file[0] == 'M' && file[1] == 'Z')
	{
		if (size > 0x40 && size > (DWORD) file[0x3C] + 3 && file[(DWORD) file[0x3C]] == 'P' && file[(DWORD) file[0x3C] + 1] == 'E')
			return RIPE_EXE;
		return RIPE_DOS;
	}		
	return RIPE_NO;
}

void crypt_rect_in_picture(rect_in_picture* r,const char* key)
{
	if (r->rip_h.flags & RIP_CRYPTED || r->rip_h.flags & RIP_HIDED)
		return;
	begin_str_crypt(key);
	unsigned int i = 0;
	char c;
	for (i = r->rip_h.offset ; i < r->rip_h.size + r->rip_h.offset ;i++)
	{
		c = (char) r->rgb_pixels[i];
		c -= get_current_crypt_char();
		r->rgb_pixels[i] = c;
		next_crypt_char();
	}
	r->rip_h.flags = r->rip_h.flags | RIP_CRYPTED;
}

void decrypt_rect_in_picture(rect_in_picture* r,const char* key)
{
	if (!(r->rip_h.flags & RIP_CRYPTED) || r->rip_h.flags & RIP_HIDED)
		return;
		
	begin_str_crypt(key);
	char c;
	unsigned int i = 0;
	for (i = r->rip_h.offset ; i < r->rip_h.size + r->rip_h.offset ;i++)
	{
		c = (char) r->rgb_pixels[i];
		c += get_current_crypt_char();
		r->rgb_pixels[i] = c;
		next_crypt_char();
	}
	r->rip_h.flags = (~RIP_CRYPTED) & r->rip_h.flags;
}

rect_in_picture* get_rect_at(DWORD offset, BYTE* pixels,DWORD width,DWORD height)
{
	if (width < (sizeof(rect_in_picture)/3 +1) || height < 3 ||
	    offset + (sizeof(rect_in_picture)/3 +1)*2 >= width*height*3 ||
	    pixels[offset] != 0xff || pixels[offset+1] != 0x00 || pixels[offset+2] != 0xf8 ||
	    pixels[offset+3] != 0x4e || pixels[offset+4] != 0x45 || pixels[offset+5] != 0x36 ||
	    pixels[offset+6] != 0x36 || pixels[offset+7] != 0x36 || pixels[offset+8] != 0x99) 
		return NULL;
	
	rip_header rh;
	memcpy(&rh,&(pixels[offset]),sizeof(rip_header));
	if (rh.height > (height-(offset/3)/width) || rh.width > (width-((offset/3)%width)) || rh.offset+rh.size > rh.width*height*3)
		return NULL;
	
	rect_in_picture* r = malloc(sizeof(rect_in_picture));
	r->rgb_pixels = malloc(rh.width*rh.height*3);

	for (r->y = 0; r->y < height;r->y++)
	{
		for (r->x = 0; r->x < width*3; r->x++)
		{
			if (r->x + width*3*r->y == offset)
				break;
		}
		if (r->x + width*3*r->y == offset)
				break;
	}
	
	unsigned int i = 0, x, y;
	for (y = r->y; y < rh.height + r->y;y++)
	{
		for (x=r->x; x < rh.width*3 + r->x; x++)
		{
			r->rgb_pixels[i] = pixels[y*width*3+x];
			i++;
		}
	}
	r->rip_h = rh;
	return r;
}

int is_on_rect(rect_in_picture* r,DWORD offset,DWORD width)
{
	if (r)
	{
		unsigned int y = r->y;
		for (y = r->y; y < r->rip_h.height; y++)
		{
			if (offset >= y*width*3 + r->x && offset <= y*width*3 + r->x + r->rip_h.width*3)
				return 1;
		}
		return 0;
	}
	return RIP_NULL_ERROR;
}

void free_rect_in_picture(rect_in_picture* r)
{
	if (r)
	{
		free(r->rgb_pixels);
		free(r);
	}
}

int write_rip_file(rect_in_picture* r)
{
	if (r)
	{	
		char name[43];
		strncpy(name,r->rip_h.name,42);
		name[42] = '\0';
		
		update_header(r);
		
		if (r->rip_h.flags & RIP_IS_PATCH)
			return patch(r,name);
		
		FILE* f;
		if (r->rip_h.flags & RIP_PARTED_FILE && (r->rip_h.param & 0x0000FFFF))
			f = fopen(name,"ab");
		else
			f = fopen(name,"wb");
		if (!f)
		{
			RIP_PRINTF("Cannot write %s I/O error...\n",name);
			return RIP_OPEN_FILE_ERROR;
		}
		if (r->rip_h.flags & RIP_KNOW_HEADER)
		{
			switch (r->rip_h.param >> 16)
			{
				case RIPH_FARBRAUSCH:
					fwrite(farbrausch_table,1,size_of_know_header(RIPH_FARBRAUSCH),f);
					RIP_PRINTF("%s copy farbrausch header\n",name);
					break;
				case RIPH_MINGWIN_WIN32:
					fwrite(basic_mingw_table,1,size_of_know_header(RIPH_MINGWIN_WIN32),f);
					RIP_PRINTF("%s copy mingwin win32 header\n",name);
					break;
				case RIPH_MZ_DOS:
					RIP_PRINTF("%s copy mz header yeah ! save 2 bytes ;)\n",name);
					fwrite(mz_table,1,size_of_know_header(RIPH_MZ_DOS),f);
					break;
				case RIPH_IPS:
					RIP_PRINTF("%s copy ips 'PATCH' header yeah ! save 5 bytes ;)\n",name);
					fwrite(ips_table,1,size_of_know_header(RIPH_IPS),f);
					break;
				default:
					RIP_PRINTF("Error : unknow header id\n");
					return RIP_ARG_IMPOSSIBLE;
			}
		}
		if (r->rip_h.flags & RIP_CRYPTED)
			RIP_PRINTF("Warning : writing crypted file !\n");
		
		fwrite(&(r->rgb_pixels[r->rip_h.offset]),1,r->rip_h.size,f);
		unsigned int i = 0;
		BYTE stub_byte = 0x00;
		for (i=0; i<r->rip_h.stub;i++)
			fwrite(&stub_byte,1,1,f);
			
		fclose(f);
		return 0;
	}
	return RIP_NULL_ERROR;
}

int write_rip_bmp(rect_in_picture* r, char* filename)
{
	if (r && filename && r->rgb_pixels)
	{
		return stbi_write_bmp(filename,r->rip_h.width,r->rip_h.height, 3, r->rgb_pixels);
	}
	return RIP_NULL_ERROR;
}
#ifndef write_rip_png
int write_rip_png(rect_in_picture* r, char* filename)
{
	if (r && filename && r->rgb_pixels)
	{
		return stbi_write_png(filename,r->rip_h.width,r->rip_h.height, 3, r->rgb_pixels,r->rip_h.width*3);
	}
	return RIP_NULL_ERROR;
}
#endif
int delete_magic(BYTE* pixels, DWORD w, DWORD h)
{
	if (!pixels || !w || !h)
		return RIP_ARG_ERROR;
	int magic_deleted = 0;
	unsigned int i = 0;
	for (i=0; i < w*h*3 - 9; i+= 3)
	{
		if (pixels[i] == 0xff && pixels[i+1] == 0x00 && pixels[i+2] == 0xf8 &&
			pixels[i+3] == 0x4e && pixels[i+4] == 0x45 && pixels[i+5] == 0x36 &&
			pixels[i+6] == 0x36 && pixels[i+7] == 0x36 && pixels[i+8] == 0x99) 
			{
				pixels[i] = 0xfe;
				magic_deleted++;
			}
	}
	return magic_deleted;
}

int unpack_image(BYTE* rgb_pixels,DWORD width,DWORD height, char* password)
{
	unsigned int i,k, nb_rect = 0, need_test = 1;
	rect_in_picture *r[RIP_MAX_RECT_BY_PICTURES];
	rect_in_picture* current_r;
	
	for (i=0;i<height*width*3;i+=3)
	{
		need_test = 1;
		for (k=0;k<nb_rect;k++)
		{
			if (is_on_rect(r[k],i,width))
			{
				need_test = 0;
				break;
			}
		}
		if (rgb_pixels[i] != 0xff)
			continue;
			
		if (need_test)
		{
			current_r = get_rect_at(i, rgb_pixels,width,height);
			if (current_r)
			{
				RIP_PRINTF("Found magic at 0x%X (%u;%u)\n",i,current_r->x,current_r->y);
				char filename[43];
				strncpy(filename,current_r->rip_h.name,42);
				filename[42] = '\0';
				RIP_PRINTF(" Filename : %s\n Rect Dimensions %ux%u\n Size of the file : %u\n Stub added size : %u\n",filename,current_r->rip_h.width,current_r->rip_h.height,current_r->rip_h.size,current_r->rip_h.stub);
				
				r[nb_rect] = current_r;
				RIP_PRINTF(" Flags : ");
				if (current_r->rip_h.flags & RIP_CRYPTED)
					RIP_PRINTF("CRYPTED ");
				if (current_r->rip_h.flags & RIP_EXECUTABLE)
					RIP_PRINTF("EXECUTABLE ");
				if (current_r->rip_h.flags & RIP_FORCE)
					RIP_PRINTF("FORCE ");
				if (current_r->rip_h.flags & RIP_KNOW_HEADER)
					RIP_PRINTF("KNOW_HEADER ");
				if (current_r->rip_h.flags & RIP_PARTED_FILE)
					RIP_PRINTF("PARTED_FILE ");	
				if (current_r->rip_h.flags & RIP_HIDED)
					RIP_PRINTF("HIDED ");
				if (current_r->rip_h.flags & RIP_IS_PATCH)
					RIP_PRINTF("PATCH ");
				if (current_r->rip_h.flags & RIP_VERSION_1 )
					RIP_PRINTF("VERSION_1");
	
				RIP_PRINTF("\n Params : 0x%x\n",current_r->rip_h.param);
				
				if (current_r->rip_h.flags & RIP_PARTED_FILE)
					RIP_PRINTF(" Part number : %u\n",current_r->rip_h.param & 0xFFFF);	
				
				nb_rect++;
			}
		}
	}
	int writed = 0;
	
	for (k=0;k<nb_rect;k++)
	{
		if (!(r[k]->rip_h.flags & RIP_VERSION_1))
		{
			RIP_PRINTF("Error rect found is not compatible with this program version (this program is version 1)\n");
			continue;
		}
		
		char filename[43]; 
		strncpy(filename,r[k]->rip_h.name,42);
		filename[42] = '\0';
		
		if (r[k]->rip_h.flags & RIP_HIDED)
		{
			int ret = unhide(r[k]);
			if (ret > 0)
				printf("%s success unhide ! %i low bits...\n",filename,ret);
			else
			{
				printf("%s failed unhide ! error %i skip the file...\n",filename,ret);
				continue;
			}
		}
		
		if(!(r[k]->rip_h.flags & RIP_FORCE))
		{
			if (r[k]->rip_h.flags & RIP_PARTED_FILE && (r[k]->rip_h.param & 0x0000FFFF))
			{
				RIP_PRINTF("Skip %s part %u\n",filename,r[k]->rip_h.param & 0x0000FFFF);
				continue;
			}
			
			if (r[k]->rip_h.flags & RIP_PARTED_FILE && !(r[k]->rip_h.param & 0x0000FFFF))
			{
				RIP_PRINTF("Found %s as parted file search for other parts...\n",filename);
				int m;
				WORD part_nb = 0;
				DWORD error = 0;
				
				for (m=0; m < nb_rect;m++)
				{
					if (!strncmp(filename,r[m]->rip_h.name,42) && r[m]->rip_h.flags & RIP_PARTED_FILE && ((r[m]->rip_h.param & 0x0000FFFF) == part_nb))
					{
						if (!(r[m]->rip_h.flags & RIP_VERSION_1))
						{
							RIP_PRINTF("Error rect found is not compatible with this program version (this program is version 1)\n");
							error = 1;
							break;
						}

						if (r[m]->rip_h.flags & RIP_CRYPTED && !password)
						{
							RIP_PRINTF("Error %s need to specify a crypt key (password)\n",filename);
							error = 1;
							break;
						}
						else if (r[m]->rip_h.flags & RIP_CRYPTED)
						{
							RIP_PRINTF("Decrypting %s if your password '%s' is wrong the result too !\n",filename,password);
							decrypt_rect_in_picture(r[m],password);
						}
						
						if (r[m]->rip_h.flags & RIP_EXPERIMENTAL)
						{
							RIP_PRINTF("Error part %u of %s : cannot write these file\n",(DWORD) part_nb,filename);
							error=1;
							break;
						}
							
						if(write_rip_file(r[m]) < 0)
						{
							RIP_PRINTF("Error while writing part %u of %s\n",(DWORD) part_nb,filename);
							error=1;
							break;
						}
						else
						{
							RIP_PRINTF("%s part %u succefull extracted\n",filename,(DWORD) part_nb);
							m=-1;
							part_nb++;
						}
					}
				}
				if (!error){
					RIP_PRINTF("%s extracted with success...\n",filename);
					writed++;
				}
				continue;
			}
		
			if (r[k]->rip_h.flags & RIP_CRYPTED && !password)
			{
				RIP_PRINTF("Error %s need to specify a crypt key (password)\n",filename);
				continue;
			}
			else if (r[k]->rip_h.flags & RIP_CRYPTED)
			{
				RIP_PRINTF("Decrypting %s if your password '%s' is wrong the result too !\n",filename,password);
				decrypt_rect_in_picture(r[k],password);
			}
		
			if (r[k]->rip_h.flags & RIP_EXPERIMENTAL)
			{
				RIP_PRINTF("Error %s is an experimental file cannot be writed with this source code\n",filename);
				continue;
			}
		}
	
		if(!(write_rip_file(r[k]) < 0))
		{
			RIP_PRINTF("%s succefull extracted\n",filename);
			writed++;
		}
	}
	for (k=0;k<nb_rect;k++)
	{
		free_rect_in_picture(r[k]);
	}
	return writed; 
}


rect_in_picture* create_rect_from_data(const char* const filename,const BYTE* const user_data, DWORD size,DWORD max_width,DWORD max_height, DWORD flags)
{

	if (size > max_height*max_width*3 || !filename || !user_data || max_width < sizeof(rip_header)/3 + 1 || !size)
		return NULL;
	
	BYTE* data = malloc(size);
	memcpy(data,user_data,size);
	switch (detect_executable(data,size))
	{
		case RIPE_ELF:
			RIP_PRINTF("Linux Executable File (ELF) detected !\n");
			flags |= RIP_EXECUTABLE;
			break;
		case RIPE_DOS:
			RIP_PRINTF("Dos executable detected !\n");
			flags |= RIP_EXECUTABLE;
			break;
		case RIPE_EXE:
			RIP_PRINTF("Windows executable (PE) detected !\n");
			flags |= RIP_EXECUTABLE;
			if (!(flags & RIP_FORCE))
			{
				data[0x2] = 'B';data[0x3] = 'D';data[0x4] = 'E';data[0x5] = ' ';data[0x6] = 'N';
				data[0x7] = 'E';data[0x8] = '6';data[0x9] = '6';data[0xA] = '6';data[0xB] = ' ';
			}
			break;
			
		case RIPE_NO:
			break;
			
		default: 
			RIP_PRINTF("Executable file detected !\n");
			flags |= RIP_EXECUTABLE;
	}
	
	DWORD h=2,w=sizeof(rip_header)/3 + 1, stub = 0;
	DWORD detected_header = detect_know_header(data, size);
	
	if (is_patch(data, size))
	{
		RIP_PRINTF("IPS Patch detected !\n");
		flags |= RIP_IS_PATCH;
	}
	
	if (detected_header && !(flags & RIP_FORCE))
	{
		size -= size_of_know_header(detected_header);
		memmove(data,&(data[size_of_know_header(detected_header)]),size);
		RIP_PRINTF("Save %u bytes by know header\n",size_of_know_header(detected_header));
		flags |= RIP_KNOW_HEADER;
	}
	else
	{
		detected_header = 0;
	}
	
	
	
	if (!(flags & RIP_FORCE))
	{
		unsigned int i;
		for (i=size-1;i > 0 && !data[i]; i--)
			stub++;
		RIP_PRINTF("Stub detected : %u original size : %u reduced to : %u \n",stub,size,i+1);
		size = i+1;
	}
	
	RIP_PRINTF("%s: squarred dimensions %ux%u\n", filename, (DWORD)(sqrt(size + sizeof(rip_header))+0.5),(DWORD)(sqrt(size + sizeof(rip_header)))+1);
	while (h*w*3 < size + sizeof(rip_header))
	{
		if (w < max_width || (max_width <= max_height && h >= max_height))
			w++;
		if (h < max_height || (max_width >= max_height && w >= max_width))
			h++;
	}
	RIP_PRINTF("%s: base image dimensions %ux%u\n", filename, w,h);
	DWORD h1=h,w1=w;
	while (h*w*3 - size - sizeof(rip_header) > w*3)
		h--;
	while (h1*w1*3 - size - sizeof(rip_header) > h1)
		w1--;
	
	if (w1 < w && w1*h1 < w*h)
	{
		w = w1; h=h1;
	}
	if (w >(DWORD)(sqrt(size + sizeof(rip_header))+0.5) && h > (DWORD)(sqrt(size + sizeof(rip_header)))+1)
	{
		w = (sqrt(size + sizeof(rip_header))+0.5);
		h = (sqrt(size + sizeof(rip_header)))+1;
	}

	if (w < sizeof(rip_header)/3 + 1)
		w = sizeof(rip_header)/3 + 1;
	if (h < 2)
		h = 2;
	
	RIP_PRINTF("%s: finals dimensions %ux%u\n", filename, w,h);
	if (max_height < h || max_width < w)
	{
		RIP_PRINTF("%s: unable to create with thoses dimensions increase dimensions %ux%u\n", filename, w,h);
		free(data);
		return NULL;
	}
	rect_in_picture* r = malloc(sizeof(rect_in_picture));
	
	
	r->rip_h = create_rip_header(size,flags, w, h,filename, stub,detected_header << 16 ,sizeof(rip_header));

	r->x = 0;
	r->y = 0;

	r->rgb_pixels = malloc(h*w*3);
	memcpy(&(r->rgb_pixels[sizeof(rip_header)]),data,size);
	memset(&r->rgb_pixels[sizeof(rip_header) + size],255,h*w*3-size-sizeof(rip_header));
	RIP_PRINTF("%s: fill %u image end bytes with %u blancks\n", filename, h*w*3-size-sizeof(rip_header),(h*w*3-size-sizeof(rip_header))/3);

	update_header(r);
	
	return r;
}

void update_header(rect_in_picture* r)
{
	if (r && r->rgb_pixels)
	{
		r->rip_h.flags = clear_flags(r->rip_h.flags);
		memcpy(r->rgb_pixels,&(r->rip_h),sizeof(rip_header));
	}
}

int hide_into_picture(rect_in_picture* file,BYTE* picture,DWORD width,DWORD height)
{
	if (!file || !picture)
		return RIP_NULL_ERROR;
	if (file->rip_h.flags & RIP_HIDED || file->rip_h.flags & RIP_PARTED_FILE)
		return RIP_ARG_ERROR;
	
	WORD p;
	
	if (height * width * 3 > file->rip_h.size*8 + file->rip_h.offset)
		p = 1;
	else if (height * width * 3 > file->rip_h.size*4 + file->rip_h.offset)
		p = 2;
	else if (height * width * 3 > file->rip_h.size*2 + file->rip_h.offset)
		p = 4;
	else
		return RIP_ARG_IMPOSSIBLE;
	
	unsigned int i, real_i = file->rip_h.offset;
	char dec;
	BYTE mask = 0xFF << p;
	for (i = file->rip_h.offset; i < file->rip_h.offset+file->rip_h.size; i++)
	{
		for (dec = 0; dec < 8; dec+=p)
		{
			picture[real_i] = mask & picture[real_i];
			picture[real_i] |= (file->rgb_pixels[i] >> dec) & ~mask;
			real_i++;
		}
	}
	
	free(file->rgb_pixels);
	file->rgb_pixels = picture;
	file->rip_h.param = (file->rip_h.param & 0xffff0000) | (p & 0xffff);
	file->rip_h.flags |= RIP_HIDED;
	file->rip_h.width = width;
	file->rip_h.height = height;
	
	update_header(file);
	
	return p;
}

int unhide(rect_in_picture* r)
{
	if (!r || !r->rgb_pixels)
		return RIP_NULL_ERROR;
	if (!(r->rip_h.flags & RIP_HIDED) || (r->rip_h.flags & RIP_PARTED_FILE))
		return RIP_ARG_ERROR;
	
	WORD p = r->rip_h.param & 0xffff;
	if (p != 1 && p != 2 && p != 4)
	{
		RIP_PRINTF("Unable to 'unhide' the image : param invalid or program version obselete\n");
		return RIP_ARG_IMPOSSIBLE;
	}
	
	unsigned int i, real_i = r->rip_h.offset;
	char dec;
	BYTE mask = 0xFF << p, cur;	
	for (i = r->rip_h.offset; i < r->rip_h.offset+r->rip_h.size; i++)
	{
		cur = 0;	
		for (dec = 0; dec <8; dec+=p)
		{
			cur |= (~mask & r->rgb_pixels[real_i]) << dec;
			real_i++;
		}
		r->rgb_pixels[i] = cur;
	}
	
	r->rip_h.flags = r->rip_h.flags & ~RIP_HIDED;
	r->rip_h.param &= 0xFFFF0000;
	
	update_header(r);
	return p;
}

int pack_into_images(const char* const outname, const char* const packed_name,const BYTE* const data, DWORD size,DWORD max_width,DWORD max_height, DWORD flags, DWORD min_stub,char* password)
{
	if (max_width < sizeof(rip_header) / 3 + 1 || max_height < 2 || !packed_name || !data || !size || sizeof(rip_header) > min_stub)
		return RIP_ARG_ERROR;
	
	flags &= RIP_FORCE;
	int images = 0;
	int canpart = 1;
	if (is_patch(data,size))
		canpart = 0;
	
	if (!(flags & RIP_FORCE))
	{
		DWORD offset = 0;
		WORD nb_part = 0;
		rect_in_picture* r;
		unsigned int k,stub=0,first_stub=0;
		for (k = 0; k < size;k++)
		{
			if ((canpart && ((stub > min_stub && data[k] != 0x00) || max_width*(max_height-1)*3 < k - offset)) || k == size - 1)
			{
				if (stub > min_stub && data[k] != 0x00)
					RIP_PRINTF("%s: detected %u stub at 0x%X split file here... (part %u)\n", packed_name, stub, first_stub,(DWORD) nb_part);
				else if (max_width*(max_height-1)*3 < k - offset)
					RIP_PRINTF("%s: split file here to respect width and height contrainsts... (part %u)\n", packed_name,(DWORD) nb_part);
				else if (nb_part)
					RIP_PRINTF("%s: final part (part %u)\n", packed_name,(DWORD) nb_part);
					
				if (!nb_part &&  k != size - 1)
					flags |= RIP_PARTED_FILE;
				r = create_rect_from_data(packed_name,&data[offset], k-offset+1,max_width,max_height,flags);

				if (!r)
					return RIP_ARG_IMPOSSIBLE;
				r->rip_h.param |= nb_part;
					
				if (password) { 
					crypt_rect_in_picture(r,password);
					RIP_PRINTF("%s: crypting with '%s' password\n", packed_name,password);
				}
				update_header(r);
				char numbuf[33];
				char* out;
				itoa(nb_part,numbuf,10);
				if (flags & RIP_PARTED_FILE)
				{
					if (outname) {
						out = malloc(strlen(outname) + 5 + 33 + 5);
						strncpy(out,outname,strlen(outname)+1);
						strncpy(&out[strlen(outname)],numbuf,34);
						strcpy(&out[strlen(out)],".png");
					}
					else{
						out = malloc(strlen(packed_name) + 5 + 33 + 5);
						strncpy(out,packed_name,strlen(packed_name)+1);
						strncpy(&out[strlen(packed_name)],numbuf,34);
						strcpy(&out[strlen(out)],".png");
					}
				}
				else
				{
					if (outname){
						out = malloc(strlen(outname) + 5);
						strcpy(out,outname);
						strcpy(&out[strlen(outname)],".png");
					}
					else {
						out = malloc(strlen(packed_name) + 5);
						strncpy(out,packed_name,strlen(packed_name)+1);
						strcpy(&out[strlen(packed_name)],".png");
					}
				}
				
				
				
				RIP_PRINTF("Writing %s ...\n", out);
				images += write_rip_png(r, out);
			
				free(out);
				free_rect_in_picture(r);
				nb_part++;
				offset = k + 1;
				stub = 0;
			}
			else if (!stub && data[k] == 0x00)
			{
				first_stub = k;
				stub++;
			}
			else if (data[k] == 0x00)
				stub++;
			else
				stub = 0;
		}
	}
	else 
	{
		rect_in_picture* r = create_rect_from_data(packed_name,data, size,max_width,max_height,flags);
		if (!r)
			return RIP_ARG_ERROR;
		
		update_header(r);
		
		char* out;
		if (outname){
			out = malloc(strlen(outname) + 5);
			strcpy(out,outname);
			strcpy(&out[strlen(outname)],".png");
		}
		else {
			out = malloc(strlen(packed_name) + 5);
			strncpy(out,packed_name,strlen(packed_name)+1);
			strcpy(&out[strlen(packed_name)],".png");
		}
		
		RIP_PRINTF("Writing %s ...\n", out);
		images += write_rip_png(r, out);
	
		
		free(out);
		free_rect_in_picture(r);
	}
	
	return images;
}