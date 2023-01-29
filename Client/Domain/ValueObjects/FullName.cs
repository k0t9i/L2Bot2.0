using Client.Domain.Common;

namespace Client.Domain.ValueObjects
{
    public class FullName : NotifyPropertyChanged
    {
        private string nickname;
        private string title;

        public string Nickname { get => nickname; set { if (value != nickname) { nickname = value; OnPropertyChanged("Nickname"); } } }
        public string Title { get => title; set { if (value != title) { title = value; OnPropertyChanged("Title"); } } }

        public FullName(string nickname, string title)
        {
            this.nickname = nickname;
            this.title = title;
        }
    }
}
