// TOP_work_03.07.22.cpp : 
// Binary_Tree (Бинарное дерево).
// + Балансировка дерева  (AVL -Tree).



#include <iostream>
#include <cmath>

using namespace std;


template <typename T>
struct Node                       // Структура "Узел".
{
    T id;                         // значение.
    Node<T>* left = nullptr;      // указатель на левый объект(потомок)
    Node<T>* right = nullptr;     // указатель на правый объект(потомок)
    Node<T>* parent = nullptr;    // родитель.
    unsigned char height = 0;     //высота поддерева с корнем в данном узле.

};

template <typename T>
class BinaryTree                 // класс "Бинарное дерево"
{
    Node <T>* root = nullptr;    // указатель на корень дерева.
    int size = 0;                // размер дерева (количество эл. дерева).

public:
    BinaryTree(){}

    BinaryTree(T data)
    {
        Node <T>* node = new Node<T>{ data };   //???
        root = node;
        root->height = 1;
        size++;
    }
    ~BinaryTree()
    {
        delete_tree(root);
    }

    //рекурсивно удаляет левое поддерево, правое поддерево, затем сам элемент.
    void delete_tree(Node<T>* node)
    {
        if (node)
        {
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }
    }

    // 1. обёртка для поля height, может работать с пустыми деревьями.
    unsigned char heightNode(Node<T>* node)
    {
        return node ? node->height : 0;
    }
    // 2. вычисляет balance factor (коэффициент сбалансированности узла).
    // Разницу высоты  между левым и правым поддеревом.
    int b_factor(Node<T>* node)
    {
        cout << " b_factor: " << node->id <<" node heigth: "
            << heightNode(node->right) - heightNode(node->left) << endl;//////////////
        return heightNode(node->right) - heightNode(node->left);
    }

    // 3. корректирует значение height в процессе балансировки.
    // Новая высота узла равна max height из поддеревьев.
    void correct_height(Node<T>* node)
    {
        unsigned char hl = heightNode(node->left);
        unsigned char hr = heightNode(node->right);
        node->height = (hl > hr ? hl : hr) + 1; // 1 - parent поддеревьев.
    }
    //правый (малое вращение) поворот вокруг node;
    Node<T>* rotationRight(Node<T>* node)
    {
        cout << " rotationRight: " << node->id << endl;///////////

        Node<T>* buf = node->left;
        buf->left->parent = buf;
        buf->parent = node->parent;
        buf->parent->left = buf;
        buf->right = node;
        node->parent = buf;
        node->left = nullptr;

        correct_height(node);
        correct_height(buf);
        correct_height(node->parent);
        
         return buf;
    }
    // левый (малое вращение) поворот вокруг node.
    // возвращает новый корень полученного дерева.
    Node<T>* rotationLeft(Node<T>* node)
    {
        cout << " rotationLeft: " << node->id << endl;//////

        Node<T>* buf = node->right;
        buf->right->parent = buf;
        buf->parent = node->parent;
        buf->parent->right = buf;
        buf->left = node;
        node->parent = buf;
        node->right = nullptr;

        correct_height(node);
        correct_height(buf);
        correct_height(node->parent);

        return buf;

    }
    // балансировка узла p, сводится к проверке условия и
    //выполнению повортов (вращения) поддеревьев.
    Node<T>* balance(Node<T>* node)
    {

        cout << " balance: " <<node->id<< endl;//////////
        correct_height(node);              //больший heght из поддеревьев.

        if (b_factor(node) == 2)            //right > left;
        {
            cout << " b_factor(node->right): " << node->right->id << ": "
                << b_factor(node->right) << endl;/////

            if (b_factor(node->right) < 0)
                node->right = rotationRight(node->right);
            return rotationLeft(node);
        }
        if (b_factor(node) == -2)                 //left > right;
        {
            cout << " b_factor(node->left): " << node->left->id << ": "
                << b_factor(node->left) << endl;/////

            if (b_factor(node->left) > 0)
                node->left = rotationLeft(node->left);
            return rotationRight(node);
        }
        return node; //балансировка не нужна.
    }

