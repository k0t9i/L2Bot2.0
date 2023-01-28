namespace Client.Domain.ValueObjects
{
    public class Reputation
    {
        public uint Karma { get; set; }
        public uint PkKills { get; set; }
        public uint PvpKills { get; set; }
        public uint RecRemaining { get; set; }
        public uint EvalScore { get; set; }

        public Reputation(uint karma, uint pkKills, uint pvpKills, uint recRemaining, uint evalScore)
        {
            Karma = karma;
            PkKills = pkKills;
            PvpKills = pvpKills;
            RecRemaining = recRemaining;
            EvalScore = evalScore;
        }
    }
}
