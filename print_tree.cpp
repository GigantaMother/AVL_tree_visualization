#include "tree.hpp"

//=============================================================================================================================================================
//================ РЕЖИМ ПЕЧАТИ ДЕРЕВА С БУФЕРОМ (ДЕРЕВО НЕМНОГО МЕНЬШЕ ПО ШИРИНЕ, НО ПЕЧАТАЕТСЯ ДОЛГО, ЕСТЬ РЕЖИМЫ ПЕЧАТИ ДОП ДАННЫХ)=========================
//=============================================================================================================================================================

// Переводит число в строку (число, символ заполнения если длина числа меньше)
std::string print_num(int num, char ch)
{
	std::string temp = "";

	if (num < 0 || num > 999)
		temp += "???";
	else
	{
		if (num < 10)
			temp = temp + ch + ch;
		else if (num < 100)
			temp += ch;
		temp += to_string(num);
	}

	return (temp);
}

// Правильно добавляет данные для печати (tree - дерево, lev_tree массив строк для записи элементов дерева,
// lev_tree_height - массив строк для записи доп. данных(глубина, слой), lev - слой дерева, mod - нужный отступ,
// mod_h - режим доп. данных(1 - добовлять глубину, 2 - добавлять слой, по умолчанию ничего не добавляется))
void fill_depth(node *tree, std::string *lev_tree, std::string *lev_tree_height, int lev, int mod, int mod_h = 0)
{
	if (tree == NULL)
		return ;

	int		d = d = depth(tree), skip_pred = pow(2, d - 1) - 2, arm = pow(2, d - 1);
	string	num_str = print_num(tree->info, '0'), height_str;

	if (mod == -1)
		mod = pow(2, d) - 2;

	if (mod_h == 1)
		height_str = print_num(tree->height, '.');
	else if (mod_h == 2)
		height_str = print_num(tree->parent ? tree->parent->info : 0, '.');
	
	lev_tree[lev].replace(mod, 3, num_str);
	if (mod_h)
		lev_tree_height[lev].replace(mod, 3, height_str);

	lev++;
	if (tree->left != NULL)
	{
		if (mod_h)
			lev_tree_height[lev - 1].replace(mod - arm, arm, arm, '_');
		else
			lev_tree[lev - 1].replace(mod - arm, arm, arm, '_');
		fill_depth(tree->left, lev_tree, lev_tree_height, lev, mod - skip_pred - 2, mod_h);
	}
	if (tree->right != NULL)
	{
		if (mod_h)
			lev_tree_height[lev - 1].replace(mod + 3, arm, arm, '_');
		else
			lev_tree[lev - 1].replace(mod + 3, arm, arm, '_');
		fill_depth(tree->right, lev_tree, lev_tree_height, lev, mod + skip_pred + 2, mod_h);
	}
}

// Удаляет пустые столбцы из массива строк
void delete_columns(string	*lev_tree, string *lev_tree_height, int skip, int d, int mod_h = 0)
{
	int		start = -1, end = -1, size_del = -1;
	bool	flag = false;

	for (int i = skip - 1; i >= 0; i--)
	{
		flag = true;

		for (size_t j = 0; j < d; j++)
		{
			if (lev_tree[j][i] != ' ' && lev_tree[j][i] != '_')
			{
				flag = false;
				break ;
			}
		}
		if (flag == true && start == -1)
			start = i;
		if (flag == false && start != -1)
		{
			end = i;
			size_del = start - i;
			for (size_t j = 0; j < d; j++)
			{
				lev_tree[j].erase(end + 1, size_del);
				if (mod_h)
					lev_tree_height[j].erase(end + 1, size_del);
			}
			start = end = -1;
		}
	}
	if (start != - 1)
	{
		size_del = start - end;
		for (size_t j = 0; j < d; j++)
		{
			lev_tree[j].erase(0, size_del);
			if (mod_h)
				lev_tree_height[j].erase(0, size_del);
		}
	}
}

// Печать дерева МЕТОД С БУФЕРОМ (tree - дерево, mod_h - режим доп. данных(1 - добовлять глубину, 2 - добавлять слой, по умолчанию ничего не добавляется))
void print_tree(node *tree, int mod_h)
{
	int		time_tree = clock(), d = depth(tree), s = size(tree), skip = (pow(2, d) - 2) * 2 + 3;
	string	lev_tree[d], lev_tree_height[d];

	cout << "Tree buf\n" << "d= " << d << endl << "s= " << s << endl;
	// cout << "skip(max)= " << skip << endl;
	cout << "Заполненно: " << (float)s/(float)(pow(2, d) - 1) * 100 << "%" << endl;

	if (mod_h != 1 && mod_h != 2)
		mod_h = 0;
	// Заполняем строки пробелами
	if (d > 0)
	{
		lev_tree[0].insert(0, skip, ' ');
		if (mod_h == 1 || mod_h == 2)
			lev_tree_height[0] = lev_tree[0];
	}
	for (size_t i = 1; i < d; i++)
	{
		lev_tree[i] = lev_tree[0];
		if (mod_h == 1 || mod_h == 2)
			lev_tree_height[i] = lev_tree[0];
	}
	// Заполняем строки нужными данными (0, -1 ОБЯЗАТЕЛЬНО!!!)
	fill_depth(tree, lev_tree, lev_tree_height, 0, -1, mod_h);
	// Удаляем лишнии столбцы
	delete_columns(lev_tree, lev_tree_height, skip, d, mod_h);
	// Печатаем дерево
	for (size_t i = 0; i < d; i++)
	{
		cout << "|" << lev_tree[i] << endl;
		if (mod_h == 1 || mod_h == 2)
			cout << "|" << lev_tree_height[i] << endl;
	}

	std::cout << "\nTime: " << (float)(clock() - time_tree) / (float)1000000 << " c" << endl;
	rez_check_parent(check_parent(tree));
}

// Проверяет родителей (правильно ли они поменялись при балансировке)
int	check_parent(node *tree)
{
	int rez = 0;

	if (tree == NULL)
		return (0);
	if (tree->left != NULL)
	{
		if (tree->left->parent->info != tree->info)
		{
			cout << tree->left->parent->info << " != " << tree->info << endl;
			rez++;
		}
		else
			rez += check_parent(tree->left);
	}
	if (tree->right != NULL)
	{
		if (tree->right->parent->info != tree->info)
		{
			cout << tree->right->parent->info << " != " << tree->info << endl;
			rez++;
		}
		else
			rez += check_parent(tree->right);
	}
	return (rez);
}

// Если родители были поменены неверно, то программа завершится 
void	rez_check_parent(int t)
{
	cout << "parents: ";
	if (!t)
		cout << "OK\n";
	else
	{
		cout << "KO KO KO KO KO KO KO KO \n";
		exit(0);
	}
}
