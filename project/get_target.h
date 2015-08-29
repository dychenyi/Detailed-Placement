/*
 * get_target.h
 *
 *  Created on: Apr 20, 2015
 *      Author: yh7483
 */

#ifndef GET_TARGET_H_
#define GET_TARGET_H_

#include "class_define.h"
ofstream ffout;
const int x_far = 12;
const int y_far = 12;
const int x_move_random = 64;
const int y_move_random =16;

const int x_far_3 = 12;
const int y_far_3 = 1;
//const int x_move_random_3 = 32;
//const int y_move_random_3 =2;
double possibility = 1.0;		//the larger, the more likely swap


void get_optimal_region(cell* ptr_cell, int& xl, int& xr, int& yl, int& yh){
    multiset<int> x_series;
    multiset<int> y_series;

    int xl_temp, xr_temp, yl_temp, yh_temp;

    for(auto ptr_net = (ptr_cell->list_net).begin(); ptr_net != (ptr_cell->list_net).end(); ++ptr_net){
    	(*ptr_net)->bounding_exclude(ptr_cell, xl_temp, xr_temp, yl_temp, yh_temp);
    	//xl_temp = xr_temp = yl_temp = yh_temp = 16;
    	x_series.insert(xl_temp);
    	x_series.insert(xr_temp);
    	y_series.insert(yl_temp);
    	y_series.insert(yh_temp);
    }
/*
    int x_size_half = x_series.size() / 3;////////////////
    int y_size_half = y_series.size() / 3;////////////////
    auto x_begin = x_series.begin();
    auto y_begin = y_series.begin();
    std::advance(x_begin, x_size_half - 1);
    xl = *x_begin;
    std::advance(x_begin, x_size_half - 1);
    xr = *x_begin;
    std::advance(y_begin, y_size_half - 1);
    yl = *y_begin;
    std::advance(y_begin, y_size_half - 1);
    yh = *y_begin;
*/

												//original same as paper
    int x_size_half = x_series.size() / 2;
    int y_size_half = y_series.size() / 2;
    auto x_begin = x_series.begin();
    auto y_begin = y_series.begin();
    std::advance(x_begin, x_size_half - 1);
    xl = *x_begin;
    ++x_begin;
    xr = *x_begin;
    std::advance(y_begin, y_size_half - 1);
    yl = *y_begin;
    ++y_begin;
    yh = *y_begin;
}

void get_s(map<int, cell*>::iterator current_cell, int& s1, int& s2, int& s3, int& s4, map<int, cell*>::iterator be, map<int, cell*>::iterator en){
	auto current_copy1 = current_cell;
	auto current_copy2 = current_cell;
	if(current_copy1 == be){
		s1 = s2 = 0;
		++current_copy2;
		s3 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		++current_copy1; ++current_copy2;
		s4 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		return;
	}
	--current_copy1;
	if(current_copy1 == be){
		s1= 0;
		s2 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		++current_copy1; ++current_copy2;
		s3 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		++current_copy1; ++current_copy2;
		s4 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		return;
	}
	--current_copy1; --current_copy2;

	s1 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
	++current_copy1; ++current_copy2;
	s2 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
	++current_copy1; ++current_copy2;
	if(current_copy2 == en){
		s3 = s4 = 0;
		return;
	}
	s3 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
	++current_copy1; ++current_copy2;
	if(current_copy2 == en){
		s4 = 0;
		return;
	}
	s4 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
}

void get_s_space(map<int, cell*>::iterator current_cell, int& s1, int& s2, int& s,map<int, cell*>::iterator be, map<int, cell*>::iterator en){
	auto current_copy1 = current_cell;
	auto current_copy2 = current_cell;
	if(current_copy1 == be){
		s1 = 0;
		++current_copy2;
		s = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		++current_copy1; ++current_copy2;
		s2 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
		return;
	}
	--current_copy1;
	s1 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
	++current_copy1; ++current_copy2;
	if(current_copy2 == en){
		s = s2 = 0;
		return;
	}
	s = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
	++current_copy1; ++current_copy2;
	if(current_copy2 == en){
		s2 = 0;
		return;
	}
	s2 = current_copy2->second->x_pos - current_copy1->second->x_pos - current_copy1->second->width;
}

