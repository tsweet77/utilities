
// work2Dlg.cpp : implementation file
//

#include "pch.h" // Must be first if using precompiled headers
#include "framework.h"
#include "work2.h"
#include "work2Dlg.h"
#include "afxdialogex.h" // Header for CDialogEx

#include "sha/sha3.h"
#include <string>
#include <iomanip>
#include <sstream>

// #include <afxwin.h> // Already included via pch.h or framework.h usually

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwork2Dlg dialog



Cwork2Dlg::Cwork2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORK2_DIALOG, pParent)
	, m_strPlain(_T("I am Love."))
	, m_nRepeat(10000)
	, m_nLevel(100)
	, m_strResult(_T(""))
	, m_bUppercase(FALSE)
	, m_bDarkMode(false) // ++ Initialize Dark Mode Flag ++
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// ++ Add Destructor Implementation ++
Cwork2Dlg::~Cwork2Dlg()
{
	// Clean up GDI objects
	if (m_darkBrush.GetSafeHandle()) {
		m_darkBrush.DeleteObject();
	}
	if (m_lightBrush.GetSafeHandle()) {
		m_lightBrush.DeleteObject();
	}
}
// ++ End Add ++

void Cwork2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PLAIN, m_strPlain);
	DDX_Text(pDX, IDC_EDT_REPEAT, m_nRepeat);
	DDX_Text(pDX, IDC_EDT_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_EDT_RET, m_strResult);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbType);
	DDX_Check(pDX, IDC_CHK_UPPERCASE, m_bUppercase);
}

BEGIN_MESSAGE_MAP(Cwork2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// ++ Add Dark Mode Message Handlers ++
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_TOGGLE_DARK_MODE, &Cwork2Dlg::OnBnClickedToggleDarkMode)
	ON_WM_DRAWITEM() // <-- ADD THIS LINE
	// ++ End Add ++
	ON_BN_CLICKED(IDOK, &Cwork2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BTN_INSERT_FILE, &Cwork2Dlg::OnBnClickedBtnInsertFile)
	// Note: BN_CLICKED still works for owner-draw buttons
END_MESSAGE_MAP()


// Cwork2Dlg message handlers

BOOL Cwork2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_cmbType.AddString(L"SHA-512");
	m_cmbType.AddString(L"SHA-64");

	m_cmbType.SetCurSel(0);

	// ++ Add Dark Mode Initialization ++
	// Create brushes
	m_darkBrush.CreateSolidBrush(DARK_MODE_BG);
	m_lightBrush.CreateSolidBrush(LIGHT_MODE_BG);

	// Set initial button text (assuming the button exists)
	CWnd* pButton = GetDlgItem(IDC_BTN_TOGGLE_DARK_MODE);
	if (pButton) {
		pButton->SetWindowText(L"Dark Mode"); // Default is Light, so button shows option to switch TO Dark
	}
	// Apply initial mode (optional cosmetic call, OnCtlColor handles actual drawing)
	// We don't call ApplyVisuaMode here as OnCtlColor will be triggered during initial draw
	// ++ End Add ++


	return TRUE;  // return TRUE unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cwork2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cwork2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void FillMemory_(unsigned char* p_pBuff, unsigned char* p_szPattern, size_t p_nLen, unsigned long long p_nRepeat) {
	unsigned long long nTotal = p_nLen * p_nRepeat;
	unsigned long long nCurRepeat = 1;
	unsigned long long nCopyNode;

	memcpy(p_pBuff, p_szPattern, p_nLen);

	while (nCurRepeat < p_nRepeat) {
		nCopyNode = min(p_nRepeat - nCurRepeat, nCurRepeat);
		memcpy(&p_pBuff[(unsigned long long)p_nLen * nCurRepeat], p_pBuff, nCopyNode * p_nLen);
		nCurRepeat += nCopyNode;
	}
}

void hex2char(unsigned char* pHex, int nLen, char* p_pBuff) {
	for (int i = 0; i < nLen; i++) {
		char szBuff[3]; memset(szBuff, 0, sizeof(szBuff));
		sprintf_s(szBuff, 3, "%02x", pHex[i]);
		strcat_s(p_pBuff, 129, szBuff);
	}
}

