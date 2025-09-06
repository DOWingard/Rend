#include "stdafx.h"
#include "info_dialog.h"
#include "UI/product_details.h"
#include "Utils/utils.h"

#include <shellapi.h>
#pragma comment(lib, "shell32")

#include <LicenseSpring/LicenseHandler.h>
#include <LicenseSpring/Version.h>

using namespace LicenseSpring;
using namespace WTLSample::UI;

LRESULT InfoDialog::OnClose(
    UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
    EndDialog(eEC_exitCode);
    return eEC_exitCode;
}

LRESULT InfoDialog::OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
{
    NMHDR *pNMHDR = (NMHDR *)lParam;

    switch (pNMHDR->code)
    {
    case UDN_DELTAPOS:
        UpDownChange((LPNMUPDOWN)lParam, (UINT)pNMHDR->idFrom);
        break;
    default:
        bHandled = FALSE; // not handled
    }

    return 0;
}

LRESULT InfoDialog::OnCtlColor(UINT, WPARAM wParam, LPARAM, BOOL &bHandled)
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

LRESULT InfoDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
    SetIcon(hIcon);

    if (!m_brBkgColor.m_hBrush)
        m_brBkgColor.CreateSolidBrush(CLR_CTRLBKG);

    DoDataExchange(DDX_LOAD);

    CRect rect;

    m_listViewLicenseFeatures.AddColumn(_T( "Code" ), eLFC_code);
    m_listViewLicenseFeatures.AddColumn(_T( "Name" ), eLFC_name);
    m_listViewLicenseFeatures.AddColumn(_T( "Type" ), eLFC_featureType);
    m_listViewLicenseFeatures.AddColumn(_T( "Max Consumption" ), eLFC_maxConsumption);
    m_listViewLicenseFeatures.AddColumn(_T( "Total Consumption" ), eLFC_totalConsumption);
    m_listViewLicenseFeatures.AddColumn(_T( "Local Consumption" ), eLFC_localConsumption);
    m_listViewLicenseFeatures.AddColumn(_T( "Allow Overages" ), eLFC_allowOverages);
    m_listViewLicenseFeatures.AddColumn(_T( "Max Overages" ), eLFC_maxOverages);
    m_listViewLicenseFeatures.AddColumn(_T( "Expiry Date" ), eLFC_expiryDate);
    m_listViewLicenseFeatures.AddColumn(_T( "Reset Consumption" ), eLFC_resetConsumption);
    m_listViewLicenseFeatures.AddColumn(_T( "Consumption Period" ), eLFC_consumptionPeriod);

    int columnCount = m_listViewLicenseFeatures.GetHeader().GetItemCount();
    m_listViewLicenseFeatures.GetClientRect(rect);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_code, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_name, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_featureType, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_maxConsumption, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_totalConsumption, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_localConsumption, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_allowOverages, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_maxOverages, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_expiryDate, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_resetConsumption, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetColumnWidth(eLFC_consumptionPeriod, rect.Width() / columnCount);
    m_listViewLicenseFeatures.SetExtendedListViewStyle(
        m_listViewLicenseFeatures.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT);

    UpdateInfoFields();

    m_listViewInstallationFiles.AddColumn(_T( "Version" ), eIFC_version);
    m_listViewInstallationFiles.AddColumn(_T( "Url" ), eIFC_url);
    m_listViewInstallationFiles.AddColumn(_T( "Hash" ), eIFC_hash);

    columnCount = m_listViewInstallationFiles.GetHeader().GetItemCount();
    m_listViewInstallationFiles.GetClientRect(rect);
    m_listViewInstallationFiles.SetColumnWidth(eIFC_version, rect.Width() / columnCount);
    m_listViewInstallationFiles.SetColumnWidth(eIFC_url, rect.Width() / columnCount);
    m_listViewInstallationFiles.SetColumnWidth(eIFC_hash, rect.Width() / columnCount);

    m_listViewCustomFields.AddColumn(_T( "Name" ), eCFC_name);
    m_listViewCustomFields.AddColumn(_T( "Value" ), eCFC_value);

    columnCount = m_listViewCustomFields.GetHeader().GetItemCount();
    m_listViewCustomFields.GetClientRect(rect);
    m_listViewCustomFields.SetColumnWidth(eCFC_name, rect.Width() / columnCount);
    m_listViewCustomFields.SetColumnWidth(eCFC_value, rect.Width() / columnCount);

    auto &lh = LicenseHandler::instance();
    auto fields = lh.getLicenseCustomFields();
    for (int i = 0; i < static_cast<int>(fields.size()); ++i)
    {
        m_listViewCustomFields.AddItem(i, eCFC_name, CA2T(fields[i].fieldName().c_str()));
        m_listViewCustomFields.AddItem(i, eCFC_value, CA2T(fields[i].fieldValue().c_str()));
    }

    m_spinConsumption = GetDlgItem(IDC_SPIN_CONSUMPTION);
    m_spinConsumption.SetRange(-1000, 1000);
    m_spinConsumption.SetPos(0);
    m_spinConsumption.SetBuddy(GetDlgItem(IDC_EDIT_CONSUMPTION));
    GetDlgItem(IDC_EDIT_CONSUMPTION).SetWindowText(CA2T("0"));

    m_spinConsumption = GetDlgItem(IDC_SPIN_FEATURE_CONSUMPTION);
    m_spinConsumption.SetRange(-1000, 1000);
    m_spinConsumption.SetPos(0);
    m_spinConsumption.SetBuddy(GetDlgItem(IDC_EDIT_FEATURE_CONSUMPTION));
    GetDlgItem(IDC_EDIT_FEATURE_CONSUMPTION).SetWindowText(CA2T("0"));

    auto type = lh.getLicenseType();
    if (type != LicenseTypeConsumption)
    {
        m_buttonUpdateConsumption.EnableWindow(FALSE);
        m_buttonSyncConsumption.EnableWindow(FALSE);
        GetDlgItem(IDC_SPIN_CONSUMPTION).EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_CONSUMPTION).EnableWindow(FALSE);
    }

    return TRUE;
}

