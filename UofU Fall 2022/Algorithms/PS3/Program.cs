using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS3
{
    class Program
    {
        public static int[,,] memo;
        static void Main(string[] args)
        {
            int[] NK = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();

            int N = NK[0];
            int K = NK[1];
            int[,] gallery = new int[N, 2];
            memo = new int[N + 1, 3, K + 1];

            string currLine;
            List<string> inputList = new List<string>();
            int listIter = 0;
            while(listIter != (N + 1))
            {
                currLine = Console.ReadLine();
                inputList.Add(currLine);
                listIter++;
            }

            //Line per line, split the string and turn into two ints then put into gallery array.
            int tempRow = 0;
            int tempCol = 0;
            for (int i = 0; i < inputList.Count - 1; i++)
            {
                int[] tempArr = inputList[i].Split(' ').Select(int.Parse).ToArray();
                gallery[tempRow, tempCol] = tempArr[0];
                gallery[tempRow, tempCol + 1] = tempArr[1];
                tempRow++;
            }
            Console.WriteLine(maxValue(0, -1, K, gallery));
            
        }

        public static int maxValue (int r, int unclosableRoom, int numRoomsToClose, int[,] gallery)
        {
            int roomsToClose = numRoomsToClose;
            int ucRoom = translateUncloseableRoom(unclosableRoom);
            // i = current row
            // j = uncloseableRoom(?) col 0 = -1, col 1 = 0, col 2 = 1
            // k = # of rooms left to close
            for (int i = memo.GetLength(0) - 1; i >= r; i--)
            {
                for (int j = ucRoom; j <= memo.GetLength(1) - 1; j++)
                {
                    for (int k = roomsToClose; k >= 0; k--)
                    {
                        //base cases
                        if ((k == 0) && (i == memo.GetLength(0) -1))
                        {
                            memo[i, j, k] = 0;
                            continue;
                        }

                        if ((k != 0) && (i == memo.GetLength(0) - 1))
                        {
                            memo[i, j, k] = int.MinValue;
                            continue;
                        }

                        // "Recursive Case"
                        //Case: uncloseableRoom is -1
                        switch (j) {
                            case 0:
                                if ((k - 1) < 0)
                                {
                                    memo[i, j, k] = Math.Max(Math.Max((gallery[i, 0] + 0), (gallery[i, 1] + 0)), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                    break;
                                }
                                memo[i, j, k] = Math.Max(Math.Max((gallery[i, 0] + memo[i + 1, 1, k - 1]), (gallery[i, 1] + memo[i + 1, 2, k - 1])), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                break;

                            case 1:
                                if ((k - 1) < 0)
                                {
                                    memo[i, j, k] = Math.Max((gallery[i, 0] + 0), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                    break;
                                }
                                memo[i, j, k] = Math.Max((gallery[i, 0] + memo[i + 1, 1, k - 1]), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                break;

                            case 2:
                                if ((k - 1) < 0)
                                {
                                    memo[i, j, k] = Math.Max((gallery[i, 1] + 0), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                    break;
                                }
                                memo[i, j, k] = Math.Max((gallery[i, 1] + memo[i + 1, 2, k - 1]), (gallery[i, 0] + gallery[i, 1] + memo[i + 1, 0, k]));
                                break;
                        }
                    }
                    
                }
            }

            return memo[r, ucRoom, numRoomsToClose];
        }

        public static int translateUncloseableRoom (int uncloseableRoom)
        {
            if (uncloseableRoom == -1)
            {
                return 0;
            }

            else if (uncloseableRoom == 0)
            {
                return 1;
            }

            return 2;
        }

        public static void Print2DArray<T>(T[,,] matrix)
        {
            for (int i = 0; i < matrix.GetLength(0); i++)
            {
                for (int j = 0; j < matrix.GetLength(1); j++)
                {
                    for (int k = 0; k < matrix.GetLength(2); k++)
                    {
                        Console.Write("[" + i + " " + j + " " + k + "] " + matrix[i, j, k] + "\t");
                    }
                }
                Console.WriteLine();
            }
        }
    }
}
