class KR_KingOfTheHillClientSyncModel
{
    protected string m_PlayerCount;
    protected float m_CaptureProgress;
    protected bool m_LeavedZone;

    void SetData(string PlayerCount, float CaptureProgress, bool LeavedZone)
    {
        this.m_PlayerCount = PlayerCount;
        this.m_CaptureProgress = CaptureProgress;
        this.m_LeavedZone = LeavedZone;
    }

    string GetPlayerCount()
    {
        return this.m_PlayerCount;
    }

    float GetProgress()
    {
        return this.m_CaptureProgress;
    }

    bool GetLeavedZone()
    {
        return this.m_LeavedZone;
    }
};