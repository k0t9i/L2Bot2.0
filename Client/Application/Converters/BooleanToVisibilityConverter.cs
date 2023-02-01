using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace Client.Application.Converters
{
    public class BooleanToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var isInverted = parameter == null ? false : true;
            var preparedValue = value == null ? false : (bool)value;
            preparedValue = isInverted ? !preparedValue : preparedValue;
            return innerConverter.Convert(preparedValue, targetType, parameter, culture);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        System.Windows.Controls.BooleanToVisibilityConverter innerConverter = new System.Windows.Controls.BooleanToVisibilityConverter();
    }
}
