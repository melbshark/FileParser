class CNodeBase;
class CNodeClass;
#ifndef UNICODE  
typedef std::string String; 
#else
typedef std::wstring String; 
#endif
class CNodeBase; //forward declare for CNodeClass
std::vector<CNodeClass*> Classes;
bool is64bit=true;

//we need different namespaces for enums cuz c++ is stupid
namespace Bit64
{
	enum NodeType
	{
		nt_base,
		nt_instance,
		nt_struct,
		nt_hidden,
		nt_hex32,
		nt_hex64,
		nt_hex16,
		nt_hex8,
		nt_pointer,
		nt_int64,
		nt_int32,
		nt_int16,
		nt_int8,
		nt_float,
		nt_double,
		nt_uint32,
		nt_uint16,
		nt_uint8,
		nt_text,
		nt_unicode,
		nt_function,
		nt_custom,
		nt_vec2,
		nt_vec3,
		nt_quat,
		nt_matrix,
		nt_vtable,
		nt_array,
		nt_class,
		nt_enum
	};
}

namespace Bit32
{
	enum NodeType
	{
		nt_base,
		nt_instance,
		nt_struct,
		nt_hidden,
		nt_hex32,
		nt_hex16,
		nt_hex8,
		nt_pointer,
		nt_int32,
		nt_int16,
		nt_int8,
		nt_float,
		nt_uint32,
		nt_uint16,
		nt_uint8,
		nt_text,
		nt_function,
		nt_custom,
		nt_vec2,
		nt_vec3,
		nt_quat,
		nt_matrix,
		nt_vtable,
		nt_array,
		nt_class,
		nt_enum,
		nt_hex64,
		nt_int64,
		nt_double,
		nt_unicode,
		nt_ptrarray,
		nt_vec4,
	}; 
}

int AreEqual(String str1,String str2) //needed something to compare up to end of both strings, ignores if one line is longer 
{
	bool equal=true;
	for(int i=0;i<str1.length() && i<str2.length();i++)
	{
		if(str1.at(i) != str2.at(i))
		{
			equal=false;
		}
	}
	return equal;
}
class CNodeBase
{
String Type;
String Name;
int size;
public:
	CNodeBase(String type,String name)
	{
		Type=type;
		Name=name;
	}
	String GetType(void);
	void SetType(String);
	String GetName(void);
	void SetName(String);
	void SetSize(int);
	int GetSize(void);
	virtual	void CalculateSize(void);
	int GetReclassType(void);
};
class CNodeClass : public CNodeBase
{	
public:
	CNodeClass():CNodeBase("Class","No Name")
	{

	}
	CNodeClass(String type,String name):CNodeBase(type,name)
	{
		Name=name;
	}
	String Name;
	std::vector<CNodeBase*> Nodes;
	void CalculateSize()
	{
		int size=0;
		for(int i=0;i<Nodes.size();i++)
		{
			Nodes.at(i)->CalculateSize();
			size+=Nodes.at(i)->GetSize();
		}
		SetSize(size);
	}
};

