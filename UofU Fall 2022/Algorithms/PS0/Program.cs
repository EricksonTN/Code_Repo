using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _4150_PS0
{
    class Program
    {
        static void Main(string[] args)
        {
            //List<String> test1 = new List<String> { "3 3\n", "eat\n", "ate\n", "pat\n" };
            //Console.WriteLine("test1 result: " + anagramChecker(test1));

            List<String> interimList = new List<String>();

            String currLine;
            Console.ReadLine();
            while ((currLine = Console.ReadLine()) != null && currLine != "")
            {
                currLine = String.Concat(currLine.OrderBy(ch => ch));
                // Store value of line in some data structure
                interimList.Add(currLine);
            }

            //Create new list based on a set of parameters (group by)
            var noDupeList = interimList
                .GroupBy(x => x)
                .Where(g => g.Count() == 1)
                .SelectMany(g => g);

            Console.WriteLine(noDupeList.Count());
        }

        //static int anagramChecker(List<String> list)
        //{
        //    List<String> interimList = new List<String>();

        //    String currLine;
            
        //    for (int i = 1; i < list.Count; i++)
        //    {
        //        currLine = String.Concat(list[i].OrderBy(ch => ch));
        //        Console.WriteLine(currLine);
        //        // Store value of line in some data structure
        //        interimList.Add(currLine);
        //    }

        //    //Create new list based on a set of parameters (group by)
        //    var noDupeList = interimList
        //        .GroupBy(x => x)
        //        .Where(g => g.Count() == 1)
        //        .SelectMany(g => g);

        //    return noDupeList.Count();
        //}
    }
}
