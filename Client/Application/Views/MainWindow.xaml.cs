using Client.Application.Components;
using Client.Application.ViewModels;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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

namespace Client.Application.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly MainViewModel mainViewModel;
        private readonly AIConfigViewModel aiConfigViewModel;
        private AIConfig aiConfigView;

        public MainWindow(MainViewModel mainViewModel, AIConfigViewModel aiConfigViewModel)
        {
            InitializeComponent();

            DataContext = mainViewModel;
            this.mainViewModel = mainViewModel;
            this.aiConfigViewModel = aiConfigViewModel;
            aiConfigView = new AIConfig(aiConfigViewModel);
        }

        private void AIConfig_Click(object sender, RoutedEventArgs e)
        {
            aiConfigView.Owner = this;
            aiConfigView.ShowDialog();
        }
    }
}
