/*
 * placerow.h
 *
 *  Created on: Apr 11, 2015
 *      Author: zz3793
 */

#ifndef PLACEROW_H_
#define PLACEROW_H_


#include "class_define.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::list;
using std::map;
using std::ostream;
using std::cout;
using std::endl;

int whitespaceswap=0;
int samerow_non_whitespace=0;

/*** swap criterion
    if new_hpwl<old_hpwl + thresh, it is a "good" swap
 ***/

double thresh=50;


class row_placer{
public:
    vector<map<int,cell*>>& grid;
    int x_size;
    int y_size;
    int x_max;
    int sitewidth;
    int decrease_count;
    int increase_count;
    int swapcount;
    int displacement;
    bool merged;
    cluster* c_head, * c_tail, * t_head, * t_tail;
    
    vector<cluster*> cluster_v;
    
    void check(cluster* clp){
        if(c_head!=nullptr&&c_head==t_head){
//            cout<<""<<endl;
        }
    }
    
    void initialize(void){
        int i=0;
        for(auto row:grid){
            auto b=row.begin();
            auto e=row.end();
            int lastx=0;
            cluster* clu_p;
            cluster* head=nullptr;
            cluster* current=nullptr;
            int cluster_count=0;
            while(b!=e){
                if(head==nullptr||((*b).second)->fixed==1||lastx!=((*b).second)->x_pos){
                    clu_p=new cluster();
                    clu_p->y_pos=i;
                    if(((*b).second)->y_pos!=i){
                        cout<<"error in initialization"<<endl;
                    }
                    ++cluster_count;
                    clu_p->e=1;
                    clu_p->x1=((*b).second)->x_pos;//the x coordinate of the first cell
                    clu_p->q=((*b).second)->x_pos-clu_p->width;
                    clu_p->width=((*b).second)->width; //the width of cell, the name may change
                    clu_p->celllist.push_back((*b).second);
                    ((*b).second)->ptr_cluster=clu_p;
                    lastx=((*b).second)->x_pos+((*b).second)->width;
                    if(head==nullptr){
                        head=clu_p;
                        current=clu_p;
                    }else{
                        current->next=clu_p;
                        clu_p->prev=current;
                        current=clu_p;
                    }
                }
                else{
                    ++(clu_p->e);
                    clu_p->q+=clu_p->e*(((*b).second)->x_pos-clu_p->width);
                    clu_p->width+=((*b).second)->width;
                    clu_p->celllist.push_back((*b).second);
                    ((*b).second)->ptr_cluster=clu_p;
                    lastx+=((*b).second)->width;
                }
                ++b;
            }
            ++i;
      //      cout<<"this row has "<<cluster_count<<" clusters"<<endl;
            cluster_v.push_back(head);
            

        }
    }
    
    row_placer(vector<map<int,cell*>>& table, int x_s, int y_s, int unit):displacement(0),grid(table), x_size(x_s),y_size(y_s),sitewidth(unit){
        x_max=x_size*sitewidth;
        decrease_count=0;
        increase_count=0;
        swapcount=0;
        idnum=0;
        initialize();
        cluster_v.reserve(y_size);
        
    }
    
