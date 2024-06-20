using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    public class DummyClassInterface : TestInterface.TestInterface
    {
        public string GetString()
        {
            return "Dummy interface";
        }
    }
}
