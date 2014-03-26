// FileParser.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "FileParser.h"
#include <windows.h>
#include "Parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include "..\tinyxml\tinystr.cpp"
#include "..\tinyxml\tinyxml.cpp"
#include "..\tinyxml\tinyxmlerror.cpp"
#include "..\tinyxml\tinyxmlparser.cpp"
#include <ShlObj.h>

#define MAX_LOADSTRING 100

VOID CreateConsole( )
{
	int hConHandle = 0; 
	HANDLE lStdHandle = 0;
	FILE *fp = 0;
	AllocConsole( );
	lStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	hConHandle = _open_osfhandle( PtrToUlong( lStdHandle ), _O_TEXT );
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
}  
HWND hWnd;
HWND TextboxHwnd;
HWND button,button2,button3,button4,button5;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
#define ID_ADDCLASS 5
#define ID_PRINTCLASS 6
#define ID_PRINTCLASSWPTRS 7
#define ID_SWITCHBIT 8
#define ID_SAVECLASS 9
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void Parse();
void Print(CNodeClass*,bool);
void savetoFile();
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FILEPARSER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILEPARSER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILEPARSER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FILEPARSER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   TextboxHwnd=CreateWindowExW(WS_EX_CLIENTEDGE,L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER |ES_MULTILINE | WS_VSCROLL, 100, 20, 700, 500, hWnd, NULL, NULL, NULL);
   button = CreateWindowExW(WS_EX_CLIENTEDGE,L"Button",L"Add Class",
	   BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,800,20,100,25,hWnd,(HMENU)ID_ADDCLASS,hInst,0);
   
   button2 = CreateWindowExW(WS_EX_CLIENTEDGE,L"Button",L"Print Classes",
	   BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,800,100,100,25,hWnd,(HMENU)ID_PRINTCLASS,hInst,0);

   button3 = CreateWindowExW(WS_EX_CLIENTEDGE,L"Button",L"Print Classes and Show Ptrs",
	   BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,800,180,160,25,hWnd,(HMENU)ID_PRINTCLASSWPTRS,hInst,0);

   button4 = CreateWindowExW(WS_EX_CLIENTEDGE,L"Button",L"64 Bit",
	   BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,800,260,100,25,hWnd,(HMENU)ID_SWITCHBIT,hInst,0);

   button5 = CreateWindowExW(WS_EX_CLIENTEDGE,L"Button",L"Save to File",
	   BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE ,800,340,100,25,hWnd,(HMENU)ID_SAVECLASS,hInst,0);

   NONCLIENTMETRICS ncm;
   ncm.cbSize = sizeof(ncm);
   SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
   HFONT hNewFont = CreateFontIndirect(&ncm.lfMessageFont);
   SendMessageW(TextboxHwnd, WM_SETFONT, (WPARAM)hNewFont, 1);
   SendMessageW(button, WM_SETFONT, (WPARAM)hNewFont, 1);
   SendMessageW(button2, WM_SETFONT, (WPARAM)hNewFont, 1);
   SendMessageW(button3, WM_SETFONT, (WPARAM)hNewFont, 1);
   SendMessageW(button4, WM_SETFONT, (WPARAM)hNewFont, 1);
   SendMessageW(button5, WM_SETFONT, (WPARAM)hNewFont, 1);
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   CreateConsole();
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_ADDCLASS:
			__try{
				Parse();
				char output[500];
				sprintf_s(output,"Add Class:%d ",Classes.size());
				SetWindowText(button,output);
			}__except(1){
				printf(" Error parsing file, please check formatting \n");
			}
			break;
		case  ID_PRINTCLASS:
			__try{
				for(int i=0;i<Classes.size();i++)
				{
					Print(Classes.at(i),false);
				}
			}__except(1){
				printf(" Error printing classes, this shouldn't happen \n");
			}
			break;
		case  ID_PRINTCLASSWPTRS:
			__try{
				for(int i=0;i<Classes.size();i++)
				{
					Print(Classes.at(i),true);
				}
			}__except(1){
				printf(" Error printing classes with ptrs, this shouldn't happen \n");
			}
			break;
		case ID_SWITCHBIT:
			is64bit=!is64bit;

			char output2[500];
			sprintf_s(output2,"%s Bit",is64bit? "64":"32");
			SetWindowText(button4,output2);
			break;
		case ID_SAVECLASS:
			__try{
				savetoFile();
				printf("Saved to File\n");
			}__except(1){
				printf("Failed to Save to File\n");
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Parse()
{
	int count=0;
	Parser parser=Parser(TextboxHwnd);
	parser.GetTextAtLine(0);
	count=parser.GetMemberCount();
	/*std::cout<<"Members: "<<count<<"\n";*/

	CNodeClass* Class =new CNodeClass();
	String name=parser.GetClassNameA();
	trim(name);
	Class->SetName(name);

	std::vector<String> Types; //get all member types and build array
	parser.GetMemberTypes(Types);

	std::vector<String> Names; //get all member name and build array
	parser.GetMemberNames(Names);
	bool insideVtable=false;
	for(int i=0;i<parser.GetMemberCount();i++) //loop through members
	{
		String type=Types.at(i);
		String name=Names.at(i);

		if(type =="nt_function") //are we a virtual function
		{
			if(insideVtable) //yea we are a virtual function, now is the vtable created already
			{
				CNodeVTable* pVtable=(CNodeVTable*)Class->Nodes.back(); //yup vtable is made so get it, it's the last element in array
				CNodeBase* pBase=new CNodeBase(type,name); //create new vfunction
				pVtable->Nodes.push_back(pBase);//add our vfunction to member array of vtable
			}else{                                                     //no vtable not made yet, so we make it
				CNodeVTable* pVtable=new CNodeVTable("nt_vtable","VTable");
				CNodeBase* pBase=new CNodeBase(type,name); //we made the vtable but we still have to make the current vfunction a member of it
				pVtable->Nodes.push_back(pBase); //add to member arrary of vtable
				Class->Nodes.push_back(pVtable); //add array to member array of class
				insideVtable=true; //tell program future vfunctions go into this vtable
			}
		}else if(type=="nt_pointer"){
			if(insideVtable)
				insideVtable=false;
			String pointedtoname=parser.GetPtrOrInstanceName(name);//get name of class being pointed to
			trim(pointedtoname);
			printf("Pointed to type:%s \n",pointedtoname.c_str());
			CNodePtr* pNode=new CNodePtr(type,name);               //create pointer class
			CNodeClass* pClass=new CNodeClass("nt_class",pointedtoname);
			for(int i=0;i<Classes.size();i++)             //loop through all existing classes         
			{
				String classname=Classes.at(i)->GetName();
				trim(classname);
				if(pointedtoname==classname)//check if class exists, if so set pclass equal to found class
				{
					pClass=Classes.at(i);
				}
			}
			pNode->pNode=pClass; //tell cnode it points to this class
			Class->Nodes.push_back(pNode); //add as member to class object
		}else if(type=="nt_instance"){
			if(insideVtable)
				insideVtable=false;
			String instancename=parser.GetPtrOrInstanceName(name);//get name of class being pointed to
			trim(instancename);
		
			CNodeClassInstance* pNode=new CNodeClassInstance(type,name);
			CNodeClass* pClass=new CNodeClass("nt_class",instancename);
			for(int i=0;i<Classes.size();i++)      //loop through all existing classes                
			{
				String classname=Classes.at(i)->GetName();
				trim(classname);
				if(instancename==classname)//check if class exists, if so set pclass equal to found class
				{
					pClass=Classes.at(i);
				}
			}
			pNode->pNode=pClass;
			Class->Nodes.push_back(pNode);
		}else if(type=="nt_array"){ //are we an array
			if(insideVtable)
				insideVtable=false;
			String instancename=parser.GetPtrOrInstanceName(name);//get name of class being pointed to
			trim(instancename);
			printf("Instance type:%s \n",instancename.c_str());
			CNodeArray* pNode=new CNodeArray(type,name); //create array object
			CNodeClass* pClass=new CNodeClass("nt_class",instancename); //create default class, incase no existing one is found
			for(int i=0;i<Classes.size();i++)      //loop through all existing classes                
			{
				String classname=Classes.at(i)->GetName(); //get existing classes name
				trim(classname);
				if(instancename==classname)//check if class is equal to instance name, if so set pclass equal to found class
				{
					pClass=Classes.at(i);
				}
			}
			pNode->SetElements(parser.GetArraySize(name)); //set number of elements
			pNode->pNode=pClass;
			Class->Nodes.push_back(pNode);
		}else if(type=="nt_text"){
			if(insideVtable)
				insideVtable=false;
			CNodeText* pNode=new CNodeText(type,name,parser.GetArraySize(name));
			Class->Nodes.push_back(pNode);
		}else if(type=="nt_unicode"){
			if(insideVtable)
				insideVtable=false;

			CNodeUnicode* pNode=new CNodeUnicode(type,name,parser.GetArraySize(name));
			Class->Nodes.push_back(pNode);
		}else{
			if(insideVtable)
				insideVtable=false;
			CNodeBase* pNode=new CNodeBase(type,name); //create basic type
			Class->Nodes.push_back(pNode);
		}
	}
	Class->CalculateSize();
	Classes.push_back(Class); //add parsed class to array of classes
	return;
}
void Print(CNodeClass* Class,bool printpointedto)
{
	printf("Class Name:%s \n",Class->GetName().c_str());
	for(int i=0;i<Class->Nodes.size();i++)
	{
		CNodeBase* pNode=Class->Nodes.at(i);
		String type=pNode->GetType();
		trim(type);
		if(type == "nt_vtable")
		{
			printf("Member Type:%s   MemberName:%s \n",pNode->GetType().c_str(),pNode->GetName().c_str());
			CNodeVTable* pVtable=(CNodeVTable*)pNode;
			for(int i=0;i<pVtable->Nodes.size();i++)
			{
				printf("     -VFunct Type:%s   VFunc name:%s \n",pVtable->Nodes.at(i)->GetType().c_str(),pVtable->Nodes.at(i)->GetName().c_str());
			}
		}else if(type=="nt_pointer"){
			CNodePtr* pPtr=(CNodePtr*)pNode;
			printf("Member Type:%s  MemberName:%s Pointer to:%s\n",pPtr->GetType().c_str(),pPtr->GetName().c_str(),pPtr->pNode->GetName().c_str());
			CNodeClass* pClass=(CNodeClass*)pPtr->pNode;
			if(printpointedto)
			{
				printf(" Pointer to-> \n\n");
				Print(pClass,true); //recursion careful
			}
		}else if(type=="nt_instance"){
			CNodeClassInstance* pInst=(CNodeClassInstance*)pNode;
			printf("Member Type:%s  MemberName:%s \n",pInst->GetType().c_str(),pInst->GetName().c_str());
			CNodeClass* pClass=(CNodeClass*)pInst->pNode;
			
			if(printpointedto)
			{
				printf(" Instance of:\n\n");
				Print(pClass,true); //again recursion careful
			}
		}else if(type=="nt_array"){
			CNodeArray* pArray=(CNodeArray*)pNode;
			printf("Member Type:%s  MemberName:%s Of Type:%s \n",pArray->GetType().c_str(),pArray->GetName().c_str(),pArray->pNode->GetName().c_str());
			CNodeClass* pClass=(CNodeClass*)pArray->pNode;

			if(printpointedto)
			{
				printf("Array of Class: \n\n");
				Print(pClass,true); //again recursion careful
			}
		}else{
			printf("Member Type:%s   MemberName:%s \n",pNode->GetType().c_str(),pNode->GetName().c_str());
		}
	}
	printf("\n");
}
void savetoFile()
{
	CString tdHex("char");
	CString tdInt64("__int64");
	CString tdInt32("__int32");
	CString tdInt16("__int16");
	CString tdInt8("__int8");
	CString tdDWORD("DWORD");
	CString tdWORD("WORD");
	CString tdBYTE("BYTE");
	CString tdVec2("D3DXVECTOR2");
	CString tdVec3("D3DXVECTOR3");
	CString tdQuat("D3DXQUATERNION");
	CString tdMatrix("D3DXMATRIX");

	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement( "ReClass" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue("ReClass by DrUnKeN ChEeTah, DogMatt, Pat,and ICY. Parsed with Reclass-Parser by Stevemk14ebr" );  
	root->LinkEndChild( comment );  
	//---------------------------------------------
	TiXmlElement* settings;
	settings = new TiXmlElement("TypeDef");
	settings->SetAttribute("tdHex",tdHex);
	settings->SetAttribute("tdInt32",tdInt32);
	settings->SetAttribute("tdInt16",tdInt16);
	settings->SetAttribute("tdInt8",tdInt8);
	settings->SetAttribute("tdDWORD",tdDWORD);
	settings->SetAttribute("tdWORD",tdWORD);
	settings->SetAttribute("tdBYTE",tdBYTE);
	settings->SetAttribute("tdVec2",tdVec2);
	settings->SetAttribute("tdVec3",tdVec3);
	settings->SetAttribute("tdQuat",tdQuat);
	settings->SetAttribute("tdMatrix",tdMatrix);
	root->LinkEndChild(settings);

	settings = new TiXmlElement("Header");
	settings->SetAttribute("Text","");
	root->LinkEndChild(settings);
	settings = new TiXmlElement("Footer");
	settings->SetAttribute("Text","");
	root->LinkEndChild(settings);
	settings = new TiXmlElement("Notes");
	settings->SetAttribute("Text","");
	root->LinkEndChild(settings);

	for(int i=0;i<Classes.size();i++)
	{
		CNodeClass* pClass=Classes.at(i);

		TiXmlElement* classnode=new TiXmlElement("Class");
		classnode->SetAttribute("Name",pClass->GetName().c_str());
		classnode->SetAttribute("Type",pClass->GetReclassType());
		classnode->SetAttribute("Comment","");
		classnode->SetAttribute("Offset",is64bit ? 140000000:14000000);
		classnode->SetAttribute("strOffset","");
		classnode->SetAttribute("Code","");
		root->LinkEndChild(classnode);

		for(int n=0;n<pClass->Nodes.size();n++)
		{
			CNodeBase* pNode=pClass->Nodes.at(n);
			printf("Type:%s Type2:%d\n",pNode->GetType().c_str(),pNode->GetReclassType());

			TiXmlElement* node=new TiXmlElement("Node");
			node->SetAttribute("Name",pNode->GetName().c_str());
			node->SetAttribute("Type",pNode->GetReclassType());
			node->SetAttribute("Size",pNode->GetSize());
			node->SetAttribute("bHidden",FALSE);
			node->SetAttribute("Comment","");
			classnode->LinkEndChild(node);
			if(pNode->GetType()=="nt_array")
			{
				CNodeArray* pptr=(CNodeArray*)pNode;
				node->SetAttribute("Total",pptr->GetElements());

				TiXmlElement* item=new TiXmlElement("Array");
				item->SetAttribute("Name",pptr->pNode->GetName().c_str());
				item->SetAttribute("Type",pptr->pNode->GetReclassType());
				item->SetAttribute("Size",pptr->pNode->GetSize());
				item->SetAttribute("Comment","");
				node->LinkEndChild(item);
			}
			if(pNode->GetType()== "nt_pointer")
			{
				CNodePtr* pptr=(CNodePtr*)pNode;
				node->SetAttribute("Pointer",pptr->pNode->GetName().c_str());
			}
			if(pNode->GetType()=="nt_vtable")
			{
				CNodeVTable* pVTable=(CNodeVTable*)pNode;
				for(int i=0;i<pVTable->Nodes.size();i++)
				{
					CNodeBase* pFunc=(CNodeBase*)pVTable->Nodes.at(i);
					TiXmlElement* fun=new TiXmlElement("Function");
					fun->SetAttribute("Name",pFunc->GetName().c_str());
					fun->SetAttribute("Comment","");
					fun->SetAttribute("bHidden",FALSE);
					node->LinkEndChild(fun);
				}
			}
			if(pNode->GetType()=="nt_instance")
			{
				CNodeClassInstance* pptr=(CNodeClassInstance*)pNode;
				node->SetAttribute("Instance",pptr->pNode->GetName().c_str());
			}
		}
	}
	char path[MAX_PATH];
	SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,0,NULL,path); //get desktop folder
	strcat_s(path,"\\ReclassParser.reclass");//add Sigs.text to path
	doc.SaveFile(path);
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
		break;
	
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
