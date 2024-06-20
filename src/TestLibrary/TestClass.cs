using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestLibrary
{
    public class TestClass : TestInterface.TestInterface
    {
        public TestClass()
        {
        }

        public string GetString()
        {
            return "Successful Injection!";
        }
    }
}
