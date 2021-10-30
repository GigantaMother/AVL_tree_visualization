#include "tree.hpp"

node	*tree = NULL;
node	*tree_copy = NULL;

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
	if( k < p->info)
		p->left = insert(p->left, k, p);
	else if (k > p->info)
		p->right = insert(p->right, k, p);
	else
		cout << "Элемент есть\n";

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

void	test_cycle()
{
	string	str_test;
	int		rand_elem = 0, int_mod = 0, delay = 300000;

	srand(time(NULL));
	
	while (42)
	{
		// Вставим рандомный элемент
		rand_elem = rand() % 999;
		tree = insert(tree, rand_elem, tree);
		// Удалим рандомный элемент
		rand_elem = rand() % 999;
		tree = remove(tree, rand_elem);
		// Напечатаем дерево
		print_tree(tree, 0);
		// Подождём, что бы пользователь успел увидеть изменения
		usleep(delay);
		//getline(cin, str_test);
		clear();
	}
}

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
	test_cycle();

	return (0);
}