#include "sha\sha3.h"

bool g_b512 = false;
void get_sha3_512(unsigned char* p_pMessage, size_t p_nLen, char* p_pHash) {

	KECCAK1600_CTX ctx;
	ossl_sha3_init(&ctx, '\x06', 512);
	unsigned char pBuff[32]; memset(pBuff, 1, sizeof(pBuff));
	ossl_sha3_update(&ctx, p_pMessage, p_nLen);
	unsigned char pHash[64]; memset(pHash, 0, sizeof(pHash));
	ossl_sha3_final(&ctx, pHash, 64);
	hex2char(pHash, 64, p_pHash);
}

UINT TF_CALC(void* p) {
	Cwork2Dlg* pThis = (Cwork2Dlg*)p;
	pThis->calc();
	return 0;
}

std::string truncate_to_64_bit(const std::string& hash512) {
	// Each 64-bit group is 16 hex characters
	unsigned long long group1 = std::stoull(hash512.substr(0, 16), nullptr, 16);
	unsigned long long group2 = std::stoull(hash512.substr(16, 16), nullptr, 16);
	unsigned long long group3 = std::stoull(hash512.substr(32, 16), nullptr, 16);
	unsigned long long group4 = std::stoull(hash512.substr(48, 16), nullptr, 16);
	unsigned long long group5 = std::stoull(hash512.substr(64, 16), nullptr, 16);
	unsigned long long group6 = std::stoull(hash512.substr(80, 16), nullptr, 16);
	unsigned long long group7 = std::stoull(hash512.substr(96, 16), nullptr, 16);
	unsigned long long group8 = std::stoull(hash512.substr(112, 16), nullptr, 16);

	// Add the eight 64-bit groups together
	unsigned long long combined_64_bit = group1 + group2 + group3 + group4 + group5 + group6 + group7 + group8;

	// Since we are working with 64-bit integers, the modulo operation is inherent
	// We directly convert the result to a hex string
	std::stringstream ss;
	ss << std::hex << std::uppercase << (combined_64_bit & 0xFFFFFFFFFFFFFFFFULL);
	return ss.str();
}

void Cwork2Dlg::calc()
{
	unsigned char pData[32]; memset(pData, 1, sizeof(pData));
	char szHash[129]; memset(szHash, 0, sizeof(szHash));
	get_sha3_512(pData, 32, szHash);

	DWORD tick = GetTickCount();

	char pHashNow[130]; memset(pHashNow, 0, 130);
	char* pRepeatedPlainText = NULL;
	unsigned char* pRepeated = NULL;
	unsigned long long nRepeatedLen;

	int nPatternLen = m_strPlain.GetLength() + 2;
	char* pPattern = (char*)malloc(nPatternLen);
	memset(pPattern, 0, nPatternLen);
	sprintf_s(pPattern, nPatternLen, "%S\n", m_strPlain.GetBuffer());

	int nTailSize = 2 + m_strPlain.GetLength();
	unsigned char* pTail = (unsigned char*)malloc(nTailSize + 1);
	memset(pTail, 0, nTailSize + 1);
	sprintf_s((char*)pTail, nTailSize + 1, ": %S", m_strPlain.GetBuffer());

	// calc repeated plain text
	unsigned long long nRepeatedPlainTextLen = (nPatternLen - 1) * m_nRepeat + nTailSize;
	pRepeatedPlainText = (char*)malloc(nRepeatedPlainTextLen);
	if (pRepeatedPlainText == NULL) {
		m_strResult = L"Failed to malloc the memory. Repetition is too much.";
		goto L_EXIT;
	}
	FillMemory_((unsigned char*)pRepeatedPlainText, (unsigned char*)pPattern, nPatternLen - 1, m_nRepeat);
	memcpy(&pRepeatedPlainText[(nPatternLen - 1) * m_nRepeat], pTail, nTailSize);

	get_sha3_512((unsigned char*)pRepeatedPlainText, nRepeatedPlainTextLen, pHashNow);
	//	sha3_512_hash((unsigned char*)pRepeatedPlainText, nRepeatedPlainTextLen, pHashNow);

	nRepeatedLen = 129 * m_nRepeat + nTailSize;
	pRepeated = (unsigned char*)malloc(129 * m_nRepeat + nTailSize);
	if (pRepeated == NULL) {
		m_strResult = L"Failed to malloc the memory. Repetition is too much.";
		goto L_EXIT;
	}

	for (int i = 1; i <= m_nLevel; i++) {
		m_strResult.Format(L"Calculate hash for level %d", i);
		PostMessage(WM_USER + 1);

		DWORD dwTick1 = GetTickCount();
		pHashNow[128] = '\n';
		FillMemory_(pRepeated, (unsigned char*)pHashNow, 129, m_nRepeat);
		memcpy(&pRepeated[129 * m_nRepeat], pTail, nTailSize);
		DWORD dwTick2 = GetTickCount();

		memset(pHashNow, 0, 130);
		get_sha3_512((unsigned char*)pRepeated, nRepeatedLen, pHashNow);
		//		sha3_512_hash((unsigned char*)pRepeated, nRepeatedLen, pHashNow);
		DWORD dwTick3 = GetTickCount();

		m_strResult.Format(L"%S", pHashNow);
	}

	if (!g_b512) {
		std::string ret = truncate_to_64_bit(pHashNow);
		m_strResult.Format(L"%S", ret.c_str());
	}
	else {
		m_strResult.Format(L"%S", pHashNow);
	}

	if (m_bUppercase)
		m_strResult.MakeUpper();

L_EXIT:

	if (pPattern) free(pPattern);
	if (pTail) free(pTail);
	if (pRepeatedPlainText) free(pRepeatedPlainText);
	if (pRepeated) free(pRepeated);

	PostMessage(WM_USER + 1);
}

