#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

vector<int> adj[MAXN];
int sz[MAXN]; // subtree size when rooted at node 1
int n, k;

// Iterative DFS to compute subtree sizes, avoiding stack overflow on deep trees
void dfs(int root) {
    vector<int> parent(n + 1, 0);
    vector<int> order;
    vector<int> stk;

    stk.push_back(root);
    while (!stk.empty()) {
        int u = stk.back();
        stk.pop_back();
        order.push_back(u);
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                stk.push_back(v);
            }
        }
    }

    // Process in reverse order (leaves to root) to compute subtree sizes
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i];
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != parent[u]) {
                sz[u] += sz[v];
            }
        }
    }
}

int main() {
    scanf("%d %d", &n, &k);

    for (int i = 1; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // Root at node 1, compute subtree sizes
    dfs(1);

    // For each node u, removing it splits the tree into components:
    // 1. For each child v of u (in the rooted tree): component of size sz[v]
    // 2. The "parent side" component: size n - sz[u]
    // All components must have size <= k
    bool found = false;
    vector<int> ans;

    for (int u = 1; u <= n; u++) {
        // Check parent-side component: n - sz[u]
        if (n - sz[u] > k) continue;

        // Check all child subtrees
        bool ok = true;
        for (int v : adj[u]) {
            // When rooted at 1: if v is parent of u, then sz[v] > sz[u]
            // If v is child of u, then sz[v] < sz[u]
            if (sz[v] < sz[u] && sz[v] > k) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ans.push_back(u);
            found = true;
        }
    }

    if (!found) {
        printf("None\n");
    } else {
        // Output from largest to smallest
        for (int i = (int)ans.size() - 1; i >= 0; i--) {
            if (i < (int)ans.size() - 1) printf(" ");
            printf("%d", ans[i]);
        }
        printf("\n");
    }

    return 0;
}
