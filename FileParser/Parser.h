#include <atlstr.h>
#include <vector>
#include <sstream>
#include <iostream>
#include "ReclassClasses.h"
#include <stdlib.h>

void trim(String& s) //http://www.toptip.ca/2010/03/trim-leading-or-trailing-white-spaces.html
{
	size_t p = s.find_first_not_of(" \t");
	s.erase(0, p);
		 
	p = s.find_last_not_of(" \t");
	if (String::npos != p)
		 s.erase(p+1);
}
void Seperate(String str,std::vector<String> &Strings)
{
	std::istringstream ss(str);
	String tempstring;
	while(std::getline(ss,tempstring,' ')) //break input into lines seperated by new line character \n
	{
		Strings.push_back(tempstring); //add to vector
	}
}

class Parser
{
	HWND TxtBoxHwnd;
	int NumberOfLinesInTxtBox;
public:
	Parser(HWND TextBoxHandle)
	{
		TxtBoxHwnd=TextBoxHandle;
	}
	String GetTextAtLine(int line);
	String GetMemberType(int line);
	String GetMemberName(int line);
	String GetClassName(void);
	String GetPtrOrInstanceName(String membername); //get type pointed to or class instance name
	int GetMemberCount(void);
	void GetMemberTypes(std::vector<String> &Types);
	void GetMemberNames(std::vector<String> &Names);
	int GetArraySize(String membername); //get array size of member
};