UINT64 s_maxRepetitions = 10000000;

void Cwork2Dlg::OnBnClickedOk()
{
	g_b512 = (m_cmbType.GetCurSel() == 0);

	UpdateData(TRUE);

	int intentionLen = m_strPlain.GetLength();
	if (intentionLen == 0)
	{
		AfxMessageBox(L"Intention must not be empty.");
		return;
	}

	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
	if (GlobalMemoryStatusEx(&memoryStatus)) {
		s_maxRepetitions = memoryStatus.ullAvailPhys / (2 * (intentionLen + 150));;
		if (s_maxRepetitions < m_nRepeat) {
			m_nRepeat = s_maxRepetitions;
			UpdateData(FALSE);
		}
	}
	else {
		AfxMessageBox(L"Failed to retrieve memory status.");
		return;
	}

	if (m_nRepeat < 1 || m_nRepeat > s_maxRepetitions)
	{
		CString strMsg;
		strMsg.Format(L"Repetitions must be between 1 and %u", s_maxRepetitions);
		AfxMessageBox(strMsg);
		return;
	}

	if (m_nLevel < 1 || m_nLevel > 1000)
	{
		AfxMessageBox(L"Hash levels must be between 1 and 1000.");
		return;
	}

	m_strResult = L"Calculate hash ..."; UpdateData(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	DWORD dwTID = 0;
	CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)TF_CALC, (void*)this, 0, &dwTID);

}


