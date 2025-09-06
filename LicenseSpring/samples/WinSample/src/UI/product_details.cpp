#include "stdafx.h"

#include "resource.h"
#include "product_details.h"
#include "Utils/utils.h"
#include <LicenseSpring/LicenseHandler.h>

using namespace LicenseSpring;
using namespace WTLSample::UI;

LRESULT ProductDetailsDialog::OnBnClickedOK(
    WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
    EndDialog(0);
    return NO_ERROR;
}

LRESULT ProductDetailsDialog::OnCtlColor(
    UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
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

LRESULT ProductDetailsDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON));
    SetIcon(hIcon);

    if (!m_brBkgColor.m_hBrush)
        m_brBkgColor.CreateSolidBrush(CLR_CTRLBKG);

    m_editProductName = GetDlgItem(IDC_EDIT_PRODUCT_NAME);
    m_editProductCode = GetDlgItem(IDC_EDIT_PRODUCT_CODE);
    m_editIsTrial = GetDlgItem(IDC_EDIT_IS_TRIAL);
    m_editTrialPeriod = GetDlgItem(IDC_EDIT_TRIAL_PERIOD);
    m_editAuthorizationType = GetDlgItem(IDC_EDIT_AUTHORIZATION_TYPE);

    auto &lh = LicenseHandler::instance();
    auto details = lh.getProductDetails();

    if (lh.wasError())
    {
        Utils::ShowErrorMsgBox(lh.getLastErrorMsg(), m_hWnd);
        return E_FAIL;
    }

    if (details == nullptr)
        return E_FAIL;

    m_editProductName.SetWindowText(CA2CT(details->productName().c_str()));
    m_editProductCode.SetWindowText(CA2CT(details->productCode().c_str()));
    m_editIsTrial.SetWindowText(details->isTrialAllowed() ? CA2CT("True") : CA2CT("False"));
    m_editTrialPeriod.SetWindowText(CA2CT(std::to_string(details->trialPeriod()).c_str()));
    m_editAuthorizationType.SetWindowText(details->authorizationMethod() == AuthMethodKeyBased
                                              ? CA2CT("Key Based")
                                              : CA2CT("User Based"));
    return NO_ERROR;
}

LRESULT ProductDetailsDialog::OnClose(
    UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
    EndDialog(0);
    return NO_ERROR;
}