LRESULT InfoDialog::OnBnClickedCheckUpdates(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();

    std::string version;
    auto installFile = lh.getInstallFile();
    if (installFile)
        version = installFile->version();

    if (version.empty() || lh.wasError())
    {
        std::string msg = "Couldn't get app version info.";
        if (lh.wasError())
            msg.append("\n" + lh.getLastErrorMsg());
        Utils::ShowErrorMsgBox(msg);
        return E_FAIL;
    }

    Version serverVer(version);
    Version currentVer(APP_VERSION);

    if (currentVer >= serverVer)
        Utils::ShowInfoMsgBox("The application is up to date.");
    else
        Utils::ShowInfoMsgBox(
            "There is a new version of the application available."); // propose download and install

    return NO_ERROR;
}

// Open license file location
LRESULT InfoDialog::OnBnClickedGetLicenseFilePath(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();
    if (!lh.isLicenseExists())
    {
        Utils::ShowErrorMsgBox("License not found!", m_hWnd);
        return E_FAIL;
    }

    auto dirPath = lh.dataLocation();
    ShellExecuteW(NULL, L"open", dirPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    return NO_ERROR;
}

LRESULT InfoDialog::OnBnClickedClearLicenseFolder(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();
    lh.clearLocalStorage();
    EndDialog(eEC_licenseIsNotActive);
    return S_OK;
}

// Check license
LRESULT InfoDialog::OnBnClickedCheckLicense(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();
    LRESULT checkResult = S_OK;

    lh.checkLicenseLocal();
    if (lh.wasError())
    {
        checkResult = E_FAIL;
        Utils::ShowErrorMsgBox("Local license check failed: " + lh.getLastErrorMsg());
    }

    lh.checkLicense();
    UpdateInfoFields();
    if (lh.wasError())
    {
        checkResult = E_FAIL;
        Utils::ShowErrorMsgBox("Online license check failed: " + lh.getLastErrorMsg());
    }

    if (SUCCEEDED(checkResult))
        Utils::ShowInfoMsgBox("License check passed successfully.");

    return checkResult;
}

LRESULT InfoDialog::OnBnClickedDeactivateLicense(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();

    // try to deactivate license
    lh.deactivateLicense();

    // if it was error with internet connection propose create request file for offline deactivation
    if (lh.wasError() && lh.isNoInternetError())
    {
        std::string str = lh.getLastErrorMsg() +
                          "\n Would you like to make request file for offline deactivation?";

        if (IDOK ==
            MessageBoxA(0, str.data(), "License notification", MB_OKCANCEL | MB_ICONWARNING))
        {
            CFileDialog saveDlg(false, _T( "req" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
                _T( "Request Files\0*.req\0All Files\0*.*\0" ));

            auto result = saveDlg.DoModal();

            std::filesystem::path path = (IDOK == result) ? saveDlg.m_szFileName : _T( "" );
            path = lh.deactivateLicenseOffline(path.wstring());
            Utils::ShowInfoMsgBox("Request path: " + path.string());
            return NO_ERROR;
        }
        return E_FAIL;
    }

    // if there wasn't an error - open activation screen
    if (!lh.wasError())
    {
        EndDialog(eEC_licenseIsNotActive);
        return eEC_licenseIsNotActive;
    }

    Utils::ShowErrorMsgBox(lh.getLastErrorMsg());

    return NO_ERROR;
}

LRESULT InfoDialog::OnBnClickedUpdateConsumption(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CEdit cEdit = (CEdit)GetDlgItem(IDC_EDIT_CONSUMPTION);

    CString buffer;
    cEdit.GetWindowText(buffer);
    int value = ::_ttoi(buffer);
    ;

    auto &lh = LicenseHandler::instance();
    lh.updateConsumption(value);

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg());
        return E_FAIL;
    }

    auto totalCount = std::to_string(lh.getLicenseTotalConsumption());
    m_editTotalCount = CA2T(totalCount.c_str());
    if (lh.isLicenseUnlimitedConsumptionAllowed())
        m_editMaxCount = CA2T("Unlimited");
    else
    {
        auto maxCount = std::to_string(lh.getLicenseMaxConsumption());
        m_editMaxCount = CA2T(maxCount.c_str());
    }

    DoDataExchange(DDX_LOAD, IDC_EDIT_TOTAL_COUNT);
    DoDataExchange(DDX_LOAD, IDC_EDIT_MAX_COUNT);

    cEdit.SetWindowText(CA2T(std::to_string(value).c_str()));
    return NO_ERROR;
}

