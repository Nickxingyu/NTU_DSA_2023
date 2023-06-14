#include <stdio.h>
#include <stdlib.h>
#define Black 0
#define Red 1
#define TRUE 1
#define bool char
#define ll long long int
#define H 1000000007

typedef struct dp_rb_node {
    ll cheap;
    ll expensive;
    ll sum_odd;
    ll sum_even;
    struct dp_rb_node* p;
    struct dp_rb_node* left;
    struct dp_rb_node* right;
    bool color;
    bool is_odd;
} Node;

typedef struct dp_rb_tree {
    ll max;
    ll cost;
    Node* root;
} Tree;

ll diff(Node* node)
{
    return node->cheap - node->expensive;
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
        x->sum_odd += diff(x);
    else
        x->sum_even += diff(x);

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
    if (x->left == NULL)
        x->is_odd = TRUE;
    update_sum(x);
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
    if (z == NULL)
        return;

    tree->max += z->expensive;

    Node *y = NULL, *x = tree->root;
    z->color = Red;
    z->left = NULL;
    z->right = NULL;
    z->is_odd = TRUE;
    z->sum_odd = diff(z);
    z->sum_even = 0;

    while (x != NULL) {
        y = x;
        if (diff(z) < diff(x))
            x = x->left;
        else
            x = x->right;
    }

    z->p = y;
    if (y == NULL)
        tree->root = z;
    else if (diff(z) < diff(y))
        y->left = z;
    else
        y->right = z;

    update(z);
    insert_fixup(tree, z);
}

void delete_fixup(Tree* tree, Node* x, Node* x_p)
{
    Node* w;
    while (x != tree->root && (COLOR(x) == Black)) {

        /**
         * 如果我是爸爸左邊的小孩
         */
        if (x == x_p->left) {
            w = x_p->right;

            /**
             * 如果我兄弟是紅的
             */
            if (COLOR(w) == Red) {
                w->color = Black;
                x_p->color = Red;
                left_rotate(tree, x_p);
                w = x_p->right;
            }

            /**
             * 如果姪子們都是黑的
             */
            if ((COLOR(w->left) == Black) && (COLOR(w->right) == Black)) {
                w->color = Red;
                x = x_p;
                x_p = x_p->p;
            } else {
                if (COLOR(w->right) == Black) {
                    if (w->left != NULL)
                        w->left->color = Black;
                    w->color = Red;
                    right_rotate(tree, w);
                    w = x_p->right;
                }
                w->color = x_p->color;
                x_p->color = Black;
                if (w->right != NULL)
                    w->right->color = Black;
                left_rotate(tree, x_p);
                x = tree->root;
                break;
            }
        }

        /**
         * 如果我是爸爸右邊的小孩
         */
        else {
            w = x_p->left;

            /**
             * 如果我兄弟是紅的
             */
            if (COLOR(w) == Red) {
                w->color = Black;
                x_p->color = Red;
                right_rotate(tree, x_p);
                w = x_p->left;
            }

            /**
             * 如果姪子們都是黑的
             */
            if ((COLOR(w->right) == Black) && (COLOR(w->left) == Black)) {
                w->color = Red;
                x = x_p;
                x_p = x_p->p;
            } else {
                if (COLOR(w->left) == Black) {
                    if (w->right != NULL)
                        w->right->color = Black;
                    w->color = Red;
                    left_rotate(tree, w);
                    w = x_p->left;
                }
                w->color = x_p->color;
                x_p->color = Black;
                if (w->left != NULL)
                    w->left->color = Black;
                right_rotate(tree, x_p);
                x = tree->root;
                break;
            }
        }
    }

    if (x != NULL)
        x->color = Black;
}

void replace(Tree* tree, Node* z, Node* y)
{
    if (z->p == NULL)
        tree->root = y;

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

    tree->max -= z->expensive;

    if (z->left == NULL || z->right == NULL)
        y = z;
    else {
        y = z->right;
        while (y->left != NULL) {
            y = y->left;
        }
    }

    bool y_color = y->color;

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

    if (z == y->p)
        x_p = y;
    if (y != z)
        replace(tree, z, y);

    if (x != NULL)
        update(x);
    else if (x_p != NULL) {
        update(x_p);
    }

    if (y_color == Black)
        delete_fixup(tree, x, x_p);
}

void in_order(Node* root)
{
    if (root->left != NULL)
        in_order(root->left);
    printf("%lld ", diff(root));
    if (root->right != NULL)
        in_order(root->right);
}

void insert_process(Tree* tree, Node* node_list, int N)
{
    int b, s, tmp;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &b, &s);
        if (s > b) {
            tmp = b;
            b = s;
            s = tmp;
        }
        node_list[i].expensive = b;
        node_list[i].cheap = s;
        insert_node(tree, &node_list[i]);
    }
    tree->cost = tree->max + tree->root->sum_odd;
    printf("%lld\n", tree->cost);
}

void change(Tree* tree, Node* node, int t, int c, int d, int e, int f)
{
    ll cost = tree->cost, b, s, tmp;
    b = ((c % H) * (cost % H) + (d % H)) % H;
    s = ((e % H) * (cost % H) + (f % H)) % H;
    if (s > b) {
        tmp = b;
        b = s;
        s = tmp;
    }
    node->expensive = b;
    node->cheap = s;
}

void change_process(Tree* tree, Node* node_list, int M)
{
    int t, c, d, e, f;
    for (int i = 0; i < M - 1; i++) {
        scanf(" %d %d %d %d %d", &t, &c, &d, &e, &f);
        t -= 1;
        delete_node(tree, &(node_list[t]));
        change(tree, &(node_list[t]), t, c, d, e, f);
        insert_node(tree, &(node_list[t]));
        tree->cost = tree->max + tree->root->sum_odd;
        printf("%lld\n", tree->cost);
    }
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    Node* node_list = (Node*)calloc(N, sizeof(Node));
    Tree* rb_tree = (Tree*)calloc(1, sizeof(Tree));
    insert_process(rb_tree, node_list, N);
    change_process(rb_tree, node_list, M);
}