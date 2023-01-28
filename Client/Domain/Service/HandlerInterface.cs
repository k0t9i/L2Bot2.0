using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.DTO;
using Client.Domain.Enums;

namespace Client.Domain.Service
{
    public interface HandlerInterface
    {
        void Update(MessageOperationEnum operation, string content);
    }
}