LRESULT InfoDialog::OnBnClickedSyncConsumption(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();
    lh.syncConsumption();
    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg());
        return E_FAIL;
    }
    auto totalCount = std::to_string(lh.getLicenseTotalConsumption());
    m_editTotalCount = CA2T(totalCount.c_str());
    if (lh.isLicenseUnlimitedConsumptionAllowed())
        m_editMaxCount = CA2T("Unlimited");
    else
    {
        auto maxCount = std::to_string(lh.getLicenseMaxConsumption());
        m_editMaxCount = CA2T(maxCount.c_str());
    }

    DoDataExchange(DDX_LOAD, IDC_EDIT_TOTAL_COUNT);
    DoDataExchange(DDX_LOAD, IDC_EDIT_MAX_COUNT);

    Utils::ShowInfoMsgBox("Consumption synchronized!");

    return NO_ERROR;
}

// Get version list and then get url and hash for each version
LRESULT InfoDialog::OnBnClickedLoadVersionList(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    auto &lh = LicenseHandler::instance();
    auto list = lh.getVersionList();
    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg());
        return E_FAIL;
    }

    for (int i = 0; i < static_cast<int>(list.size()); ++i)
    {
        auto installFile = lh.getInstallFile(LicenseID(), list[i]);
        if (lh.wasError())
        {
            Utils::ShowErrorMsgBox("Failed to get install file info for app v" + list[i] + "\n " +
                                   lh.getLastErrorMsg());
            return E_FAIL;
        }

        m_listViewInstallationFiles.AddItem(i, eIFC_version, CA2T(installFile->version().c_str()));
        m_listViewInstallationFiles.AddItem(i, eIFC_url, CA2T(installFile->url().c_str()));
        m_listViewInstallationFiles.AddItem(i, eIFC_hash, CA2T(installFile->md5Hash().c_str()));
    }

    return NO_ERROR;
}

