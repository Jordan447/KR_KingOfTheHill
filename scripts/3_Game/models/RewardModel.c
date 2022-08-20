class KR_KingOfTheHillReward
{
    string ItemName;
    int Quantity;
    autoptr TStringArray Attachments;
    
    void KR_KingOfTheHillReward(string ItemName, TStringArray Attachments, int Quantity)
    {
        this.ItemName = ItemName;
        this.Attachments = Attachments;
        this.Quantity = Quantity;
    }

    void SetNewDefaultValues()
    {
        Quantity = 1;
    }
};