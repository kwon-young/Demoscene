#include "binasm.hpp"

BinAsm::BinAsm()
{

}

BinAsm::~BinAsm()
{
	
}

bool BinAsm::load(const std:string& filename)
{
	_src = "";
	_fullsrc = "";
	_filename = filename;
	_labels.clear();
	
	
	FILE* f = fopen(filename.c_str(),"r");
	if (!f)
	{
		std::cerr << "error : " << filename << " not found !" << std::endl;
	    return false;
	}

	unsigned int size = fsize(f);
	char* buff = new char[size+1];
	fread(buff,1,size,buff);
	fclose(f);
	
	_fullsrc = buff;
	_src=_fullsrc;
	remove_comments(_src);
	
	delete buff;
	return true;
}

void BinAsm::remove_comments(std::string& str)
{
	std::string::iterator b = str.begin();
	std::string::iterator e = str.end();
	bool in_comment = false;
	bool special_case = false;
	for (std::string::iterator i=b; i!=e;i++)
	{
		if (special_case && (*i == '\'' || *i == '"') && *(-i) != '\\')
			special_case = false;
		else if (*i == '\'' || *i == '"')
			special_case = true;
		else if (*i == ';' && !special_case && !in_comment)
		{
			b = i;
			in_comment = true;
		}
		else if (*i == '\n' && in_comment) 
		{
			in_comment = false;
			e = i;
			i = str.erase(b,e);
		}
	}
	if (in_comment && !special_case) str.erase(b,str.end());
}

std::vector<std::string> BinAsm::split_text(const std::string& text)
{
	std::vector<std::string> lines;
	lines.reserve(text.size()/8);
	unsigned int b=0
	for (unsigned int i=0; i<text.size();i++)
	{
		if (text[i] == '\n' && i-b <= 1)
		{
			b = i+1;
		}
		else if (text[i] == '\n') 
		{
			lines.push_back(text.substr(b,i));
			b = i+1;
		}
	}
	if (i-b <= 1)
		lines.push_back(text.substr(b,lines.end()));
	return lines;
}

std::vector<std::string> BinAsm::split_line(const std::string& line)
{
	std::vector<std::string>  words;
	words.reserve(line.size()/3);
	unsigned int b=0;
	bool validate=false;
	char special_case=0;
	for (unsigned int i=0; i<line.size();i++)
	{
	    if (!special_case && line[i] != ' ' 
		  && line[i] != ',' && line[i] != '\t')
		{
			b=i;
			validate=true;
			if ((line[i] == '\'' || line[i] == '"') && 
			   (!i || line[i-1] != '\\'))
			  special_case = line[i];
		}
		else if (special_case == line[i])
		{
			words.push_back(line.substr(b,i-b+1));
			validate=false;
			special_case=0;
		}
		else if (!special_case && 
		        (line[i] == ' ' || line[i] != ',' || line[i] != '\t'))
		{
			words.push_back(line.substr(b,i-b));
			validate=false;
		}
	}
	if (validate) words.push_back(line.substr(b,line.size()));
	return words;
}

uint16_t BinAsm::get_value(const std::string& word, std::string& err)
{
	unsigned int s = word.size();
    if (!s) return 0;
	
	
	if (word[0]== '\'' && s < 3)
	{
		err = "invalid symbol value " <<  word;
		return 0;
	}
	if (word[0]== '\'' && s >= 3)
	{
		if (word[1] == '\\' && s >= 4)
		{
			switch (word[2])
			{
				case 'n':
					return '\n';
				case 't':
					return '\t';
				case '\\':
					return '\\';
				case 'r':
					return '\r';
				default:
					err = "invalid special symbol value " +  word;
					return 0;
			}
		}
		else
			return word[1];
	}
	const char* c = word.c_str();
	int number=strol(word.c_str(),&c,0);
	if (c==word.c_str())
	{
		if (_labels.find(word)!=_labels.end())
		{
			return _labels[word].offset;
		}
		err = "label " +  word + " not defined";
		return 0;
	}
	if (number > 0xFFFF || number < -(0xFFFF))
	{
		err = "value " +  word + " overflow (max 0xFFFF)";
		return 0;
	}
	uint16_t v=number;
	if (number < 0)
	{
	    int16_t n = number;
		v = *(uint16_t*)(&n); //convert signed value into unsigned
	}
	return v;
}

