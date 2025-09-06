#pragma once

#include "resource.h"
#include "defines.h"
#include <atldlgs.h>
#include <LicenseSpring/LicenseFeature.h>

namespace WTLSample::UI
{
class InfoDialog : public CDialogImpl<InfoDialog>, public CWinDataExchange<InfoDialog>
{
public:
    enum EEndCode
    {
        eEC_exitCode = 0,
        eEC_licenseIsNotActive = 1
    };

    enum EInstallationFilesColumns
    {
        eIFC_version = 0,
        eIFC_url,
        eIFC_hash
    };

    enum ECustomFieldsColumns
    {
        eCFC_name = 0,
        eCFC_value
    };

    enum ELicenseFeaturesColumns
    {
        eLFC_code = 0,
        eLFC_name,
        eLFC_featureType,
        eLFC_maxConsumption,
        eLFC_totalConsumption,
        eLFC_localConsumption,
        eLFC_expiryDate,
        eLFC_allowOverages,
        eLFC_maxOverages,
        eLFC_resetConsumption,
        eLFC_consumptionPeriod
    };

    enum
    {
        IDD = IDD_INFO
    };

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnBnClickedCheckUpdates(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedGetLicenseFilePath(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedClearLicenseFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedCheckLicense(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedDeactivateLicense(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedUpdateConsumption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedSyncConsumption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedLoadVersionList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedProductDetails(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedUpdateFeatureConsumption(
        WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnBnClickedSyncFeatureConsumption(
        WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);

    BEGIN_MSG_MAP(InfoDialog)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
    MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
    COMMAND_HANDLER(IDC_BUTTON_CHECK_UPDATES, BN_CLICKED, OnBnClickedCheckUpdates)
    COMMAND_HANDLER(IDC_BUTTON_GET_LICENSE_FILE_PATH, BN_CLICKED, OnBnClickedGetLicenseFilePath)
    COMMAND_HANDLER(IDC_BUTTON_CLEAR_LICENSE_FOLDER, BN_CLICKED, OnBnClickedClearLicenseFolder)
    COMMAND_HANDLER(IDC_BUTTON_CHECK_LICENSE, BN_CLICKED, OnBnClickedCheckLicense)
    COMMAND_HANDLER(IDC_BUTTON_DEACTIVATE_LICENSE, BN_CLICKED, OnBnClickedDeactivateLicense)
    COMMAND_HANDLER(IDC_BUTTON_UPDATE_CONSUMPTION, BN_CLICKED, OnBnClickedUpdateConsumption)
    COMMAND_HANDLER(IDC_BUTTON_SYNC_CONSUMPTION, BN_CLICKED, OnBnClickedSyncConsumption)
    COMMAND_HANDLER(IDC_BUTTON_LOAD_VERSION_LIST, BN_CLICKED, OnBnClickedLoadVersionList)
    COMMAND_HANDLER(IDC_BUTTON_PRODUCT_DETAILS2, BN_CLICKED, OnBnClickedProductDetails)
    COMMAND_HANDLER(
        IDC_BUTTON_UPDATE_FEATURE_CONSUMPTION, BN_CLICKED, OnBnClickedUpdateFeatureConsumption)
    COMMAND_HANDLER(
        IDC_BUTTON_SYNC_FEATURE_CONSUMPTION, BN_CLICKED, OnBnClickedSyncFeatureConsumption)
    REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(InfoDialog)
    DDX_TEXT(IDC_EDIT_PATH, m_editPath)
    DDX_TEXT(IDC_EDIT_KEY, m_editKey)
    DDX_TEXT(IDC_EDIT_USER, m_editUser)
    DDX_TEXT(IDC_EDIT_TYPE, m_editType)
    DDX_TEXT(IDC_EDIT_STATUS, m_editStatus)
    DDX_TEXT(IDC_EDIT_ACTIVATED_OFFLINE, m_editIsActivatedOffline)
    DDX_TEXT(IDC_EDIT_TRIAL, m_editIsTrial)
    DDX_TEXT(IDC_EDIT_VALID, m_editIsValid)
    DDX_TEXT(IDC_EDIT_VALIDITY_PERIOD, m_editValidityPeriod)
    DDX_TEXT(IDC_EDIT_DAYS_REMAINING, m_editDaysRemaining)
    DDX_TEXT(IDC_EDIT_MAINTENANCE_PERIOD, m_editMaintenancePeriod)
    DDX_TEXT(IDC_EDIT_LAST_CHECKED, m_editLastChecked)
    DDX_TEXT(IDC_EDIT_FLOATING, m_editIsFloating)
    DDX_TEXT(IDC_EDIT_FLOATING_TIMEOUT, m_editFloatingTimeout)
    DDX_TEXT(IDC_EDIT_TOTAL_COUNT, m_editTotalCount)
    DDX_TEXT(IDC_EDIT_MAX_COUNT, m_editMaxCount)
    DDX_TEXT(IDC_EDIT_CURRENT_APP_VERSION, m_editCurrentApplicationVersion)
    DDX_CONTROL_HANDLE(IDC_LIST_INSTALLATION_FILES, m_listViewInstallationFiles)
    DDX_CONTROL_HANDLE(IDC_LIST_CUSTOM_FIELDS, m_listViewCustomFields)
    DDX_CONTROL_HANDLE(IDC_LIST_LICENSE_FEATURES, m_listViewLicenseFeatures)
    DDX_CONTROL_HANDLE(IDC_BUTTON_UPDATE_CONSUMPTION, m_buttonUpdateConsumption)
    DDX_CONTROL_HANDLE(IDC_BUTTON_SYNC_CONSUMPTION, m_buttonSyncConsumption)
    END_DDX_MAP()

private:
    CButton m_buttonUpdateConsumption;
    CButton m_buttonSyncConsumption;
    CBrush m_brBkgColor;
    CString m_editPath;
    CString m_editKey;
    CString m_editUser;
    CString m_editType;
    CString m_editStatus;
    CString m_editIsActivatedOffline;
    CString m_editIsTrial;
    CString m_editIsValid;
    CString m_editValidityPeriod;
    CString m_editDaysRemaining;
    CString m_editMaintenancePeriod;
    CString m_editLastChecked;
    CString m_editIsFloating;
    CString m_editFloatingTimeout;
    CString m_editTotalCount;
    CString m_editMaxCount;
    CString m_editCurrentApplicationVersion;
    CListViewCtrl m_listViewInstallationFiles;
    CListViewCtrl m_listViewCustomFields;
    CListViewCtrl m_listViewLicenseFeatures;
    CUpDownCtrl m_spinConsumption;

    std::vector<LicenseSpring::LicenseFeature> features;

    void UpDownChange(LPNMUPDOWN lpNMUpDown, UINT unControlID);
    void UpdateLicenseFeaturesTable();
    void UpdateInfoFields();
};
} // namespace WTLSample::UI
