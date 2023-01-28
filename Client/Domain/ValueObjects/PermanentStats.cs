namespace Client.Domain.ValueObjects
{
    public class PermanentStats
    {
        public uint Str { get; set; }
        public uint Dex { get; set; }
        public uint Con { get; set; }
        public uint Int { get; set; }
        public uint Men { get; set; }
        public uint Wit { get; set; }

        public PermanentStats(uint str, uint dex, uint con, uint @int, uint men, uint wit)
        {
            Str = str;
            Dex = dex;
            Con = con;
            Int = @int;
            Men = men;
            Wit = wit;
        }
    }
}
