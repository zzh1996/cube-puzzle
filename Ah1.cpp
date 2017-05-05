#include <cstdio>
#include <cstdlib>
#include <set>
#include <queue>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int pi[27],pj[27],pk[27];

class State{
public:
    int d[3][3][3];
    int zi,zj,zk;
    State *parent;
    static State *target;

    State(FILE *fp){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    fscanf(fp,"%d",&d[i][j][k]);
                    if(d[i][j][k]==0){
                        zi=i;
                        zj=j;
                        zk=k;
                    }
                }
            }
        }
        parent=NULL;
    }

    void dump(){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    cout<<d[i][j][k]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
        cout<<path()<<" "<<h1()<<" "<<zi<<zj<<zk<<endl;
    }

    bool operator==(State &rhs){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    if(d[i][j][k]!=rhs.d[i][j][k])
                        return false;
                }
            }
        }
        return true;
    }

    bool can_move(int i,int j,int k){
        if(d[zi+i][zj+j][zk+k]==-1)
            return false;
        if(zi+i<0||zi+i>=3||zj+j<0||zj+j>=3||zk+k<0||zj+j>=3)
            return false;
        return true;
    }

    State *move(int i,int j,int k){
        State *next=new State(*this);
        next->zi+=i;
        next->zj+=j;
        next->zk+=k;
        next->d[zi][zj][zk]=d[next->zi][next->zj][next->zk];
        next->d[next->zi][next->zj][next->zk]=0;
        next->parent=this;
        return next;
    }

    string path(){
        if(!parent)
            return "";
        else{
            char dir;
            if(zi-parent->zi==-1)dir='F';
            else if(zi-parent->zi==1)dir='B';
            else if(zj-parent->zj==-1)dir='U';
            else if(zj-parent->zj==1)dir='D';
            else if(zk-parent->zk==-1)dir='L';
            else if(zk-parent->zk==1)dir='R';
            return parent->path()+dir;
        }
    }

    int h1(){
        int miss=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    if(d[i][j][k]!=State::target->d[i][j][k]&&d[i][j][k]>0)
                        miss++;
                }
            }
        }
        return miss;
    }

    int h2(){
        int dis=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    int n=d[i][j][k];
                    if(n>0)
                        dis+=abs(i-pi[n])+abs(j-pj[n])+abs(k-pk[n]);
                }
            }
        }
        return dis;
    }
};

class less_p{
public:
    bool operator()(State *a,State *b){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    if(a->d[i][j][k]<b->d[i][j][k])
                        return true;
                    else if(a->d[i][j][k]>b->d[i][j][k])
                        return false;
                }
            }
        }
        return false;
    }
};

class less_h1{
public:
    bool operator()(State *a,State *b){
        return a->h1()>b->h1();
    }
};

class less_h2{
public:
    bool operator()(State *a,State *b){
        return a->h2()>b->h2();
    }
};

State *State::target;

void prepare_h2(){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                int n=State::target->d[i][j][k];
                if(n>0){
                    pi[n]=i;
                    pj[n]=j;
                    pk[n]=k;
                }
            }
        }
    }
}

string astar(State &start){
    set<State*,less_p> closed;
    priority_queue<State*,vector<State*>,less_h2> open;
    open.push(&start);
    while(1){
        if(open.empty())return "No solution";
        State *s=open.top();
        open.pop();
        //
        //s->dump();
        //
        if(*s==*State::target)return s->path();
        if(closed.find(s)==closed.end()){
            closed.insert(s);
            for(int i=0;i<6;i++){
                if(s->can_move(0,-1,0))open.push(s->move(0,-1,0));
                if(s->can_move(0,1,0))open.push(s->move(0,1,0));
                if(s->can_move(0,0,-1))open.push(s->move(0,0,-1));
                if(s->can_move(0,0,1))open.push(s->move(0,0,1));
                if(s->can_move(-1,0,0))open.push(s->move(-1,0,0));
                if(s->can_move(1,0,0))open.push(s->move(1,0,0));
            }
        }
    }
}

int main(){
    FILE *fp=fopen("input.txt","r");
    State start(fp);
    fclose(fp);
    fp=fopen("target.txt","r");
    State target(fp);
    State::target=&target;
    prepare_h2();
    fclose(fp);
    cout<<astar(start)<<endl;
    return 0;
}