void get_W(cell* current_cell, cell* ptr_cell, int& W1, int& W2){
	current_cell->x_pos = ptr_cell->x_pos_old;
	current_cell->y_pos = ptr_cell->y_pos_old;
	ptr_cell->x_pos = current_cell->x_pos_old;
	ptr_cell->y_pos = current_cell->y_pos_old;

	W1 = 0; W2 = 0;
	for(auto c: current_cell->list_net)
		W1 += c->get_hpwl_old();
	for(auto c: ptr_cell->list_net)
		W1 += c->get_hpwl_old();
	for(auto c: current_cell->list_net)
		W2 += c->get_hpwl_now();
	for(auto c: ptr_cell->list_net)
		W2 += c->get_hpwl_now();

	current_cell->reverse();
	ptr_cell->reverse();
}

void get_W_space(cell* current_cell, cell* ptr_cell, int& W1, int& W2){
	ptr_cell->x_pos = current_cell->x_pos_old + current_cell->width;
	ptr_cell->y_pos = current_cell->y_pos_old;

	W1 = 0; W2 = 0;
	for(auto c: ptr_cell->list_net)
		W1 += c->get_hpwl_old();
	for(auto c: ptr_cell->list_net)
		W2 += c->get_hpwl_now();

	ptr_cell->reverse();
}
/*******************************not using penalty******************************/
void get_penalty(map<int, cell*>::iterator current_cell, cell* ptr_cell, multimap<double, point>& temp){
	double penalty;		//the smaller, the better
	int W1, W2;
	get_W(current_cell->second, ptr_cell, W1, W2);
	double P1, P2;

	point my_point{current_cell->second->x_pos, current_cell->second->y_pos};

			penalty = W2 - W1;

		if(penalty < 0){
//			ffout<<"W1-W2="<<W1 - W2<<" P1="<<P1<<" P2="<<P2<<"   W1="<<W1<<" W2="<<W2<<std::endl;/////////////////////////////////////
			temp.insert(std::pair<double, point>(penalty, my_point));
		}
}

void get_penalty_space(map<int, cell*>::iterator current_cell, cell* ptr_cell, multimap<double, point>& temp){
	double penalty;
	int W1, W2;
	get_W_space(current_cell->second, ptr_cell, W1, W2);

	point my_point{current_cell->second->x_pos + current_cell->second->width, current_cell->second->y_pos};

		penalty = W2 -W1;

		if(penalty < 0){
//			ffout<<"W1-W2="<<W1 - W2<<" P1="<<P1<<" P2="<<P2<<"   W1="<<W1<<" W2="<<W2<<std::endl;/////////////////////////////////////
			temp.insert(std::pair<double, point>(penalty, my_point));
		}
}


/***************************using penalty
void get_penalty(map<int, cell*>::iterator current_cell, cell* ptr_cell, multimap<double, point>& temp){
	double penalty;		//the smaller, the better
	int W1, W2;
	get_W(current_cell->second, ptr_cell, W1, W2);
	double P1, P2;

	point my_point{current_cell->second->x_pos, current_cell->second->y_pos};

		if(current_cell->second->width == ptr_cell->width){
			penalty = W2 - W1;
		}
		if(current_cell->second->width < ptr_cell->width){
			int s1, s2, s3, s4;
			get_s(current_cell, s1, s2, s3, s4, subrow[current_cell->second->y_pos].begin(), subrow[current_cell->second->y_pos].end());
			int S = s1+ s2 + s3 + s4;
			P1 = (ptr_cell->width - current_cell->second->width - s2 - s3) * wt1;
			P2 = (ptr_cell->width - current_cell->second->width - S) * wt2;
			penalty = P1 + P2 + W2 -W1;
		}
		if(current_cell->second->width > ptr_cell->width){
			int row_pos = ptr_cell->y_pos;
			auto it_ptr_cell = subrow[row_pos].lower_bound(ptr_cell->x_pos);

			int s1, s2, s3, s4;
			get_s(it_ptr_cell, s1, s2, s3, s4, subrow[row_pos].begin(), subrow[row_pos].end());
			int S = s1+ s2 + s3 + s4;
			P1 = (current_cell->second->width - ptr_cell->width - s2 - s3) * wt1;
			P2 = (current_cell->second->width - ptr_cell->width - S) * wt2;
			penalty = P1 + P2 + W2 -W1;
		}
		if(penalty < 0){
//			ffout<<"W1-W2="<<W1 - W2<<" P1="<<P1<<" P2="<<P2<<"   W1="<<W1<<" W2="<<W2<<std::endl;/////////////////////////////////////
			temp.insert(std::pair<double, point>(penalty, my_point));
		}
}

void get_penalty_space(map<int, cell*>::iterator current_cell, cell* ptr_cell, multimap<double, point>& temp){
	double penalty;
	int W1, W2;
	get_W_space(current_cell->second, ptr_cell, W1, W2);

	point my_point{current_cell->second->x_pos + current_cell->second->width, current_cell->second->y_pos};

		int s1,s2,s;
		get_s_space(current_cell, s1, s2, s, subrow[current_cell->second->y_pos].begin(), subrow[current_cell->second->y_pos].end());
		int S = s1 + s2 + s;
		double P1 = (ptr_cell->width - s) * wt1;
		double P2 = (ptr_cell->width - S) * wt2;
		penalty = P1 + P2 + W2 -W1;

		penalty = W2 -W1;

		if(penalty < 0){
//			ffout<<"W1-W2="<<W1 - W2<<" P1="<<P1<<" P2="<<P2<<"   W1="<<W1<<" W2="<<W2<<std::endl;/////////////////////////////////////
			temp.insert(std::pair<double, point>(penalty, my_point));
		}
}
*/


