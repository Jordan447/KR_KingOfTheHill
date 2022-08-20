class KR_KingOfTheHillRewardPool
{
    string RewardContainerName;
    autoptr array<autoptr KR_KingOfTheHillReward> m_Rewards;
    int m_LootTier;
    int m_MaxWinningItems;
    int m_MinWinningItems;
    void KR_KingOfTheHillRewardPool(string ContainerName, autoptr array <autoptr KR_KingOfTheHillReward> Rewards, int LootTier, int MaxWinningItems, int MinWinningItems)
    {
        this.RewardContainerName = ContainerName;
        this.m_LootTier = LootTier;
        this.m_Rewards = Rewards;
        this.m_MaxWinningItems = MaxWinningItems;
        this.m_MinWinningItems = MinWinningItems;
    }

    void SetNewDefaultValues()
    {
        m_LootTier = 1;
        m_MaxWinningItems = m_Rewards.Count();
        m_MinWinningItems = 1;
        foreach(KR_KingOfTheHillReward reward: m_Rewards)
        {
            reward.SetNewDefaultValues();
        }
    }
};