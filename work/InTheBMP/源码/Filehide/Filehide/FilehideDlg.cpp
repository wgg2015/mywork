
// FilehideDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Filehide.h"
#include "FilehideDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////位图文件头数据结构（14个字节）
typedef struct
{
	short bfType;//表明位图文件的类型，必为BM（0x424D）
	short bfSize[2];//表明位图文件的大小，以字节为主
	short bfReserved1;//保留字，为0
	short bfReserved2;//保留字，为0
	short bfOFFBits[2];//位图阵列的起始位置
}BITMAP_FILE_HEADER;

////////////////////////////////////////////位图信息头数据结构（40个字节）
typedef struct
{
	long biSize;//指出本数据结构所需要的字节数
	long biWidth;//以像素为单位，给出BMP图像的宽度
	long biHeight;//意像素为单位，给出BMP图像的高度
	short biPlanes;//输出设备的为平面数，必须位1
	short biBitCount;//给出每个像素的位数，1、4、8、24
	long biCompress;//给出位图的压缩类型
	long biSizeImage;//给出图像字节数的多少
	long biXPelsPerMeter;//图像的水平 分辨率
	long biYelsPerMeter;//图像的垂直分辨率
	long biClrUsed;//调色板中图像实际使用的颜色数
	long biClrImportant;//给出重要颜色的索引值
}BITMAP_INFO_HEADER;

///////////////////////////////////////////位图数据结构（4个字节）
typedef struct
{
	unsigned char rgbBlue;//蓝
	unsigned char rgbGreen;//绿
	unsigned char rgbRed;//红
	unsigned char rgbReserved;//保留

}BITMAP_COLOR;

//////////////////////////////////////////位图信息数据结构（包好位图信息头与位图）
typedef struct
{
      BITMAP_INFO_HEADER bmiHeader;
	  BITMAP_COLOR       bmiClor[];
}BITMAP_INFO;
///////////////////////////////////////隐藏到文件的位置(每两个像素可以隐藏一个字节)
typedef struct 
{
    unsigned char  BHide1:1;
	unsigned char  :7;
    unsigned char  GHide1:2;
    unsigned char  :6; 
    unsigned char  RHide1:1;
	unsigned char  :7;
    unsigned char  :8;

	unsigned char  BHide2:1;
	unsigned char  :7;
    unsigned char  GHide2:2;
    unsigned char  :6; 
    unsigned char  RHide2:1;
	unsigned char  :7;
    unsigned char  :8;
} BIT_HIDE;
/////////////////////////////////////要隐藏的数据
typedef struct
{
	unsigned char Date1:1;
	unsigned char Date2:2;
	unsigned char Date3:1;
	unsigned char Date4:1;
	unsigned char Date5:2;
	unsigned char Date6:1;
}NewDate;

// CFilehideDlg 对话框
#pragma region 全局变量
CString FilePathName;//路径文件名
CString HFilePathName;
CString FilePath;//路径
CString HfilePath;
CString FileName;//文件名
CString FileNameN;
CString HFileName;
CString HFileNameN;

CString Info;//要打印的提示信息
	//wchar_t *FPN=NULL;
FILE *BmpFile=NULL;//文件句柄
FILE *HFile=NULL;
FILE *NewBmpFile=NULL;
FILE *NewHFile=NULL;
unsigned char *BmpFileBuffer=NULL;//bmp文件数据缓冲区
unsigned char *HFileBuffer=NULL;//文件数据缓冲区
unsigned char *Point=NULL;//文件指针
unsigned char *HPoint=NULL;
int BmpLen;//原始bmp文件长度
int HFileLen;//文件数据长度
int HFileNameLen;//文件名长度
long BmpSizeColor;//像素的字节数
long HFileMaxSize;//要插入的文件大小上限
BITMAP_FILE_HEADER *bmpFH;//结构话bmp头
BITMAP_INFO *bitmap_info;//结构化位图信息
CFont font;//字体
#pragma endregion