vector<point> get_swap_target(cell* ptr_cell, int& xl, int& xr, int& yl, int& yh){
	multimap<double,point> temp;
	vector<point> good_target;
	int limit = 0;//////////////////////////////////////////////////////////////////////////////dfjsafkjadkfjalf/
	for(int y_row = std::max<int>(yl, ptr_cell->y_pos_origin - y_displacement); y_row < std::min<int>(yh, ptr_cell->y_pos_origin + y_displacement) + 1; ++y_row){

		int x_lower_bound = std::max<int>(xl, ptr_cell->x_pos_origin - x_displacement);
		int x_upper_bound = std::min<int>(xr, ptr_cell->x_pos_origin + x_displacement);
		if (x_upper_bound < x_lower_bound) break;

		//cout<<"lower_bound: "<<x_lower_bound<<" upper_bound: "<<x_upper_bound<<endl;////////////////
		////////////////////////////////////////////////////////////////////////////////here may be farther optimized
		auto start_cell = subrow[y_row].lower_bound(x_lower_bound);
		auto end_cell = subrow[y_row].lower_bound(x_upper_bound);
		//if(start_cell == subrow[y_row].end()) break;
		//if(end_cell == subrow[y_row].end()) break;

		//cout<<"start: "<<start_cell->second->x_pos<<" "<<start_cell->second->y_pos<<"       end: "<<end_cell->second->x_pos<<" "<<end_cell->second->y_pos<<endl;

		for(auto current_cell = start_cell; (current_cell != end_cell) && limit < limit_num; ++current_cell){
			++limit;
			if(current_cell->second == ptr_cell) continue;
			get_penalty(current_cell, ptr_cell, temp);
			auto next_cell = current_cell;
			++next_cell;
			if(next_cell == subrow[y_row].end())		//next also should not be end()
				break;
			if(next_cell->second->x_pos > (current_cell->second->x_pos + current_cell->second->width))	//avoid two cell is a cluster
				get_penalty_space(current_cell, ptr_cell, temp);
		}
	}
	auto ptr_temp = temp.begin();
	good_target.clear();
	for(int i = 0; (i < good_num) && (ptr_temp != temp.end()); ++i){
		good_target.push_back(ptr_temp->second);
		++ptr_temp;
	}
	return good_target;
}

vector<point> get_target(cell* ptr_cell){
	int xl, xr, yl, yh;
	get_optimal_region(ptr_cell, xl, xr, yl, yh);
	return get_swap_target(ptr_cell, xl, xr, yl, yh);
}




bool far_from_optimal(cell* ptr_cell){
	int xl, xr, yl, yh;
	get_optimal_region(ptr_cell, xl, xr, yl, yh);
	if(xl - ptr_cell->x_pos > x_far) return true;
	if(ptr_cell->x_pos - xr > x_far) return true;
	if(yl - ptr_cell->y_pos > y_far) return true;
	if(ptr_cell->y_pos - yh > y_far) return true;

	return false;
}

