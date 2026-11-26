#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

vector<int> adj[MAXN];
int sz[MAXN]; // subtree size when rooted at 1
int n, k;

// Iterative DFS to compute subtree sizes, rooted at node 1
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
    
    // Process in reverse order (leaves to root)
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
        if (n - sz[u] > k) {
            continue;
        }
        
        // Check all child subtrees
        bool ok = true;
        for (int v : adj[u]) {
            // If v is parent of u (in rooted tree), skip - already checked via n - sz[u]
            // If v is child of u, the component size is sz[v]
            // How to distinguish? 
            // When rooted at 1: if v is parent of u, then sz[v] > sz[u] (since sz[v] includes sz[u])
            // Wait, not necessarily. If u is a leaf, sz[u]=1, and parent could have sz[parent] > 1.
            // Actually yes: if v is parent of u, then u is in v's subtree, so sz[v] >= sz[u].
            // And if v is child of u, then sz[v] < sz[u] (strictly less since u itself adds 1).
            // So: if sz[v] < sz[u], v is a child; if sz[v] > sz[u], v is parent.
            // Edge case: what if sz[v] == sz[u]? That can't happen because one contains the other.
            
            if (sz[v] < sz[u]) {
                // v is a child of u
                if (sz[v] > k) {
                    ok = false;
                    break;
                }
            }
            // else v is parent of u, already checked via n - sz[u]
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