char* CharToCString(CString cs)
{
int nLength = cs.GetLength();
int nBytes = WideCharToMultiByte(CP_ACP,0,cs,nLength,NULL,0,NULL,NULL);
char* ch = new char[ nBytes + 1];
memset(ch,0,nLength + 1);
WideCharToMultiByte(CP_OEMCP, 0, cs, nLength, ch, nBytes, NULL, NULL);
ch[nBytes] = 0;
return ch;
//return cs.GetString();
}
CFilehideDlg::CFilehideDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilehideDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFilehideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFilehideDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFilehideDlg::OnBnClickedOk)
	ON_BN_CLICKED(Btn_Close, &CFilehideDlg::OnBnClickedClose)
	ON_BN_CLICKED(Btn_Open_BMP, &CFilehideDlg::OnBnClickedOpenBmp)
	ON_BN_CLICKED(Btn_Insert_File, &CFilehideDlg::OnBnClickedInsertFile)
	ON_BN_CLICKED(Btn_Release_File, &CFilehideDlg::OnBnClickedReleaseFile)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CFilehideDlg 消息处理程序

BOOL CFilehideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFilehideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		LOGFONT _logfont;
memset(&_logfont, 0, sizeof(_logfont));
_logfont.lfHeight = 18;
_logfont.lfWidth  =  0;
_logfont.lfWeight = 400;
//_logfont.lfItalic=true;
_logfont.lfQuality = DEFAULT_QUALITY ;
//_logfont.lfUnderline=true;
_logfont.lfPitchAndFamily = FF_SCRIPT ;
_tcscpy(_logfont.lfFaceName, _T("宋体"));
//memset(&_logfont, 0, sizeof(_logfont));

font.CreateFontIndirect(&_logfont);
GetDlgItem(Edit_Info)->SetFont(&font, FALSE);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
font.DeleteObject();
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint();
		
	}
	  #pragma region 设置字体

#pragma endregion
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFilehideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFilehideDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

  // AfxMessageBox(L"hello");
}


void CFilehideDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	//FreeConsole();
	 free(BmpFileBuffer);
	 free(HFileBuffer);
	CDialogEx::OnOK();
}


void CFilehideDlg::OnBnClickedOpenBmp()
{
	
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(LPCTSTR)_TEXT("BMP Files|*.bmp|"),NULL);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	
    if(dlg.DoModal()==IDOK)
   { 
	   FilePathName=dlg.GetPathName();
       FileName=FilePathName.Mid(FilePathName.ReverseFind( '\\')+1);
	   FilePath=FilePathName.Left(FilePathName.ReverseFind( '\\')+1);
	   FileNameN=FileName.Left(FileName.ReverseFind('.'));
	   if(FileName.ReverseFind('.')==-1) FileNameN=FileName;
	   BmpFile=_tfopen(FilePathName,_T("rb"));
	   if(BmpFile==NULL)
	   { Info=L"文件载入失败！";AddInfo(Info);fclose(BmpFile);}
	   else
	   {
		   Info=L"载入bmp文件:"+FilePathName;AddInfo(Info);

		   fseek(BmpFile, 0, SEEK_END);
	       size_t len = ftell(BmpFile);//获取文件大小
		   BmpLen=len;
	       fseek(BmpFile, 0, SEEK_SET);
		   BmpFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*len);
		   fread(BmpFileBuffer,sizeof(unsigned char),len,BmpFile);
		   BmpFile==NULL;
		   fclose(BmpFile);

		   Point=BmpFileBuffer;

		   bmpFH=(BITMAP_FILE_HEADER*)Point;
		   if(bmpFH->bfType!=0x4d42)
		   {
			   Info=L"无效的bmp文件！";
			   AddInfo(Info);
			   BmpFileBuffer=NULL;
			   BmpFile=NULL;
			   
		   }
		   else
		   {
		   //short s=*(int *)(&bmpFH->bfReserved1);
		      if(*(int *)(&bmpFH->bfReserved1)!=0)
		      {
			     Info=L"检测到有数据隐藏在bmp文件中,可以释放隐藏文件";
			     AddInfo(Info);
		      }
			  Point=Point+0x0E;
			
			  bitmap_info=(BITMAP_INFO*)Point;
	          BmpSizeColor=bitmap_info->bmiHeader.biSizeImage;  
		   }
		  

	   }
	}
	free(dlg);
	//else{AfxMessageBox(L"文件打开失败");}

}

void CFilehideDlg::AddInfo(CString NewInfo)
{
	CString Info=NULL;
	GetDlgItem(Edit_Info)->GetWindowText(Info);
	Info+=(L"\r\n"+NewInfo+L"……");
	GetDlgItem(Edit_Info)->SetWindowText(Info);
}