LRESULT InfoDialog::OnBnClickedProductDetails(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    ProductDetailsDialog dlg;
    auto res = dlg.DoModal();
    return 0;
}

LRESULT InfoDialog::OnBnClickedUpdateFeatureConsumption(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    CEdit cEdit = (CEdit)GetDlgItem(IDC_EDIT_FEATURE_CONSUMPTION);
    CString buffer;
    cEdit.GetWindowText(buffer);

    int value = ::_ttoi(buffer);
    int selectedIndex = m_listViewLicenseFeatures.GetSelectedIndex();

    if (selectedIndex < 0)
    {
        int featuresCount = static_cast<int>(features.size());
        for (int i = 0; i < featuresCount; i++)
        {
            if (features[i].featureType() == FeatureTypeConsumption)
            {
                selectedIndex = i;
                break;
            }
        }
        if (selectedIndex < 0)
        {
            Utils::ShowWarningMsgBox("Consumption features not found.");
            return E_FAIL;
        }
    }

    auto &lh = LicenseHandler::instance();
    lh.updateFeatureConsumption(features[selectedIndex].code(), value);

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg());
        return E_FAIL;
    }

    UpdateLicenseFeaturesTable();
    m_listViewLicenseFeatures.SelectItem(selectedIndex);

    cEdit.SetWindowText(CA2T(std::to_string(value).c_str()));
    return NO_ERROR;
}

LRESULT InfoDialog::OnBnClickedSyncFeatureConsumption(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    int selectedIndex = m_listViewLicenseFeatures.GetSelectedIndex();

    auto &lh = LicenseHandler::instance();

    if (selectedIndex >= 0)
        lh.syncFeatureConsumption(features[selectedIndex].code());
    else
        lh.syncFeatureConsumption(); // sync all consumption features

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg());
        return E_FAIL;
    }

    UpdateLicenseFeaturesTable();
    return NO_ERROR;
}

//-------------------- Private methods --------------------

void InfoDialog::UpDownChange(LPNMUPDOWN lpNMUpDown, UINT unControlID)
{
    m_spinConsumption = GetDlgItem(unControlID);
    CEdit cEdit = (CEdit)m_spinConsumption.GetBuddy();

    CString szBuffer;
    cEdit.GetWindowText(szBuffer);

    int nValue = ::_ttoi(szBuffer);
    ;

    int nLower, nUpper = 0;
    m_spinConsumption.GetRange(nLower, nUpper);

    if (lpNMUpDown->iDelta > 0)
    {
        if (++nValue > nUpper)
            nValue = nLower;
    }
    else if (lpNMUpDown->iDelta < 0)
        nValue--;

    cEdit.SetWindowText(CA2T(std::to_string(nValue).c_str()));
}