String CNodeBase::GetType()
{
	return Type;
}
void CNodeBase::SetType(String type)
{
	Type=type;
}
String CNodeBase::GetName()
{
	return Name;
}
void CNodeBase::SetName(String name)
{
	Name=name;
}
void CNodeBase::SetSize(int sz)
{
	size=sz;
}
int CNodeBase::GetSize()
{
	return size;
}
void CNodeBase::CalculateSize()
{
	String type=GetType(); 
	if(AreEqual(type,"nt_uint8"))
	{
		size=1;
	}
	if(AreEqual(type,"nt_uint16"))
	{
		size=2;
	}
	if(AreEqual(type,"nt_uint32"))
	{
		size=4;
	}
	if(AreEqual(type,"nt_function"))
	{
		size=8;
	}
	if(AreEqual(type,"nt_int8"))
	{
		size=1;
	}
	if(AreEqual(type,"nt_int16"))
	{
		size=2;
	}
	if(AreEqual(type,"nt_int32"))
	{
		size=4;
	}
	if(AreEqual(type,"nt_int64"))
	{
		size=8;
	}
	if(AreEqual(type,"nt_pointer"))
	{
		if(is64bit)
		{
			size=8;
		}else{
			size=4;
		}
	}
	if(AreEqual(type,"nt_PChar"))
	{
		size=8;
	}
	if(AreEqual(type,"nt_double"))
	{
		size=8;
	}
	if(AreEqual(type,"nt_float"))
	{
		size=4;
	}
	if(AreEqual(type,"nt_matrix"))
	{
		size=4*4*4;
	}
	if(AreEqual(type,"nt_vec4"))
	{
		size=16;
	}
	if(AreEqual(type,"nt_vec3"))
	{
		size=12;
	}
	if(AreEqual(type,"nt_vec2"))
	{
		size=8;
	}
}
int CNodeBase::GetReclassType()
{
	String type=GetType(); 
	if(is64bit)
	{
		if(AreEqual(type,"Class"))
		{
			return Bit64::nt_class;
		}
		if(AreEqual(type,"nt_uint8"))
		{
			return Bit64::nt_uint8;
		}
		if(AreEqual(type,"nt_uint16"))
		{
			return Bit64::nt_uint16;
		}
		if(AreEqual(type,"nt_uint32"))
		{
			return Bit64::nt_uint32;
		}
		if(AreEqual(type,"nt_function"))
		{
			return Bit64::nt_function;
		}
		if(AreEqual(type,"nt_int8"))
		{
			return Bit64::nt_int8;
		}
		if(AreEqual(type,"nt_int16"))
		{
			return Bit64::nt_int16;
		}
		if(AreEqual(type,"nt_int32"))
		{
			return Bit64::nt_int32;
		}
		if(AreEqual(type,"nt_int64"))
		{
			return Bit64::nt_int64;
		}
		if(AreEqual(type,"nt_pointer"))
		{
			return Bit64::nt_pointer;
		}
		if(AreEqual(type,"nt_PChar"))
		{
			return Bit64::nt_text;
		}
		if(AreEqual(type,"nt_double"))
		{
			return Bit64::nt_double;
		}
		if(AreEqual(type,"nt_float"))
		{
			return Bit64::nt_float;
		}
		if(AreEqual(type, "nt_unicode"))
		{
			return Bit64::nt_unicode;
		}
		if(AreEqual(type,"nt_text"))
		{
			return Bit64::nt_text;
		}
		if(AreEqual(type,"nt_matrix"))
		{
			return Bit64::nt_matrix;
		}
		if(AreEqual(type,"nt_vec4"))
		{
			return Bit64::nt_quat;
		}
		if(AreEqual(type,"nt_vec3"))
		{
			return Bit64::nt_vec3;
		}
		if(AreEqual(type,"nt_vec2"))
		{
			return Bit64::nt_vec2;
		}
		if(AreEqual(type,"nt_instance"))
		{
			return Bit64::nt_instance;
		}
		if(AreEqual(type,"nt_array"))
		{
			return Bit64::nt_array;
		}
		if(AreEqual(type,"nt_vtable"))
		{
			return Bit64::nt_vtable;
		}
	}else{ //switch to 32 bit indexes
		if(AreEqual(type,"Class"))
		{
			return Bit32::nt_class;
		}
		if(AreEqual(type,"nt_uint8"))
		{
			return Bit32::nt_uint8;
		}
		if(AreEqual(type,"nt_uint16"))
		{
			return Bit32::nt_uint16;
		}
		if(AreEqual(type,"nt_uint32"))
		{
			return Bit32::nt_uint32;
		}
		if(AreEqual(type,"nt_function"))
		{
			return Bit32::nt_function;
		}
		if(AreEqual(type,"nt_int8"))
		{
			return Bit32::nt_int8;
		}
		if(AreEqual(type,"nt_int16"))
		{
			return Bit32::nt_int16;
		}
		if(AreEqual(type,"nt_int32"))
		{
			return Bit32::nt_int32;
		}
		if(AreEqual(type,"nt_int64"))
		{
			return Bit32::nt_int64;
		}
		if(AreEqual(type,"nt_pointer"))
		{
			return Bit32::nt_pointer;
		}
		if(AreEqual(type,"nt_PChar"))
		{
			return Bit32::nt_text;
		}
		if(AreEqual(type,"nt_double"))
		{
			return Bit32::nt_double;
		}
		if(AreEqual(type,"nt_float"))
		{
			return Bit32::nt_float;
		}
		if(AreEqual(type, "nt_unicode"))
		{
			return Bit32::nt_unicode;
		}
		if(AreEqual(type,"nt_text"))
		{
			return Bit32::nt_text;
		}
		if(AreEqual(type,"nt_matrix"))
		{
			return Bit32::nt_matrix;
		}
		if(AreEqual(type,"nt_vec4"))
		{
			return Bit32::nt_quat;
		}
		if(AreEqual(type,"nt_vec3"))
		{
			return Bit32::nt_vec3;
		}
		if(AreEqual(type,"nt_vec2"))
		{
			return Bit32::nt_vec2;
		}
		if(AreEqual(type,"nt_instance"))
		{
			return Bit32::nt_instance;
		}
		if(AreEqual(type,"nt_array"))
		{
			return Bit32::nt_array;
		}
		if(AreEqual(type,"nt_vtable"))
		{
			return Bit32::nt_vtable;
		}
	}
}


