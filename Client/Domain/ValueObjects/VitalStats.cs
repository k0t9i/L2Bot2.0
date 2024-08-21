﻿using Client.Domain.Common;
using System;

namespace Client.Domain.ValueObjects
{
    public class VitalStats : ObservableObject
    {
        private uint hp;
        private uint maxHp;
        private uint mp;
        private uint maxMp;
        private uint cp;
        private uint maxCp;
        private bool isDead;

        public uint Hp { get => hp; set { if (value != hp) { hp = value; OnPropertyChanged(); OnPropertyChanged("MaxHp"); } } }
        public uint MaxHp { get => Math.Max(hp, maxHp); set { if (value != maxHp) { maxHp = value; OnPropertyChanged(); } } }
        public uint Mp { get => mp; set { if (value != mp) { mp = value; OnPropertyChanged(); } } }
        public uint MaxMp { get => maxMp; set { if (value != maxMp) { maxMp = value; OnPropertyChanged(); } } }
        public uint Cp { get => cp; set { if (value != cp) { cp = value; OnPropertyChanged(); } } }
        public uint MaxCp { get => maxCp; set { if (value != maxCp) { maxCp = value; OnPropertyChanged(); } } }
        public bool IsDead { get => isDead; set { if (value != isDead) { isDead = value; OnPropertyChanged(); } } }

        public double HpPercent
        {
            get
            {
                return maxHp != 0 ? hp / (double)maxHp * 100 : 0;
            }
        }
        public double MpPercent
        {
            get
            {
                return maxMp != 0 ? mp / (double)maxMp * 100 : 0;
            }
        }
        public double CpPercent
        {
            get
            {
                return maxCp != 0 ? cp / (double)maxCp * 100 : 0;
            }
        }

        public VitalStats(uint hp, uint maxHp, uint mp, uint maxMp, uint cp, uint maxCp, bool isDead)
        {
            this.hp = hp;
            this.maxHp = maxHp;
            this.mp = mp;
            this.maxMp = maxMp;
            this.cp = cp;
            this.maxCp = maxCp;
            this.isDead = isDead;
        }
    }
}
