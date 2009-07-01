class TToolTip
{

private:
    HWND m_hwndTooltip;
    TOOLINFO m_ToolInfo;
    bool m_TrackingMouse;

public:
    TToolTip(HWND hwndOwner)
    {
        m_hwndTooltip = CreateTrackingToolTip(hwndOwner);
        m_TrackingMouse = false;
    }

    virtual ~TToolTip()
    {
    }

    //-------------------------------------------------------------------------
    void Show()
    {
        if (!m_TrackingMouse)
        {
            ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, (WPARAM)true, (LPARAM)&m_ToolInfo);
            m_TrackingMouse = true;
        }
    }

    //-------------------------------------------------------------------------
    void Hide()
    {
        if (m_TrackingMouse)
        {
            SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM)&m_ToolInfo);
            m_TrackingMouse = false;
        }
    }

    //-------------------------------------------------------------------------
    void Update(const TRect& rc)
    {
        Show();

        // Update tooltip text.
        String sCoords;
        sCoords.printf("X: %d  Y: %d  W: %d  H: %d", rc.left, rc.top, rc.Width(), rc.Height());

        // Calculate the space required for our tooltip
        SIZE tipsize;
        HDC dcTooltip = ::GetDC(m_hwndTooltip);
        ::GetTextExtentPoint32(dcTooltip, sCoords.c_str(), sCoords.Length(), &tipsize);
        ::ReleaseDC(m_hwndTooltip, dcTooltip);

        // Set the tooltip text
        m_ToolInfo.lpszText = sCoords.c_str();
        ::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);

        // Set the tooltip position.
        // Tooltip shows above left-top of window unless it is
        // off-screen, in which case we position it at the screen edge
        POINT pt = { rc.left, rc.top - tipsize.cy - 1};

        // Stay on screen
        if (rc.left < 0)
        {
            pt.x = 0;
        }

        if (rc.left > Screen->DesktopWidth - tipsize.cx)
        {
            pt.x = Screen->DesktopWidth - tipsize.cx;
        }

        if (rc.top < tipsize.cy)
        {
            pt.y = rc.top + rc.Height();
        }

        ::SendMessage(m_hwndTooltip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x, pt.y));
    }

private:

    //-------------------------------------------------------------------------
    HWND CreateTrackingToolTip(HWND hwndOwner)
    {
        // Create a tooltip window.
        HWND hwndToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                          hwndOwner, NULL, Application->Handle, NULL);

        if (!hwndToolTip)
        {
            return NULL;
        }

        // Set up tool information.
        // In this case, the "tool" is the entire parent window.
        m_ToolInfo.cbSize = sizeof(TOOLINFO);
        m_ToolInfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
        m_ToolInfo.hwnd = hwndOwner;
        m_ToolInfo.hinst = Application->Handle;
        m_ToolInfo.lpszText = "pText";
        m_ToolInfo.uId = (UINT_PTR)hwndOwner;
        ::GetClientRect (hwndOwner, &m_ToolInfo.rect);

        // Associate the ToolTip with the tool window.
        ::SendMessage(hwndToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);

        return hwndToolTip;
    }

};