bool BinAsm::print_error(unsigned int line, 
				bool warning,
				const std::string& err)
{
	if (!err.size()) return false;
	if (warning)
		std::cerr << "warning line " << line << ": " << err << std::endl;
	else
		std::cerr << "error line " << line << ": " << err << std::endl;
	return true;
}

uint16_t BinAsm::get_data(const std::string& line, uint16_t* target, 
								std::string& err)
{
	std::vector<std::string> datas = split_line(line);
	
	for (std::vector<std::string>::iterator k=datas.begin();
				i!=datas.end(); i++;
	{
		std::string p = std::transform(k->begin(), 
								   k->end(), 
								   k->begin(), ::tolower);
		if (p == ".dat" || p == "dat")
		{
			uint16_t size = 0;
			for (std::vector<std::string>::iterator i=k+1;
					i!=datas.end(); i++;
			{
				if (i->size() > 0 && (*i)[0] == '"')
				{
					for (unsigned int j=0; j < i->size()-1;j++)
					{
						if (word[j-1] == '\\')
						{
							switch (word[j+1])
							{
								case 'n':
									target[size] = '\n';
								case 't':
									target[size] = '\t';
								case '\\':
									target[size] = '\\';
								case 'r':
									target[size] = '\r';
								default:
									err = "invalid special symbol value " +  word;
									return 0;
							}
							
						}
					    else if (word[j] == '\\')
							continue;
						else	
							target[size] = (*i)[j];
							
						size++;
					}
				}
				else
				{
					target[size] = get_value(*i, err);
					size++;
				}
			}
			return size;
		}
	}
	return 0;
}

uint8_t BinAsm::get_op(const std::string& word)
{
	std::string p = std::transform(word.begin(), 
								   word.end(), 
								   word.begin(), ::toupper);
	 if (p=="SET") return 0x01;
	 else if (p=="ADD") return 0x02;
	 else if (p=="SUB") return 0x03;
	 else if (p=="MUL") return 0x04;
	 else if (p=="MLI") return 0x05;
	 else if (p=="DIV") return 0x06;
	 else if (p=="DVI") return 0x07;
	 else if (p=="MOD") return 0x08;
	 else if (p=="MDI") return 0x09;
	 else if (p=="AND") return 0x0A;
	 else if (p=="BOR") return 0x0B;
	 else if (p=="XOR") return 0x0C;
	 else if (p=="SHR") return 0x0D;
	 else if (p=="ASR") return 0x0E;   
	 else if (p=="SHL") return 0x0F;
	 else if (p=="IFB") return 0x10;
	 else if (p=="IFC") return 0x11;
	 else if (p=="IFE") return 0x12;
	 else if (p=="IFN") return 0x13;
	 else if (p=="IFG") return 0x14;
	 else if (p=="IFA") return 0x15;
	 else if (p=="IFL") return 0x16;
	 else if (p=="IFU") return 0x17;
	 else if (p=="ADX") return 0x1A;
	 else if (p=="SBX") return 0x1B;
	 else if (p=="STI") return 0x1E;
	 else if (p=="STD") return 0x1F;
	 else
	 {
		return 0;
	 }
}

uint8_t BinAsm::get_sop(const std::string& word, std::string& err)
{
	std::string p = std::transform(word.begin(), 
								   word.end(), 
								   word.begin(), ::toupper);
	if (p=="JSR") return 0x01;
	else if ("HCF") return 0x07;
	else if ("INT") return 0X08;
	else if ("IAG") return 0x09;
	else if ("IAS") return 0x0A;
	else if ("RFI") return 0x0B;
	else if ("IAQ") return 0x0C;
	else if ("HWN") return 0X10;
	else if ("HWQ") return 0x11;
	else if ("HWI") return 0x12;
	else
	{
		err = "unknow operator " + err; 
		return 0;
	}
}

uint8_t BinAsm::get_a(const std::string& word, std::string& err)
{
	std::string p = std::transform(word.begin(), 
								   word.end(), 
								   word.begin(), ::toupper);
	if (p=="JSR") return 0x01;
	else if ("HCF") return 0x07;
	else if ("INT") return 0X08;
	else if ("IAG") return 0x09;
	else if ("IAS") return 0x0A;
	else if ("RFI") return 0x0B;
	else if ("IAQ") return 0x0C;
	else if ("HWN") return 0X10;
	else if ("HWQ") return 0x11;
	else if ("HWI") return 0x12;
	else
	{
		err = "unknow operator " + err; 
		return 0;
	}
}