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
    public partial class StatsBar : UserControl
    {
        public static readonly DependencyProperty BackgroundSourceProperty =
            DependencyProperty.Register("BackgroundSource", typeof(ImageSource), typeof(StatsBar), new PropertyMetadata(default(ImageSource)));
        public static readonly DependencyProperty ForegroundSourceProperty =
            DependencyProperty.Register("ForegroundSource", typeof(ImageSource), typeof(StatsBar), new PropertyMetadata(default(ImageSource)));
        public static readonly DependencyProperty CurrentProperty =
            DependencyProperty.Register("Current", typeof(double), typeof(StatsBar), new PropertyMetadata(0.0, OnDataChanged));
        public static readonly DependencyProperty TotalProperty =
            DependencyProperty.Register("Total", typeof(double), typeof(StatsBar), new PropertyMetadata(0.0, OnDataChanged));
        public static readonly DependencyProperty FormatProperty =
            DependencyProperty.Register("Format", typeof(string), typeof(StatsBar), new PropertyMetadata("{0}/{1}", OnFormatChanged));
        public static readonly DependencyProperty ForegroundWidthProperty =
            DependencyProperty.Register("ForegroundWidth", typeof(double), typeof(StatsBar), new PropertyMetadata(0.0, null, OnCoerceForegroundWidth));
        public static readonly DependencyProperty TextProperty =
            DependencyProperty.Register("Text", typeof(string), typeof(StatsBar), new PropertyMetadata("", null, OnCoerceText));
        public static readonly DependencyProperty OnlyBarProperty =
            DependencyProperty.Register("OnlyBar", typeof(bool), typeof(StatsBar), new PropertyMetadata(false, OnOnlyBarChanged));     

        private static void OnDataChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var model = (StatsBar)d;
            model.CoerceValue(ForegroundWidthProperty);
            model.CoerceValue(TextProperty);
        }

        private static void OnOnlyBarChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var model = (StatsBar)d;
            model.CoerceValue(TextProperty);
        }

        private static void OnFormatChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var model = (StatsBar)d;
            model.CoerceValue(TextProperty);
        }
         
        private static object OnCoerceForegroundWidth(DependencyObject d, object baseValue)
        {
            var model = (StatsBar)d;
            var actualWidth = (double)model.GetValue(ActualWidthProperty);

            return actualWidth / 100 * model.GetPercent();
        }
        
        private static object OnCoerceText(DependencyObject d, object baseValue)
        {
            var model = (StatsBar)d;

            return model.OnlyBar ? "" : string.Format(model.Format, model.Current, model.Total, model.GetPercent());
        }

        public StatsBar()
        {
            InitializeComponent();
        }

        public override void OnApplyTemplate()
        {
            SizeChanged += StatsBar_SizeChanged;
        }

        private void StatsBar_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (e.WidthChanged)
            {
                var model = (StatsBar)sender;
                model.CoerceValue(ForegroundWidthProperty);
            }
        }

        private double GetPercent()
        {
            var percent = Current;
            if (Total > 0)
            {
                percent = Current / Total * 100;
            }

            return percent;
        }

        public ImageSource BackgroundSource
        {
            get { return (ImageSource)GetValue(BackgroundSourceProperty); }
            set { SetValue(BackgroundSourceProperty, value); }
        }
        public ImageSource ForegroundSource
        {
            get { return (ImageSource)GetValue(ForegroundSourceProperty); }
            set { SetValue(ForegroundSourceProperty, value); }
        }
        public double ForegroundWidth
        {
            get { return (double)GetValue(ForegroundWidthProperty); }
            set { SetValue(ForegroundWidthProperty, value); }
        }
        public double Current
        {
            get { return (double)GetValue(CurrentProperty); }
            set { SetValue(CurrentProperty, value); }
        }
        public double Total
        {
            get { return (double)GetValue(TotalProperty); }
            set { SetValue(TotalProperty, value); }
        }
        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }
        public string Format
        {
            get { return (string)GetValue(FormatProperty); }
            set { SetValue(FormatProperty, value); }
        }
        public bool OnlyBar
        {
            get { return (bool)GetValue(OnlyBarProperty); }
            set { SetValue(OnlyBarProperty, value); }
        }
    }
}
