using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS4
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.ReadLine();

            double[] plots = Console.ReadLine().Split(' ').Select(double.Parse).ToArray();

            double[] circleHomes = Console.ReadLine().Split(' ').Select(double.Parse).ToArray();

            double[] squareHomes = Console.ReadLine().Split(' ').Select(double.Parse).ToArray();

            double[] homes = new double[circleHomes.Length + squareHomes.Length];

            Array.Copy(circleHomes, homes, circleHomes.Length);

            int tempInd = circleHomes.Length;

            for (int i = 0; i < squareHomes.Length; i++)
            {
                double val = squareHomes[i] / Math.Sqrt(2);
                homes[tempInd] = val;
                tempInd++;
            }

            Array.Sort(homes);
            Array.Sort(plots);
            if ((plots.Length != 0) && (homes.Length != 0))
            {
                Console.WriteLine(plotsToFill(plots, homes));
            }

            else
            {
                Console.WriteLine("0");
            }
        }

        static int plotsToFill(double[] plots, double[] homes)
        {
            int count = 0;
            for (int m = 0; m < plots.Length; m++)
            {
                for (int n = homes.Length - 1; n >= 0; n--)
                {
                    if (homes[n] < plots[m])
                    {
                        count++;
                        homes[n] = int.MaxValue;
                        break;
                    }
                }
            }
            return count;
        }
    }
}
