/*
 * https://courses.csail.mit.edu/6.851/spring12/lectures/L19.html
 * 
 * Modified based on http://wulala.logdown.com/posts/192053-bzoj-2049-sdoi2008cave-cave-surveying
 * 
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <set>
#include <ctime>
using namespace std;
#define INF 1e9
#define eps 0x7fffffff
#define N 10008

struct node
{
    int l,r,f;
    bool rev;
}p[N];

int n,m;

inline bool isroot(int u)
    {return(!p[u].f || (p[p[u].f].l != u && p[p[u].f].r != u));}

inline void push(int x)
{
    if (!x) return ;
    if (p[x].rev)
    {
        p[p[x].l].rev ^= 1;
        swap(p[p[x].l].l, p[p[x].l].r);
        p[p[x].r].rev ^= 1;
        swap(p[p[x].r].l, p[p[x].r].r);
        p[x].rev = false;
    }
}

inline void zig(int x)
{
    int y = p[x].f,z = p[y].f;
    push(y); 
    push(x);
    if (z)
        if (p[z].l == y) p[z].l = x;
        else if (p[z].r == y) p[z].r = x;
    p[x].f = z;
    p[y].l = p[x].r; p[p[x].r].f = y;
    p[x].r = y; p[y].f = x;
}

inline void zag(int x)
{
    int y = p[x].f,z = p[y].f;
    push(y); 
    push(x);
    if (z)
        if (p[z].l == y) p[z].l = x;
        else if (p[z].r == y) p[z].r = x;
    p[x].f = z;
    p[y].r = p[x].l; p[p[x].l].f = y;
    p[x].l = y; p[y].f = x; 
}

void splay(int x)
{
    push(x);
    while (!isroot(x))
    {
        int y = p[x].f,z = p[y].f;
        if (isroot(y))
            if (p[y].l == x) zig(x);
            else zag(x);
        else if (p[z].l == y)
            if (p[y].l == x) zig(y),zig(x);
            else zag(x),zig(x);
        else if (p[z].r == y)
            if (p[y].r == x) zag(y),zag(x);
            else zig(x),zag(x);
    }
}

int access(int v)
{
    splay(v);
    p[v].r = 0;
    int w;
    int last = v;
    while (w=p[v].f)
    {
        splay(w);
        last = w;
        p[w].r = v;
        splay(v);
    }
    return last;
}
int LCA(int u,int v)
{
    access(v);
    return access(u);
}

void makeroot(int u)
{
    access(u);
    p[u].rev ^= 1;
    swap(p[u].l,p[u].r);
}

void connect(int u,int v)
{
    makeroot(u);
    p[u].f = v;
}

void destroy(int u,int v)
{
    access(u); splay(v);
    if (p[v].f == u){
        p[v].f = 0; 
        return ;
    }
    access(v); splay(u);
    p[u].f = 0;
}

bool query(int u,int v)
{
    int x = LCA(u,v),y = LCA(v,u);
    if (x != y) return false;
    return true;
}

int main()
{
    scanf("%d%d",&n,&m);
    while (m--)
    {
        char ch[20]; int u,v;
        scanf("%s%d%d",ch,&u,&v);
        if (ch[0] == 'Q') 
            query(u,v)? printf("Yes\n") : printf("No\n"); 
        else if (ch[0] == 'C') connect(u,v);
        else if (ch[0] == 'D') destroy(u,v);
    }    
    return 0;
}
