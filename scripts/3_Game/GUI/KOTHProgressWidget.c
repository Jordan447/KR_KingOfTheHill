class KR_KingOfTheHillProgressWidget
{
    autoptr KR_KingOfTheHillClientSyncModel m_serverData;
    protected ref Widget				    m_Root;
    protected ref Widget					m_HandlePanel;
    protected ref Widget                    m_InfoPanel;
    protected TextWidget                    m_ZoneText;

    protected TextWidget                    m_PlayerCount;
    protected ProgressBarWidget             m_ProgressBar;
    protected ImageWidget                   m_Icon;

    void KR_KingOfTheHillProgressWidget()
    {
        m_Root 			= GetGame().GetWorkspace().CreateWidgets(GetLayoutPath());
        m_HandlePanel   = m_Root.FindAnyWidget("MainPanel");
        m_InfoPanel     = m_Root.FindAnyWidget("InfoPanel");
        m_ZoneText      = TextWidget.Cast(m_Root.FindAnyWidget("Text"));
        m_PlayerCount   = TextWidget.Cast(m_Root.FindAnyWidget("PlayerCount"));
        m_ProgressBar   = ProgressBarWidget.Cast(m_Root.FindAnyWidget("ProgressBar"));
        m_Icon          = ImageWidget.Cast(m_Root.FindAnyWidget("Icon"));
        m_Icon.LoadImageFile(0, GetImageIcon(), true);

        HandleShow(false);
    }

    void ~KR_KingOfTheHillProgressWidget()
    {
        m_Root.Unlink();
    }

    void RefreshHud()
    {
        m_PlayerCount.SetText(m_serverData.GetPlayerCount());
        m_ProgressBar.SetCurrent(m_serverData.GetProgress());
    }

    void UpdateInfo(KR_KingOfTheHillClientSyncModel data)
    {
        m_serverData = data;
        RefreshHud();
    }

    void HandleShow(bool show)
    {
        m_HandlePanel.Show(show);
        m_InfoPanel.Show(show);
    }

    /* 
    For Better Modding Support! 
    If you want to use diffrent Layout use:

    modded class KR_KingOfTheHillProgressWidget
    {
        overrride string GetLayoutPath()
        {
            return "YourMod/layout.layout";
        }
    }
    */
    string GetLayoutPath()
    {
        return "KR_KingOfTheHill/GUI/layouts/KOTH_ZoneWidget.layout";
    }

    //Same here!
    string GetImageIcon()
    {
        return "KR_KingOfTheHill/GUI/images/FlagIcon.edds";
    }
};