LRESULT Cwork2Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_USER + 1) {
		UpdateData(FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void Cwork2Dlg::OnBnClickedBtnInsertFile()
{
	UpdateData(TRUE); // Get current value of m_strPlain from the edit box

	// Create a file dialog
	CFileDialog fileDlg(TRUE); // TRUE for Open dialog

	// Show the dialog and check if the user selected a file
	if (fileDlg.DoModal() == IDOK)
	{
		// Get the selected file path
		CString filePath = fileDlg.GetPathName();

		// Open the file using CFile
		CFile file;
		try
		{
			if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary)) {
				AfxMessageBox(L"Failed to open file for reading.");
				return;
			}

			ULONGLONG nFileLen = file.GetLength(); // Use ULONGLONG for potentially large files
			if (nFileLen == 0) {
				// Optional: Append nothing, show message, or append a specific marker
				// AfxMessageBox(L"Selected file is empty.");
				file.Close();
				// Decide if you want to proceed or return
				// For now, let's just return without changing the text
				return;
			}
			// Add a check for excessively large files if memory is a concern
			const size_t MAX_FILE_SIZE_BYTES = 500 * 1024 * 1024; // Example: 500 MB limit
			if (nFileLen > MAX_FILE_SIZE_BYTES) {
				AfxMessageBox(L"File is too large to process in memory.");
				file.Close();
				return;
			}

			size_t nLen = static_cast<size_t>(nFileLen); // Cast to size_t for buffer allocation

#ifdef new // Check if 'new' is defined as a macro (it will be in _DEBUG via DEBUG_NEW)
#undef new // Temporarily revert 'new' to its keyword status
#define _UNDEF_NEW_USED // Flag to indicate we undef'd it
#endif

			BYTE* pBuf = new (::std::nothrow) BYTE[nLen]; // Add :: before std

#ifdef _UNDEF_NEW_USED // Check if we undef'd 'new' above
#define new DEBUG_NEW // Redefine 'new' back to the debug version
#undef _UNDEF_NEW_USED // Clean up the flag
#endif

			if (pBuf == nullptr) {
				AfxMessageBox(L"Failed to allocate memory to read the file.");
				file.Close();
				return;
			}

			// Check if file length exceeds UINT_MAX before reading
			if (nFileLen > UINT_MAX) {
				AfxMessageBox(L"File is too large to read with CFile::Read (exceeds UINT_MAX).");
				delete[] pBuf;
				file.Close();
				return;
			}

			UINT uBytesToRead = static_cast<UINT>(nLen); // Safe cast after check

			file.Read(pBuf, uBytesToRead); // Read using UINT
			file.Close(); // Close file ASAP

			char szHash[129]; memset(szHash, 0, sizeof(szHash));
			get_sha3_512(pBuf, nLen, szHash); // Hash calculation

			CStringW strHashW; // Use CStringW for Unicode compatibility with MFC controls
			strHashW = CA2W(szHash); // Convert char* (ANSI/UTF8 from hash func) to WCHAR* (Unicode)

			// ++ Modify: Append instead of replace ++
			CStringW currentText = m_strPlain; // Keep a copy if needed, or just append directly
			CStringW textToAppend = strHashW;

			if (m_bUppercase) { // Apply uppercase to the hash *before* appending if desired
				textToAppend.MakeUpper();
			}

			if (!currentText.IsEmpty()) {
				m_strPlain += L"\r\n"; // Add a separator (e.g., newline) if text already exists
			}
			m_strPlain += textToAppend; // Append the hash (already uppercased if requested)
			// Note: If you wanted the *entire* box uppercased after insert, remove the MakeUpper above
			// and uncomment the line below instead:
			// if (m_bUppercase) m_strPlain.MakeUpper();
			// ++ End Modify ++


			delete[] pBuf; // Use delete[] for array allocated with new[]

			UpdateData(FALSE); // Update the dialog control with the new m_strPlain
		}
		catch (CFileException* e)
		{
			// Ensure file handle is closed in case of exception before GetLength or Read
			if (file.m_hFile != CFile::hFileNull) file.Close();

			TCHAR errorMessage[1024];
			e->GetErrorMessage(errorMessage, 1024);
			CString msg;
			msg.Format(L"File error: %s", errorMessage);
			AfxMessageBox(msg);
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			// Ensure file handle is closed
			if (file.m_hFile != CFile::hFileNull) file.Close();
			AfxMessageBox(L"Memory allocation error processing the file.");
			e->Delete();
		}
		catch (...) // Catch any other unexpected exceptions
		{
			// Ensure file handle is closed
			if (file.m_hFile != CFile::hFileNull) file.Close();
			AfxMessageBox(L"An unexpected error occurred while processing the file.");
		}
	}
}

