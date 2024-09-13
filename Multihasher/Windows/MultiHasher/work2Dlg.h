
// work2Dlg.h : header file
//

#pragma once


// Cwork2Dlg dialog
class Cwork2Dlg : public CDialog
{
// Construction
public:
	Cwork2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORK2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strPlain;
	UINT64 m_nRepeat;
	int m_nLevel;
	CString m_strResult;
	afx_msg void OnBnClickedOk();
	void calc();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CComboBox m_cmbType;
};