vector<point> get_target_s2(cell* ptr_cell){
	vector<point> xx;
	xx.clear();

	if(!far_from_optimal(ptr_cell))
		return xx;

	srand((unsigned)time(NULL));
	int x = rand() % x_move_random - x_move_random / 2 + ptr_cell->x_pos;
	if(x < ptr_cell->x_pos_old - x_displacement) x = ptr_cell->x_pos_old - x_displacement;
	if(x > ptr_cell->x_pos_old + x_displacement) x = ptr_cell->x_pos_old + x_displacement;
	if(x <= 0) x = 0;
	if(x >= column_num - 1) x = column_num - 1;

	srand((unsigned)time(NULL));
	int y = rand() % y_move_random - y_move_random / 2 + ptr_cell->y_pos;
	if(y < ptr_cell->y_pos_old - y_displacement) y = ptr_cell->y_pos_old - y_displacement;
	if(y > ptr_cell->y_pos_old + y_displacement) y = ptr_cell->y_pos_old + y_displacement;
	if(y <= 0) y = 0;
	if(y >= row_num - 1) y = row_num - 1;


	if(x != ptr_cell->x_pos || y != ptr_cell->y_pos){
		point my_point{x, y};
		xx.push_back(my_point);
	}
	return xx;
}



vector<point> get_target_s3(cell* ptr_cell){
	vector<point> xx;
	xx.clear();

	int xl, xr, yl, yh;
	get_optimal_region(ptr_cell, xl, xr, yl, yh);

	int x_center = (xl + xr) / 2;
	int y_center = (yl + yh) / 2;

	int x = -1;
	int y = -1;

	if(xl - ptr_cell->x_pos > x_far_3){
		srand((unsigned)time(NULL));
		//x = rand() % x_move_random_3 + ptr_cell->x_pos;
		x = rand() % (2 * (x_center - ptr_cell->x_pos)) + ptr_cell->x_pos + 1;
		if(x < ptr_cell->x_pos_old - x_displacement) x = ptr_cell->x_pos_old - x_displacement;
		if(x > ptr_cell->x_pos_old + x_displacement) x = ptr_cell->x_pos_old + x_displacement;
		if(x <= 0) x = 0;
		if(x >= column_num - 1) x = column_num - 1;

	}
	else if(ptr_cell->x_pos - xr > x_far_3){
		srand((unsigned)time(NULL));
		//x = ptr_cell->x_pos - rand() % x_move_random_3;
		x = ptr_cell->x_pos - rand() % (2 * (x_center - ptr_cell->x_pos)) - 1;
		if(x < ptr_cell->x_pos_old - x_displacement) x = ptr_cell->x_pos_old - x_displacement;
		if(x > ptr_cell->x_pos_old + x_displacement) x = ptr_cell->x_pos_old + x_displacement;
		if(x <= 0) x = 0;
		if(x >= column_num - 1) x = column_num - 1;

	}

	if(yl - ptr_cell->y_pos > y_far_3){
		srand((unsigned)time(NULL));
		//y = rand() % y_move_random_3 + ptr_cell->y_pos;
		y = rand() % (2 * (y_center - ptr_cell->y_pos)) + ptr_cell->y_pos + 1;
		if(y < ptr_cell->y_pos_old - y_displacement) y = ptr_cell->y_pos_old - y_displacement;
		if(y > ptr_cell->y_pos_old + y_displacement) y = ptr_cell->y_pos_old + y_displacement;
		if(y <= 0) y = 0;
		if(y >= row_num - 1) y = row_num - 1;

	}
	else if(ptr_cell->y_pos - yh > y_far_3){
		srand((unsigned)time(NULL));
		//y = ptr_cell->y_pos - rand() % y_move_random_3;
		y = ptr_cell->y_pos - rand() % (2 * (y_center - ptr_cell->y_pos)) - 1;
		if(y < ptr_cell->y_pos_old - y_displacement) y = ptr_cell->y_pos_old - y_displacement;
		if(y > ptr_cell->y_pos_old + y_displacement) y = ptr_cell->y_pos_old + y_displacement;
		if(y <= 0) y = 0;
		if(y >= row_num - 1) y = row_num - 1;

	}

	if(x == -1 && y != -1 ){
		point my_point{ptr_cell->x_pos, y};
		xx.push_back(my_point);
	}
	if(x != -1 && y == -1){
		point my_point{x, ptr_cell->y_pos};
		xx.push_back(my_point);
	}
	if(x != -1 && y != -1){
		point my_point{x, y};
		xx.push_back(my_point);
	}


	srand((unsigned)time(NULL));
	double possi = rand() / (double)RAND_MAX;
	if(possi <  (1 - possibility))
		xx.clear();
	return xx;

}



#endif /* GET_TARGET_H_ */
