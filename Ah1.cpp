#include <cstdio>
#include <cstdlib>
#include <set>
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <stack>
#include <ctime>

#define h_func h1
#define algo astar
#define filename "output_Ah1.txt"

using namespace std;

int pi[27],pj[27],pk[27];

class State{
public:
    int d[3][3][3];
    int zi,zj,zk;
    State *parent;
    int g;
    int f;
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
        g=0;
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
        cout<<path()<<" "<<f<<" "<<zi<<zj<<zk<<endl<<endl;
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
        if(zi+i<0||zi+i>=3||zj+j<0||zj+j>=3||zk+k<0||zk+k>=3)
            return false;
        if(d[zi+i][zj+j][zk+k]==-1)
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
        next->g=g+1;
        next->h_func();
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

    void h1(){
        int miss=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<3;k++){
                    if(d[i][j][k]!=State::target->d[i][j][k]&&d[i][j][k]>0)
                        miss++;
                }
            }
        }
        f=miss+g;
    }

    void h2(){
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
        f=dis+g;
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

class less_h{
public:
    bool operator()(State *a,State *b){
        return a->f>b->f;
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
    priority_queue<State*,vector<State*>,less_h> open;
    open.push(&start);
    while(1){
        if(open.empty())return "No solution";
        State *s=open.top();
        open.pop();
        if(s->f==s->g)
            return s->path();
        if(closed.find(s)==closed.end()){
            closed.insert(s);
            if(s->can_move(0,-1,0))open.push(s->move(0,-1,0));
            if(s->can_move(0,1,0))open.push(s->move(0,1,0));
            if(s->can_move(0,0,-1))open.push(s->move(0,0,-1));
            if(s->can_move(0,0,1))open.push(s->move(0,0,1));
            if(s->can_move(-1,0,0))open.push(s->move(-1,0,0));
            if(s->can_move(1,0,0))open.push(s->move(1,0,0));
        }
    }
}

/*string idastar(State &start){
    int d_limit=start.f;
    while(d_limit<INT_MAX){
        int next_d_limit=INT_MAX;
        stack<State*> list;
        list.push(new State(start));
        while(!list.empty()){
            //cout<<list.size()<<endl;
            State *s=list.top();
            list.pop();
            if(s->f>d_limit){
                if(s->f<next_d_limit)
                    next_d_limit=s->f;
            }else{
                if(s->f==s->g)
                    return s->path();
                if(s->can_move(0,-1,0))list.push(s->move(0,-1,0));
                if(s->can_move(0,1,0))list.push(s->move(0,1,0));
                if(s->can_move(0,0,-1))list.push(s->move(0,0,-1));
                if(s->can_move(0,0,1))list.push(s->move(0,0,1));
                if(s->can_move(-1,0,0))list.push(s->move(-1,0,0));
                if(s->can_move(1,0,0))list.push(s->move(1,0,0));
            }
        }
        d_limit=next_d_limit;
        cout<<d_limit<<endl;
    }
    return "No solution";
}*/

State *ans;

int search(State *s,int bound){
    if(s->f>bound){
        int r=s->f;
        delete s;
        return r;
    }
    if(s->f==s->g){
        ans=s;
        return -1;
    }
    int min=INT_MAX,t;
    if(s->can_move(0,-1,0)){t=search(s->move(0,-1,0),bound);if(t<min)min=t;}
    if(s->can_move(0,1,0)){t=search(s->move(0,1,0),bound);if(t<min)min=t;}
    if(s->can_move(0,0,-1)){t=search(s->move(0,0,-1),bound);if(t<min)min=t;}
    if(s->can_move(0,0,1)){t=search(s->move(0,0,1),bound);if(t<min)min=t;}
    if(s->can_move(-1,0,0)){t=search(s->move(-1,0,0),bound);if(t<min)min=t;}
    if(s->can_move(1,0,0)){t=search(s->move(1,0,0),bound);if(t<min)min=t;}
    if(min!=-1)
       delete s;
    return min;
}

string idastar(State &start){
    int bound=start.f;
    while(1){
        int t=search(new State(start),bound);
        if(t==-1)
            return ans->path();
        if(t==INT_MAX)
            return "No solution";
        bound=t;
        //cout<<bound<<endl;
    }
}

int main(){
    FILE *fp=fopen("target.txt","r");
    State target(fp);
    State::target=&target;
    prepare_h2();
    fclose(fp);
    fp=fopen("input.txt","r");
    State s(fp);
    s.h_func();
    fclose(fp);
    clock_t start, finish;
    double totaltime;
    start = clock();
    string answer=algo(s);
    finish = clock();
    totaltime = (double)(finish-start)/CLOCKS_PER_SEC;
    fp=fopen(filename,"w");
    fprintf(fp,"%.3f\n%s\n",totaltime,answer.c_str());
    fclose(fp);
    return 0;
}