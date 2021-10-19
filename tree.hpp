#include <iostream>
#include <map>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <iomanip>

#define clear() printf("\033[H\033[J")

using namespace std;

// Структура для дерева
struct node
{
	int				info;	// ключ
	unsigned char	height;	// высота(глубина) дерева
	node			*l, *r;	// левая, правая ветка

	node(int k)	{ info = k; l = r = 0; height = 1;}
};

// cpp_tree_visualization.cpp
int				size(node *tree);
int				depth(node *tree);
node*			insert(node* p, int k);

// print_tree
std::string		print_num(int num, char ch);
void			fill_depth(node *tree, std::string *lev_tree, std::string *lev_tree_height, int lev, int mod, int mod_h);
void			delete_columns(string	*lev_tree, string *lev_tree_height, int skip, int d, int mod_h);
void			print_tree(node *tree, int mod_h = 0);
