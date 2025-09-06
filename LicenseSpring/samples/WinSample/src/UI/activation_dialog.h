#pragma once

#include "resource.h"
#include "defines.h"
#include <atldlgs.h>

namespace WTLSample::UI
{
class ActivationDialog : public CDialogImpl<ActivationDialog>
{
public:
    enum
    {
        IDD = IDD_ACTIVATION
    };

    LRESULT OnBnClickedActivateByKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedActivateByUser(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedCreateRequestFileByKey(
        WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedCreateRequestFileByUser(
        WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedOpenActivationPage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedGetProductDetails(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedGetTrialLicense(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedOpenActivationFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    BEGIN_MSG_MAP(ActivationDialog)
    COMMAND_HANDLER(IDC_BUTTON_ACTIVATE_KEY, BN_CLICKED, OnBnClickedActivateByKey)
    COMMAND_HANDLER(IDC_BUTTON_ACTIVATE_USER, BN_CLICKED, OnBnClickedActivateByUser)
    COMMAND_HANDLER(IDC_BUTTON_REQUEST_KEY, BN_CLICKED, OnBnClickedCreateRequestFileByKey)
    COMMAND_HANDLER(IDC_BUTTON_REQUEST_USER, BN_CLICKED, OnBnClickedCreateRequestFileByUser)
    COMMAND_HANDLER(IDC_BUTTON_TRIAL, BN_CLICKED, OnBnClickedGetTrialLicense)
    COMMAND_HANDLER(IDC_BUTTON_FILEOPEN, BN_CLICKED, OnBnClickedOpenActivationFile)
    COMMAND_HANDLER(IDC_BUTTON_ACTIVATION_PAGE, BN_CLICKED, OnBnClickedOpenActivationPage)
    COMMAND_HANDLER(IDC_BUTTON_PRODUCT_DETAILS, BN_CLICKED, OnBnClickedGetProductDetails)
    MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
    MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    END_MSG_MAP()

private:
    CBrush m_brBkgColor;
    CEdit m_editKey;
    CEdit m_editUser;
    CEdit m_editUserID;
    CEdit m_editPassword;

    void registerAndSetCallback();
    void runInfoDialog();
    void setWindowToCenter(HWND hWnd);
    HWND createToolTip(int toolID, HWND hDlg, PCTSTR pszText);
};
} // namespace WTLSample::UI
