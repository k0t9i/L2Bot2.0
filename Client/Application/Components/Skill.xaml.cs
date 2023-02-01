using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client.Application.Components
{
    /// <summary>
    /// Interaction logic for Skill.xaml
    /// </summary>
    public partial class Skill : Button
    {
        public static readonly DependencyProperty IsButtonActiveProperty =
            DependencyProperty.Register("IsButtonActive", typeof(bool), typeof(Skill), new PropertyMetadata(true, null, OnCoerceButtonActivity));
        public static readonly DependencyProperty ImageSourceProperty =
            DependencyProperty.Register("ImageSource", typeof(ImageSource), typeof(Skill), new PropertyMetadata(default(ImageSource)));
        public static readonly DependencyProperty IsReadyToUseProperty =
            DependencyProperty.Register("IsReadyToUse", typeof(bool), typeof(Skill), new PropertyMetadata(true, OnButtonActivityChanged));
        public static readonly DependencyProperty SkillNameProperty =
            DependencyProperty.Register("SkillName", typeof(string), typeof(Skill), new PropertyMetadata("Skill"));
        public static readonly DependencyProperty LevelProperty =
            DependencyProperty.Register("Level", typeof(uint), typeof(Skill), new PropertyMetadata((uint)1));
        public static readonly DependencyProperty CostProperty =
            DependencyProperty.Register("Cost", typeof(uint?), typeof(Skill), new PropertyMetadata(null));
        public static readonly DependencyProperty RangeProperty =
            DependencyProperty.Register("Range", typeof(int?), typeof(Skill), new PropertyMetadata(null));
        public static readonly DependencyProperty DescriptionProperty =
            DependencyProperty.Register("Description", typeof(string), typeof(Skill), new PropertyMetadata(null));
        public static readonly DependencyProperty IsActiveProperty =
            DependencyProperty.Register("IsActive", typeof(bool), typeof(Skill), new PropertyMetadata(true, OnButtonActivityChanged));

        private static void OnButtonActivityChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var model = (Skill)d;
            model.CoerceValue(IsButtonActiveProperty);
        }

        private static object OnCoerceButtonActivity(DependencyObject d, object baseValue)
        {
            var model = (Skill)d;

            return model.IsReadyToUse && model.IsActive;
        }

        public Skill()
        {
            InitializeComponent();
        }

        public bool IsButtonActive
        {
            get { return (bool)GetValue(IsButtonActiveProperty); }
            set { SetValue(IsButtonActiveProperty, value); }
        }
        public ImageSource ImageSource
        {
            get { return (ImageSource)GetValue(ImageSourceProperty); }
            set { SetValue(ImageSourceProperty, value); }
        }
        public bool IsReadyToUse
        {
            get { return (bool)GetValue(IsReadyToUseProperty); }
            set { SetValue(IsReadyToUseProperty, value); }
        }
        public string SkillName
        {
            get { return (string)GetValue(SkillNameProperty); }
            set { SetValue(SkillNameProperty, value); }
        }
        public uint Level
        {
            get { return (uint)GetValue(LevelProperty); }
            set { SetValue(LevelProperty, value); }
        }
        public uint? Cost
        {
            get { return (uint?)GetValue(CostProperty); }
            set { SetValue(CostProperty, value); }
        }
        public int? Range
        {
            get { return (int?)GetValue(RangeProperty); }
            set { SetValue(RangeProperty, value); }
        }
        public string? Description
        {
            get { return (string?)GetValue(DescriptionProperty); }
            set { SetValue(DescriptionProperty, value); }
        }
        public bool IsActive
        {
            get { return (bool)GetValue(IsActiveProperty); }
            set { SetValue(IsActiveProperty, value); }
        }
    }
}
