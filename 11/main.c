#include <stdio.h>
#include <stdlib.h>
#define Black 0
#define Red 1
#define TRUE 1
#define bool char
#define ll long long int

typedef struct dp_rb_node {
    ll diff;
    ll sum_odd;
    ll sum_even;
    struct dp_rb_node* p;
    struct dp_rb_node* left;
    struct dp_rb_node* right;
    bool color;
    bool is_odd;
} Node;

typedef struct dp_rb_tree {
    struct dp_rb_node* root;
} Tree;

void init_node_list(Node* node_list, ll* max, int N)
{
    int b, s;
    *max = 0;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &b, &s);
        node_list[i].diff = b >= s ? s - b : b - s;
        *max += b >= s ? b : s;
    }
}

bool COLOR(Node* node)
{
    if (node == NULL)
        return Black;
    return node->color;
}

void update_sum(Node* x)
{
    x->sum_odd = 0;
    x->sum_even = 0;

    if (x->left != NULL) {
        x->sum_odd += x->left->sum_odd;
        x->sum_even += x->left->sum_even;
    }

    if (x->is_odd == TRUE)
        x->sum_odd += x->diff;
    else
        x->sum_even += x->diff;

    if (x->right != NULL) {
        if (x->is_odd == TRUE) {
            x->sum_odd += x->right->sum_even;
            x->sum_even += x->right->sum_odd;
        } else {
            x->sum_odd += x->right->sum_odd;
            x->sum_even += x->right->sum_even;
        }
    }
}

void update(Node* x)
{
    if (x == NULL)
        return;
    while (x->p != NULL) {
        if (x == x->p->left)
            x->p->is_odd = (x->p->is_odd + 1) % 2;
        update_sum(x->p);
        x = x->p;
    }
}

void left_rotate(Tree* tree, Node* x)
{
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->p = x;

    y->p = x->p;

    if (x->p == NULL)
        tree->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;

    if (x->is_odd == TRUE)
        y->is_odd = (y->is_odd + 1) % 2;

    update_sum(x);
    update_sum(y);
}

void right_rotate(Tree* tree, Node* y)
{
    Node* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->p = y;

    x->p = y->p;

    if (y->p == NULL)
        tree->root = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;

    x->right = y;
    y->p = x;

    if (x->is_odd == TRUE)
        y->is_odd = (y->is_odd + 1) % 2;

    update_sum(y);
    update_sum(x);
}

void insert_fixup(Tree* tree, Node* z)
{
    Node* y;
    while (COLOR(z->p) == Red) {
        /**
         *  爸爸是爺爺左邊的小孩
         */
        if (z->p == z->p->p->left) {
            y = z->p->p->right;

            /**
             * 爸爸跟叔叔都是紅色
             */
            if (COLOR(y) == Red) {
                z->p->color = Black;
                y->color = Black;
                z->p->p->color = Red;
                z = z->p->p;
            }

            /**
             * 叔叔是黑色
             */
            else {
                /**
                 * 我是爸爸右邊的小孩
                 */
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(tree, z);
                }
                z->p->color = Black;
                z->p->p->color = Red;
                right_rotate(tree, z->p->p);
            }
        }

        /**
         *  爸爸是爺爺右邊的小孩
         */
        else {
            y = z->p->p->left;

            /**
             * 爸爸跟叔叔都是紅色
             */
            if (COLOR(y) == Red) {
                z->p->color = Black;
                y->color = Black;
                z->p->p->color = Red;
                z = z->p->p;
            }

            /**
             * 叔叔是黑色
             */
            else {
                /**
                 * 我是爸爸左邊的小孩
                 */
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(tree, z);
                }
                z->p->color = Black;
                z->p->p->color = Red;
                left_rotate(tree, z->p->p);
            }
        }
    }
    tree->root->color = Black;
}