// ++ Add ApplyVisuaMode Implementation ++
void Cwork2Dlg::ApplyVisuaMode()
{
	// ... (update toggle button text) ...

	// Set the dialog's background color using CDialogEx feature
	// *** Use COLOR_BTNFACE for standard dialog feel in light mode ***
	COLORREF dlgBgColor = m_bDarkMode ? DARK_MODE_BG : ::GetSysColor(COLOR_BTNFACE);
	SetBackgroundColor(dlgBgColor); // This helps CDialogEx paint correctly

	// Invalidate the dialog itself to trigger WM_ERASEBKGND / OnCtlColor(CTLCOLOR_DLG)
	Invalidate();

	// Invalidate all child controls to trigger their WM_CTLCOLOR requests
	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild) {
		pChild->Invalidate();
		pChild->UpdateWindow();
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}
}
// ++ End Add ++

// ++ Add OnBnClickedToggleDarkMode Implementation ++
void Cwork2Dlg::OnBnClickedToggleDarkMode()
{
	m_bDarkMode = !m_bDarkMode; // Toggle the mode flag
	ApplyVisuaMode();          // Apply the changes
}
// ++ End Add ++


// ++ Add OnCtlColor Implementation ++
HBRUSH Cwork2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Call base class FIRST to get the default brush, especially for light mode.
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int controlId = pWnd->GetDlgCtrlID(); // Get control ID if needed later

	// --- Only intervene if in Dark Mode ---
	if (m_bDarkMode)
	{
		// Determine dark mode colors
		COLORREF darkBgColor = DARK_MODE_BG;
		COLORREF darkTextColor = DARK_MODE_TEXT;
		CBrush* pDarkBrush = &m_darkBrush; // Assuming m_darkBrush is valid

		switch (nCtlColor)
		{
		case CTLCOLOR_STATIC: // Handle labels, group boxes etc.
		{
			// Apply Dark Mode: White text, transparent background
			pDC->SetTextColor(darkTextColor);
			pDC->SetBkMode(TRANSPARENT);
			// Return the dark background brush so transparency works correctly
			if (pDarkBrush->GetSafeHandle()) {
				return (HBRUSH)pDarkBrush->GetSafeHandle(); // Return immediately for this control
			}
			// Fallback if brush is somehow invalid (shouldn't happen)
			// Allow default 'hbr' from base call to be returned below.
			break; // Go to end of function
		}

		case CTLCOLOR_EDIT:
		case CTLCOLOR_LISTBOX:
		{
			// Apply Dark Mode: White text, dark background
			pDC->SetTextColor(darkTextColor);
			pDC->SetBkColor(darkBgColor); // OPAQUE background
			if (pDarkBrush->GetSafeHandle()) {
				return (HBRUSH)pDarkBrush->GetSafeHandle(); // Return immediately
			}
			break; // Go to end of function
		}

		case CTLCOLOR_BTN: // Checkbox/Radio (box part), GroupBox text
		{
			// Apply ONLY to GroupBox text perhaps? Checkboxes are tricky.
			// Let's try setting text color & transparent bg for group boxes too.
			CString className;
			::GetClassName(pWnd->GetSafeHwnd(), className.GetBufferSetLength(MAX_PATH), MAX_PATH);
			className.ReleaseBuffer();
			if (className.CompareNoCase(_T("Button")) == 0) {
				DWORD style = pWnd->GetStyle();
				if ((style & BS_GROUPBOX) == BS_GROUPBOX) {
					pDC->SetTextColor(darkTextColor);
					pDC->SetBkMode(TRANSPARENT);
					if (pDarkBrush->GetSafeHandle())
						return (HBRUSH)pDarkBrush->GetSafeHandle(); // Return dark bg brush
				}
			}
			// For the actual checkbox control, don't interfere much here.
			// Let the default 'hbr' be returned.
			break; // Go to end of function
		}

		case CTLCOLOR_DLG:
		{
			// Apply Dark Mode dialog background
			if (pDarkBrush->GetSafeHandle()) {
				return (HBRUSH)pDarkBrush->GetSafeHandle(); // Return immediately
			}
			break; // Go to end of function
		}

		// Add cases for other controls if needed (e.g., scrollbars)

		} // End switch for Dark Mode

		// If we fell through the switch in dark mode without returning,
		// return the dark background brush as a default for dark mode.
		if (pDarkBrush->GetSafeHandle())
			return (HBRUSH)pDarkBrush->GetSafeHandle();

	} // End if (m_bDarkMode)

	// --- If NOT in Dark Mode ---
	// We simply return the HBRUSH obtained from the base class call at the top.
	// CDialogEx::OnCtlColor already provides the correct system brushes/colors
	// for light mode. No need to set colors or BkMode here.
	return hbr;
}
// ++ End Add ++


