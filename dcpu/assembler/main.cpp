#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "disassembler.hpp"


void print_help(const char* program_name)
{
	std::cout << "usage : " << program_name << " filename" << std::endl;
	std::cout << "options : " << std::endl;
	std::cout << " -d              : disassemble instead of assemble\n";
	std::cout << " -o <target name>: specify target name (default a.out)\n";
}

int main(int argc, char** argv)
{
    bool disassemble = false;
	std::vector<std::string> filenames;
	std::string target = "a.out";
	if (argc == 1)
	{
		std::string pn = argv[0];
		pn.erase(0,pn.find_last_of('/')+1);
		pn.erase(0,pn.find_last_of('\\')+1);
	    print_help(pn.c_str());
	}
	
	for (int k = 0; k < argc; k++)
	{
		if (std::string("-d") == argv[k])
			disassemble = true;
	    else if (std::string("-o") == argv[k] && argc > k+1){
			target = argv[k+1];
			k++;
		}
		else if (std::string("-o") == argv[k]) {
		    std::cout << "error: -o need filename argument" << std::endl;
			return 0xdead;
	    }
		else if (argv[k][0] == '-'){
		    std::cout << "error: unknown command " << argv[k] << std::endl;
			return 0xdead;
	    }
		else {
		    filenames.push_back(std::string(argv[k]));
	    }
	}
	for (std::vector<std::string>::iterator i = filenames.begin()
	      ; i != filenames.end() ; i++)
	{
		if (disassemble)
		{
			std::string dis=disassembly_file(i->c_str());
			fwritetext(target.c_str(), dis.c_str());
		}
		else
		{
			//TODO Assembler 
		}
	}
	return 0x600D; // return good
}