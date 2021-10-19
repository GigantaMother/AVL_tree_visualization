#include "tree.hpp"

node	*tree = NULL;

//=============================================================================================================================================================
//==================================================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ДЕРЕВА =====================================================================
//=============================================================================================================================================================
// Считает кол-во элементов в дереве
int size(node *tree)
{
	int s = 0;

	if (tree == NULL)
		return (0);
	s++;

	s += size(tree->l);
	s += size(tree->r);

	return (s);
}

// Считает глубину дерева
int depth(node *tree)
{
	if (tree == NULL)
		return (0);

	int leftDepth = depth(tree->l);
	int rightDepth = depth(tree->r);

	return (leftDepth > rightDepth ? leftDepth + 1 : rightDepth + 1);
}

//=============================================================================================================================================================

unsigned char height(node* p)
{
	return p?p->height:0;
}

int bfactor(node* p)
{
	return height(p->r)-height(p->l);
}

void fixheight(node* p)
{
	unsigned char hl = height(p->l);
	unsigned char hr = height(p->r);
	p->height = (hl>hr?hl:hr)+1;
}

node* rotateright(node* p) // правый поворот вокруг p
{
	node* q = p->l;
	p->l = q->r;
	q->r = p;
	fixheight(p);
	fixheight(q);
	return q;
}

node* rotateleft(node* q) // левый поворот вокруг q
{
	node* p = q->r;
	q->r = p->l;
	p->l = q;
	fixheight(q);
	fixheight(p);
	return p;
}

node* balance(node* p) // балансировка узла p
{
	fixheight(p);
	if( bfactor(p)==2 )
	{
		cout << "ЛЕВО\n";
		if( bfactor(p->r) < 0 )
			p->r = rotateright(p->r);
		return rotateleft(p);
	}
	if( bfactor(p)==-2 )
	{
		cout << "ПРАВО\n";
		if( bfactor(p->l) > 0 )
			p->l = rotateleft(p->l);
		return rotateright(p);
	}
	return p; // балансировка не нужна
}

node* insert(node* p, int k) // вставка ключа k в дерево с корнем p
{
	if(!p)
		return new node(k);
	if( k < p->info)
		p->l = insert(p->l, k);
	else
		p->r = insert(p->r, k);
	return balance(p);
}

node* findmin(node* p) // поиск узла с минимальным ключом в дереве p 
{
	return p->l ? findmin(p->l) : p;
}

node* removemin(node* p) // удаление узла с минимальным ключом из дерева p
{
	if( p->l == 0)
		return p->r;
	p->l = removemin(p->l);
	return balance(p);
}

node* remove(node* p, int k) // удаление ключа k из дерева p
{
	if( !p ) return 0;
	if( k < p->info )
		p->l = remove(p->l ,k);
	else if( k > p->info )
		p->r = remove(p->r, k);	
	else //  k == p->key 
	{
		node* q = p->l;
		node* r = p->r;
		delete p;
		if( !r ) return q;
		node* min = findmin(r);
		min->r = removemin(r);
		min->l = q;
		return balance(min);
	}
	return balance(p);
}
//=============================================================================================================================================================
//================================================================ БЛОК С ТЕСТОМ ==============================================================================
//=============================================================================================================================================================

int	main ()
{
	string	str_test;
	int		rand_elem = 0;

	srand(time(NULL));
	
	// for (size_t i = 0; i < 1000 ; i++)
	// {
	// 	clear();
	// 	cout << "=======================" << endl;
	// 	print_tree(tree, 1);

	// 	//rand_elem = rand() % 1000;
	// 	cout << "Введите число\n";
	// 	cin >> rand_elem;
	// 	tree = insert(tree, rand_elem);
	// }
	tree = insert(tree, 40);
	tree = insert(tree, 30);
	tree = insert(tree, 60);
	tree = insert(tree, 100);
	tree = insert(tree, 50);
	tree = insert(tree, 150);
	tree = insert(tree, 149);
	tree = insert(tree, 151);
	tree = insert(tree, 10);
	tree = insert(tree, 5);
	tree = insert(tree, 12);
	tree = insert(tree, 65);

	print_tree(tree, 1);
	cout << "pam pam" << endl;
	tree = remove(tree, 60);

	print_tree(tree, 1);


	return (0);
}
