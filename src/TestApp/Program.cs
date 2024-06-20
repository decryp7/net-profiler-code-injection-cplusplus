using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                BaseClass targetClass = new TargetClassForInjection();
                Console.WriteLine(targetClass.GetTestInterface().GetString());
                //GC.Collect(3, GCCollectionMode.Forced, true, true);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            Console.Read();
        }
    }
}
