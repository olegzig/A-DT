using System;

internal class Node
{
    public int data;
    public Node left, right;

    public Node(int item)
    {
        data = item;
        left = right = null;
    }
}

internal class BinarySearchTree
{
    private Node root;

    private BinarySearchTree()
    {
        root = null;
    }

    public void Insert(int data)
    {
        root = InsertRec(root, data);
        printAll();
    }

    private Node InsertRec(Node root, int data)
    {
        if (root == null)
        {
            root = new Node(data);
            return root;
        }

        if (data < root.data)
        {
            root.left = InsertRec(root.left, data);
        }
        else if (data > root.data)
        {
            root.right = InsertRec(root.right, data);
        }

        return root;
    }

    public Node DeleteNode(Node root, int data)
    {
        if (root == null)
        {
            return root;
        }

        if (data < root.data)
        {
            root.left = DeleteNode(root.left, data);
        }
        else if (data > root.data)
        {
            root.right = DeleteNode(root.right, data);
        }
        else
        {
            if (root.left == null)
            {
                return root.right;
            }
            else if (root.right == null)
            {
                return root.left;
            }

            root.data = MinValue(root.right);

            root.right = DeleteNode(root.right, root.data);
        }

        printAll();
        return root;
    }

    private int MinValue(Node root)
    {
        int minValue = root.data;
        while (root.left != null)
        {
            minValue = root.left.data;
            root = root.left;
        }
        return minValue;
    }

    public Node Search(int value)
    {
        if (root.data.Equals(value))
            return root;
        else
        {
            return SearchHelper(value, root);
        }
    }

    private Node SearchHelper(int Value, Node node)
    {
        if (node == null)
            return node;

        if (Value.Equals(node.data))
            return node;

        if (Value.CompareTo(node.data) == -1)
        {
            node = SearchHelper(Value, node.left);
        }
        else
        {
            node = SearchHelper(Value, node.right);
        }

        return node;
    }

    private void InOrder(Node root)
    {
        if (root != null)
        {
            InOrder(root.left);
            Console.Write(root.data + " ");
            InOrder(root.right);
        }
    }

    // Прямой обход
    private void printPreorder(Node node)
    {
        if (node == null)
            return;

        Console.Write(node.data + " ");
        printPreorder(node.left);
        printPreorder(node.right);
    }

    // Центрированый обход
    private void printInorder(Node node)
    {
        if (node == null)
            return;

        printInorder(node.left);
        Console.Write(node.data + " ");
        printInorder(node.right);
    }

    // Обратный обход
    private void printPostorder(Node node)
    {
        if (node == null)
            return;

        printPostorder(node.left);
        printPostorder(node.right);
        Console.Write(node.data + " ");
    }

    // Интерфейс для вызова этих обходов
    public void printPreorder()
    { printPreorder(root); }

    public void printInorder()
    { printInorder(root); }

    public void printPostorder()
    { printPostorder(root); }

    public void printAll()
    {
        Console.WriteLine("----------------------------------------------------");
        Console.WriteLine("Прямой обход:");
        printPreorder();

        Console.WriteLine("\nСимметричный обход:");
        printInorder();

        Console.WriteLine("\nОбратный обход:");
        printPostorder();
        Console.WriteLine("\n----------------------------------------------------");
    }

    private static void Main()
    {
        BinarySearchTree tree = new BinarySearchTree();
        int[] elements = new int[15];

        Console.WriteLine("Ручной ввод, или автоматический?\nРучной ввод: 1\nАвтоматический: 0\n");
        var input = Console.ReadLine();
        if (input == "1")
        {
            for (int i = 0; i < 15; i++)
            {
                Console.Write("Введите элемент №"+ i +": ");
                input = Console.ReadLine();
                elements[i] = int.Parse(input);
            }
        }
        else
        {
            if (input != "0")
            {
                Console.WriteLine("Неправильный ввод. Ставлю значения по умолчанию");
            }
            elements = new int[] { 5, 2, 3, 4, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
        }

        foreach (int element in elements)
        {
            tree.Insert(element);
        }

        tree.printAll();

        Console.ReadKey();
    }
}