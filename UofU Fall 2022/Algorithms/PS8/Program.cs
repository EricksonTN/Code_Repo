using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS8
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int[] Dungeon_info = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();

            Node[] intersections = new Node[Dungeon_info[0]];
            Dictionary<Node, List<Edge>> G = new Dictionary<Node, List<Edge>>();
            Node newNode;

            for (int i = 0; i < Dungeon_info[0]; i++)
            {
                if (i == 0)
                {
                   newNode  = new Node(i, 1.0, null, false);
                }
                else
                {
                    newNode = new Node(i, double.MinValue, null, false);
                }
                intersections[i] = newNode;
                G.Add(newNode, new List<Edge>());                
            }


            double[] temp;
            for (int j = 0; j < Dungeon_info[1]; j++) 
            {
                temp = Console.ReadLine().Split(' ').Select(double.Parse).ToArray();
                Node start = intersections[(int)temp[0]];
                Node end = intersections[(int)temp[1]];
                double factor = temp[2];

                G[start].Add(new Edge(start, end, factor));
                G[end].Add(new Edge(end, start, factor));
            }

            bool s = false;

            Djikstra(intersections[0], G);
            Console.WriteLine(intersections[Dungeon_info[0] - 1].dist);
            Console.Read();
        }

        public static void Djikstra(Node s, Dictionary<Node, List<Edge>> G)
        {
            PriorityQueue<Node, double> pq = new PriorityQueue<Node, double>();
            pq.Enqueue(s, s.dist);
            while (pq.Count > 0)
            {
                Node u = pq.Dequeue();
                if (u.isTraversed)
                {
                    continue;
                }
                u.isTraversed = true;
                
                foreach (Edge e in G[u])
                {
                    double dist = u.dist * e.ec;
                    if (dist > e.end.dist)
                    {
                        Relax(e);
                        pq.Enqueue(e.end, -(e.end.dist));
                    }
                }
            }
        }

        public static void Relax(Edge e)
        {
            e.end.dist = e.start.dist * e.ec;
        }
    }

    public class Node
    {
        public int id;
        public double dist;
        public Node? pred;
        public bool isTraversed;

        public Node(int ID, double Dist, Node Pred, bool hasTraversed)
        {
            id = ID;
            isTraversed = hasTraversed;
            dist = Dist;
            pred = Pred;
            isTraversed = hasTraversed;
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



