class ConcreteAuctionTree : public AuctionTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        int itemID;
        int price;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(int id, int p) : itemID(id), price(p), color(RED),left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    Node* NIL;
    void rotateLeft(Node* x);
    void rotateRight(Node* y);
    void fixInsert(Node* z);
    void fixDelete(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node);
    Node* searchByID(Node* node, int itemID);

public:
    ConcreteAuctionTree() {
        NIL = new Node(-1, -1);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    void insertItem(int itemID, int price) override;
    void deleteItem(int itemID) override;
};

void ConcreteAuctionTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL){
        root = y;
    }
    else if (x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void ConcreteAuctionTree::rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NIL){
        root = x;
    }
    else if (y == y->parent->right){
        y->parent->right = x;
    }
    else{
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void ConcreteAuctionTree::insertItem(int itemID, int price) {
    Node* z = new Node(itemID, price);
    z->left = z->right = NIL;
    Node* y = NIL;
    Node* x = root;
    while (x != NIL) {
        y = x;
        if (price < x->price){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }

    z->parent = y;

    if (y == NIL){
        root = z;
    }
    else if (price < y->price){
        y->left = z;
    }
    else{
        y->right = z;
    }

    fixInsert(z);
}

void ConcreteAuctionTree::fixInsert(Node* z) {
    while (z->parent != NIL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } 
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(z->parent->parent);
            }
        } 
        else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } 
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

ConcreteAuctionTree::Node*
ConcreteAuctionTree::searchByID(Node* node, int itemID) {
    if (node == NIL) return NIL;
    if (node->itemID == itemID) return node;

    Node* left = searchByID(node->left, itemID);
    if (left) return left;

    return searchByID(node->right, itemID);
}

void ConcreteAuctionTree::deleteItem(int itemID) {
    Node* z = searchByID(root, itemID);
    if (z == NIL) {
        return;
    }

    Node* y = z;
    Node* x;
    Color originalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } 
    else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    }
     else {
        y = minimum(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (originalColor == BLACK)
        fixDelete(x);

    delete z;
}


void ConcreteAuctionTree::transplant(Node* u, Node* v) {
    if (u->parent == NIL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

ConcreteAuctionTree::Node*
ConcreteAuctionTree::minimum(Node* node) {
    while (node->left != NIL)
        node = node->left;
    return node;
}

void ConcreteAuctionTree::fixDelete(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK &&
                w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        } 
        else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK &&
                w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}
