
// FilehideDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Filehide.h"
#include "FilehideDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////λͼ�ļ�ͷ���ݽṹ��14���ֽڣ�
typedef struct
{
	short bfType;//����λͼ�ļ������ͣ���ΪBM��0x424D��
	short bfSize[2];//����λͼ�ļ��Ĵ�С�����ֽ�Ϊ��
	short bfReserved1;//�����֣�Ϊ0
	short bfReserved2;//�����֣�Ϊ0
	short bfOFFBits[2];//λͼ���е���ʼλ��
}BITMAP_FILE_HEADER;

////////////////////////////////////////////λͼ��Ϣͷ���ݽṹ��40���ֽڣ�
typedef struct
{
	long biSize;//ָ�������ݽṹ����Ҫ���ֽ���
	long biWidth;//������Ϊ��λ������BMPͼ��Ŀ��
	long biHeight;//������Ϊ��λ������BMPͼ��ĸ߶�
	short biPlanes;//����豸��Ϊƽ����������λ1
	short biBitCount;//����ÿ�����ص�λ����1��4��8��24
	long biCompress;//����λͼ��ѹ������
	long biSizeImage;//����ͼ���ֽ����Ķ���
	long biXPelsPerMeter;//ͼ���ˮƽ �ֱ���
	long biYelsPerMeter;//ͼ��Ĵ�ֱ�ֱ���
	long biClrUsed;//��ɫ����ͼ��ʵ��ʹ�õ���ɫ��
	long biClrImportant;//������Ҫ��ɫ������ֵ
}BITMAP_INFO_HEADER;

///////////////////////////////////////////λͼ���ݽṹ��4���ֽڣ�
typedef struct
{
	unsigned char rgbBlue;//��
	unsigned char rgbGreen;//��
	unsigned char rgbRed;//��
	unsigned char rgbReserved;//����

}BITMAP_COLOR;

//////////////////////////////////////////λͼ��Ϣ���ݽṹ������λͼ��Ϣͷ��λͼ��
typedef struct
{
      BITMAP_INFO_HEADER bmiHeader;
	  BITMAP_COLOR       bmiClor[];
}BITMAP_INFO;
///////////////////////////////////////���ص��ļ���λ��(ÿ�������ؿ�������һ���ֽ�)
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
/////////////////////////////////////Ҫ���ص�����
typedef struct
{
	unsigned char Date1:1;
	unsigned char Date2:2;
	unsigned char Date3:1;
	unsigned char Date4:1;
	unsigned char Date5:2;
	unsigned char Date6:1;
}NewDate;

// CFilehideDlg �Ի���
#pragma region ȫ�ֱ���
CString FilePathName;//·���ļ���
CString HFilePathName;
CString FilePath;//·��
CString HfilePath;
CString FileName;//�ļ���
CString FileNameN;
CString HFileName;
CString HFileNameN;

CString Info;//Ҫ��ӡ����ʾ��Ϣ
	//wchar_t *FPN=NULL;
FILE *BmpFile=NULL;//�ļ����
FILE *HFile=NULL;
FILE *NewBmpFile=NULL;
FILE *NewHFile=NULL;
unsigned char *BmpFileBuffer=NULL;//bmp�ļ����ݻ�����
unsigned char *HFileBuffer=NULL;//�ļ����ݻ�����
unsigned char *Point=NULL;//�ļ�ָ��
unsigned char *HPoint=NULL;
int BmpLen;//ԭʼbmp�ļ�����
int HFileLen;//�ļ����ݳ���
int HFileNameLen;//�ļ�������
long BmpSizeColor;//���ص��ֽ���
long HFileMaxSize;//Ҫ������ļ���С����
BITMAP_FILE_HEADER *bmpFH;//�ṹ��bmpͷ
BITMAP_INFO *bitmap_info;//�ṹ��λͼ��Ϣ
CFont font;//����
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


// CFilehideDlg ��Ϣ�������

