namespace Client.Domain.ValueObjects
{
    public class ExperienceInfo
    {
        public uint Level { get; set; }
        public uint Exp { get; set; }
        public uint Sp { get; set; }

        public ExperienceInfo(uint level, uint exp, uint sp)
        {
            Level = level;
            Exp = exp;
            Sp = sp;
        }
    }
}
