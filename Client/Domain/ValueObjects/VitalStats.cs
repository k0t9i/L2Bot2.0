namespace Client.Domain.ValueObjects
{
    public class VitalStats
    {
        public uint Hp { get; set; }
        public uint MaxHp { get; set; }
        public uint Mp { get; set; }
        public uint MaxMp { get; set; }
        public uint Cp { get; set; }
        public uint MaxCp { get; set; }

        public VitalStats(uint hp, uint maxHp, uint mp, uint maxMp, uint cp, uint maxCp)
        {
            Hp = hp;
            MaxHp = maxHp;
            Mp = mp;
            MaxMp = maxMp;
            Cp = cp;
            MaxCp = maxCp;
        }
    }
}