    bool double_collapse_c(cluster* clup){
        cluster* prevc=clup->prev;
        cluster* nextc=clup->next;
        if(clup==nullptr){
 //           cout<<""<<endl;
        }
        if(clup->x1<0||clup->x1+clup->width>x_size) return false;

        bool forward_collapse=(prevc!=nullptr)&&(clup->x1<=prevc->x1+prevc->width);
        bool backward_collapse=(nextc!=nullptr)&&((clup->x1+clup->width)>=nextc->x1);
        
        /*** for test ****/
        check(clup);
        check(prevc);
        check(nextc);
        /**** for test ****/
        
        
        if(forward_collapse&&backward_collapse){
            /******* copy prevc and nextc and merge them with clup   *******/
            if(prevc->fixed==1||nextc->fixed==1) return false;
            /*** old code
            clup->e+=prevc->e;
            clup->q=prevc->q+clup->q-clup->e*prevc->width;
            clup->width+=prevc->width;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            
            clup->e+=nextc->e;
            clup->q=clup->q+nextc->q-nextc->e*clup->width;
            clup->width+=nextc->width;
            list<cell*> temp1=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp1);
            ***/
            
            /*** new code **/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            list<cell*> temp1=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp1);

            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;

            }
            /*** end of new code ***/
            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /******** update c_head, c_tail  **********/
            c_head=prevc;
            c_tail=nextc;
            /***** clup points to new neighbours, but neighbours still points to original clusters ****/
            clup->prev=prevc->prev;
            clup->next=nextc->next;
            /** for test **/
          //  prevc->abandoned=true;
          //  nextc->abandoned=true;
            /** end of test ***/
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_c(clup);
        }else if(forward_collapse){
            /******** copy prevc and merge it with clup *********/
            if(prevc->fixed==1) return false;
            /* previous code
            clup->e+=prevc->e;
            clup->q=prevc->q+clup->q-clup->e*prevc->width;
            clup->width+=prevc->width;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            
             
             /** new code to prevent collapse **/
            int nextx;
            int prevx=prevc->x1+prevc->width;
            
            if(nextc==nullptr) nextx=x_size;
            else nextx=nextc->x1;
            
            if(nextx-prevx>=clup->width){
                clup->x1=prevx;
                return true;
            }
            /**** end of new code ****/
            
            /*** new code to calculate x1 ***/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;
                
            }
            /*** end of new code ***/
            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /****** update c_head and c_tail  ******/
            c_head=prevc;
            if(c_tail==nullptr) c_tail=c_head;
            /******** clup points to new neighbours   *******/
            clup->prev=c_head->prev;
            
            /** for test **/
            prevc->abandoned=true;
            /** end of test ***/
            
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_c(clup);
        }else if(backward_collapse){
            /********* copy nextc and merge it with clup ******/
            if(nextc->fixed==1) return false;
            /*** old code
            clup->e+=nextc->e;
            clup->q=clup->q+nextc->q-nextc->e*clup->width;
            clup->width+=nextc->width;
            list<cell*> temp=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp);
            ***/
            
            /** new code to prevent collapse**/
            int nextx=nextc->x1;
            int prevx;
            
            if(prevc==nullptr) nextx=0;
            else prevx=prevc->x1+prevc->width;
            
            if(nextx-prevx>=clup->width){
                clup->x1=nextx-clup->width;
                return true;
            }
            /**** end of new code ****/
            
            /*** new code **/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=nextc->celllist;
            
            clup->celllist.splice(clup->celllist.end(), temp);
            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;
                
            }
            /*** end of new code ***/
            
            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /****** update c_head and c_tail  ******/
            c_tail=nextc;
            if(c_head==nullptr) c_head=c_tail;
            /******** clup points to new neighbours   *******/
            clup->next=c_tail->next;
            
            /** for test **/
            nextc->abandoned=true;
            /** end of test ***/
            
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_c(clup);
        }else{
            return true;
        }
    }
    
    bool double_collapse_t(cluster* clup){
        
        cluster* prevc=clup->prev;
        cluster* nextc=clup->next;
        if(clup->x1<0||clup->x1+clup->width>x_size) return false;
        bool forward_collapse=(prevc!=nullptr)&&(clup->x1<=prevc->x1+prevc->width);
        bool backward_collapse=(nextc!=nullptr)&&((clup->x1+clup->width)>=nextc->x1);
        
        
        /*** for test ****/
        check(clup);
        check(prevc);
        check(nextc);
        /**** for test ****/
        
        if(forward_collapse&&backward_collapse){
            /****** copy prevc and nextc and merge them with clup  *******/
            if(prevc->fixed==1||nextc->fixed==1) return false;
            /**** old code
            clup->e+=prevc->e;
            clup->q=prevc->q+clup->q-clup->e*prevc->width;
            clup->width+=prevc->width;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            
            clup->e+=nextc->e;
            clup->q=clup->q+nextc->q-nextc->e*clup->width;
            clup->width+=nextc->width;
            temp=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp);
            ***/
            
            /*** new code **/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            list<cell*> temp1=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp1);
            
            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;
                
            }
            /*** end of new code ***/

            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /******** update t_head, t_tail  **********/
            t_head=prevc;
            t_tail=nextc;
            /******** clup points to new neighbours   *******/
            clup->prev=prevc->prev;
            clup->next=nextc->next;
            
            /** for test **/
            prevc->abandoned=true;
            nextc->abandoned=true;
            /** end of test ***/
            
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_t(clup);
        }else if(forward_collapse){
            
            
            /****** copy prevc and merge them with clup  *******/
            if(prevc->fixed==1) return false;
            /*** old code
            clup->e+=prevc->e;
            clup->q=prevc->q+clup->q-clup->e*prevc->width;
            clup->width+=prevc->width;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            ***/
            
            
            /** new code  to prevent collapse **/
            int nextx;
            int prevx=prevc->x1+prevc->width;
            
            if(nextc==nullptr) nextx=x_size;
            else nextx=nextc->x1;
            
            if(nextx-prevx>=clup->width){
                clup->x1=prevx;
                return true;
            }
            /**** end of new code ****/

            
            /*** new code **/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=prevc->celllist;
            temp.splice(temp.end(), clup->celllist);
            clup->celllist=std::move(temp);
            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;
                
            }
            /*** end of new code ***/

            
            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /******** update t_head, t_tail  **********/
            t_head=prevc;
            if(t_tail==nullptr) t_tail=t_head;
            /******** clup points to new neighbours   *******/
            clup->prev=prevc->prev;
            
            /** for test **/
            prevc->abandoned=true;
            /** end of test ***/
            
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_t(clup);
        }else if(backward_collapse){
            /****** copy nextc and merge them with clup  *******/
            if(nextc->fixed==1) return false;
            /*** old code
            clup->e+=nextc->e;
            clup->q=clup->q+nextc->q-nextc->e*clup->width;
            clup->width+=nextc->width;
            list<cell*> temp=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp);
            ***/
            
            
            /** new code  **/
            int nextx=nextc->x1;
            int prevx;
            
            if(prevc==nullptr) nextx=0;
            else prevx=prevc->x1+prevc->width;
            
            if(nextx-prevx>=clup->width){
                clup->x1=nextx-clup->width;
                return true;
            }
            /*** end of new code ***/
            
            /*** new code **/
            clup->e=0;
            clup->q=0;
            clup->width=0;
            list<cell*> temp=nextc->celllist;
            clup->celllist.splice(clup->celllist.end(), temp);
            for(auto c_it:clup->celllist){
                ++clup->e;
                clup->q+=c_it->x_pos-c_it->width;
                clup->width+=c_it->width;
                
            }
            /*** end of new code ***/

            
            clup->x1=clup->q/clup->e;
            if(clup->x1>x_size-clup->width)
                clup->x1=x_size-clup->width;
            if(clup->x1<0)
                clup->x1=0;
            /******** update t_head, t_tail  **********/
            t_tail=nextc;
            if(t_head==nullptr) t_head=t_tail;
            /******** clup points to new neighbours   *******/
            clup->next=nextc->next;
            
            /** for test **/
            nextc->abandoned=true;
            /** end of test ***/
            
            /*** for test ****/
            check(clup);
            check(prevc);
            check(nextc);
            /**** for test ****/
            
            return double_collapse_t(clup);
        }else{
            return true;
        }
    }
    
    
    
    void same_row_swap(int x, int y, cell* c){
        ++swapcount;
        merged=false;
		if(swapcount%10000==0)
			cout<<"swapcount is "<<swapcount<<endl;
        
        int cell_y=c->y_pos;
        int cell_x=c->x_pos;
        bool whitespace;
        int same_x1;
        cell* t=nullptr;
        map<int,cell*>& target_row=grid[y];
        map<int,cell*>& cell_row=grid[c->y_pos];
        unordered_map<string,net*> netlist;
        
        c_head=nullptr;
        c_tail=nullptr;
        t_head=nullptr;
        t_tail=nullptr;
        
        auto b=target_row.begin();
        auto e=target_row.end();
        
        cluster* c_backup=c->ptr_cluster;
        cluster* t_backup=nullptr;
        
        /** iterator to indicate where the cell and target are in their cluster ***/
        list<cell*>::iterator cell_temp, target_temp;
        
        /**** new clusters to be passed to double_collapse and modfied ****/
        cluster* cluster_new=nullptr;  // cluster_new is the cluster to replace in cell_row
        cluster* target_new=nullptr; //target_new is the cluster to replace in the target_row
        
        
        
        
        
        /******************* get all the nets involved  ****************/
        while(b!=e){
            for(net* n:((*b).second)->list_net){
                if(netlist.find(n->net_name)==netlist.end()) netlist[n->net_name]=n;
            }
            ++b;
        }
        
        /******** calculate the HPWL of the netlist before swap ************/
        int old_hpwl=0;
        for(auto i:netlist){
            old_hpwl+=(i.second)->get_hpwl_now();
        }
        /******************* back up the old position of the cell ****************/
        c->x_pos_old=c->x_pos;
        c->y_pos_old=c->y_pos;
        c->x_pos=x;
        c->y_pos=y;
        cluster* target_cluster=cluster_v[y];
        cluster* cell_cluster=cluster_v[cell_y];
        
        
        bool legalswap=true;
        bool samecluster=false;
        
        if(target_row.upper_bound(x)==target_row.begin()||
           ((*(--target_row.upper_bound(x))).second)->x_pos+((*(--target_row.upper_bound(x))).second)->width<x){
            ++whitespaceswap;
            c->reverse();
            return ;

        }else{
            t=(*(--target_row.upper_bound(x))).second;
            if(c==t) {
                c->reverse();
                ++whitespaceswap;
                return;}  //can do better
            c_backup=c->ptr_cluster;
            t_backup=t->ptr_cluster;
            t->x_pos_old=t->x_pos;
            t->y_pos_old=t->y_pos;
            t->x_pos=cell_x;
            t->y_pos=cell_y;
            if(c_backup==t_backup){  // in the same cluster
                samecluster=true;
                target_new=new cluster(*c_backup);
                cell_temp=std::find(target_new->celllist.begin(), target_new->celllist.end(),c);
                target_temp=std::find(target_new->celllist.begin(), target_new->celllist.end(),t);
                *cell_temp=t;
                *target_temp=c;
                target_new->e=0;
                target_new->q=0;
                target_new->width=0;
                for(auto c_it:target_new->celllist){
                    ++target_new->e;
                    target_new->q+=c_it->x_pos-c_it->width;
                    target_new->width+=c_it->width;
                }
                target_new->x1=target_new->q/target_new->e;
                if(target_new->x1<0) target_new->x1=0;
                if(target_new->x1+target_new->width>x_size) target_new->x1=x_size-target_new->width;
                
                cluster* prevc=target_new->prev;
                cluster* nextc=target_new->next;
                
                bool forward_collapse=(prevc!=nullptr)&&(target_new->x1<prevc->x1+prevc->width);
                bool backward_collapse=(nextc!=nullptr)&&((target_new->x1+target_new->width)>nextc->x1);
                if(forward_collapse)
                    target_new->x1=prevc->x1+prevc->width;
                if(backward_collapse)
                    target_new->x1=nextc->x1-target_new->width;
            }else{
                ++samerow_non_whitespace;
                //not in the same cluster
                c->reverse();
                t->reverse();
                return;
            }
        }
        
            if(legalswap){
                if(samecluster){
                    if(target_new->update_cell()==false) legalswap=false;
                }
            }
            
            int new_hpwl=0;
            if(legalswap){
                for(auto c_it:netlist){
                    new_hpwl+=(c_it.second)->get_hpwl_now();
                }
                if(new_hpwl>=old_hpwl+thresh) {
                    legalswap=false;
                    ++increase_count;
                }
                else{
                    ++decrease_count;
                }
            }
            
            if(legalswap){
                if(samecluster){
                    for(auto it_net:c_backup->celllist){
                        it_net->uniform();
                    }
                    for(auto c_it:target_new->celllist){
                        c_it->ptr_cluster=target_new;
                    }
                    if(target_new->prev!=nullptr) target_new->prev->next=target_new;
                    else cluster_v[y]=target_new;
                    if(target_new->next!=nullptr) target_new->next->prev=target_new;
                    delete c_backup;
                }
                for(auto it_net:netlist){
                    (it_net.second)->uniform();
                }

//                target_row.clear();
//                cluster* t_temp=cluster_v[y];
//                while(t_temp!=nullptr){
//                    for (auto cell_ptr:t_temp->celllist){
//                        target_row[cell_ptr->x_pos]=cell_ptr;
//                    }
//                    t_temp=t_temp->next;
//                }
                cluster* t_temp=cluster_v[y];
                auto row_it=target_row.begin();
                while(t_temp!=nullptr){
                    for (auto cell_ptr:t_temp->celllist){
                        target_row.change_key(row_it,cell_ptr->x_pos);
                        ++row_it;
                    }
                    t_temp=t_temp->next;
                }
                

            }
            else{
                if(samecluster){
                    for(auto it_net:c_backup->celllist){
                        it_net->reverse();
                    }
                    delete target_new;
                }
            
            }
    }

    int swap(int x, int y, cell* c){
        ++swapcount;
		if(swapcount%10000==0)
			cout<<"swapcount is "<<swapcount<<endl;
        int cell_y=c->y_pos;
        int cell_x=c->x_pos;
     bool whitespace;
        cell* t=nullptr;
        map<int,cell*>& target_row=grid[y];
        map<int,cell*>& cell_row=grid[c->y_pos];
        unordered_map<string,net*> netlist;
        
        c_head=nullptr;
        c_tail=nullptr;
        t_head=nullptr;
        t_tail=nullptr;
        /**** whether to split or not ***/
        bool c_split=false;
        bool t_split=false;
        
        auto b=target_row.begin();
        auto e=target_row.end();
        
        cluster* c_backup=c->ptr_cluster;
        cluster* t_backup=nullptr;
        
        /** iterator to indicate where the cell and target are in their cluster ***/
        list<cell*>::iterator cell_temp, target_temp;
        
        /**** new clusters to be passed to double_collapse and modfied ****/
        cluster* cluster_new=nullptr;  // cluster_new is the cluster to replace in cell_row
        cluster* target_new; //target_new is the cluster to replace in the target_row
        
        
        
        
        
        /******************* get all the nets involved  ****************/
        while(b!=e){
            for(net* n:((*b).second)->list_net){
                if(netlist.find(n->net_name)==netlist.end()) netlist[n->net_name]=n;
            }
            ++b;
        }
        b=cell_row.begin();
        e=cell_row.end();
        while(b!=e){
            for(net* n:((*b).second)->list_net){
                if(netlist.find(n->net_name)==netlist.end()) netlist[n->net_name]=n;
            }
            ++b;
        }
        /******** calculate the HPWL of the netlist before swap ************/
        int old_hpwl=0;
        for(auto i:netlist){
            old_hpwl+=(i.second)->get_hpwl_now();
        }
        /******************* back up the old position of the cell ****************/
        c->x_pos_old=c->x_pos;
        c->y_pos_old=c->y_pos;
        c->x_pos=x;
        c->y_pos=y;
        cluster* target_cluster=cluster_v[y];
        cluster* cell_cluster=cluster_v[cell_y];

        
        bool legalswap=true;
        
    //    if(target_row.find(x)==target_row.end()){  //target is a whitespace
        if(target_row.upper_bound(x)==target_row.begin()||
           ((*(--target_row.upper_bound(x))).second)->x_pos+((*(--target_row.upper_bound(x))).second)->width<=x)
        {
            whitespace=true;
            /***** put a nullptr in the cell position of c_backup ****/
            cell_temp=std::find(c_backup->celllist.begin(), c_backup->celllist.end(),c);
            *cell_temp=nullptr;   // when target and cell are in the same cluster, or probably in the same row, could be a problem
            
            /****** for the cell row *****/
            c_head=nullptr;
            c_tail=nullptr;
            
            /***** new a cluster which is the cell itself *****/
            target_new=new cluster();
            target_new->x1=x;
            target_new->y_pos=y;
            target_new->e=1;
            target_new->width=c->width;
            target_new->q=x-c->width;
            target_new->celllist.push_back(c);
            /****** find the previous cluster and next cluster in the new position ****/
           
            cluster* next_cluster;
            cluster* prev_cluster;
            
            if(target_row.upper_bound(x)!=target_row.end()){
                next_cluster=(*(target_row.upper_bound(x))).second->ptr_cluster;
                prev_cluster=next_cluster->prev;
            }else{
                next_cluster=nullptr;
                prev_cluster=cluster_v[y];
                while(prev_cluster->next!=nullptr){
                    prev_cluster=prev_cluster->next;
                }
            }

            target_new->next=next_cluster;
            target_new->prev=prev_cluster;
            t_head=nullptr;
            t_tail=nullptr;
            //only deal with the target row
            if(!double_collapse_t(target_new)) legalswap=false;
            
            /******* for the cellrow simply split the original cluster, which is to do nothing, for now ******/
            
        }
    else{
        whitespace=false;
        
            t=(*(--target_row.upper_bound(x))).second;
            t_backup=t->ptr_cluster;
            c->x_pos=t->x_pos;
            t->x_pos_old=t->x_pos;
            t->y_pos_old=t->y_pos;
            t->y_pos=cell_y;
            t->x_pos=cell_x;
            /**** new clusters to be passed to double_collapse and modfied ****/
            cluster_new=new cluster(*c_backup);
            target_new=new cluster(*t_backup);
            /******* change the cellist of the new cluster, i.e swap c and t to corresponding cluster  ******/
            cell_temp=std::find(cluster_new->celllist.begin(), cluster_new->celllist.end(), c);
            target_temp=std::find(target_new->celllist.begin(), target_new->celllist.end(), t);
            *cell_temp=t;
            *target_temp=c;
            /******* modify the new clusters and collapse them *****/
      
        c_head=c_backup;
        c_tail=c_backup;
        if(t->width>=c->width){
            cluster_new->width=0;
            cluster_new->e=0;
            cluster_new->q=0;
            cluster_new->x1=0;
            for(auto c_it:cluster_new->celllist){
                ++cluster_new->e;
                cluster_new->q+=c_it->x_pos-c_it->width;
                cluster_new->width+=c_it->width;
            }
            cluster_new->x1=cluster_new->q/cluster_new->e;
            
            if(cluster_new->x1<0)
                cluster_new->x1=0;
            if(cluster_new->x1+cluster_new->width>x_size)
                cluster_new->x1=x_size-cluster_new->width;
            
            if(!double_collapse_c(cluster_new))  {legalswap=false;}
        }else{
            c_split=true;
        }
        /*** modify the target clusters and collapse them  ****/
        
            t_head=t_backup;
            t_tail=t_backup;
            
            if(c->width>=t->width){
                target_new->width=0;
                target_new->q=0;
                target_new->e=0;
                for(auto c_it:target_new->celllist){
                ++target_new->e;
                target_new->q+=c_it->x_pos-c_it->width;
                target_new->width+=c_it->width;
            }
                target_new->x1=target_new->q/target_new->e;
                if(target_new->x1<0)
                    target_new->x1=0;
                if(target_new->x1+target_new->width>x_size)
                    target_new->x1=x_size-target_new->width;

                if(!double_collapse_t(target_new)) legalswap=false;
            }else{
                t_split=true;
            }
            
    }
        int new_hpwl=0;
        /***  update the cell position in the cluster ***/
        if(!legalswap) {
 //           cout<<""<<endl;
        }
        if(legalswap){
            cluster* clup=cluster_new;
            if((!whitespace)&&(!c_split)){
                if(clup->update_cell()==false) {   legalswap=false;}
            }
        }
        
        if(legalswap){
            cluster* clup=target_new;
            if(!t_split)
            if(clup->update_cell()==false) {   legalswap=false;}
        }
        
       // if(!legalswap) ++displacement;
        
        /**** get the new hpwl from the netlist ****/
        if(legalswap){
            for(auto c_it:netlist){
                new_hpwl+=(c_it.second)->get_hpwl_now();
            }
            if(new_hpwl>=old_hpwl+thresh) {
                legalswap=false;
                ++increase_count;
            }
            else{
                ++decrease_count;
            }
        }
        
        
        
        /******* the swap decreases the hpwl ***/
        if(legalswap){
            /******* update cell row cluster ********/
           
            for(auto it_net:netlist){
                (it_net.second)->uniform();
            }

            
            /***** delete the original clusters in cell row *****/
            cluster* dtemp=c_head;
            if((!whitespace)&&(!c_split)){
                /*** points the other cluster to cluster_new  **/
                if(c_head->prev!=nullptr) c_head->prev->next=cluster_new;
                else cluster_v[cell_y]=cluster_new;
                if(c_tail->next!=nullptr) c_tail->next->prev=cluster_new;

                /**** update the ptr_cluster in the cellist ***/
                for(auto it: cluster_new->celllist){
                    if(cluster_new->celllist.size()!=0)
                    it->ptr_cluster=cluster_new;
                    else{
                        cout<<"bug found in line 601 "<<endl;
                    }
                }
                while(dtemp!=c_tail){
                    cluster* dnext=dtemp->next;
                    delete dtemp;
                    dtemp=dnext;
                }
                delete dtemp;
            }else if(whitespace){
                cluster* c_front=nullptr;
                cluster* c_back=nullptr;
                /****** if the target is white space, split the original cluster into two parts ****/
                auto it=c_backup->celllist.begin();
                if(c_backup->celllist.size()==1&&it==cell_temp){
                 //   cout<<"here we got a problem in swapcount "<<swapcount<<endl;
                        if(c_backup->prev==nullptr) { cluster_v[cell_y]=c_backup->next; c_backup->next->prev=nullptr; }
                        else{
                            c_backup->prev->next=c_backup->next;
                        }
                        if(c_backup->next!=nullptr) c_backup->next->prev=c_backup->prev;
                        delete c_backup;
                }else if(it==cell_temp||cell_temp==(--c_backup->celllist.end())){
                    c_backup->celllist.erase(cell_temp);
                    c_backup->width=0;
                    c_backup->e=0;
                    c_backup->q=0;
                    int c_x1=(*(c_backup->celllist.begin()))->x_pos;
                    for(auto cell_it:c_backup->celllist){
                        ++c_backup->e;
                        c_backup->q+=cell_it->x_pos-cell_it->width;
                        c_backup->width+=cell_it->width;
                    }
                    c_backup->x1=c_x1;
                }else{
                    c_front=new cluster();
                    c_front->y_pos=c_backup->y_pos;
                    while(it!=cell_temp){
                        c_front->celllist.push_back(*it);
                        ++c_front->e;
                        c_front->q+=(*it)->x_pos-(*it)->width;
                        c_front->width+=(*it)->width;
                        ++it;
                    }
                    c_front->x1=c_backup->x1;
                    for(auto cellit:c_front->celllist){
                        if(c_front->celllist.size()!=0)
                        cellit->ptr_cluster=c_front;
                        else{
                            cout<<"bug found in line 648"<<endl;
                        }
                    }
                    ++it;
                    if(it!=c_backup->celllist.end()) {
                        int x1temp=(*it)->x_pos;
                        c_back=new cluster();
                        c_back->y_pos=c_backup->y_pos;
                        while (it!=c_backup->celllist.end()) {
                            c_back->celllist.push_back(*it);
                            ++c_back->e;
                            c_back->q+=(*it)->x_pos-(*it)->width;
                            c_back->width+=(*it)->width;
                            ++it;
                        }
                        c_back->x1=x1temp;
                        for(auto cellit:c_back->celllist){
                            if(c_back->celllist.size()!=0)
                            cellit->ptr_cluster=c_back;
                            else
                            cout<<"bug found in line 648"<<endl;

                        }
                        if(c_backup->prev!=nullptr) c_backup->prev->next=c_front; else cluster_v[cell_y]=c_front;
                        if(c_backup->next!=nullptr) c_backup->next->prev=c_back;
                        c_front->prev=c_backup->prev;
                        c_back->next=c_backup->next;
                        c_back->prev=c_front;
                        c_front->next=c_back;
                        delete c_backup;
                    }
                    else{
                        if(c_backup->prev!=nullptr) c_backup->prev->next=c_front; else cluster_v[cell_y]=c_front;
                        if(c_backup->next!=nullptr) c_backup->next->prev=c_front;
                        c_front->prev=c_backup->prev;
                        c_front->next=c_backup->next;
                        delete c_backup;
                    }
            }
            }else{
                delete c_backup;
                cluster* c_front=nullptr;
                cluster* c_back=nullptr;
                /****** if the target is white space, split the original cluster into two parts ****/
                auto it=cluster_new->celllist.begin();
              if(cell_temp==(--cluster_new->celllist.end())){
                  cluster_new->width=0;
                  cluster_new->q=0;
                  cluster_new->e=0;
                  for(auto c_it:cluster_new->celllist){
                      ++cluster_new->e;
                      cluster_new->q+=c_it->x_pos-c_it->width;
                      cluster_new->width+=c_it->width;
                  }
                  if(cluster_new->prev!=nullptr) cluster_new->prev->next=cluster_new;
                  else cluster_v[cell_y]=cluster_new;
                  if(cluster_new->next!=nullptr) cluster_new->next->prev=cluster_new;
                  
                  for(auto c_it:cluster_new->celllist){
                      c_it->ptr_cluster=cluster_new;
                  }
                  
                }else{
                /*** get c_front  ***/
                    c_front=new cluster();
                    c_front->y_pos=cluster_new->y_pos;
                    while(it!=cell_temp){
                        c_front->celllist.push_back(*it);
                        ++c_front->e;
                        c_front->q+=(*it)->x_pos-(*it)->width;
                        c_front->width+=(*it)->width;
                        ++it;
                    }
                    c_front->celllist.push_back(*it);
                    ++c_front->e;
                    c_front->q+=(*it)->x_pos-(*it)->width;
                    c_front->width+=(*it)->width;
                    ++it;
                    c_front->x1=cluster_new->x1;
                    for(auto cellit:c_front->celllist){
                            cellit->ptr_cluster=c_front;
                    }
                /*** get c_back ***/
                    int x1temp=(*it)->x_pos;
                    c_back=new cluster();
                    c_back->y_pos=cluster_new->y_pos;
                    while (it!=cluster_new->celllist.end()) {
                        c_back->celllist.push_back(*it);
                        ++c_back->e;
                        c_back->q+=(*it)->x_pos-(*it)->width;
                        c_back->width+=(*it)->width;
                        ++it;
                    }
                        c_back->x1=x1temp;
                        for(auto cellit:c_back->celllist){
                                cellit->ptr_cluster=c_back;
                        }
                        if(cluster_new->prev!=nullptr) cluster_new->prev->next=c_front; else cluster_v[cell_y]=c_front;
                        if(cluster_new->next!=nullptr) cluster_new->next->prev=c_back;
                        c_front->prev=cluster_new->prev;
                        c_back->next=cluster_new->next;
                        c_back->prev=c_front;
                        c_front->next=c_back;
                        delete cluster_new;
                }
            }
        
            
            if(!t_split){
                /**** update the ptr_cluster in the cellist ***/
                for(auto it: target_new->celllist){
                    if(target_new->celllist.size()!=0)
                        it->ptr_cluster=target_new;
                    else
                        cout<<"bug found in line 695"<<endl;

                }
                /****** update target row cluster ****/
                if(t_head!=nullptr){
                    if(t_head->prev!=nullptr) t_head->prev->next=target_new;
                    else cluster_v[y]=target_new;
                    if(t_tail->next!=nullptr) t_tail->next->prev=target_new;
                }else{
                    if(target_new->prev!=nullptr) target_new->prev->next=target_new;
                    else cluster_v[y]=target_new;
                    if(target_new->next!=nullptr) target_new->next->prev=target_new;
                }
                
                /***** delete the original clusters *****/
                dtemp=t_head;
                while(dtemp!=t_tail){
                    cluster* dnext=dtemp->next;
                    delete dtemp;
                    dtemp=dnext;
                }
                if(dtemp!=nullptr) delete dtemp;
            }else{  /***** for t_split  ****/
                delete t_backup;
                cluster* t_front=nullptr;
                cluster* t_back=nullptr;

                auto it=target_new->celllist.begin();
                
                if(target_temp==(--target_new->celllist.end())){
                    target_new->width=target_new->width-t->width+c->width;
                    
                    if(target_new->prev!=nullptr) target_new->prev->next=target_new;
                    else cluster_v[y]=target_new;
                    if(target_new->next!=nullptr) target_new->next->prev=target_new;
                    
                    for(auto c_it:target_new->celllist){
                        c_it->ptr_cluster=target_new;
                    }
                    
                }else{
                    /*** get t_front  ***/
                    t_front=new cluster();
                    t_front->y_pos=target_new->y_pos;
                    while(it!=target_temp){
                        t_front->celllist.push_back(*it);
                        ++t_front->e;
                        t_front->q+=(*it)->x_pos-(*it)->width;
                        t_front->width+=(*it)->width;
                        ++it;
                    }
                    t_front->celllist.push_back(*it);
                    ++t_front->e;
                    t_front->q+=(*it)->x_pos-(*it)->width;
                    t_front->width+=(*it)->width;
                    ++it;
                    t_front->x1=target_new->x1;
                    for(auto cellit:t_front->celllist){
                        cellit->ptr_cluster=t_front;
                    }
                    /*** get t_back ***/
                    int x1temp=(*it)->x_pos;
                    t_back=new cluster();
                    t_back->y_pos=target_new->y_pos;
                    while (it!=target_new->celllist.end()) {
                        t_back->celllist.push_back(*it);
                        ++t_back->e;
                        t_back->q+=(*it)->x_pos-(*it)->width;
                        t_back->width+=(*it)->width;
                        ++it;
                    }
                    t_back->x1=x1temp;
                    for(auto cellit:t_back->celllist){
                        cellit->ptr_cluster=t_back;
                    }
                    if(target_new->prev!=nullptr) target_new->prev->next=t_front; else cluster_v[y]=t_front;
                    if(target_new->next!=nullptr) target_new->next->prev=t_back;
                    t_front->prev=target_new->prev;
                    t_back->next=target_new->next;
                    t_back->prev=t_front;
                    t_front->next=t_back;
                    delete target_new;
                }
            }
            
            
            
            // update row map
            target_row.clear();
            cluster* t_temp=cluster_v[y];
            while(t_temp!=nullptr){
                for (auto cell_ptr:t_temp->celllist){
                    if(cell_ptr==nullptr){
                        cout<<"WTF"<<endl;
                    }
                    target_row[cell_ptr->x_pos]=cell_ptr;
                }
                t_temp=t_temp->next;
            }
            cell_row.clear();
            cluster* c_temp=cluster_v[cell_y];
            while(c_temp!=nullptr){
                for (auto cell_ptr:c_temp->celllist){
                    cell_row[cell_ptr->x_pos]=cell_ptr;
                }
                c_temp=c_temp->next;
            }

        }else{  //the swap increases the hpwl, need to reverse
            //reverse cell x_pos, y_pos
            for(auto it_net:netlist){
                (it_net.second)->reverse();
            }
            //reverse cluster
            if(c==nullptr) cout<<"bug in line 765"<<endl;
            if(whitespace) {
                //restore the nullptr in c_backup
                *cell_temp=c;
            }
            else{
                //restore the ptr in c_backup
                *cell_temp=c;
                delete cluster_new;
            }
            /*
            if(t_head==nullptr) {
                target_new->prev->next=target_new->next;
                if(target_new->next!=nullptr) target_new->next->prev=target_new->prev;
            }
            else{
                if(target_new->next!=nullptr){
                    target_new->prev->next=c_head;
                    target_new->next->prev=c_tail;
                }
            }
             */
            //resotre the ptr in t_backup, then delete target_new
            if(!whitespace) { if(t==nullptr) cout<<"bug in line 787"<<endl; *target_temp=t; }
            delete target_new;
            // row map untouched, no need to reverse!!
            
          //  if(cell_row.find(c->x_pos)!=cell_row.end())  target_row[x]=cell_row[c->x_pos];
          //  cell_row[c->x_pos]=c;
        }
//        
//                if(check_legal()==false){
//                cout<<swapcount<<endl;
//                cout<<"shit happens"<<endl;
//            }

        if(legalswap)
        return new_hpwl-old_hpwl;
        else
            return 1;
    }
    
    
    
    void addcluster(cluster* prevc, cluster* c){
        prevc->width+=c->width;
        prevc->e+=c->e;
        prevc->q+=c->q-prevc->e*c->width;
        prevc->x1=prevc->q/prevc->e;
        if(prevc->x1<0) prevc->x1=0;
        if(prevc->x1>x_size*sitewidth-prevc->width) prevc->x1=x_size*sitewidth-prevc->width;
    }
    
    bool collapse(list<cluster*>& clist, cluster* clu_p){
        clu_p->x1=clu_p->q/clu_p->e;
        if(clu_p->x1<0) clu_p->x1=0;
        else if(clu_p->x1>x_size-clu_p->width) clu_p->x1=x_size-clu_p->width;
        else{
            auto it=clist.end();
            --it;
            if(it==clist.begin()) return true;
            --it;
            if((*it)->fixed==1) return false;
            if((*it)->x1+(*it)->width>clu_p->x1){
                addcluster(*it,clu_p);
                clist.pop_back();
                return collapse(clist, *it);
            }
            
        }
        return true;
    }
    
   
    
    /*
    bool placerow(map<int,cell*>& row, list<cluster*>& clist){
        auto b=row.begin();
        auto e=row.end();
        int lastx=0;
        cluster* clu_p;
        while(b!=e){
            if(clist.empty()||((*b).second)->fixed==1||lastx!=((*b).second)->x_pos){
                clu_p=new cluster();
                if(((*b).second)->fixed==1){
                    if(((*b).second)->x_pos<lastx)
                    { delete clu_p; return false;}
                    else{
                        clu_p->fixed=1;
                    }
                }
                clu_p->e=1;
                clu_p->x1=((*b).second)->x_pos;//the x coordinate of the first cell
                clu_p->q=((*b).second)->x_pos-clu_p->width;
                clu_p->width=((*b).second)->width; //the width of cell, the name may change
                clu_p->celllist.push_back((*b).second);
                clist.push_back(clu_p);
            }
            else{
                ++(clu_p->e);
                clu_p->q+=clu_p->e*(((*b).second)->x_pos-clu_p->width);
                clu_p->width+=((*b).second)->width;
                clu_p->celllist.push_back((*b).second);
                if(collapse(clist,clu_p)==false){
                    delete clu_p; return false;
                }
            }
            ++b;
            
        }
        return true;
    }
    */
    
    
};





#endif /* PLACEROW_H_ */
