#ifndef ____BIN__ASM
#define ____BIN__ASM

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "dcpu_opcodes.hpp"
#include "file.h"

class BinAsm
{
	public:
		BinAsm();
		~BinAsm();
		
		typedef struct
		{
			uint16_t offset;
			uint16_t size;
			uint16_t* data;
		} Label;
		
		//Load from filename;
		bool load(const std:string& filename);
		
		//Remove Comments from the text
		void remove_comments(std::string& str);
		//Make lines from text
	    std::vector<std::string> split_text(const std::string& text); 
		//Make word from lines (separator ' ' ',') "" conservate the integrity
		std::vector<std::string> split_line(const std::string& line);
		
		//take a value from word
		uint16_t get_value(const std::string& word, std::string& err);
		
		//get_data from line return size target must have line.size()*2 
		//to be safe
		uint16_t get_data(const std::vector<std::string>& line, 
						  uint16_t* target, std::string& err);
		
		//Get op from word
		uint8_t get_op(const std::string& word);
		//Get sop from word
		uint8_t get_sop(const std::string& word, std::string& err);
		//Get A operator from word
		uint8_t get_type(const std::string& word, std::string& err);
		
		//translate full instruction
		uint16_t translate(const std::vector<std::string>& line, bool p);

		
		void save(const std::string& filename);
		
		bool print_error(unsigned int line, 
						 bool warning,
						 const std::string& err);
		
	protected:
		std::map<std::string,AsmLabel> _labels;
		std::string _fullsrc;
		std::string _src;
		std::string _filename;
		

};

#endif