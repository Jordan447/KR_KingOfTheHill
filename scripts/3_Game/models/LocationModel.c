class KR_KingOfTheHillLocation
{
    string Name;
    float Radius;
    float CaptureRadius;
    bool ResetCaptureTimeAfterAllPlayersLeavedZone;
    vector Position;
    int AISpawnCount;
    string ObjectListName;
    int LootTier;

    void KR_KingOfTheHillLocation(string ZoneName, float Radius, float CaptureRadius, bool ResetCaptureTimeAfterAllPlayersLeavedZone, vector Position, int AISpawnCount, int LootTier, string ObjectListName = "none")
    {
        this.Name = ZoneName;
        this.Radius = Radius;
        this.CaptureRadius = Radius;
        this.ResetCaptureTimeAfterAllPlayersLeavedZone = ResetCaptureTimeAfterAllPlayersLeavedZone;
        this.Position = Position;
        this.AISpawnCount = AISpawnCount;
        this.ObjectListName = ObjectListName;
        this.LootTier = LootTier;
    }

    void SetNewDefaultValues()
    {
        CaptureRadius = Radius;
        ResetCaptureTimeAfterAllPlayersLeavedZone = true;
        LootTier = 1;
    }   
};