void CFilehideDlg::OnBnClickedInsertFile()
{

	if(BmpFile==NULL)
	{
		Info=L"应先载入bmp文件！";
		AddInfo(Info);
	}
	else
	{
		CFileDialog dlg(true);
		if(dlg.DoModal()==IDOK)
		{
			HFilePathName=dlg.GetPathName();
            HFileName=HFilePathName.Mid(HFilePathName.ReverseFind( '\\')+1);
			HFileNameN=HFileName.Left(HFileName.ReverseFind('.'));
			if(HFileName.ReverseFind('.')==-1) HFileNameN=HFileName;
			HFile=_tfopen(HFilePathName,_T("rb"));
			fseek(HFile, 0, SEEK_END);
	        size_t len = ftell(HFile);//获取文件大小
	        fseek(HFile, 0, SEEK_SET);

		    HFileNameLen= WideCharToMultiByte(CP_ACP,0,HFileName,HFileName.GetLength(),NULL,0,NULL,NULL);//获取文件名的大小
			HFileMaxSize=BmpSizeColor/8-HFileNameLen;
			//printf("HFileNameLen=%d\n",HFileNameLen);
			//printf("BmpSizeColor=%d\n",BmpSizeColor);
			if(len>HFileMaxSize)
			{
				Info=L"文件过大，超出了bmp文件所能容纳的数据！";
				AddInfo(Info);
				fclose(HFile);
				HFile=NULL;
			}
			else
			{
				Info=L"载入要隐藏的文件:"+HFilePathName;AddInfo(Info);
				HFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*len);
				fread(HFileBuffer,sizeof(unsigned char),len,HFile);
				fclose(HFile);
				
		      // printf("%d",len);
				*((int *)&bmpFH->bfReserved1)=len;//将文件大小写入保留字段
				*(((unsigned char *)&bmpFH->bfReserved2)+1)=HFileNameLen;//将文件名的大小写入保留字段最后一个字节
#pragma region 处理数据
				HPoint=HFileBuffer;
				BIT_HIDE *bit_hide=(BIT_HIDE*)bitmap_info->bmiClor;//格式化要隐藏的数据在母版中的位置
		        NewDate *newdate=(NewDate*)HPoint;//格式化要隐藏的数据
		        for(int i=0;i<len;i++)//将文件数据写入bmp文件
		        {
			       (bit_hide)->BHide1=(newdate)->Date1;
		           (bit_hide)->GHide1=(newdate)->Date2;
		           (bit_hide)->RHide1=(newdate)->Date3;
		           (bit_hide)->BHide2=(newdate)->Date4;
		           (bit_hide)->GHide2=(newdate)->Date5;
		           (bit_hide++)->RHide2=(newdate++)->Date6;
		        }
				
				//NewDate *namedate=(NewDate*)&HFileName;
				char *CHFN=CharToCString(HFileName);
				NewDate *namedate=(NewDate*)CHFN;
				HFileName.Format(_T("%s"), CStringW(CHFN));
				//AfxMessageBox(HFileName);
				for(int i=0;i<HFileNameLen;i++)//把文件名写入bmp文件
				{
				   (bit_hide)->BHide1=(namedate)->Date1;
		           (bit_hide)->GHide1=(namedate)->Date2;
		           (bit_hide)->RHide1=(namedate)->Date3;
		           (bit_hide)->BHide2=(namedate)->Date4;
		           (bit_hide)->GHide2=(namedate)->Date5;
		           (bit_hide++)->RHide2=(namedate++)->Date6;
				}
#pragma endregion
#pragma region 生成新文件
				NewBmpFile=_tfopen(FilePath+FileNameN+L"-"+HFileNameN+L".bmp",L"wb+");
				if(NewBmpFile==NULL)
				{Info=L"文件创建失败";AddInfo(Info);}
				else
				{
				  fwrite(BmpFileBuffer,sizeof(unsigned char),BmpLen,NewBmpFile);
				  fclose(NewBmpFile);
				  Info=L"成功隐藏文件："+FilePath+FileNameN+L"-"+HFileNameN+L".bmp";AddInfo(Info);

				}
#pragma endregion
			}
		}
		free(dlg);
	}
}


