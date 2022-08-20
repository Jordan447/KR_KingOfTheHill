class KR_KingOfTheHillOldConfig
{
    string                                              m_ModVersion;
    float                                               m_CaptureTime;
    float                                               m_UpdateInterval;
    float                                               m_ServerStartDelay;
    float                                               m_HillEventInterval;
    float                                               m_EventCleanupTime;
    float                                               m_EventPreStart;
    string                                              m_EventPreStartMessage;
    string                                              m_EventCapturedMessage;
    string                                              m_EventDespawnedMessage;
    string                                              m_EventStartMessage;
    bool                                                m_DoLogsToCF;
    int                                                 m_PlayerPopulationToStartEvents;
    int                                                 m_MaxEvents;
    string                                              m_FlagName;
    autoptr array<autoptr KR_KingOfTheHillLocation>     m_HillLocations;
    autoptr array<autoptr KR_KingOfTheHillRewardPool>   m_RewardPools;
    autoptr TStringArray                                m_Creatures;
}