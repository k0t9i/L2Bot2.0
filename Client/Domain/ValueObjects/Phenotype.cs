using Client.Domain.Enums;

namespace Client.Domain.ValueObjects
{
    public class Phenotype
    {
        public RaceEnum Race { get; set; }
        public bool IsMale { get; set; }
        public ClassEnum Class { get; set; }
        public ClassEnum ActiveClass { get; set; }

        public Phenotype(RaceEnum race, bool isMale, ClassEnum @class, ClassEnum activeClass)
        {
            Race = race;
            IsMale = isMale;
            Class = @class;
            ActiveClass = activeClass;
        }
    }
}
