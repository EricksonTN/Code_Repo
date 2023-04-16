using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS7
{
    internal class Program
    {
        static double retCount;
        static void Main(string[] args)
        {
            retCount = 0.0;
            int[] TH_info = Console.ReadLine ().Split(' ').Select(int.Parse).ToArray();

            Node[] THs = new Node[TH_info[0]];
            Dictionary<Node, List<Edge>> G = new Dictionary<Node, List<Edge>>();
            Dictionary<Node, List<Edge>> F = new Dictionary<Node, List<Edge>>();

            double[] tempArr;

            // Dictionary Initializations and Key propogation
            for (int numTH = 0; numTH < TH_info[0]; numTH++)
            {
                tempArr = Console.ReadLine().Split(' ').Select(double.Parse).ToArray();
                THs[numTH] = new Node(numTH, tempArr[0], tempArr[1], false, -1);

                G.Add(THs[numTH], new List<Edge>());

                F.Add(THs[numTH], new List<Edge>());
            }

            // If e is greater than or equal to 2, add zero weight edges to F
            if (TH_info[1] >= 2)
            {
                for (int i = 0; i < TH_info[1] - 1; i++)
                {
                    F[THs[i]].Add(new Edge(THs[i], THs[i + 1], 0.0));
                    F[THs[i + 1]].Add(new Edge(THs[i + 1], THs[i], 0.0));
                }
            }

            int[] tArr;
            // Handling pre-existing cables in F
            for (int prePaths = 0; prePaths < TH_info[2]; prePaths++)
            {
                tArr = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();
                Node start = THs[tArr[0] - 1];
                Node end = THs[tArr[1] - 1];
                double lineLen = lineDist(start.x, start.y, end.x, end.y);

                F[start].Add(new Edge(start, end, lineLen));
                F[end].Add(new Edge(end, start, lineLen));
            }

            // Add edges to G for ever pair of vertices in THs
            for (int i = 0; i < THs.Length; i++)
            {
                for (int j = i + 1; j < THs.Length; j++)
                {
                    Node start = THs[i];
                    Node end = THs[j];
                    double lineLen = lineDist(start.x, start.y, end.x, end.y);
                    G[start].Add(new Edge(start, end, lineLen));
                    G[end].Add(new Edge(end, start, lineLen));
                }
            }

            Boruvka(G, F);
            Console.WriteLine(retCount);
            Console.Read();
        }

        static void Boruvka(Dictionary<Node, List<Edge>> G, Dictionary<Node, List<Edge>> F)
        {
            int count = CountAndLabel(F);

            while (count > 1)
            {
                AddAllSafeEdges(G, F, count);
                count = CountAndLabel(F);
            }
        }

        static void AddAllSafeEdges(Dictionary<Node, List<Edge>> G, Dictionary<Node, List<Edge>> F, int count)
        {
            Edge[] safe = new Edge[G.Keys.Count];

            for (int i = 0; i < count; i++)
            {
                safe[i] = null;
            }

            foreach (List<Edge> edges in G.Values)
            {
                foreach (Edge e in edges)
                {
                    if (e.start.comp != e.end.comp)
                    {
                        if (safe[e.start.comp] == null || e.ec < safe[e.start.comp].ec)
                        {
                            safe[e.start.comp] = e;
                        }

                        if (safe[e.end.comp] == null || e.ec < safe[e.end.comp].ec)
                        {
                            safe[e.end.comp] = e;
                        }
                    }
                }
            }

            for (int j = 0; j < count; j++)
            {
                Node start = safe[j].start;
                Node end = safe[j].end;
                double ec = safe[j].ec;
                if (!F[start].Contains(safe[j]))
                {
                    F[start].Add(safe[j]);
                    F[end].Add(new Edge(end, start, ec));
                    retCount += ec;
                }
            }
        }

        static int CountAndLabel(Dictionary<Node, List<Edge>> graph)
        {
            int count = 0;

            foreach (Node v in graph.Keys)
            {
                v.hasTraversed = false;
            }

            foreach (Node n in graph.Keys)
            {
                if (!n.hasTraversed)
                {

                    LabelOne(graph, n, count);
                    count += 1;
                }
            }

            return count;
        }

        static void LabelOne(Dictionary<Node, List<Edge>> graph, Node v, int count)
        {
            Stack<Node> stk = new Stack<Node>();
            stk.Push(v);

            while (stk.Count > 0)
            {
                Node node = stk.Pop();
                if (!node.hasTraversed)
                {
                    node.hasTraversed = true;
                    node.comp = count;
                    foreach (Edge e in graph[node])
                    {
                        stk.Push(e.end);
                    }
                }
            }
        }

        static double lineDist(double x1, double y1, double x2, double y2)
        {
            return Math.Sqrt(Math.Pow((x2 - x1), 2.0) + Math.Pow((y2 - y1), 2));
        }
    }

    public class Node
    {
        public int id;
        public double x;
        public double y;
        public bool hasTraversed;
        public int comp;

        public Node(int ID, double X, double Y, bool traversed, int Component)
        {
            id = ID;
            x = X;
            y = Y;
            hasTraversed = traversed;
            comp = Component;
        }
    }

    public class Edge
    {
        public Node start;
        public Node end;
        public double ec;

        public Edge(Node s, Node e, double edgeCost)
        {
            start = s;
            end = e;
            ec = edgeCost;
        }
    }
}