void InfoDialog::UpdateLicenseFeaturesTable()
{
    auto &lh = LicenseHandler::instance();
    features = lh.getLicenseFeatures();
    m_listViewLicenseFeatures.DeleteAllItems();
    for (int i = 0; i < static_cast<int>(features.size()); ++i)
    {
        m_listViewLicenseFeatures.AddItem(i, eLFC_code, CA2T(features[i].code().c_str()));
        m_listViewLicenseFeatures.AddItem(i, eLFC_name, CA2T(features[i].name().c_str()));
        m_listViewLicenseFeatures.AddItem(i, eLFC_featureType,
            CA2T(features[i].FeatureTypeToString(features[i].featureType()).c_str()));
        if (features[i].isUnlimitedConsumptionAllowed())
            m_listViewLicenseFeatures.AddItem(i, eLFC_maxConsumption, CA2T("Unlimited"));
        else
            m_listViewLicenseFeatures.AddItem(
                i, eLFC_maxConsumption, CA2T(std::to_string(features[i].maxConsumption()).c_str()));
        m_listViewLicenseFeatures.AddItem(
            i, eLFC_totalConsumption, CA2T(std::to_string(features[i].totalConsumption()).c_str()));
        m_listViewLicenseFeatures.AddItem(
            i, eLFC_localConsumption, CA2T(std::to_string(features[i].localConsumption()).c_str()));
        m_listViewLicenseFeatures.AddItem(
            i, eLFC_allowOverages, CA2T(features[i].isOveragesAllowed() ? "True" : "False"));
        m_listViewLicenseFeatures.AddItem(
            i, eLFC_maxOverages, CA2T(std::to_string(features[i].maxOverages()).c_str()));
        m_listViewLicenseFeatures.AddItem(
            i, eLFC_expiryDate, CA2T(TmToString(features[i].expiryDate()).c_str()));
        m_listViewLicenseFeatures.AddItem(i, eLFC_resetConsumption,
            CA2T(features[i].isResetConsumptionEnabled() ? "True" : "False"));
        m_listViewLicenseFeatures.AddItem(i, eLFC_consumptionPeriod,
            CA2T(features[i].consumptionPeriod().toFormattedString().c_str()));
    }
}

void InfoDialog::UpdateInfoFields()
{
    auto &lh = LicenseHandler::instance();
    m_editKey = CA2T(lh.getLicenseKey().c_str());
    m_editUser = CA2T(lh.getLicenseUser().c_str());
    auto type = lh.getLicenseType();
    m_editType = CA2T(type.toFormattedString().c_str());
    m_editStatus = CA2T(lh.getLicenseStatusStr().c_str());
    m_editIsActivatedOffline = lh.isLicenseOfflineActivated() ? "True" : "False";
    m_editIsTrial = lh.isLicenseTrial() ? "True" : "False";
    m_editIsValid = lh.isLicenseValid() ? "True" : "False";
    m_editValidityPeriod = CA2T(TmToString(lh.getLicenseExpiryDateUtc()).c_str());
    m_editDaysRemaining = CA2T(std::to_string(lh.getDaysRemaining()).c_str());
    m_editMaintenancePeriod = CA2T(TmToString(lh.getLicenseMaintenancePeriodUtc()).c_str());
    m_editLastChecked = CA2T(TmToString(lh.getLicenseLastCheckDate(), "%d-%m-%Y %T").c_str());
    m_editIsFloating = lh.isLicenseFloating() ? "True" : "False";
    m_editFloatingTimeout = CA2T(std::to_string(lh.getLicenseFloatingTimeout()).c_str());
    auto totalCount = std::to_string(lh.getLicenseTotalConsumption());
    m_editTotalCount = CA2T(totalCount.c_str());
    if (lh.isLicenseUnlimitedConsumptionAllowed())
        m_editMaxCount = CA2T("Unlimited");
    else
    {
        auto maxCount = std::to_string(lh.getLicenseMaxConsumption());
        m_editMaxCount = CA2T(maxCount.c_str());
    }
    m_editCurrentApplicationVersion = _T(APP_VERSION);
    m_editPath = CW2T(lh.licenseFilePath().c_str());

    UpdateLicenseFeaturesTable();

    DoDataExchange(DDX_LOAD);
}
