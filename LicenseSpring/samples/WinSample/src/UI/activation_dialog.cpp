#include "stdafx.h"
#include "activation_dialog.h"

#include "resource.h"
#include "product_details.h"
#include "info_dialog.h"
#include "Utils/utils.h"
#include "../../AppConfig.h"

#include <LicenseSpring/LicenseHandler.h>

using namespace LicenseSpring;
using namespace WTLSample::UI;

LRESULT ActivationDialog::OnBnClickedActivateByKey(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CString key;
    m_editKey.GetWindowText(key);

    if (key.IsEmpty())
    {
        Utils::ShowInfoMsgBox("Enter license key or choose file for activation.", m_hWnd);
        return E_FAIL;
    }

    auto &lh = LicenseHandler::instance();
    lh.activateLicense(LicenseID::fromKey(CT2CA(key)));

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }
    if (lh.isLicenseFloating())
        registerAndSetCallback();
    if (lh.isLicenseValid())
        runInfoDialog();
    return NO_ERROR;
}

LRESULT ActivationDialog::OnBnClickedActivateByUser(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CString user, password;
    m_editUser.GetWindowText(user);
    m_editPassword.GetWindowText(password);

    if (user.IsEmpty() || password.IsEmpty())
    {
        Utils::ShowInfoMsgBox("Enter user credentials or choose file for activation.", m_hWnd);
        return E_FAIL;
    }

    auto &lh = LicenseHandler::instance();
    lh.activateLicense(LicenseID::fromUser(CT2CA(user), CT2CA(password)));

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }

    if (lh.isLicenseFloating())
        registerAndSetCallback();
    if (lh.isLicenseValid())
        runInfoDialog();

    return NO_ERROR;
}

LRESULT ActivationDialog::OnBnClickedCreateRequestFileByUser(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CString user, password;
    m_editUser.GetWindowText(user);
    m_editPassword.GetWindowText(password);

    if (user.IsEmpty() || password.IsEmpty())
    {
        Utils::ShowWarningMsgBox("Please enter user credentials first.", m_hWnd);
        return E_FAIL;
    }

    CFileDialog saveDlg(false, _T( "req" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "Request Files\0*.req\0All Files\0*.*\0" ));

    // Ask for path of new activation request file.
    auto result = saveDlg.DoModal();

    std::wstring path = (IDOK == result) ? std::wstring(CT2CW(saveDlg.m_szFileName)) : L"";
    auto &lh = LicenseHandler::instance();
    path = lh.createOfflineActivationFile(LicenseID::fromUser(CT2A(user), CT2A(password)), path);

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
        ;
    }

    ::MessageBoxW(m_hWnd, path.c_str(), L"Request file path", MB_OK);
    return NO_ERROR;
}

LRESULT ActivationDialog::OnBnClickedCreateRequestFileByKey(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CString key;
    m_editKey.GetWindowText(key);

    if (key.IsEmpty())
    {
        Utils::ShowWarningMsgBox("Please enter license key first.", m_hWnd);
        return E_FAIL;
    }

    CFileDialog saveDlg(false, _T( "req" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "Request Files\0*.req\0All Files\0*.*\0" ));

    // Ask for path of new activation request file
    auto result = saveDlg.DoModal();

    std::wstring path = (IDOK == result) ? std::wstring(CT2CW(saveDlg.m_szFileName)) : L"";
    auto &lh = LicenseHandler::instance();
    path = lh.createOfflineActivationFile(LicenseID::fromKey(CT2A(key)), path);

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }

    ::MessageBoxW(m_hWnd, path.c_str(), L"Request file path", MB_OK);
    return NO_ERROR;
}

LRESULT ActivationDialog::OnBnClickedOpenActivationPage(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    ShellExecute(0, 0, _T( "https://saas.licensespring.com/offline" ), 0, 0, SW_SHOW);
    return 0;
}

