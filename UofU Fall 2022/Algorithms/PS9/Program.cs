using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS9
{
    internal class Program
    {

        static void Main(string[] args)
        {
            int numNodes = int.Parse(Console.ReadLine());
            double [,] memo = new double[numNodes, numNodes];
            for (int i = 0; i < numNodes; i++)
            {
                for (int j = 0; j < numNodes; j++)
                {
                    if (i == j)
                    {
                        memo[i, j] = 0;
                    }
                    else
                    {
                        memo[i, j] = double.MaxValue;
                    }
                }
            }

            Tuple<int, int>[] nodes = new Tuple<int, int>[numNodes];
            List<Dictionary<int, double>> G = new List<Dictionary<int, double>>();

            for (int i = 0; i < numNodes; i++)
            {
                int[] currNode = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();
                nodes[i] = new Tuple<int, int>(currNode[0], currNode[1]);
                G.Add(new Dictionary<int, double>());
            }

            int preEdges = int.Parse(Console.ReadLine());

            for (int j = 0; j < preEdges; j++)
            {
                int[] currEdge = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();
                double weight = calcEdgeWeight(nodes[currEdge[0]], nodes[currEdge[1]]);
                G[currEdge[0]].Add(currEdge[1], weight);
                G[currEdge[1]].Add(currEdge[0], weight);
            }

            Console.WriteLine(AddNewRoad(G, nodes, memo));
            Console.Read();
        }

        static double AddNewRoad(List<Dictionary<int, double>> G, Tuple<int, int>[] nodes, double[,] memo)
        {

            double bestDist = FloydWarshall(G, new int[0], nodes, memo);

            int[] r;
            //Start finding new edges
            for (int i = 0; i < nodes.Length; i++)
            {
                for (int j = 0; j < nodes.Length; j++)
                {
                    if (!G[i].ContainsKey(j) && i != j)
                    {
                        double[,] memoCopy = new double[nodes.Length, nodes.Length];
                        Array.Copy(memo, memoCopy, memo.Length);

                        r = new int[2];
                        r[0] = i;
                        r[1] = j;
                        double weight = calcEdgeWeight(nodes[i], nodes[j]);
                        List<Dictionary<int, double>> GPrime = new List<Dictionary<int, double>>(G);
                        GPrime[i].Add(j, weight);
                        GPrime[j].Add(i, weight);

                        double commDist = FloydWarshall(GPrime, r, nodes, memoCopy);

                        GPrime[i].Remove(j);
                        GPrime[j].Remove(i);

                        if (commDist < bestDist)
                        {
                            bestDist = commDist;
                        }
                    }
                }
            }

            return bestDist;
        }

        static double FloydWarshall(List<Dictionary<int, double>> G, int[] r, Tuple<int, int>[] nodes, double[,] Memo)
        {
            for (int u = 0; u < nodes.Length; u++)
            {
                for (int v = 0; v < nodes.Length; v++)
                {
                    if (G[u].ContainsKey(v))
                    {
                        Memo[u, v] = G[u][v];
                    }
                }
            }

            //All r mode
            if (r.Length == 0)
            {
                for (int rVal = 0; rVal < nodes.Length; rVal++)
                {
                    for (int uVal = 0; uVal < nodes.Length; uVal++)
                    {
                        for (int vVal = 0; vVal < nodes.Length; vVal++)
                        {
                            if (Memo[uVal, vVal] > (Memo[uVal, rVal] + Memo[rVal, vVal]))
                            {
                                Memo[uVal, vVal] = Memo[uVal, rVal] + Memo[rVal, vVal];
                            }
                        }
                    }
                }
            }

            else
            {
                for (int rVal = 0; rVal < r.Length; rVal++)
                {
                    for (int uVal = 0; uVal < nodes.Length; uVal++)
                    {
                        for (int vVal = 0; vVal < nodes.Length; vVal++)
                        {
                            if (Memo[uVal, vVal] > (Memo[uVal, r[rVal]] + Memo[r[rVal], vVal]))
                            {
                                Memo[uVal, vVal] = Memo[uVal, r[rVal]] + Memo[r[rVal], vVal];
                            }
                        }
                    }
                }
            }

            return calcTotal(nodes.Length, Memo);
        }
        static double calcEdgeWeight(Tuple<int, int> start, Tuple<int, int> end)
        {
            return Math.Sqrt(Math.Pow(start.Item1 - end.Item1, 2) + Math.Pow(start.Item2 - end.Item2, 2));
        }

        static double calcTotal(int numVerts, double[,] memo)
        {
            double total = 0.0;
            for (int i = 0; i < numVerts; i++)
            {
                for (int j = 0; j < numVerts; j++)
                {
                    if (memo[i, j] < double.MaxValue)
                    {
                        total += memo[i, j];
                    }
                }
            }
            return total * 0.5;
        }

    }    
}



