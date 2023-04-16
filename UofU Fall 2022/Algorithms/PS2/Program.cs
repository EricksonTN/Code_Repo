using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS2
{
    class Program
    {
        public static int[,] memo = new int[0, 0];

        static void Main(string[] args)
        {
            int[] D = new int[int.Parse(Console.ReadLine())];
            string str = Console.ReadLine();
            D = str.Split(' ').Select(int.Parse).ToArray();

            int distAccum = 0;

            if (!str.Equals(""))
            { 

                for (int i = 0; i < D.Length; i++)
                {
                    //D[i] = int.Parse(distances[i]);
                    distAccum += D[i];
                }

                //Memoization array of n (n+1 if 1 based indexing) rows, and H columns.
                memo = new int[D.Length + 1, distAccum + 1];

                Console.WriteLine(minWall(0, 0, D));
            }

            else
            {
                Console.WriteLine(100000);
            }
        }

        public static int minWall (int index, int height, int[] D)
        {
            //Row iteration from bottom - up
            for (int i = memo.GetLength(0) - 1; i >= index; i--)
            {
                //Column iteration from left to right
                for (int h = height; h < memo.GetLength(1); h++)
                {
                    if (i == memo.GetLength(0) - 1 && h == 0)
                    {
                        memo[i, h] = 0;
                        continue;
                    }

                    if (i == memo.GetLength(0) - 1 && h != 0)
                    {
                        memo[i, h] = 100000;
                        continue;
                    }

                    //Grab the current array slot's dependencies
                    int addCase = h + D[i];
                    int subCase = h - D[i];

                    if (subCase < 0 && addCase >= memo.GetLength(1))
                    {
                        memo[i, h] = 100000;
                        continue;
                    }

                    if (subCase < 0)
                    {
                        memo[i, h] = Math.Max(memo[i + 1, addCase], h);
                        continue;
                    }
                    else if (addCase >= memo.GetLength(1))
                    {
                        memo[i, h] = Math.Max(memo[i + 1, subCase], h);
                        continue;
                    }
                    else
                    {
                        memo[i, h] = Math.Max(Math.Min(memo[i + 1, addCase], memo[i + 1, subCase]), h);
                        continue;
                    }
                }
            }

            return memo[index, height];
        }

        public static void Print2DArray<T>(T[,] matrix)
        {
            for (int i = 0; i < matrix.GetLength(0); i++)
            {
                for (int j = 0; j < matrix.GetLength(1); j++)
                {
                    Console.Write(matrix[i, j] + "\t");
                }
                Console.WriteLine();
            }
        }
    }
}
