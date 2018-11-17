#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class coordMat;

class Punct
{
    int x,y;
public:
    Punct(int xx=0, int yy=0)
    {
        x=xx; y=yy;
    }

    void set(int xx=0, int yy=0)
    {
        x=xx; y=yy;
    }

    void putValue(vector < vector <int> >& mat, int val)
    {
        mat[x][y]=val;
    }

    int get(const vector < vector <int> >& mat) {return mat[x][y];}

    int operator== (const Punct& p)
    {
        x=p.x;
        y=p.y;
        return 1;
    }

friend class coordMat;
friend ifstream& operator>> (ifstream& fin, Punct& p);
friend ostream& operator<< (ostream& out, Punct& p);
};

ifstream& operator>> (ifstream& fin, Punct& p)
{
    fin>>p.x>>p.y;
    return fin;
}

ostream& operator<< (ostream& out, Punct& p)
{
    out<<"P("<<p.x<<" "<<p.y<<")";
    return out;
}

class coordMat
{
    int ll, lh, cl, ch;     ///LINE / COLUMN - LOWER / HIGHRT
public:

    coordMat()
    {
        ll=0; lh=0;
        cl=0; ch=0;
    }

    coordMat(int n)
    {
        n = pow(2,n);
        ll = cl = 1;
        lh = ch = n;
    }


    void set(int l1=0,int l2=0,int c1=0, int c2=0)
    {
        ll=l1; lh=l2;
        cl=c1; ch=c2;
    }

    int zonaP(Punct p)
    {   int nr=0;
        int cmij=(cl+ch)/2;
        int lmij=(ll+lh)/2;

        if(p.y >= cl && p.y <= cmij) nr+=1;
        if(p.y >=cmij+1 && p.y <= ch) nr+=2;

        if(p.x >= ll && p.x <= lmij) nr+=0;
        if(p.x >=lmij+1 && p.x <= lh) nr+=2;

    return nr;
    }

    Punct marginPoint(int n)
    {Punct mr;
        switch(n)
        {
        case(1):
            mr.set(lh,ch);
            break;
        case(2):
            mr.set(lh,cl);
            break;
        case(3):
            mr.set(ll,ch);
            break;
        case(4):
            mr.set(ll,cl);
            break;
        default: break;
        }
    return mr;
    }

    int marime() {return lh-ll+1;}

    void fillIt(vector < vector < int > >& mat, int val)
    {
        for(int i=ll;i<=lh;i++)
            for(int j=cl;j<=ch;j++)
                mat[i][j]=val;
    }

    vector<coordMat> splitIn4 ()
    {
        int lmij = (ll+lh)/2;
        int cmij = (cl+ch)/2;

        vector<coordMat> miniMat;
        miniMat.resize(5);
        miniMat[1].set(ll,lmij,cl,cmij);
        miniMat[2].set(ll,lmij,cmij+1,ch);
        miniMat[3].set(lmij+1,lh,cl,cmij);
        miniMat[4].set(lmij+1,lh,cmij+1,ch);

        return miniMat;
    }

    void afis(const vector < vector < int > > matr)
    {
        for(int i=ll;i<=lh;i++)
        {
            for(int j=cl;j<=ch;j++)
                {
                    int x=matr[i][j];
                    if(x<=9 && x>=0) cout<<" "<<x<<" ";
                    else cout<<x<<" ";
                    if(j%2==0) cout<<" ";
                }
            if(i%2==0) cout<<"\n";
            cout<<"\n";
        }
    }

friend ostream& operator<< (ostream& out, coordMat& coo);
};

ostream& operator<< (ostream& out, coordMat& coo)
{
    out<<"L("<<coo.ll<<"->"<<coo.lh<<") ";
    out<<"C("<<coo.cl<<"->"<<coo.ch<<")";
    return out;
}

vector < vector < int > > createMat (int n)
{   n = pow(2,n);
    vector < vector < int > > mat;
    mat.resize(n+1);
    for(int i=0;i<=n;i++)
        mat[i].resize(n+1);

    return mat;
}

int umpleL (vector < vector < int > >& mat, coordMat& coo, Punct blk)
{
static int deUmplut = 0;
deUmplut++;

    int emis = coo.zonaP(blk);
    vector <coordMat> split;
    split=coo.splitIn4();
    vector<Punct> mp; //margin point
    mp.resize(5);

    for(int i=1;i<=4;i++)
    if(i!=emis)
    {
        mp[i]=split[i].marginPoint(i);
        mp[i].putValue(mat,deUmplut);
    }

    if(coo.marime()>2)
    for(int i=1;i<=4;i++)
    {
        if(i==emis) umpleL(mat,split[i],blk);
        else umpleL(mat,split[i],mp[i]);
    }

    split.clear();


}

void delMatr(vector < vector <int> >& matr)
{
    for(int i=0;i<matr.size();i++)
        matr[i].clear();
    matr.clear();
}

void fillMatr(vector < vector <int> >& matr, int val)
{
    for(int i=1;i<matr.size();i++)
        for(int j=1;j<matr[i].size();j++)
            matr[i][j]=val;
}

int main()
{
ifstream f ("input.in");
int n;
Punct blk;
f>>n;
f>>blk;

coordMat coo(n);
vector < vector <int> > matr;
matr=createMat(n);
fillMatr(matr,-1);

blk.putValue(matr,0);


umpleL(matr,coo,blk);
coo.afis(matr);

delMatr(matr);
f.close();

return 0;
}
