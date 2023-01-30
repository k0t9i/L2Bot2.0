using Client.Domain.Common;

namespace Client.Domain.ValueObjects
{
    public class ExperienceInfo : NotifyPropertyChanged
    {
        private uint level;
        private uint exp;
        private uint sp;
        private ulong expToLevel;
        private ulong expPrevToLevel;

        public uint Level { get => level; set { if (value != level) { level = value; OnPropertyChanged("Level"); } } }
        public uint Exp { get => exp; set { if (value != exp) { exp = value; OnPropertyChanged("Exp"); } } }
        public uint Sp { get => sp; set { if (value != sp) { sp = value; OnPropertyChanged("Sp"); } } }
        public ulong ExpToLevel { get => expToLevel; set { if (value != expToLevel) { expToLevel = value; OnPropertyChanged("ExpToLevel"); } } }
        public ulong ExpToPrevLevel { get => expPrevToLevel; set { if (value != expPrevToLevel) { expPrevToLevel = value; OnPropertyChanged("ExpToPrevLevel"); } } }
        public double ExpPercent
        {
            get
            {
                return ExpToLevel != 0 ? (Exp - ExpToPrevLevel) / (double) (ExpToLevel - ExpToPrevLevel) * 100 : 0;
            }
        }

        public ExperienceInfo(uint level, uint exp, uint sp)
        {
            this.level = level;
            this.exp = exp;
            this.sp = sp;
        }
    }
}