BOOL CFilehideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFilehideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		LOGFONT _logfont;
memset(&_logfont, 0, sizeof(_logfont));
_logfont.lfHeight = 18;
_logfont.lfWidth  =  0;
_logfont.lfWeight = 400;
//_logfont.lfItalic=true;
_logfont.lfQuality = DEFAULT_QUALITY ;
//_logfont.lfUnderline=true;
_logfont.lfPitchAndFamily = FF_SCRIPT ;
_tcscpy(_logfont.lfFaceName, _T("����"));
//memset(&_logfont, 0, sizeof(_logfont));

font.CreateFontIndirect(&_logfont);
GetDlgItem(Edit_Info)->SetFont(&font, FALSE);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
font.DeleteObject();
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialogEx::OnPaint();
		
	}
	  #pragma region ��������

#pragma endregion
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFilehideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFilehideDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

  // AfxMessageBox(L"hello");
}


void CFilehideDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//FreeConsole();
	 free(BmpFileBuffer);
	 free(HFileBuffer);
	CDialogEx::OnOK();
}


void CFilehideDlg::OnBnClickedOpenBmp()
{
	
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(LPCTSTR)_TEXT("BMP Files|*.bmp|"),NULL);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	
    if(dlg.DoModal()==IDOK)
   { 
	   FilePathName=dlg.GetPathName();
       FileName=FilePathName.Mid(FilePathName.ReverseFind( '\\')+1);
	   FilePath=FilePathName.Left(FilePathName.ReverseFind( '\\')+1);
	   FileNameN=FileName.Left(FileName.ReverseFind('.'));
	   if(FileName.ReverseFind('.')==-1) FileNameN=FileName;
	   BmpFile=_tfopen(FilePathName,_T("rb"));
	   if(BmpFile==NULL)
	   { Info=L"�ļ�����ʧ�ܣ�";AddInfo(Info);fclose(BmpFile);}
	   else
	   {
		   Info=L"����bmp�ļ�:"+FilePathName;AddInfo(Info);

		   fseek(BmpFile, 0, SEEK_END);
	       size_t len = ftell(BmpFile);//��ȡ�ļ���С
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
			   Info=L"��Ч��bmp�ļ���";
			   AddInfo(Info);
			   BmpFileBuffer=NULL;
			   BmpFile=NULL;
			   
		   }
		   else
		   {
		   //short s=*(int *)(&bmpFH->bfReserved1);
		      if(*(int *)(&bmpFH->bfReserved1)!=0)
		      {
			     Info=L"��⵽������������bmp�ļ���,�����ͷ������ļ�";
			     AddInfo(Info);
		      }
			  Point=Point+0x0E;
			
			  bitmap_info=(BITMAP_INFO*)Point;
	          BmpSizeColor=bitmap_info->bmiHeader.biSizeImage;  
		   }
		  

	   }
	}
	free(dlg);
	//else{AfxMessageBox(L"�ļ���ʧ��");}

}

void CFilehideDlg::AddInfo(CString NewInfo)
{
	CString Info=NULL;
	GetDlgItem(Edit_Info)->GetWindowText(Info);
	Info+=(L"\r\n"+NewInfo+L"����");
	GetDlgItem(Edit_Info)->SetWindowText(Info);
}

