#include "tree.hpp"

//=============================================================================================================================================================
//==================================================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ДЕРЕВА =====================================================================
//=============================================================================================================================================================
// Считает кол-во элементов в дереве
int	size(node *tree)
{
	int s = 0;

	if (tree == NULL)
		return (0);
	s++;

	s += size(tree->left);
	s += size(tree->right);

	return (s);
}

// Считает глубину дерева
int	depth(node *tree)
{
	if (tree == NULL)
		return (0);

	int leftDepth = depth(tree->left);
	int rightDepth = depth(tree->right);

	return (leftDepth > rightDepth ? leftDepth + 1 : rightDepth + 1);
}

unsigned char	height(node* p)
{
	return (p ? p->height : 0);
}

int	bfactor(node* p)
{
	return (height(p->right) - height(p->left));
}

void	fixheight(node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);

	p->height = (hl > hr ? hl : hr) + 1;
}

// правый поворот вокруг p
node*	rotateright(node* p)
{
	node* q = p->left;

	p->left->parent = p->parent;
	p->parent = p->left;
	if (p->left->right)
		p->left->right->parent = p;

	p->left = q->right;
	q->right = p;

	fixheight(p);
	fixheight(q);
	return (q);
}

// левый поворот вокруг q
node*	rotateleft(node* q)
{
	node* p = q->right;

	q->right->parent = q->parent;
	q->parent = q->right;
	if (q->right->left)
		q->right->left->parent = q;

	q->right = p->left;
	p->left = q;

	fixheight(q);
	fixheight(p);

	return (p);
}