void insert_node(Tree* tree, Node* z)
{
    Node *y = NULL, *x = tree->root;
    z->color = Red;
    z->left = NULL;
    z->right = NULL;
    z->is_odd = TRUE;
    z->sum_odd = z->diff;
    z->sum_even = 0;

    while (x != NULL) {
        y = x;
        if (z->diff < x->diff)
            x = x->left;
        else
            x = x->right;
    }

    z->p = y;
    if (y == NULL)
        tree->root = z;
    else if (z->diff < y->diff)
        y->left = z;
    else
        y->right = z;

    update(z);
    insert_fixup(tree, z);
}

void delete_fixup(Tree* tree, Node* x)
{
    Node* w;
    while (x != tree->root && (COLOR(x) == Black)) {

        /**
         * 如果我是爸爸左邊的小孩
         */
        if (x == x->p->left) {
            w = x->p->right;

            /**
             * 如果我兄弟是紅的
             */
            if (COLOR(w) == Red) {
                w->color = Black;
                x->p->color = Red;
                left_rotate(tree, x->p);
                w = x->p->right;
            }

            /**
             * 如果姪子們都是黑的
             */
            if ((COLOR(w->left) == Black) && (COLOR(w->right) == Black)) {
                w->color = Red;
                x = x->p;
            } else {
                if (COLOR(w) == Black) {
                    if (w->left != NULL)
                        w->left->color = Black;
                    w->color = Red;
                    right_rotate(tree, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = Black;
                if (w->right != NULL)
                    w->right->color = Black;
                left_rotate(tree, x->p);
                x = tree->root;
                break;
            }
        }

        /**
         * 如果我是爸爸右邊的小孩
         */
        else {
            w = x->p->left;

            /**
             * 如果我兄弟是紅的
             */
            if (COLOR(w) == Red) {
                w->color = Black;
                x->p->color = Red;
                right_rotate(tree, x->p);
                w = x->p->left;
            }

            /**
             * 如果姪子們都是黑的
             */
            if ((COLOR(w->right) == Black) && (COLOR(w->left) == Black)) {
                w->color = Red;
                x = x->p;
            } else {
                if (COLOR(w->left) == Black) {
                    if (w->right != NULL)
                        w->right->color = Black;
                    w->color = Red;
                    left_rotate(tree, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = Black;
                if (w->left != NULL)
                    w->left->color = Black;
                right_rotate(tree, x->p);
                x = tree->root;
                break;
            }
        }
    }

    if (x != NULL)
        x->color = Black;
}

void replace(Node* z, Node* y)
{
    y->color = z->color;
    y->p = z->p;
    y->left = z->left;
    y->right = z->right;
    y->is_odd = z->is_odd;
    y->sum_odd = z->sum_odd;
    y->sum_even = z->sum_even;

    if (y->left != NULL)
        y->left->p = y;
    if (y->right != NULL)
        y->right->p = y;

    if (y->p == NULL)
        return;
    if (z == y->p->left)
        y->p->left = y;
    else
        y->p->right = y;
}

void delete_node(Tree* tree, Node* z)
{
    Node *x, *y, *x_p = NULL;
    if (z == NULL)
        return;

    if (z->left == NULL || z->right == NULL)
        y = z;
    else {
        y = z->right;
        while (y->left != NULL) {
            y = y->left;
        }
    }

    if (y->left != NULL)
        x = y->left;
    else
        x = y->right;

    x_p = y->p;
    if (x != NULL)
        x->p = x_p;

    if (x_p == NULL)
        tree->root = x;
    else if (y == x_p->left)
        x_p->left = x;
    else
        x_p->right = x;

    if (y != z)
        replace(z, y);

    if (x != NULL)
        update(x);
    else if (x_p != NULL)
        update(x_p);

    if (y->color == Black)
        delete_fixup(tree, x);
}

void in_order(Node* root)
{
    if (root->left != NULL)
        in_order(root->left);
    printf("%lld ", root->diff);
    if (root->right != NULL)
        in_order(root->right);
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    ll max = 0;
    Node* node_list = (Node*)calloc(N, sizeof(Node));
    Tree* rb_tree = (Tree*)calloc(1, sizeof(Tree));
    init_node_list(node_list, &max, N);
    for (int i = 0; i < N; i++) {
        insert_node(rb_tree, &node_list[i]);
    }
    printf("%lld\n", max + rb_tree->root->sum_odd);
}