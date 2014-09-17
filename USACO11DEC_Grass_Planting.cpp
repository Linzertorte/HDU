#include<iostream>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<utility>
#include<queue>
#include<limits.h>
using namespace std;
const int MAXN = 100001;
int N,M;
vector<int> g[MAXN];
int size[MAXN],f[MAXN], son[MAXN],dep[MAXN];
int top[MAXN], w[MAXN]; //top of the chain
int A[MAXN];
int tot; 
void dfs(int u,int fa,int d){
    f[u] = fa;
    size[u] = 1;
    dep[u] = d;
    int best = INT_MIN;
    for(int i=0;i<g[u].size();i++){
        int v = g[u][i];
        if(v==fa) continue;
        dfs(v,u,d+1);
        size[u] += size[v];
        if(size[v]>best) best = size[v], son[u] = v;
    }
}
void dfs1(int u,int fa,int tp){
    w[u] = tot++;
    top[u] = tp;
    if(son[u]) dfs1(son[u],u,tp);
    for(int i=0;i<g[u].size();i++){
        int v = g[u][i];
        if(v==fa || v==son[u]) continue;
        dfs1(v,u,v);
    }
}

struct Node{
    int a,sum;
    Node():a(0),sum(0){}
}tree[4*MAXN];
void push_down(int rt,int l,int r){
    if(tree[rt].a){
        int d = tree[rt].a;
        int mid = (l+r)>>1;
        tree[rt*2].a += d;
        tree[rt*2+1].a += d;
        tree[rt*2].sum += d*(mid-l+1);
        tree[rt*2+1].sum += d*(r-mid);
        tree[rt].a  -= d;
    }
}
void add(int rt,int l,int r,int ll,int rr,int d){
    //printf("ll %d rr %d\n",ll,rr);
    if(ll<=l && rr>=r){
        tree[rt].a += d;
        tree[rt].sum += (r-l+1)*d;
        return ;
    }
    push_down(rt,l,r);
    int mid = (l+r)>>1;
    if(ll<=mid) add(rt*2,l,mid,ll,rr,d);
    if(rr>=mid+1) add(rt*2+1,mid+1,r,ll,rr,d);
    
}
int get(int rt,int l,int r,int ll,int rr){
    if(ll<=l && rr>=r) return tree[rt].sum;
    push_down(rt,l,r);
    int sum = 0;
    int mid = (l+r)>>1;
    if(ll<=mid) sum+=get(rt*2,l,mid,ll,rr);
    if(rr>=mid+1) sum+=get(rt*2+1,mid+1,r,ll,rr);
    return sum;
}


void plant(int u,int v){
    int f1 = top[u], f2 = top[v];
    //printf("f1 %d f2 %d\n",f1,f2);
    while(f1!=f2){
        if(dep[f1]<dep[f2]) swap(f1,f2),swap(u,v);
        add(1,1,tot-1,w[f1],w[u],1);
        u = f[f1], f1 = top[u];
        //cout<<"good"<<endl;
    }
    if(u==v) return ;
    if(dep[u]>dep[v]) swap(u,v);
    add(1,1,tot-1,w[son[u]],w[v],1);
    return;
}
int query(int u,int v){
    int f1=top[u], f2 = top[v];
    int sum = 0;
    while(f1!=f2){
        if(dep[f1]<dep[f2]) swap(f1,f2), swap(u,v);
        sum+=get(1,1,tot-1,w[f1],w[u]);
        u = f[f1], f1 = top[u];
    }
    if(u==v) return sum;
    if(dep[u]>dep[v]) swap(u,v);
    return sum+get(1,1,tot-1,w[son[u]],w[v]);
}
int main()
{
    scanf("%d%d",&N,&M);
    int u,v;
    char cmd;
    for(int i=0;i<N-1;i++){
        scanf("%d%d",&u,&v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    //prepare heavy-light decomposition
    dfs(1,0,0);// size, f, son
    tot = 1;
    dfs1(1,0,1);
    //for(int i=1;i<=N;i++)
    //    printf("w[%d]->%d\n",i,w[i]);
    //begin plant and query
    for(int i=0;i<M;i++){
        scanf(" %c%d%d",&cmd,&u,&v);
        if(cmd=='P'){
            plant(u,v);
        }else{
            printf("%d\n",query(u,v));
        }
    }
    return 0;
}
