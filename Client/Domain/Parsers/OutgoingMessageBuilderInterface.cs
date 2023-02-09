using Client.Domain.DTO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Parsers
{
    public interface OutgoingMessageBuilderInterface
    {
        string Build<T>(OutgoingMessage<T> message);
    }
}