void CFilehideDlg::OnBnClickedInsertFile()
{

	if(BmpFile==NULL)
	{
		Info=L"Ӧ������bmp�ļ���";
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
	        size_t len = ftell(HFile);//��ȡ�ļ���С
	        fseek(HFile, 0, SEEK_SET);

		    HFileNameLen= WideCharToMultiByte(CP_ACP,0,HFileName,HFileName.GetLength(),NULL,0,NULL,NULL);//��ȡ�ļ����Ĵ�С
			HFileMaxSize=BmpSizeColor/8-HFileNameLen;
			//printf("HFileNameLen=%d\n",HFileNameLen);
			//printf("BmpSizeColor=%d\n",BmpSizeColor);
			if(len>HFileMaxSize)
			{
				Info=L"�ļ����󣬳�����bmp�ļ��������ɵ����ݣ�";
				AddInfo(Info);
				fclose(HFile);
				HFile=NULL;
			}
			else
			{
				Info=L"����Ҫ���ص��ļ�:"+HFilePathName;AddInfo(Info);
				HFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*len);
				fread(HFileBuffer,sizeof(unsigned char),len,HFile);
				fclose(HFile);
				
		      // printf("%d",len);
				*((int *)&bmpFH->bfReserved1)=len;//���ļ���Сд�뱣���ֶ�
				*(((unsigned char *)&bmpFH->bfReserved2)+1)=HFileNameLen;//���ļ����Ĵ�Сд�뱣���ֶ����һ���ֽ�
#pragma region ��������
				HPoint=HFileBuffer;
				BIT_HIDE *bit_hide=(BIT_HIDE*)bitmap_info->bmiClor;//��ʽ��Ҫ���ص�������ĸ���е�λ��
		        NewDate *newdate=(NewDate*)HPoint;//��ʽ��Ҫ���ص�����
		        for(int i=0;i<len;i++)//���ļ�����д��bmp�ļ�
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
				for(int i=0;i<HFileNameLen;i++)//���ļ���д��bmp�ļ�
				{
				   (bit_hide)->BHide1=(namedate)->Date1;
		           (bit_hide)->GHide1=(namedate)->Date2;
		           (bit_hide)->RHide1=(namedate)->Date3;
		           (bit_hide)->BHide2=(namedate)->Date4;
		           (bit_hide)->GHide2=(namedate)->Date5;
		           (bit_hide++)->RHide2=(namedate++)->Date6;
				}
#pragma endregion
#pragma region �������ļ�
				NewBmpFile=_tfopen(FilePath+FileNameN+L"-"+HFileNameN+L".bmp",L"wb+");
				if(NewBmpFile==NULL)
				{Info=L"�ļ�����ʧ��";AddInfo(Info);}
				else
				{
				  fwrite(BmpFileBuffer,sizeof(unsigned char),BmpLen,NewBmpFile);
				  fclose(NewBmpFile);
				  Info=L"�ɹ������ļ���"+FilePath+FileNameN+L"-"+HFileNameN+L".bmp";AddInfo(Info);

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
	{Info=L"��bmp�ļ�ʧ�ܣ�";AddInfo(Info);}
	else
	{
		   fseek(BmpFile, 0, SEEK_END);
	       size_t len = ftell(BmpFile);//��ȡ�ļ���С
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
		   {Info=L"�ļ����ݲ����ڻ��Ѿ���,�ͷ�ʧ��";AddInfo(Info);}
		   else
		   {
		   Point=Point+0x0E;
		   bitmap_info=(BITMAP_INFO*)Point;
		   BIT_HIDE *bit_hide=(BIT_HIDE*)bitmap_info->bmiClor;//��ʽ��Ҫ���ص�������ĸ���е�λ��
#pragma region �������ļ�
		   HFileBuffer=(unsigned char*)malloc(sizeof(unsigned char)*HFileLen);
		   HPoint=HFileBuffer;
		    NewDate *HideDate=(NewDate*)HPoint;
		 for(int i=0;i<HFileLen;i++)//���ļ����ݴ�bmp�ж�����
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
		 {Info=L"�ļ�����ʧ��";AddInfo(Info);}
		 else
		 {
		 Info=L"�����ļ���"+FilePath+HFileName;
		 AddInfo(Info);
		 fwrite(HFileBuffer,sizeof(unsigned char),HFileLen,HFile);
		 fclose(HFile);
		 
		 Info=L"�ɹ��ͷ����ļ�:"+HFileName;
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
 lpMMI->ptMaxTrackSize.x=465;
 lpMMI->ptMaxTrackSize.y=330;
 lpMMI->ptMinTrackSize.x=465;
 lpMMI->ptMinTrackSize.y=330;
 CDialogEx::OnGetMinMaxInfo(lpMMI);
}