LRESULT ActivationDialog::OnBnClickedGetProductDetails(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    ProductDetailsDialog dlg;
    auto res = dlg.DoModal();
    auto &lh = LicenseHandler::instance();
    auto details = lh.getProductDetails();
    if (details->authorizationMethod() == AuthMethodKeyBased)
    {
        m_editKey.EnableWindow(true);
        GetDlgItem(IDC_BUTTON_ACTIVATE_KEY).EnableWindow(true);
        GetDlgItem(IDC_BUTTON_REQUEST_KEY).EnableWindow(true);

        m_editPassword.EnableWindow(false);
        m_editUser.EnableWindow(false);
        GetDlgItem(IDC_BUTTON_ACTIVATE_USER).EnableWindow(false);
        GetDlgItem(IDC_BUTTON_REQUEST_USER).EnableWindow(false);
    }
    else
    {
        m_editKey.EnableWindow(false);
        GetDlgItem(IDC_BUTTON_ACTIVATE_KEY).EnableWindow(false);
        GetDlgItem(IDC_BUTTON_REQUEST_KEY).EnableWindow(false);

        m_editPassword.EnableWindow(true);
        m_editUser.EnableWindow(true);
        GetDlgItem(IDC_BUTTON_ACTIVATE_USER).EnableWindow(true);
        GetDlgItem(IDC_BUTTON_REQUEST_USER).EnableWindow(true);
    }

    return 0;
}

LRESULT ActivationDialog::OnBnClickedGetTrialLicense(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CString customerId;
    m_editUserID.GetWindowText(customerId);

    auto &lh = LicenseHandler::instance();
    auto licenseId = lh.getTrialLicense(std::string(CT2CA(customerId)));
    if (lh.wasError())
    {
        Utils::ShowWarningMsgBox("Get trial license error:\n" + lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }

    if (!licenseId.isValid())
    {
        Utils::ShowWarningMsgBox("Invalid trial license", m_hWnd);
        return 0;
    }

    // Set trial license data to UI
    if (licenseId.key().empty())
    {
        m_editUser.SetWindowText(CA2CT(licenseId.user().c_str()));
        m_editPassword.SetWindowText(CA2CT(licenseId.password().c_str()));
    }
    else
        m_editKey.SetWindowText(CA2CT(licenseId.key().c_str()));

    return 0;
}

LRESULT ActivationDialog::OnBnClickedOpenActivationFile(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CFileDialog fileDlg(true, _T( "lic" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "License Files\0*.lic\0All Files\0*.*\0" ));

    if (fileDlg.DoModal() != IDOK)
        return E_ABORT;

    auto &lh = LicenseHandler::instance();
    CString selectedFile = fileDlg.m_szFileName;
    if (selectedFile.IsEmpty())
        return E_FAIL;

    lh.activateLicenseOffline(std::wstring(CT2CW(selectedFile)));

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }
    assert(lh.isLicenseValid());

    runInfoDialog();
    return NO_ERROR;
}

LRESULT ActivationDialog::OnCtlColor(UINT, WPARAM wParam, LPARAM, BOOL &bHandled)
{
    CDCHandle dc = reinterpret_cast<HDC>(wParam);
    LOGBRUSH lb;
    if (!m_brBkgColor.m_hBrush)
        m_brBkgColor.CreateSolidBrush(CLR_CTRLBKG);
    m_brBkgColor.GetLogBrush(lb);
    dc.SetBkColor(lb.lbColor);

    bHandled = TRUE;
    return reinterpret_cast<LRESULT>(m_brBkgColor.m_hBrush);
}

