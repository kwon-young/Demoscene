#ifndef RECT__IN__PICTURE____
#define RECT__IN__PICTURE____

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "string_crypt.h"
#include <math.h>
#include "stb_image_write.h"

#ifdef WIN32
	#include <wtypes.h>
#else //32bit implementation take a look if you want 64
	typedef unsigned int DWORD;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
#endif


//import extern printf here...
#ifndef RIP_PRINTF
#define RIP_PRINTF(...) printf(__VA_ARGS__)
#endif

//Flags
#define RIP_KNOW_HEADER   0x0001
#define RIP_PARTED_FILE   0x0002 //Cannot use with Hided Rect
#define RIP_CRYPTED       0x0004
#define RIP_EXPERIMENTAL  0x0008 //Cannot be writed with this source code
#define RIP_EXECUTABLE    0x0010
#define RIP_FORCE         0x0020
#define RIP_HIDED         0x0040 //Cannot use with Parted Files
#define RIP_IS_PATCH      0x0080 //File is patch cannot use with PARTED FILE

//Flags of version
#define RIP_VERSION_1     0x1000

//Const
#define RIP_MAX_RECT_BY_PICTURES 0xFF //can be changed and may be unused i don't remenber =)

//Errors
#define RIP_NULL_ERROR -1
#define RIP_OPEN_FILE_ERROR -2
#define RIP_ARG_ERROR -3
#define RIP_ARG_IMPOSSIBLE -4


//Know header flags (save some bytes)
#define RIPH_NO            0
#define RIPH_FARBRAUSCH    1  //64kb demoscene ;)
#define RIPH_MINGWIN_WIN32 2
#define RIPH_MZ_DOS        3
#define RIPH_IPS           4 //PATCH ;)

//Executables types...
#define RIPE_NO            0 //Must be zero here !
#define RIPE_ELF           1
#define RIPE_DOS           2 
#define RIPE_EXE           3

typedef struct
{
	BYTE magic[9]; //Rect Detection Magic
	WORD flags;  //Flags about the Rect (adding PE etc...)
	DWORD width;  //Width of the Rect in matrix
	DWORD height; //Height of the Rect in matrix
	char name[42]; //Name of the file
	DWORD size;   //Size without stub end
	DWORD stub;   //added stub data at the end of the file
	DWORD offset;   //Where begin the "packed into" file in the rect
	DWORD param;  //Optionnal param depending of the flags
	
} rip_header;

typedef struct
{
	rip_header rip_h;
	BYTE* rgb_pixels; //Pixels in the rect
	DWORD x; //Coord of the rect (use only for collisions)
	DWORD y;
	
} rect_in_picture;

#ifdef __cplusplus
extern "C" {
#endif

//Somes technic to creates rect... returns NULL if failed...
//From file
rect_in_picture* create_rect_from_data(const char* const packed_name,const BYTE* const data, DWORD size,DWORD max_width,DWORD max_height, DWORD flags);
//From image (top left then lines by lines RGB colors in the same time) offset must be a multiple of 3
rect_in_picture* get_rect_at(DWORD offset, BYTE* rgb_pixels,DWORD width,DWORD height);


//Then delete a rect : Warning delete the struct too !
void free_rect_in_picture(rect_in_picture* r);

//Rect Header operations :
DWORD clear_flags(DWORD flags); //In normal use this is not needed ! 
void update_header(rect_in_picture* r); //Finnishing modifing header juste copy it to the pixels...
rip_header create_rip_header(DWORD size,DWORD flags,DWORD width,DWORD height,const char* name, DWORD stub,DWORD param,DWORD offset);
//just fill the header struct no verifications...

//Crypt operations need first_init_str_crypt();
void crypt_rect_in_picture(rect_in_picture* r,const char* key);
void decrypt_rect_in_picture(rect_in_picture* r,const char* key);

//Hidding into pictures
//Hide "file" into "picture" then include it into "file" -> file is now a 
int hide_into_picture(rect_in_picture* file,BYTE* picture,DWORD width,DWORD height);// error < 0, else nb of low bit hidding... > 0 (the result can't be 0)
int unhide(rect_in_picture* r); //try unhide r if > 0 nb of low bit  unhidding else < 0 error

//Collisions
int is_on_rect(rect_in_picture* r,DWORD offset,DWORD width);

//Just delete magics from image (usefull to clean targets images to hide)
int delete_magic(BYTE* rgb_pixels, DWORD w, DWORD h); //return nb of magics deleted

//Gain some space
unsigned int detect_executable(const BYTE* file, DWORD size); //return executable type
WORD detect_know_header(const BYTE* const file, DWORD size); //return know header type
DWORD size_of_know_header(WORD id); //return the size (in bytes) of the knowed header (0 if id is invalid)

//Patch
int is_patch(const BYTE* const file,DWORD size); //return 1 if file is an ips patch else 0
int patch(rect_in_picture* r, const char* const target); //returns errors 0 if success if target file is not found write the ips file...

//Writes rect files 
int write_rip_file(rect_in_picture* r); //returns errors 0 if success

//you need include stb_image_write.h by Sean Barrett (define STB_IMAGE_WRITE_IMPLEMENTATION one time before last include)
//to use theses 2 functions :
//#define write_rip_png(a,b) write_rip_bmp(a, b)
int write_rip_bmp(rect_in_picture* r, char* filename);
int write_rip_png(rect_in_picture* r, char* filename);


//High level ! need stb_image_write and first_init_str_crypt(); !
//return nb of depacked files password can be NULL and can be not used by the image
int unpack_image(BYTE* rgb_pixels,DWORD width,DWORD height, char* password); 
//returns nb of images exported the only autorised flags is RIP_FORCE else 0 use NULL is password if you don't want crypt the file
//outname can be NULL then the file will be named automaticaly
int pack_into_images(const char* const outname, const char* const packed_name,const BYTE* const data, DWORD size,DWORD max_width,DWORD max_height, DWORD flags, DWORD min_stub,char* password); 

#ifdef __cplusplus
}
#endif

#endif