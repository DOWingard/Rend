#pragma once

#include "resource.h"
#include "defines.h"
#include <atldlgs.h>

namespace WTLSample::UI
{
class ProductDetailsDialog : public CDialogImpl<ProductDetailsDialog>
{
public:
    enum
    {
        IDD = IDD_PRODUCT_DETAILS
    };

    LRESULT OnBnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    BEGIN_MSG_MAP(ProductDetailsDialog)
    COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOK)
    MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
    MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    END_MSG_MAP()

private:
    CBrush m_brBkgColor;
    CEdit m_editProductName;
    CEdit m_editProductCode;
    CEdit m_editIsTrial;
    CEdit m_editTrialPeriod;
    CEdit m_editAuthorizationType;
};
} // namespace WTLSample::UI
