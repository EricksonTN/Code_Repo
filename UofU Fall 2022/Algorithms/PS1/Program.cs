using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _4150_PS1
{
    class Program
    {
        static void Main(string[] args)
        {

            int arrLength = int.Parse(Console.ReadLine());
            int[] IDarr = new int[arrLength];
            int[] WeightArr = new int[arrLength];

            string Ids = Console.ReadLine();
            string weights = Console.ReadLine();

            int j = 0;

            string[] s = Ids.Split(' ');
            string[] w = weights.Split(' ');

            for (int i = 0; i < s.Length; i++)
            {

                    //IDarr[j] = (int) Char.GetNumericValue(Ids[i]);
                    //WeightArr[j] = (int) Char.GetNumericValue(weights[i]);

                IDarr[i] = int.Parse(s[i]);
                WeightArr[i] = int.Parse(w[i]);
                
            }

            Console.WriteLine("" + WeightedMedian(IDarr, WeightArr, arrLength));
            Console.Read();
        }

        static int WeightedMedian(int[] IDs, int[] Weights, int arrLength)
        {
            int pivot = arrLength / 2;

            // Comparing value. both teams should have a median less than arrSum
            int arrSum = 0;

            for (int i = 0; i < arrLength; i++)
            {
                arrSum += Weights[i];
            }

            arrSum = arrSum / 2;

            //Start of Recursion base case
            if (arrLength == 1)
            {
                return IDs[0];
            }

            int[] IDsCopy = new int[arrLength];
            Array.Copy(IDs, IDsCopy, arrLength);

            int medVal = MOMSelect(IDsCopy, pivot);
            int medianIndexVal = 0;

            for (int i = 0; i < arrLength; i++)
            {
                if (IDs[i] == medVal)
                {
                    medianIndexVal = i;
                    break;
                }
            }

            int partInd = WeightPartition(IDs, Weights, medianIndexVal);
            int leftSideMed = 0;
            int rightSideMed = 0;
            //Find the Weighted median of the left side
            for (int i = 0; i < partInd; i++)
            {
                leftSideMed += Weights[i];
            }

            //Find the Weighted median of the right side
            for (int i = partInd + 1; i < Weights.Length; i++)
            {
                rightSideMed += Weights[i];
            }

            if (leftSideMed <= arrSum && rightSideMed <= arrSum)
            {
                return medVal;
            }

            if (leftSideMed > arrSum)
            {
                int[] LSIDarr = new ArraySegment<int>(IDs, 0, (partInd - 1)).ToArray();
                int[] LSWarr = new ArraySegment<int>(Weights, 0, (partInd - 1)).ToArray();
                return WeightedMedian(LSIDarr, LSWarr, LSIDarr.Length);
            }

            if (rightSideMed > arrSum)
            {
                int[] RSIDarr = new ArraySegment<int>(IDs, (partInd + 1), IDs.Length - partInd - 1).ToArray();
                int[] RSWarr = new ArraySegment<int>(Weights, (partInd + 1), IDs.Length - partInd - 1).ToArray();
                return WeightedMedian(RSIDarr, RSWarr, RSIDarr.Length);
            }

            return 0;
        }

        static int MOMSelect (int[] IDs, int k)
        {
            
            int m;
            //MOMSelect Implementation
            if (IDs.Length <= 25)
            {
                Array.Sort(IDs);
                return IDs[k];
            }

            else
            {
                m = (int) Math.Ceiling(IDs.Length / 5.00);
                int[] last5Block = new int[5];
                int[] M = new int[m];

                int lastIndex = 0;
                for (int j = ((5 * m) - 5); j < (5 * m); j++)
                {
                    if (j <= IDs.Length - 1)
                    {
                        last5Block[lastIndex] = IDs[j];
                    }
                    else
                    {
                        last5Block[lastIndex] = int.MaxValue;
                    }
                    lastIndex++;
                }

                for (int i = 1; i <= m; i++)
                {
                    int[] arr = new int[5];
                    if (i == m)
                    {
                        M[i - 1] = MedianOfFive(last5Block);
                        break;
                    }
                    Array.Copy(IDs, ((5 * i) - 5), arr, 0, 5);
                    M[i - 1] = MedianOfFive(arr);
                }

                
                int mom = MOMSelect(M, (m / 2));

                int r = Partition(IDs, mom);

                if (k < r)
                {
                    int[] recurseArr = new ArraySegment<int>(IDs, 0, r - 1).ToArray();
                    return MOMSelect(recurseArr, k);
                }

                else if (k > r)
                {
                    int[] recurseArr = new ArraySegment<int>(IDs, r+1, IDs.Length - r - 1).ToArray();
                    return MOMSelect(recurseArr, k - r);
                }

                else
                {
                    return mom;
                }
            }

            return 0;
        }

        static int MedianOfFive (int[] arr)
        {
            Array.Sort(arr);
            return arr[arr.Length / 2];
        }

        static int Partition (int[] arr, int pivot)
        {
            int pVal = arr[pivot];
            int nVal = arr[arr.Length - 1];

            arr[arr.Length - 1] = pVal;
            arr[pivot] = nVal;
            
            //# of items less than pivot
            int l = 0;

            for (int i = 0; i < arr.Length - 1; i++)
            {
                if (arr[i] < arr[arr.Length - 1])
                {

                    int tVal1 = arr[i];
                    int tVal2 = arr[l];

                    arr[l] = tVal1;
                    arr[i] = tVal2;

                    l += 1;
                }
            }
            int lastVal = arr[arr.Length - 1];
            int lVal = arr[l];

            arr[arr.Length - 1] = lVal;
            arr[l] = lastVal;

            return l;
        }

        static int WeightPartition(int[] IDs, int[] weights, int pivot)
        {
            int pivotID = IDs[pivot];
            int pivotWeight = weights[pivot];

            int nID = IDs[IDs.Length - 1];
            int nWeight = weights[IDs.Length - 1];

            IDs[IDs.Length - 1] = pivotID;
            weights[IDs.Length - 1] = pivotWeight;

            IDs[pivot] = nID;
            weights[pivot] = nWeight;

            //# of items less than pivot
            int l = 0;

            for (int i = 0; i < IDs.Length - 1; i++)
            {
                if (IDs[i] < IDs[IDs.Length - 1])
                {

                    int tID = IDs[i];
                    int tWeight = weights[i];

                    int lID = IDs[l];
                    int lWeight = weights[i];

                    IDs[l] = tID;
                    weights[l] = tWeight;

                    IDs[i] = lID;
                    weights[i] = lWeight;

                    l += 1;
                }
            }
            int lastID = IDs[IDs.Length - 1];
            int lastWeight = weights[IDs.Length - 1];

            int lID_Val = IDs[l];
            int lWeight_Val = weights[l];

            IDs[IDs.Length - 1] = lID_Val;
            weights[IDs.Length - 1] = lWeight_Val;

            IDs[l] = lastID;
            weights[l] = lastWeight;

            return l;
        }
    }
}
