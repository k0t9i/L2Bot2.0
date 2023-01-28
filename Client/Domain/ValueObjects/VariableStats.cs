namespace Client.Domain.ValueObjects
{
    public class VariableStats
    {
        public uint Accuracy { get; set; }
        public uint CritRate { get; set; }
        public uint PAttack { get; set; }
        public uint AttackSpeed { get; set; }
        public uint PDefense { get; set; }
        public uint Evasion { get; set; }
        public uint MAttack { get; set; }
        public uint MDefense { get; set; }
        public uint CastingSpeed { get; set; }

        public VariableStats(uint accuracy, uint critRate, uint pAttack, uint attackSpeed, uint pDefense, uint evasion, uint mAttack, uint mDefense, uint castingSpeed)
        {
            Accuracy = accuracy;
            CritRate = critRate;
            PAttack = pAttack;
            AttackSpeed = attackSpeed;
            PDefense = pDefense;
            Evasion = evasion;
            MAttack = mAttack;
            MDefense = mDefense;
            CastingSpeed = castingSpeed;
        }
    }
}
