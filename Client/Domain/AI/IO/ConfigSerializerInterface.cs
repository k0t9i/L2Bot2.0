﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.IO
{
    public interface ConfigSerializerInterface
    {
        string Serialize(Config config);
    }
}