void CFilehideDlg::OnBnClickedReleaseFile()
{

	BmpFile=_tfopen(FilePathName,_T("rb"));
	if(BmpFile==NULL)
	{Info=L"打开bmp文件失败！";AddInfo(Info);}
	else
	{
		   fseek(BmpFile, 0, SEEK_END);
	       size_t len = ftell(BmpFile);//获取文件大小
		   BmpLen=len;
	       fseek(BmpFile, 0, SEEK_SET);
		   BmpFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*len);
		   fread(BmpFileBuffer,sizeof(unsigned char),len,BmpFile);
		   BmpFile==NULL;
		   fclose(BmpFile);

		   Point=BmpFileBuffer;

		   bmpFH=(BITMAP_FILE_HEADER*)Point;
		   BmpSizeColor=bitmap_info->bmiHeader.biSizeImage;
		   HFileLen=*(unsigned short *)&bmpFH->bfReserved1+(*(unsigned char*)&bmpFH->bfReserved2)*65536;
		   HFileNameLen=*((unsigned char*)&bmpFH->bfReserved2+1);
		   if(BmpSizeColor/8<=(HFileLen+HFileNameLen)||HFileLen==0)
		   {Info=L"文件数据不存在或已经损坏,释放失败";AddInfo(Info);}
		   else
		   {
		   Point=Point+0x0E;
		   bitmap_info=(BITMAP_INFO*)Point;
		   BIT_HIDE *bit_hide=(BIT_HIDE*)bitmap_info->bmiClor;//格式化要隐藏的数据在母版中的位置
#pragma region 创建新文件
		   HFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*HFileLen);
		   HPoint=HFileBuffer;
		    NewDate *HideDate=(NewDate*)HPoint;
		 for(int i=0;i<HFileLen;i++)//将文件数据从bmp中读出来
		 {
			 (HideDate)->Date1=(bit_hide)->BHide1;
			 (HideDate)->Date2=(bit_hide)->GHide1;
			 (HideDate)->Date3=(bit_hide)->RHide1;

			 (HideDate)->Date4=(bit_hide)->BHide2;
			 (HideDate)->Date5=(bit_hide)->GHide2;
			 (HideDate++)->Date6=(bit_hide++)->RHide2;
		 }
		// NewDate *HideName=(NewDate*)&HFileName;

		 char *CHFN=NULL;
		 CHFN=(char*)malloc(sizeof( char)*HFileNameLen+1);
		 CHFN[HFileNameLen]='\0';
		 NewDate *HideName=(NewDate*)CHFN;
		 for(int i=0;i<HFileNameLen;i++)
		 {
			 (HideName)->Date1=(bit_hide)->BHide1;
			 (HideName)->Date2=(bit_hide)->GHide1;
			 (HideName)->Date3=(bit_hide)->RHide1;

			 (HideName)->Date4=(bit_hide)->BHide2;
			 (HideName)->Date5=(bit_hide)->GHide2;
			 (HideName++)->Date6=(bit_hide++)->RHide2;
		 }

		 
		 HFileName.Format(_T("%s"), CStringW(CHFN));

		//printf( "%d\n",WideCharToMultiByte(CP_ACP,0,HFileName,HFileName.GetLength(),NULL,0,NULL,NULL));
		// AfxMessageBox(HFileName);
		 HFile=_tfopen(FilePath+HFileName,L"wb+");
		 if(HFile==NULL)
		 {Info=L"文件生成失败";AddInfo(Info);}
		 else
		 {
		 Info=L"创建文件："+FilePath+HFileName;
		 AddInfo(Info);
		 fwrite(HFileBuffer,sizeof(unsigned char),HFileLen,HFile);
		 fclose(HFile);
		 
		 Info=L"成功释放了文件:"+HFileName;
		 AddInfo(Info);
#pragma endregion
		 }

	}
	}
}


//BOOL CFilehideDlg::PreCreateWindow(CREATESTRUCT& cs)
//{
//	return true;
//}
//

void CFilehideDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
 lpMMI->ptMaxTrackSize.x=465;
 lpMMI->ptMaxTrackSize.y=330;
 lpMMI->ptMinTrackSize.x=465;
 lpMMI->ptMinTrackSize.y=330;
 CDialogEx::OnGetMinMaxInfo(lpMMI);
}
