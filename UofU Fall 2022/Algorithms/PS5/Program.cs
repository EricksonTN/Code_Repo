using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS4150_PS5
{
    class Program
    {
        static int count;
        static Queue<Node> queue;
        static void Main(string[] args)
        {
            count = 0;
            queue = new Queue<Node>();

            int[] StageDim = Console.ReadLine().Split(' ').Select(int.Parse).ToArray();
            string[] stageLBL = new string[StageDim[1]];
            Node[,] stage = new Node[StageDim[1], StageDim[0]];

            int startRow = 0;
            int startCol = 0;

            string currLine;
            for(int i = 0; i < StageDim[1]; i++)
            {
                currLine = Console.ReadLine();
                stageLBL[i] = currLine;
            }

            //Top and Bottom Row Cases
            for (int row = 0; row < StageDim[1]; row += StageDim[1] - 1)
            {
                for (int col = 0; col < StageDim[0]; col++)
                {
                    stage[row, col] = new Node('#', false, false, row, col);
                }
            }

            //Left and Right Column Cases
            for (int col = 0; col < StageDim[0]; col += StageDim[0] - 1)               
            {
                for (int row = 0; row < StageDim[1]; row++)
                {
                    stage[row, col] = new Node('#', false, false, row, col);
                }
            }

            // Center Tiles Cases
            for (int row = 1; row < StageDim[1]; row++)
            {
                for (int col = 1; col < StageDim[0]; col++)
                {
                    if (stageLBL[row][col] == 'P')
                    {
                        startRow = row;
                        startCol = col;
                    }
                    if (stage[row, col] != null && (stage[row, col].getType() == 'T'))
                    {
                        stage[row, col].setStinky(true);

                        //Top Neighbor
                        stage[row - 1, col].setStinky(true);

                        //Left Neighbor
                        stage[row, col - 1].setStinky(true);

                        //Right Neighbor
                        stage[row, col + 1] = new Node(stageLBL[row][col + 1], false, true, row, col);

                        //Bottom Neighbor
                        stage[row + 1, col] = new Node(stageLBL[row + 1][col], false, true, row, col);

                        continue;
                    }

                    if (stage[row, col] != null)
                    {
                        continue;
                    }

                    if (stageLBL[row][col] == 'T')
                    {
                        stage[row, col] = new Node(stageLBL[row][col], false, true, row, col);

                        //Top Neighbor
                        stage[row - 1, col].setStinky(true);

                        //Left Neighbor
                        stage[row, col - 1].setStinky(true);

                        //Right Neighbor
                        stage[row, col + 1] = new Node(stageLBL[row][col + 1], false, true, row, col);

                        //Bottom Neighbor
                        stage[row + 1, col] = new Node(stageLBL[row + 1][col], false, true, row, col);
                    }

                    else
                    {
                        stage[row, col] = new Node(stageLBL[row][col], false, false, row, col);
                    }
                }
            }

            WFS(stage[startRow, startCol], stage);
            Console.WriteLine(count);

        }

        public static void WFS(Node currNode, Node[,] stage)
        {
            queue.Enqueue(currNode);

            while (queue.Count() != 0)
            {
                Node node = queue.Dequeue();
                if (node.getTraversed() == false)
                {
                    node.setTraversed(true);

                    if (node.getType() == 'G')
                    {
                        count++;
                    }

                    if (node.getStinky())
                    {
                        continue;
                    }

                    //top nb
                    if (!(stage[node.getRow() - 1, node.getCol()].getType() == '#'))
                    {
                        queue.Enqueue(stage[node.getRow() - 1, node.getCol()]);
                    }

                    //bot nb
                    if (!(stage[node.getRow() + 1, node.getCol()].getType() == '#'))
                    {
                        queue.Enqueue(stage[node.getRow() + 1, node.getCol()]);
                    }

                    //left nb
                    if (!(stage[node.getRow(), node.getCol() - 1].getType() == '#'))
                    {
                        queue.Enqueue(stage[node.getRow(), node.getCol() - 1]);
                    }

                    //right nb
                    if (!(stage[node.getRow(), node.getCol() + 1].getType() == '#'))
                    {
                        queue.Enqueue(stage[node.getRow(), node.getCol() + 1]);
                    }
                }              
            }
        }

    }

    public class Node
    {
        private char tileType;

        private bool hasTraversed;

        private bool isStinky;

        private int rowPos;

        private int colPos;
        public Node(char type, bool traversed, bool stinky, int row, int col)
        {
            tileType = type;
            hasTraversed = traversed;
            isStinky = stinky;
            rowPos = row;
            colPos = col;
        }

        public char getType ()
        {
            return tileType;
        }

        public bool getTraversed()
        {
            return hasTraversed;
        }

        public bool getStinky()
        {
            return isStinky;
        }

        public int getRow()
        {
            return rowPos;
        }

        public int getCol()
        {
            return colPos;
        }

        public void setTraversed(bool tr)
        {
            hasTraversed = tr;
        }

        public void setStinky(bool st)
        {
            isStinky = st;
        }
    }
}
