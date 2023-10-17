using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Client.Application.ViewModels
{
    public class ChatMessageViewModel
    {
        public string Text
        {
            get
            {
                return (message.Name != "" ? message.Name + ": " : "") + message.Text;
            }
        }

        public SolidColorBrush Color
        {
            get
            {
                SolidColorBrush? color;
                if (!colors.TryGetValue(message.Channel, out color))
                {
                    color = defaultColor;
                }

                return color;
            }
        }

        public ChatMessageViewModel(ChatMessage message)
        {
            this.message = message;

            colors = new Dictionary<ChatChannelEnum, SolidColorBrush>
            {
                { ChatChannelEnum.Shout, Brushes.OrangeRed },
                { ChatChannelEnum.Tell, Brushes.Magenta },
                { ChatChannelEnum.Party, Brushes.LimeGreen },
                { ChatChannelEnum.Clan, Brushes.Violet },
                { ChatChannelEnum.Gm, Brushes.Red },
                { ChatChannelEnum.Trade, Brushes.HotPink }
            };
        }

        private readonly ChatMessage message;
        private readonly Dictionary<ChatChannelEnum, SolidColorBrush> colors;
        private readonly SolidColorBrush defaultColor = Brushes.Black;
    }
}
