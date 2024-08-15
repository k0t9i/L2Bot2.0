using Client.Application.Commands;
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
using System.Windows.Shapes;

namespace Client.Application.Views
{
    /// <summary>
    /// Interaction logic for CombatAIConfigView.xaml
    /// </summary>
    public partial class AIConfig : Window
    {
        private AIConfigViewModel viewModel;

        public AIConfig(AIConfigViewModel viewModel)
        {
            InitializeComponent();
            this.viewModel = viewModel;
            DataContext = viewModel;
            if (viewModel.Close == null)
            {
                viewModel.Close = () => Close();
            }
            if (viewModel.OpenSaveDialog == null)
            {
                viewModel.OpenSaveDialog = (string data) =>
                {
                    var dialog = new SaveFileDialog()
                    {
                        Filter = "Json files(*.json)|*.json"
                    };
                    if (dialog.ShowDialog() == true)
                    {
                        File.WriteAllText(dialog.FileName, data);
                    }
                };
            }
            if (viewModel.OpenOpenDialog == null)
            {
                viewModel.OpenOpenDialog = () =>
                {
                    var dialog = new OpenFileDialog()
                    {
                        Filter = "Json files(*.json)|*.json"
                    };
                    if (dialog.ShowDialog() == true)
                    {
                        return File.ReadAllText(dialog.FileName);
                    }

                    return null;
                };
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;
            Hide();
        }

        private void Window_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if ((bool) e.NewValue)
            {
                viewModel.LoadConfig();
            }
        }
    }
}
