
// FilehideDlg.h : ͷ�ļ�
//

#pragma once


// CFilehideDlg �Ի���
class CFilehideDlg : public CDialogEx
{
// ����
public:
	CFilehideDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILEHIDE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOpenBmp();
	afx_msg void CFilehideDlg::AddInfo(CString NewInfo);
	afx_msg void OnBnClickedInsertFile();
	afx_msg void OnBnClickedReleaseFile();
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
