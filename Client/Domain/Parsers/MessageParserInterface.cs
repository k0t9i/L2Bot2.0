using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Parsers
{
    public interface MessageParserInterface
    {
        DTO.Message Parse(string message);
    }
}
