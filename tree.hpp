#include <iostream>
#include <map>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <iomanip>

#define clear() printf("\033[H\033[J")

using namespace std;

struct node							// Структура для дерева
{
	int				info;			// ключ
	unsigned char	height;			// высота(глубина) дерева
	node			*left, *right;	// левая, правая ветка
	node			*parent;		// родитель

	node(int k, node *p)	{ info = k; left = right = 0; height = 1; parent = p;}
};

// cpp_AVL_tree_visualization.cpp
int				size(node *tree);
void 			insert(node **p, int k, node *parent);
int				depth(node *tree);

// print_tree.cpp
std::string		print_num(int num, char ch);
void			fill_depth(node *tree, std::string *lev_tree, std::string *lev_tree_height, int lev, int mod, int mod_h);
void			delete_columns(string	*lev_tree, string *lev_tree_height, int skip, int d, int mod_h);
void			print_tree(node *tree, int mod_h = 0);
int				check_parent(node *tree);
void			rez_check_parent(int t);