class CNodePtr: public CNodeBase
{
public:
	CNodePtr(String type,String name):CNodeBase(type,name)
	{

	}
	CNodeBase* pNode;
	void CalculateSize();
};
void CNodePtr::CalculateSize()
{
	SetSize(8);
}
class CNodeArray: public CNodeBase
{
public:
	CNodeArray(String type,String name): CNodeBase(type,name)
	{
		elements=1;
	}
	CNodeBase* pNode;
	void CalculateSize();
	void SetElements(int num);
	int GetElements(void);
	int elements;
};
void CNodeArray::CalculateSize()
{
	SetSize(pNode->GetSize()*elements);
	printf("Node Size:%d",pNode->GetSize());
}
void CNodeArray::SetElements(int num)
{
	elements=num;
}
int CNodeArray::GetElements(void)
{
	return elements;
}

class CNodeClassInstance: public CNodeBase
{
public:
	CNodeClassInstance(String type,String name): CNodeBase(type,name)
	{

	}
	CNodeClass* pNode;
	void CalculateSize();
};
void CNodeClassInstance::CalculateSize()
{
	SetSize(pNode->GetSize());
}
class CNodeVTable: public CNodeBase
{
public:
	CNodeVTable(String type,String name): CNodeBase(type,name) //call base constructor
	{
		
	}
	std::vector<CNodeBase*>Nodes;
	void CalculateSize();
};
void CNodeVTable::CalculateSize()
{
	if(is64bit)
	{
		SetSize(8);
	}else{
		SetSize(4);
	}
	
}

class CNodeText:public CNodeBase
{
public:
	CNodeText(String type,String name):CNodeBase(type,name)
	{
		elements=4;
	}
	CNodeText(String type,String name,int arrayelements):CNodeBase(type,name)
	{
		elements=arrayelements;
	}
	int elements;
	void CalculateSize();
};
void CNodeText::CalculateSize()
{
	SetSize(elements*1); //1 is size, just put it there to remember size
}

class CNodeUnicode:public CNodeBase
{
public:
	CNodeUnicode(String type,String name):CNodeBase(type,name)
	{
		elements=4;
	}
	CNodeUnicode(String type,String name,int arrayelements):CNodeBase(type,name)
	{
		elements=arrayelements;
	}
	int elements;
	void CalculateSize();
};
void CNodeUnicode::CalculateSize()
{
	SetSize(elements*2); //2 is size
}
