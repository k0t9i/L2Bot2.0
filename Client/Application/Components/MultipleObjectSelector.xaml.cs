using Client.Domain.Helpers;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace Client.Application.Components
{
    /// <summary>
    /// Interaction logic for MobSelector.xaml
    /// </summary>
    public partial class MultipleObjectSelector : UserControl
    {
        public static readonly DependencyProperty SourceProperty =
            DependencyProperty.Register("Source", typeof(ICollection<ObjectInfo>), typeof(MultipleObjectSelector), new PropertyMetadata(default(ICollection<ObjectInfo>)));
        public ICollection<ObjectInfo> Source
        {
            get { return (ICollection<ObjectInfo>)GetValue(SourceProperty); }
            set { SetValue(SourceProperty, value); }
        }
        public static readonly DependencyProperty TargetProperty =
            DependencyProperty.Register("Target", typeof(ICollection<ObjectInfo>), typeof(MultipleObjectSelector), new PropertyMetadata(default(ICollection<ObjectInfo>)));
        public ICollection<ObjectInfo> Target
        {
            get { return (ICollection<ObjectInfo>)GetValue(TargetProperty); }
            set { SetValue(TargetProperty, value); }
        }
        public string Header
        {
            get { return (string)GetValue(HeaderProperty); }
            set { SetValue(HeaderProperty, value); }
        }
        public static readonly DependencyProperty HeaderProperty =
            DependencyProperty.Register("Header", typeof(string), typeof(MultipleObjectSelector), new PropertyMetadata(""));

        public static RoutedUICommand AddItemCommand { get; } = new RoutedUICommand(
            "Add item",
            nameof(AddItemCommand),
            typeof(MultipleObjectSelector)
        );
        public static RoutedUICommand RemoveItemCommand { get; } = new RoutedUICommand(
            "Remove item",
            nameof(RemoveItemCommand),
            typeof(MultipleObjectSelector)
        );
        public static RoutedUICommand SearchSourceCommand { get; } = new RoutedUICommand(
            "Search in source",
            nameof(SearchSourceCommand),
            typeof(MultipleObjectSelector)
        );
        public static RoutedUICommand SearchTargetCommand { get; } = new RoutedUICommand(
            "Search in target",
            nameof(SearchTargetCommand),
            typeof(MultipleObjectSelector)
        );


        public MultipleObjectSelector()
        {
            InitializeComponent();
            CommandBindings.Add(new CommandBinding(AddItemCommand, AddItem));
            CommandBindings.Add(new CommandBinding(RemoveItemCommand, RemoveItem));
            CommandBindings.Add(new CommandBinding(SearchSourceCommand, SearchSource));
            CommandBindings.Add(new CommandBinding(SearchTargetCommand, SearchTarget));
        }

        private void AddItem(object sender, ExecutedRoutedEventArgs e)
        {
            var item = e.Parameter as ObjectInfo;
            if (item != null)
            {
                Source.Remove(item);
                Target.Add(item);
            }
        }

        private void RemoveItem(object sender, ExecutedRoutedEventArgs e)
        {
            var item = e.Parameter as ObjectInfo;
            if (item != null)
            {
                Target.Remove(item);
                Source.Add(item);
            }
        }

        private void SearchSource(object sender, ExecutedRoutedEventArgs e)
        {
            var searchPredicate = (string)e.Parameter ?? null;

            if (searchPredicate != null)
            {
                CollectionViewSource.GetDefaultView(Source).Filter = item => (item as ObjectInfo)?.Name.Contains(searchPredicate, StringComparison.OrdinalIgnoreCase) ?? false;
            }
        }

        private void SearchTarget(object sender, ExecutedRoutedEventArgs e)
        {
            var searchPredicate = (string)e.Parameter ?? null;

            if (searchPredicate != null)
            {
                CollectionViewSource.GetDefaultView(Target).Filter = item => (item as ObjectInfo)?.Name.Contains(searchPredicate, StringComparison.OrdinalIgnoreCase) ?? false;
            }
        }
    }
}
