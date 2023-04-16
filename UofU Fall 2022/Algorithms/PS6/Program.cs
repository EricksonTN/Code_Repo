using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS6
{
    class Program
    {
        static long[] returnArr;
        static void Main(string[] args)
        {
            int[] orderInfo = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();
            returnArr = Console.ReadLine().Split(' ').Select(long.Parse).ToArray();
            Dictionary<Node, List<Edge>> graph = new Dictionary<Node, List<Edge>>();

            Node[] verticesArr = new Node[orderInfo[0]];
            for (int l = 0; l < orderInfo[0]; l++)
            {
                verticesArr[l] = new Node(l, false);
                graph.Add(verticesArr[l], new List<Edge>());
            }

            string currLine;
            int[] tempArr;
            for (int i = 0; i < orderInfo[1]; i++)
            {
                currLine = Console.ReadLine();
                tempArr = currLine.Split(' ').Select(int.Parse).ToArray();

                graph[verticesArr[tempArr[1]]].Add(new Edge(verticesArr[tempArr[1]], verticesArr[tempArr[0]], tempArr[2]));
            }

            Node[] sortedNodes = topoSort(graph, verticesArr);
            foreach (Node n in sortedNodes)
            {
                ingredCalc(n, graph);
            }
            string str = "";
            str = string.Join(" ", returnArr);
            Console.WriteLine(str);
        }

        public static Node[] topoSort (Dictionary<Node, List<Edge>> dic, Node[] nodes)
        {
            Node[] sortedNodes = new Node[nodes.Length];
            int clock = nodes.Length - 1;
            foreach (Node v in nodes)
            {
                if (!v.hasTraversed)
                {
                    clock = topSortDFS(v, clock, dic, sortedNodes);
                }
            }

            return sortedNodes;
        }
        
        public static int topSortDFS(Node v, int clock, Dictionary<Node, List<Edge>> dic, Node[] sortedNodes)
        {
            v.hasTraversed = true;
            foreach (Edge e in dic[v])
            {
                if (!e.end.hasTraversed)
                {
                    clock = topSortDFS(e.end, clock, dic, sortedNodes);
                }
            }
            sortedNodes[clock] = v;
            clock -= 1;

            return clock;
        }

        public static void ingredCalc(Node vert, Dictionary<Node, List<Edge>> dic)
        {
            long amtWanted = returnArr[vert.id];

            foreach(Edge e in dic[vert])
            {
                returnArr[e.end.id] += (amtWanted * e.ec);
            }
        }
    }

    public class Node
    {
        public int id;
        public bool hasTraversed;

        public Node(int ID, bool traversed)
        {
            id = ID;
            hasTraversed = traversed;
        }
    }

    public class Edge
    {
        public Node start;
        public Node end;
        public int ec;

        public Edge (Node s, Node e, int edgeCost)
        {
            start = s;
            end = e;
            ec = edgeCost; 
        }
    }
}
