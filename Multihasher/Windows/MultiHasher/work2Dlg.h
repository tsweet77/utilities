
// work2Dlg.h : header file
//

#pragma once

// ++ Add Color Definitions ++
const COLORREF DARK_MODE_BG = RGB(45, 45, 48);
const COLORREF DARK_MODE_TEXT = RGB(241, 241, 241);
const COLORREF LIGHT_MODE_BG = RGB(255, 255, 255); // Or use GetSysColor(COLOR_WINDOW)
const COLORREF LIGHT_MODE_TEXT = RGB(0, 0, 0);      // Or use GetSysColor(COLOR_WINDOWTEXT)
// ++ End Add ++


// **** CHANGE HERE: Inherit from CDialogEx ****
class Cwork2Dlg : public CDialogEx // <-- Change from CDialog
{
	// Construction
public:
	Cwork2Dlg(CWnd* pParent = nullptr);	// standard constructor
	~Cwork2Dlg(); // ++ Add Destructor Declaration ++

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	// **** CHANGE HERE: Use the correct IDD ****
	enum { IDD = IDD_WORK2_DIALOG };  // Make sure this IDD matches your dialog resource
#endif

protected: // <-- Or public, depending on your preference
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected: // <-- Protected is typical for implementation details
	HICON m_hIcon;

	// ++ Add Dark Mode Members ++
	bool m_bDarkMode;
	CBrush m_darkBrush;
	CBrush m_lightBrush;
	void ApplyVisuaMode(); // Helper function to update visuals
	// ++ End Add ++

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// ++ Add Dark Mode Handlers ++
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedToggleDarkMode();
	// ++ Add Owner Draw Handler ++
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct); // <-- ADD THIS LINE
	// ++ End Add ++
	DECLARE_MESSAGE_MAP() // This MUST be present

public: // Public interface
	CString m_strPlain;
	UINT64 m_nRepeat;
	int m_nLevel;
	CString m_strResult;
	afx_msg void OnBnClickedOk();
	void calc();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CComboBox m_cmbType;
	BOOL m_bUppercase;
	afx_msg void OnBnClickedBtnInsertFile();
	afx_msg void OnBnClickedCancel();
};

// ++ Add Button ID for Resource Editor ++
// Ensure this is defined, often in Resource.h
// #define IDC_BTN_TOGGLE_DARK_MODE 1007 // Make sure this ID exists and is unique