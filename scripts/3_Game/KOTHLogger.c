class KOTHLoggingService
{
    
    static void Log(string Message, bool Error = false)
    {
        #ifdef KING_OF_THE_HILL_DEBUG
        if(Error)
        {
            Error("[KOTH] -> " + Message);
        }
        else
        {
            Print("[KOTH] -> " + Message);
        }
        #endif
    }    
};