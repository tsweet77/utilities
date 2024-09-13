
// work2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "work2.h"
#include "work2Dlg.h"
#include "afxdialogex.h"

#include "sha/sha3.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwork2Dlg dialog



Cwork2Dlg::Cwork2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_WORK2_DIALOG, pParent)
	, m_strPlain(_T("I am Love."))
	, m_nRepeat(10000)
	, m_nLevel(100)
	, m_strResult(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwork2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PLAIN, m_strPlain);
	DDX_Text(pDX, IDC_EDT_REPEAT, m_nRepeat);
	DDX_Text(pDX, IDC_EDT_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_EDT_RET, m_strResult);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbType);
}

BEGIN_MESSAGE_MAP(Cwork2Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cwork2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()

// Cwork2Dlg message handlers

BOOL Cwork2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_cmbType.AddString(L"SHA-512");
	m_cmbType.AddString(L"SHA-64");

	m_cmbType.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
		CDialog::OnPaint();
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

	return CDialog::WindowProc(message, wParam, lParam);
}