//Implementations
String Parser::GetTextAtLine(int line)
{
	TCHAR temp[5000];
	int length=GetWindowTextLength(TxtBoxHwnd); //getlength of text
	GetWindowText(TxtBoxHwnd,temp,length); //get text from textbox
	String input=temp;

	std::istringstream ss(input);
	String tempstring;
	std::vector<String> Strings;
	bool inmulti_linecmnt=false; //inside multiline comment
	while(std::getline(ss,tempstring)) //break input into lines seperated by new line character \n
	{
			if(tempstring.find("\r") != std::string::npos) //remove carriage return character
				tempstring.erase(tempstring.find("\r"),2);
			if(tempstring.find("//") != std::string::npos) //remove single line comments
			{
				tempstring.erase(tempstring.find("//"),std::string::npos); //remove from // to end of string
				trim(tempstring);
			}
			if(inmulti_linecmnt) //if we are inside the /*  and */ of multiline comment, look for the end */ or continue searching
			{
				if(tempstring.find("*/") != std::string::npos) //found end of comment
				{
					tempstring.erase(0,tempstring.find("*/")+2); //erase up to end of comment
					inmulti_linecmnt=false;
				}else{//didn't find end yet
					continue; //didn't find end character yet so skip this whole line
				}
			}
			if(tempstring.find("/*") !=std::string::npos && tempstring.find("*/")!=std::string::npos) //single line comment of style /* */
				tempstring.erase(tempstring.find("/*"),tempstring.find("*/")-tempstring.find("/*")+2); //remove from start of comment to end
			if(tempstring.find("/*")!=std::string::npos && tempstring.find("*/")==std::string::npos) //no end character just /*
			{
				inmulti_linecmnt=true;
				tempstring.erase(tempstring.find("/*"),std::string::npos); //remove comment from start of /* to end of line
				trim(tempstring);
			}
			if(tempstring.empty()) //check if line is blank
				continue;			//if line is blank go to next line

			trim(tempstring); //trim begginning and trailing whitespaces
			Strings.push_back(tempstring);//add to vector of valid strings
	}
	NumberOfLinesInTxtBox=Strings.size(); //might as well figure out how many lines of valid stuff we have
	return Strings.at(line); //return string at index
}
String Parser::GetClassName()
{
	// we could just split it, but we'll remove class instead
	// will have to change to split to support inheritance
	String Name=GetTextAtLine(0); //get first line ex: Class GameObject
	Name.erase(Name.find("class"),5); //find start of class and remove it so we just return name, turns into ex: GameObject
	if(Name.find("\r") != std::string::npos)  //need to remove carriage return cuz it messes up stuff
	{
		Name.erase(Name.find("\r"),1);
	}
	trim(Name); //remove trailing and ending whitespaces ex:GameObject
	return Name;
}
String Parser::GetMemberType(int linenum)
{
	String Line=GetTextAtLine(linenum); 
	String oldLine=Line;
	std::vector<String> Substrings;
	Seperate(Line,Substrings);
	Line=Substrings.at(0); //we want the first part of member

	if(Line.find("class") != std::string::npos)
	{
		return "classname";
	}
	if(Line.find("void*")!= std::string::npos){
		if(is64bit)
		{
			return "nt_int64";
		}else{
			printf("32\n");
			return "nt_uint32";
		}
	}
	if((Line.find("*") != std::string::npos) && (Line.find("char*") == std::string::npos)) //check if * is there and also make sure it's not char*
	{
		return "nt_pointer";
	}
	if((Line.find("__int64") != std::string::npos) || (Line.find("INT64")!= std::string::npos))
	{
		return "nt_int64";
	}
	if((Line.find("__int32")!= std::string::npos) || (Line.find("int") != std::string::npos) || (Line.find("INT32")!= std::string::npos) || Line.find("Int32") !=std::string::npos)
	{
		return "nt_int32";
	}
	if((Line.find("__int16")!= std::string::npos) || (Line.find("INT16") != std::string::npos) )
	{
		return "nt_int16";
	}
	if((Line.find("__int8")!= std::string::npos) || (Line.find("INT8")!= std::string::npos))
	{
		return "nt_int8";
	}
	if(Line.find("DWORD")!= std::string::npos)
	{
		return "nt_uint32";
	}
	if(Line.find("WORD")!= std::string::npos)
	{
		return "nt_uint16";
	}
	if(Line.find("BYTE")!= std::string::npos)
	{
		return "nt_uint8";
	}
	if(Line.find("D3DXVECTOR2")!= std::string::npos)
	{
		return "nt_vec2";
	}
	if(Line.find("D3DXVECTOR3") != std::string::npos)
	{
		return "nt_vec3";
	}
	if(Line.find("D3DXVECTOR4") != std::string::npos)
	{
		return "nt_vec4";
	}
	if(Line.find("D3DXMATRIX")!= std::string::npos)
	{
		return "nt_matrix";
	}
	if(Line.find("wchar_t")!= std::string::npos)
	{
		return "nt_unicode";
	}
	if(Line.find("char")!= std::string::npos)
	{
		return "nt_text";
	}
	if(Line.find("float")!= std::string::npos || Line.find("Float32") != std::string::npos)
	{
		return "nt_float";
	}
	if(Line.find("double")!= std::string::npos)
	{
		return "nt_double";
	}
	if(Line.find("char*") != std::string::npos || Line.find("CString")!=std::string::npos)
	{
		return "nt_PChar";
	}
	
	if(Line.find("bool") !=std::string::npos || Line.find("Boolean")!=std::string::npos)
	{
		return "nt_int8";
	}
	if(Line.find("virtual") != std::string::npos)
	{
		return "nt_function";
	}
	if(Line.find("{") != std::string::npos || Line.find("}") !=std::string::npos)
	{
		return "bracket";
	}
	if(Line.find("public:") != std::string::npos)
	{
		return "public";
	}
	if(Line.find("unsigned") !=std::string::npos)
	{
		return "nt_uint32"; //assume 32 bit cuz reclass supports that
	}
	String secondpart=oldLine;
	int left=secondpart.find("[");
	int right=secondpart.find("]"); 
	bool isarray=false;
	if(left !=std::string::npos && right !=std::string::npos && left<right) //check if brackets exist, and that left bracket is before right
	{
		isarray=true;
	}
	for(int i=0;i<Classes.size();i++) //instance checking
	{
		String classname=Classes.at(i)->GetName();
		trim(classname);
		trim(Line);
		if(AreEqual(Line,classname))
		{
			if(isarray)
			{
				return "nt_array";
			}else{
				return "nt_instance";
			}
		}
	}
	return "error";
}
int Parser::GetMemberCount()
{
	int count=0;
	for(int i=0;i<NumberOfLinesInTxtBox;i++)
	{
		String name=GetMemberType(i);
		if(name !="classname" && name != "bracket" && name!="error" && name!="public")
		{
			count++;
		}
	}
	return count;
};
void Parser::GetMemberTypes(std::vector<String> &Types)
{
	int membercount=GetMemberCount();
	for(int i=3;i<membercount+3;i++) //need to account for that skip so -3
	{
		String name=GetMemberType(i);
		if(name!="error")
		{
			Types.push_back(name);
		}
	}
}
String Parser::GetMemberName(int index)
{
	String type=GetMemberType(index);

	String templine=GetTextAtLine(index); //get line of textbox +3 to skip Class,bracket and public
	trim(templine); //remove trailing and beginning spaces
	std::vector<String> SubStrings;
	Seperate(templine,SubStrings); //split line by spaces
	
	String name;
	if(type != "nt_function")
	{
		name=SubStrings.at(1);
		if(name.find(";") != std::string::npos)//is there a semicolon
		{
			name.erase(name.find(";"),std::string::npos); //remove it and all text after it
		}
		if(name.find("[") != std::string::npos && name.find("]")!= std::string::npos && name.find("[")<name.find("]"))
		{
			name.erase(name.find("["),name.find("]")-name.find("[")+1); // find ( then get length by subtracting ) from (
		}
		if(name.find("\r") != std::string::npos)
		{
			name.erase(name.find("\r"),1);
		}
		return name;
	}else if(type=="nt_function"){
		name= SubStrings.at(2);
		if(name.find(";") != std::string::npos)//is there a semicolon
		{
			name.erase(name.find(";"),std::string::npos); //remove it
		}
		if(name.find("(") != std::string::npos && name.find(")")!= std::string::npos && name.find("(")<name.find(")"))
		{
			name.erase(name.find("("),name.find(")")-name.find("(")+1); // find ( then get length by subtracting ) from (
		}
		if(name.find("\r") != std::string::npos)
		{
			name.erase(name.find("\r"),1);
		}
		return name; //we want name not return type
	}

}
void Parser::GetMemberNames(std::vector<String> &Names)
{
	int membercount=GetMemberCount();
	for(int i=3;i<membercount+3;i++) //start at 3 to skip class, the bracket, and public
	{
		String name=GetMemberName(i);
		Names.push_back(name);
	}
}
String Parser::GetPtrOrInstanceName(String membername)
{
	int membercount=GetMemberCount();
	String mname=membername;
	for(int i=3;i<membercount+3;i++) //start at 3 to skip class, the bracket, and public
	{
		String name=GetMemberName(i);
		trim(mname);
		trim(name);
		if(name==mname) //are we at the right member
		{
			String memberline=GetTextAtLine(i);
			trim(memberline);

			std::vector<String> SubStrings;
			Seperate(memberline,SubStrings);
			String returnval=SubStrings.at(0);
			if(returnval.find("*") != std::string::npos)//is there a pointer
			{
				returnval.erase(returnval.find("*"),1);//remove * if it's there
			}
			
			return returnval;
		}
	}
}
int Parser::GetArraySize(String membername)
{
	int membercount=GetMemberCount();
	String mname=membername;
	for(int i=3;i<membercount+3;i++) //start at 3 to skip class, the bracket, and public
	{
		String name=GetMemberName(i);
		trim(mname);
		trim(name);
		if(name==mname) //are we at the right member
		{
			String memberline=GetTextAtLine(i);
			trim(memberline);

			if(memberline.find("[")!= std::string::npos && memberline.find("]")!=std::string::npos)// do we have a start and end bracket
			{
				String number;
				//loop from start bracket to end bracket, also checking string length
				for(int i=memberline.find("[")+1;i<strlen(memberline.c_str()) && i<memberline.find("]");i++)
				{
					number+=memberline.at(i); //build up number character by character
				}
				if(number.find("0x") != std::string::npos)//we are hex
				{
					return std::stol(number.c_str(),nullptr,16);
				}else{
					return std::stol(number.c_str(),nullptr,10); //string to int
				}
			}
		}
	}
	return 4; //if all else fails return 4, this is default char array size in reclass
}