void Cwork2Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

// ++ Add Owner Draw Implementation ++
void Cwork2Dlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Check if it's a button we need to draw
	if (lpDrawItemStruct->CtlType != ODT_BUTTON)
	{
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct); // Let base class handle others
		return;
	}

	// Only handle our specific owner-draw buttons
	if (nIDCtl != IDOK && nIDCtl != IDCANCEL &&
		nIDCtl != ID_BTN_INSERT_FILE && nIDCtl != IDC_BTN_TOGGLE_DARK_MODE)
	{
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		return;
	}


	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rect = lpDrawItemStruct->rcItem; // Button rectangle
	UINT state = lpDrawItemStruct->itemState; // Button state (selected, focus, disabled)
	CString buttonText;
	CWnd* pButtonWnd = CWnd::FromHandle(lpDrawItemStruct->hwndItem);
	pButtonWnd->GetWindowText(buttonText); // Get button caption


	// --- Define Colors based on Mode and State ---
	COLORREF bgColor, textColor, borderColor;
	bool isPressed = (state & ODS_SELECTED);
	bool hasFocus = (state & ODS_FOCUS);
	bool isDisabled = (state & ODS_DISABLED);

	if (m_bDarkMode)
	{
		// Dark Mode colors (as before)
		bgColor = isPressed ? RGB(80, 80, 80) : RGB(60, 60, 60);
		textColor = isDisabled ? RGB(120, 120, 120) : DARK_MODE_TEXT;
		borderColor = hasFocus ? RGB(0, 120, 215) : RGB(100, 100, 100);
	}
	else // Light Mode
	{
		// Use system colors for a native light mode look
		bgColor = ::GetSysColor(COLOR_BTNFACE); // Standard button background
		textColor = isDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_BTNTEXT);
		// Border color isn't directly used for DrawEdge, but good to define
		borderColor = ::GetSysColor(COLOR_BTNSHADOW);

		// Note: DrawEdge handles the pressed look visually, so we don't
		// strictly *need* to change bgColor here unless we skip DrawEdge when pressed.
		// For consistency, let's keep bgColor as COLOR_BTNFACE for light mode drawing base.
	}

	// --- Drawing ---

	// 1. Draw Background
	dc.FillSolidRect(&rect, bgColor); // Always fill the background first

	// 2. Draw Border
	if (m_bDarkMode) {
		// Dark Mode: Use FrameRect with calculated border color
		CBrush borderBrush(borderColor);
		dc.FrameRect(&rect, &borderBrush);
	}
	else { // Light Mode: Use DrawEdge for standard look
		// *** ADD THIS BLOCK FOR LIGHT MODE BORDER ***
		if (!isPressed) {
			// Standard raised edge for unpressed button
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		}
		else {
			// Sunken edge for pressed button
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		}
		// *******************************************
	}


	// 3. Draw Text
	dc.SetBkMode(TRANSPARENT); // Ensure text background is transparent
	dc.SetTextColor(textColor);
	// Create a slightly smaller rect for text to provide padding
	CRect textRect = rect;
	// Adjust padding maybe slightly more if DrawEdge was used, as it takes up space
	textRect.DeflateRect(5, 3); // Adjust padding as needed (was 4,2)
	dc.DrawText(buttonText, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 4. Draw Focus Rectangle
	if (hasFocus && !isDisabled) // Draw focus only if focused and enabled
	{
		// Use standard DrawFocusRect for both modes - it usually adapts
		CRect focusRect = rect;
		// Deflate slightly so focus rect is *inside* the button bounds/border
		focusRect.DeflateRect(3, 3); // Adjust as needed
		dc.DrawFocusRect(&focusRect);
	}

	dc.Detach();
}
// ++ End Add ++