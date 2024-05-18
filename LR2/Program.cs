using System;
using System.Collections.Generic;

namespace LR2
{
    internal class Program
    {
        const int Infinity = int.MaxValue;
        //инициализируем граф
        public static readonly int[,] adjacencyMatrix = new int[8, 8]
        {
                {Infinity, 1       , Infinity, 1       , Infinity, Infinity, Infinity, Infinity},
                {Infinity, Infinity, 1       , Infinity, 1       , Infinity, Infinity, Infinity},
                {Infinity, Infinity, Infinity, Infinity, Infinity, 1       , Infinity, Infinity},
                {Infinity, Infinity, Infinity, Infinity, Infinity, 1       , Infinity, Infinity},
                {Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, 1       , Infinity},
                {Infinity, Infinity, Infinity, 1       , Infinity, Infinity, Infinity, Infinity},
                {Infinity, Infinity, Infinity, Infinity, Infinity, 1       , Infinity, Infinity},
                {Infinity, Infinity, Infinity, Infinity, Infinity, Infinity, Infinity,        1}
        };

        private static void Main()
        {
            int verticle = 1;

            ShowAdjasmentVerticles(verticle);
            ShowVerticlesFromWichYouCanGetToVerticle(verticle);
            FloydWarshall(adjacencyMatrix);

            Console.ReadKey();
        }

        private static void ShowAdjasmentVerticles(int veticle)
        {
            List<int> adjacentVertices = new List<int>();
            for (int i = 0; i < adjacencyMatrix.GetLength(0); i++)
            {
                if (adjacencyMatrix[veticle, i] == 1)
                {
                    adjacentVertices.Add(i);
                }
            }
            if (adjacentVertices.Count == 0)
            {
                Console.WriteLine("Вершина " + veticle + " не имеет смежных вершин");
            }
            else
            {
                Console.WriteLine("Смежные вершины с вершиной " + veticle + ": " + string.Join(", ", adjacentVertices));
            }
        }

        private static void ShowVerticlesFromWichYouCanGetToVerticle(int veticle)
        {
            List<int> precedingVertices = new List<int>();
            for (int i = 0; i < adjacencyMatrix.GetLength(0); i++)
            {
                if (adjacencyMatrix[i, veticle] == 1)
                {
                    precedingVertices.Add(i);
                }
            }
            if (precedingVertices.Count == 0)
            {
                Console.WriteLine("Нет вершин, из которых можно попасть в вершину " + veticle);
            }
            else
            {
                Console.WriteLine("Из каких вершин можно попасть в вершину " + veticle + ": " + string.Join(", ", precedingVertices));
            }
        }

        private static void FloydWarshall(int[,] graph)
        {
            int verticesCount = graph.GetLength(0);

            int[,] dist = new int[verticesCount, verticesCount];
            //Заполняем матрицу расстояния
            for (int i = 0; i < verticesCount; i++)
            {
                for (int j = 0; j < verticesCount; j++)
                {
                    dist[i, j] = graph[i, j];
                }
            }

            //Алгоритм Флойда-Уоршелла
            for (int k = 0; k < verticesCount; k++)
            {
                for (int i = 0; i < verticesCount; i++)
                {
                    for (int j = 0; j < verticesCount; j++)
                    {
                        if (dist[i, k] != Infinity && dist[k, j] != Infinity &&
                            dist[i, k] + dist[k, j] < dist[i, j])
                        {
                            dist[i, j] = dist[i, k] + dist[k, j];
                        }
                    }
                }
            }

            // Выводим найденные кратчайшие расстояния
            for (int i = 0; i < verticesCount; i++)
            {
                for (int j = 0; j < verticesCount; j++)
                {
                    if (dist[i, j] == Infinity)
                    {
                        Console.Write("INF \t");
                    }
                    else
                    {
                        Console.Write(dist[i, j] + "\t");
                    }
                }
                Console.WriteLine();
            }
        }
    }
}