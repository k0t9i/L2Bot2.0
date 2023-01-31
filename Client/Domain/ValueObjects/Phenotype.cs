using Client.Domain.Common;
using Client.Domain.Enums;

namespace Client.Domain.ValueObjects
{
    public class Phenotype : ObservableObject
    {
        private RaceEnum race;
        private bool isMale;
        private ClassEnum @class;
        private ClassEnum activeClass;

        public RaceEnum Race { get => race; set { if (value != race) { race = value; OnPropertyChanged("Race"); } } }
        public bool IsMale { get => isMale; set { if (value != isMale) { isMale = value; OnPropertyChanged("IsMale"); } } }
        public ClassEnum Class { get => @class; set { if (value != @class) { @class = value; OnPropertyChanged("Class"); } } }
        public ClassEnum ActiveClass { get => activeClass; set { if (value != activeClass) { activeClass = value; OnPropertyChanged("ActiveClass"); } } }

        public Phenotype(RaceEnum race, bool isMale, ClassEnum @class, ClassEnum activeClass)
        {
            this.race = race;
            this.isMale = isMale;
            this.@class = @class;
            this.activeClass = activeClass;
        }
    }
}
