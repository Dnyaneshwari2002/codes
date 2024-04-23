#include <iostream>
#include <queue>
#include <omp.h>

using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    Node* root;

    BinaryTree() : root(nullptr) {}

    void insert(int data) {
        if (root == nullptr) {
            root = new Node(data);
            return;
        }

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current->left == nullptr) {
                current->left = new Node(data);
                return;
            } else {
                q.push(current->left);
            }

            if (current->right == nullptr) {
                current->right = new Node(data);
                return;
            } else {
                q.push(current->right);
            }
        }
    }

    void bfs() {
        if (!root) return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            // Creating a local queue for each thread
            queue<Node*> local_q;

            #pragma omp parallel num_threads(2) // Use 2 threads for this example
            {
                #pragma omp single
                {
                    local_q.push(q.front());
                    q.pop();
                }

                while (!local_q.empty()) {
                    Node* current = nullptr;

                    #pragma omp critical
                    {
                        if (!local_q.empty()) {
                            current = local_q.front();
                            local_q.pop();
                        }
                    }

                    if (current) {
                        cout << current->data << " ";

                        if (current->left) {
                            #pragma omp critical
                            q.push(current->left);
                        }

                        if (current->right) {
                            #pragma omp critical
                            q.push(current->right);
                        }
                    }
                }
            }
        }
    }
};

int main() {
    BinaryTree tree;
    char continueInsert = 'y';
    int inputData;

    while (continueInsert == 'y' || continueInsert == 'Y') {
        cout << "Enter data to insert in the tree: ";
        cin >> inputData;
        tree.insert(inputData);

        cout << "Do you want to insert more nodes? (y/n): ";
        cin >> continueInsert;
    }

    cout << "BFS traversal of the tree is: ";
    tree.bfs();
    cout << endl;

    return 0;
}