    // добавление элемента(узла) 
    // + модификация (AVL балансировка) дерева.
    void Insert(T data)
    {       
        cout <<endl<<" Insert: " << data << endl;  /////////////
        if (!size)  BinaryTree(data);
        else
        {
            Node <T>* iter = root;         //создаем указатель,для лазанья...
            Node <T>* node = new Node <T>{ data };//создаем новый узел.

            while (true)
            {
                if (node->id > iter->id)           //если(новый) наследник большой
                {     
                    if (iter->right == nullptr)     // находим лист.
                    {
                        node->parent = iter;
                        iter->right = node;
                              
                        break;
                    }
                    else
                    {
                        iter = iter->right;       //ползем по ветвям (right)
                        iter->height++;
                    }
                }
                else                               //если (новый) наследник маленький.
                {
                    if (iter->left == nullptr)     // ищем лист.
                    {
                        node->parent = iter;
                        iter->left = node;
                       
                        break;
                    }

                    else
                    {
                        iter = iter->left;       //ползем по ветвям (left)
                        iter->height++;
                    }
                }
            }
            size++;         // корректируем размер дерева
            balance1(root);      // проверяем балансировку
       

            //if (abs(depthL - depthR) > 1)
            //{
            //    cout << "balance:" << balances++ << endl;///////
            //   // balance();
            //}
        }
    }

    void balance1(Node<T>* node)
    {
        balance(node);
       if (node->left)
          balance1(node->left);      
       if (node->right)
          balance1(node->right);
    }

    //вывод(рекурсивный) (обход прямой)
    void print(Node<T>* node = nullptr) // аргумент по умолчанию.
    {
        //cout << " depthL: " << depthL<< " depthR: " << depthR << endl;////////
        if (!node) node = root;        // <=>(node==nullptr)
        cout <<" "<< node->id << " ";
        if(node->left)                 //  <=>(node->left!=nullptr) 
            print(node->left);
        if (node->right != nullptr)
            print(node->right);
     
    }

    // поиск элемента.
    Node<T>* search(T data) {
        Node<T>* iter = root;
        while (true) 
        {
            if (data == iter->id) // как только находим совпадение
                return iter;      // возвращаем адрес.

            else if (data > iter->id)
            {
                if (iter->right == nullptr)
                    return nullptr;
                else iter = iter->right;  //опускаемся по ветви вправо.
            }

            else if (data < iter->id) 
            {
                if (iter->left == nullptr)
                    return nullptr;
                else iter = iter->left;  //опускаемся по ветви влево.
            }
        }
    }


