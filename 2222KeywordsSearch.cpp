#include<vector>
#include<iostream>
#include<string>
#include<stdio.h>
#include<string.h>
#include<assert.h>
using namespace std;

class AC{
public:
    int cnt;
    int chd[10000000][26];
    int fail[10000000];
    int words[10000000];
    int root;
    int que[10000000];
    void init(){
        root = 0;
        cnt = 1;
        memset(chd[0],-1,sizeof(chd[0]));
        words[0] = 0;
    }
    void insert(char *word){
        int len=strlen(word);
        int cur=root;
        for(int i=0;i<len;i++){
            if(chd[cur][word[i]-'a']<0){
                //get a new node
                cnt++;
                //assert(cnt<1000);
                memset(chd[cnt-1],-1,sizeof(chd[cnt-1]));
                chd[cur][word[i]-'a'] = cnt-1;
                words[cnt-1] = 0;
                
            }
            cur = chd[cur][word[i]-'a'];
        }
        words[cur]++;
    }
    void build(){
        //phase I 
        for(int i=0;i<26;i++) if(chd[0][i]<0) chd[0][i]=0;
        //phase II bfs
        int s,t,q,r,v;
        s=t=0;
        for(int i=0;i<26;i++) if(q=chd[0][i]){
            fail[q] = 0;
            que[t++] = q;
        }
        while(s<t){
            r = que[s++];
            for(int i=0;i<26;i++){
                if((q=chd[r][i])!=-1){
                    que[t++] = q;
                    v = fail[r];
                    while(chd[v][i]<0) v = fail[v];
                    fail[q] = chd[v][i];
                   
                }
            }
        }
    }
    int query(char *S){
        int len = strlen(S);
        int q = 0;
        int total = 0;
        int v;
        for(int i=0;i<len;i++){
            while(chd[q][S[i]-'a']<0)
                q = fail[q];
            q=chd[q][S[i]-'a'];
            if(words[q]) {
                total+=words[q];
                words[q]=0;
                v = fail[q];
                while(v){
                    total+=words[v];
                    words[v]=0;
                    v=fail[v];
                }
            }
        }
        return total;
    }
    
};
char S[1000010];
AC ac;
int main(){
    int T,n;
    char word[60];
    scanf("%d",&T);
    while(T--){
        ac.init();
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%s",word);
            ac.insert(word);
        }
        ac.build();
        scanf("%s",S);
        printf("%d\n",ac.query(S));
    }
}
