using Client.Domain.Entities;
using Client.Domain.Helpers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client.Application.Components
{
    /// <summary>
    /// Interaction logic for SearchableCombobox.xaml
    /// </summary>
    public partial class ObjectSelector : UserControl
    {
        public static readonly DependencyProperty SourceProperty =
            DependencyProperty.Register("Source", typeof(ICollection<ObjectInfo>), typeof(ObjectSelector), new PropertyMetadata(default(ICollection<ObjectInfo>)));
        public ICollection<ObjectInfo> Source
        {
            get { return (ICollection<ObjectInfo>)GetValue(SourceProperty); }
            set { SetValue(SourceProperty, value); }
        }

        public static readonly DependencyProperty SelectedValueProperty =
            DependencyProperty.Register("SelectedValue", typeof(object), typeof(ObjectSelector), new PropertyMetadata(default(object)));
        public object SelectedValue
        {
            get { return GetValue(SelectedValueProperty); }
            set { SetValue(SelectedValueProperty, value); }
        }

        public string Header
        {
            get { return (string)GetValue(HeaderProperty); }
            set { SetValue(HeaderProperty, value); }
        }
        public static readonly DependencyProperty HeaderProperty =
            DependencyProperty.Register("Header", typeof(string), typeof(ObjectSelector), new PropertyMetadata(""));

        public ObjectSelector()
        {
            InitializeComponent();
        }
    }
}
