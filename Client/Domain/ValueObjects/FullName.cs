namespace Client.Domain.ValueObjects
{
    public class FullName
    {
        public string Nickname { get; set; }
        public string Title { get; set; }

        public FullName(string nickname, string title)
        {
            Nickname = nickname;
            Title = title;
        }
    }
}