// Initialize app, load icon, get resources, check license local if exists
LRESULT ActivationDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
    SetIcon(hIcon);

    if (!m_brBkgColor.m_hBrush)
        m_brBkgColor.CreateSolidBrush(CLR_CTRLBKG);

    m_editUser = GetDlgItem(IDC_EDIT_EMAIL);
    m_editPassword = GetDlgItem(IDC_EDIT_PASSWORD);
    m_editUserID = GetDlgItem(IDC_EDIT_USER_ID);
    m_editKey = GetDlgItem(IDC_EDIT_KEY);

    setWindowToCenter(GetDlgItem(IDC_LOGO));
    setWindowToCenter(GetDlgItem(IDC_PROMO_TEXT));

    PCTSTR toolTipText = _T( "Create request file for offline activation." );
    createToolTip(IDC_BUTTON_REQUEST_KEY, m_hWnd, toolTipText);
    createToolTip(IDC_BUTTON_REQUEST_USER, m_hWnd, toolTipText);

    auto &lh = LicenseHandler::instance();
    lh.reconfigure(AppConfig(APP_NAME, APP_VERSION).createLicenseSpringConfig());
    if (lh.isLicenseExists())
    {
        if (lh.isLicenseFloating())
            registerAndSetCallback();

        lh.checkLicenseLocal();
        if (lh.wasError())
            Utils::ShowWarningMsgBox(
                std::string("License local check failed:\n") + lh.getLastErrorMsg(), m_hWnd);

        runInfoDialog();
    }

    return ERROR_SUCCESS;
}

// Close app
LRESULT ActivationDialog::OnClose(
    UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
    EndDialog(0);
    return 0;
}

//-------------------- Private methods --------------------

void ActivationDialog::registerAndSetCallback()
{
    auto &lh = LicenseHandler::instance();
    lh.registerFloatingLicense();
    if (lh.wasError())
        Utils::ShowWarningMsgBox(
            std::string("Could not register floating license:\n") + lh.getLastErrorMsg(), m_hWnd);
    lh.setupLicenseWatchdog([this](const LicenseSpringException &ex) {
        Utils::ShowWarningMsgBox(
            "License check failed. Message: " + std::string(ex.what()), m_hWnd);

        if (ex.getCode() == eMaxFloatingReached)
        {
            Utils::ShowWarningMsgBox("Application cannot use this license at the moment because "
                                     "floating license limit reached.\n",
                m_hWnd);
            exit(0);
        }
        auto &lh = LicenseHandler::instance();
        if (lh.isLicenseValid() && lh.canIgnoreError())
            lh.resumeLicenseWatchdog();
    });
}

void ActivationDialog::runInfoDialog()
{
    ShowWindow(SW_HIDE);

    InfoDialog dlg;
    auto res = dlg.DoModal();

    if (InfoDialog::eEC_exitCode == res)
        EndDialog(0);
    else if (InfoDialog::eEC_licenseIsNotActive == res)
        ShowWindow(SW_SHOW);
}

void ActivationDialog::setWindowToCenter(HWND hWnd)
{
    if (!hWnd)
        return;

    CWindow window = hWnd;

    CRect parent, child;
    GetClientRect(parent);
    window.GetClientRect(child);

    auto screenParent = parent;
    auto screenChild = child;

    ClientToScreen(screenParent);
    window.ClientToScreen(screenChild);

    auto yPos = std::abs(screenChild.top - screenParent.top);

    auto width = child.Width();
    auto height = child.Height();
    child.left = parent.CenterPoint().x - width / 2;
    child.right = child.left + width;
    child.top = yPos;
    child.bottom = yPos + height;
    window.MoveWindow(child);
}

HWND ActivationDialog::createToolTip(int toolID, HWND hDlg, PCTSTR pszText)
{
    if (!toolID || !hDlg || !pszText)
        return FALSE;

    // Get the window of the tool
    HWND hwndTool = GetDlgItem(toolID);

    // Create the tooltip
    HWND hwndTip =
        CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hDlg, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndTool || !hwndTip)
        return (HWND)NULL;

    // Associate the tooltip with the tool
    TOOLINFO toolInfo = {0};
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = hDlg;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)hwndTool;
    toolInfo.lpszText = const_cast<TCHAR *>(pszText);
    SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

    return hwndTip;
}