// балансировка узла p
node*	balance(node* p)
{
	fixheight(p);
	if( bfactor(p)== 2)
	{
		if( bfactor(p->right) < 0 )
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if( bfactor(p)==-2 )
	{
		if( bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p;
}

// вставка ключа k в дерево с корнем p
node*	insert(node *p, int k, node *parent)
{
	if(!p)
		return new node(k, parent);
	if( k <= p->info)
		p->left = insert(p->left, k, p);
	else
		p->right = insert(p->right, k, p);

	return balance(p);
}

// поиск узла с минимальным ключом в дереве p 
node*	findmin(node* p)
{
	return (p->left ? findmin(p->left) : p);
}

// удаление узла с минимальным ключом из дерева p
node*	removemin(node* p)
{
	if( p->left == 0)
	{
		if (p->right)
			p->right->parent = p->parent;
		return (p->right);
	}
	p->left = removemin(p->left);
	return (balance(p));
}

// удаление ключа k из дерева p
node* remove(node* p, int k)
{
	if(!p)
		return (0);
	if( k < p->info )
		p->left = remove(p->left ,k);
	else if( k > p->info )
		p->right = remove(p->right, k);	
	else
	{
		node*	q = p->left;
		node*	r = p->right;
		node*	temp_parent = p->parent;

		delete p;
		if(!r)
		{
			if (q)
				q->parent = p->parent;
			return q;
		}

		node* min = findmin(r);

		min->right = removemin(r);
		if (min->right)
			min->right->parent = min;

		min->left = q;
		if (min->left)
			min->left->parent = min;

		min->parent = temp_parent;

		return (balance(min));
	}
	return (balance(p));
}
//=============================================================================================================================================================
//================================================================ БЛОК С ТЕСТОМ ==============================================================================
//=============================================================================================================================================================
// добавление/удаление случайныx элементов
void	test_automatically(node	*tree, int mod_time, int mod_step, int mod_print)
{
	string	str_test;
	int		rand_elem = 0, int_mod = 0, step = 1, delay = 300000;

	srand(time(NULL));
	clear();
	
	while (step)
	{
		if (mod_step)
		{
			cout << "============================================================ДО============================================================" << endl;
			print_tree(tree, mod_print);
			cout << "============================================================ПОСЛЕ============================================================" << endl;
		}
		rand_elem = rand() % 999;
		if (step % 2 == 1)
		{
			cout << "Вставляем: " << rand_elem << endl;
			tree = insert(tree, rand_elem, tree);
		}
		else
		{
			cout << "Удаляем: " << rand_elem << endl;
			tree = remove(tree, rand_elem);
		}
		print_tree(tree, mod_print);
		if (mod_time == 0)
			usleep(delay);
		else
			getline(cin, str_test);
		clear();
		step++;
	}
}

// Парсер введённого числа
int	parcer_num(string str)
{
	int num = 0, i = 0;

	if (str.length() == 0 || (str.length() == 1 && str[0] == '+'))
		return (-1);
	if (str[0] == '+')
		i++;
	for (; str[i]; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		else
			num = num * 10 + str[i] - 48;
		if (num > 999)
			return (-1);
	}
	return (num);
}

void	test_arm(node *tree, int mod_step, int mod_print)
{
	int		mod = 0, num = -1;
	string	str_mod;

	clear();

	while (42)
	{
		if (mod == 0)
			cout << "Введите элемент для вставки: ";
		else
			cout << "Введите элемент для удаления: ";
		if (!getline(cin, str_mod))
		{
			cout << "Error\n";
			exit (1);
		}
		if (str_mod == "add")
			mod = 0;
		else if (str_mod == "rem")
			mod = 1;
		else
		{
			if ((num = parcer_num(str_mod)) == -1)
				cout << "Число введено неверно\n";
			else
			{
				clear();
				if (mod_step)
				{
					cout << "============================================================ДО============================================================" << endl;
					print_tree(tree, mod_print);
					cout << "===========================================================ПОСЛЕ==========================================================" << endl;
				}
				if (mod == 0)
					tree = insert(tree, num, tree);
				else
					tree = remove(tree, num);
				print_tree(tree, mod_print);
			}
		}
	}
}

// Копирвоание дерева
node*	copy(node* tree)
{
	if (tree == NULL)
		return (NULL);

	node* tree_copy = new node(tree->info, tree->parent);
	tree_copy->left = copy(tree->left);
	tree_copy->right = copy(tree->right);
	tree_copy->height = height(tree);
	
	return (tree_copy);
}

int	main ()
{
	node	*tree = NULL;
	string	str_mod;

	cout
	<< "Запустите один из следующих режимов (алгоритм печати дерева может отображать только число от 0 до 999)" << endl
	<< "1 - Добавление и удаление случайных элементов автоматически" << endl
	<< "2 - Добавление и удаление случайных элементов автоматически (после любого ввода)" << endl
	<< "3 - Ручное управление, введите \"add\", что бы перейти в режим добавления элементов или \"rem\" для режима удаления" << endl
	<< "1+, 2+, 3+ - Печатается дерево до действий над ним, потом после" << endl
	<< "1h, 2h, 3h - Под значением узла дерева выводится значение его родителя (если родителя нет - nul)" << endl
	<< "1p, 2p, 3p - Под значением узла дерева выводится глубина узла (минимальная глубина = 1)" << endl
	<< "1+p, 2+p, 3+p, 1+h, 2+h, 3+h - Комбинированные режимы" << endl
	<< "control + c - выход" << endl;
	
	if (!getline(cin, str_mod))
	{
		cout << "Error\n";
		exit (1);
	}
	if (str_mod == "1")
		test_automatically(tree, 0, 0, 0);
	else if (str_mod == "1+")
		test_automatically(tree, 0, 1, 0);
	else if (str_mod == "1h")
		test_automatically(tree, 0, 0, 1);
	else if (str_mod == "1p")
		test_automatically(tree, 0, 0, 2);
	else if (str_mod == "1+h")
		test_automatically(tree, 0, 1, 1);
	else if (str_mod == "1+p")
		test_automatically(tree, 0, 1, 2);
	if (str_mod == "2")
		test_automatically(tree, 1, 0, 0);
	else if (str_mod == "2+")
		test_automatically(tree, 1, 1, 0);
	else if (str_mod == "2h")
		test_automatically(tree, 1, 0, 1);
	else if (str_mod == "2p")
		test_automatically(tree, 1, 0, 2);
	else if (str_mod == "2+h")
		test_automatically(tree, 1, 1, 1);
	else if (str_mod == "2+p")
		test_automatically(tree, 1, 1, 2);
	else if (str_mod == "3")
		test_arm(tree, 0, 0);
	else if (str_mod == "3+")
		test_arm(tree, 1, 0);
	else if (str_mod == "3h")
		test_arm(tree, 0, 1);
	else if (str_mod == "3p")
		test_arm(tree, 0, 2);
	else if (str_mod == "3+h")
		test_arm(tree, 1, 1);
	else if (str_mod == "3+p")
		test_arm(tree, 1, 2);
	else
		cout << "Неверный ввод\n";

	return (0);
}