    //удаление узла (по значению ).
    void remove(T data)
    {
        Node<T>* node = search(data);  // находим узел.

         //если элемент лист (sheet).
        if (node->left == nullptr && node->right == nullptr)
        {
            if (node->parent->left == node) // если эл.< parent.
                node->parent->left = nullptr;
            else node->parent->right = nullptr;
            delete node;
        }
        //если нет предка < node (предок > parent)
        else if (node->left == nullptr)
        {
            if (node->parent->left == node) 
                node->parent->left = node->right;
            else node->parent->right = node->right;
            delete node;
        }
         //если нет предка > node (предок < parent)
        else if (node->right == nullptr) 
        {
            if (node->parent->left == node) //лишнее.
                node->parent->left = node->left;
            else node->parent->right = node->left;
            delete node;
        }
        else 
        {
            //если удаляемый узел является корнем. 
            if (node == root)
            {
                // переходим на right ветвь.
                Node<T>* iter = node->right;
                //ищем самый левый эл. в right ветве.
                while (iter->left != nullptr)
                {
                    iter = iter->left;
                }
                Node<T>* tmp = node;  // tmp = root
                node = iter;
                
                if (node->left == nullptr && node->right == nullptr)
                    node->parent->left = nullptr;
                else node->parent->left = node->right;

                //node->parent=tmp->parent;
                // node становится root
                node->left = tmp->left;      //меняем у root left
                node->right = tmp->right;    //меняем у root right
                //node->parent->left=node;
                //переписываем у потомков родителя.
                node->left->parent = node;
                node->right->parent = node;
                node->parent = nullptr;//удаляем у root parent.
                delete tmp;
                root = node;
            }
            //если узел меньше предка.
            else if (node->id < node->parent->id)
            {
                // 1 шаг вправо.
                Node<T>* iter = node->right;
                // идем влево до конца.
                while (iter->left != nullptr) 
                {
                    iter = iter->left;
                }
                Node<T>* tmp = node; //сохр. узел во врем. переменную.
                node = iter;  // узел самый маленький.
                //если нет потомков right(особенно!)
                if (node->left == nullptr && node->right == nullptr)
                    node->parent->left = nullptr;
                else //если есть right потомок.
                {
                    node->parent->left = node->right;
                }

                node->parent = tmp->parent;
                node->left = tmp->left;
                node->right = tmp->right;
                node->parent->left = node;
                node->left->parent = node;
                node->right->parent = node;
                delete tmp;
            }
            else   //если узел больше предка.
            {
                // 1 шаг влево.
                Node<T>* iter = node->left;
                 // идем вправо до конца.
                while (iter->right != nullptr) 
                {
                    iter = iter->right;
                }
                Node<T>* tmp = node; // сохр.узел во врем.переменную.
                node = iter;
                // если нет предков.
                if (node->left == nullptr && node->right == nullptr)
                    node->parent->right = nullptr;
                else // если есть предки (left) переписываем указ.
                    //у parent.
                {
                    node->parent->right = node->left;
                }
                node->parent = tmp->parent;
                node->left = tmp->left;
                node->right = tmp->right;
                node->parent->right = node;
                node->left->parent = node;
                node->right->parent = node;
                delete tmp;
            }
        }
        size--;
    }
};



int main()
{
    setlocale(LC_ALL, "ru");



    BinaryTree <int> tree1(25);
    cout << " tree1: "; tree1.print();            //обход дерева прямой.
    tree1.Insert(10);
    cout << endl << " tree2: "; tree1.print();
    tree1.Insert(26);
    cout << endl << " tree3: "; tree1.print();
    tree1.Insert(27);
    cout << endl << " tree4: "; tree1.print();
    tree1.Insert(9);
    cout << endl << " tree5: "; tree1.print();
    tree1.Insert(5);
    cout << endl << " tree6: "; tree1.print();
    tree1.Insert(3);
    cout << endl << " tree7: "; tree1.print();
    tree1.Insert(28);
    cout << endl << " tree8: "; tree1.print();
    tree1.Insert(29);
    cout << endl << " tree9: "; tree1.print();
    cout << endl;

    tree1.remove(10);
    cout << endl << " tree delete 10: "; tree1.print();







   // BinaryTree <double> tree(15);
   // tree.Insert(10);
   // tree.Insert(8);
   // tree.Insert(6);
   // tree.Insert(12);
   // tree.Insert(11);
   // tree.Insert(11.5);
   // tree.Insert(14);
   // tree.Insert(13);
   // tree.Insert(20);
   // tree.Insert(24);
   // tree.Insert(19);
   // tree.Insert(18);
   // tree.Insert(19.5);
   // tree.print();
   // cout << endl;
   // //cout << endl << tree.search(18);
   //// cout << endl;
   // tree.remove(15);
   // tree.print();
   // cout << endl;
   // tree.remove(12);  //ошибка!!!, при удалении "12"меняются местами следующии
   // // два объекта (элемента).
   // tree.print();
   // cout << endl;
   // cout <<"search(24): " << tree.search(24) << endl;  //адрес.
   // cout << "search(25): " << tree.search(25) << endl; //0000000.



    /*BinaryTree <int> tree(15);
    tree.Insert(9);
    tree.Insert(1);
    tree.Insert(0);
    tree.Insert(19);
    tree.Insert(25);
    tree.Insert(24);
    tree.Insert(18);
    tree.Insert(16);
    tree.Insert(17);
    tree.Insert(14);
    tree.Insert(30);
    tree.Insert(5);
    tree.Insert(10);
    tree.print();
    cout << endl;
    cout << endl << tree.search(18);
    cout << endl;
    tree.remove(10);
    tree.print();
    cout << endl;*/



    return 0;
}
