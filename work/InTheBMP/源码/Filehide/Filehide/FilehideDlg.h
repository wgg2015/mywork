
// FilehideDlg.h : 头文件
//

#pragma once


// CFilehideDlg 对话框
class CFilehideDlg : public CDialogEx
{
// 构造
public:
	CFilehideDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILEHIDE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
