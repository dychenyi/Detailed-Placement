/*
 * result_func.h
 *
 *  Created on: Apr 20, 2015
 *      Author: yh7483
 */

#ifndef RESULT_FUNC_H_
#define RESULT_FUNC_H_

#include "class_define.h"


bool check_legal(){
	int** globe_map = new int*[row_num];
	for(int i = 0; i < row_num; ++i)
		globe_map[i] = new int[column_num];
	for(int i = 0; i < row_num; ++i)
		for(int j = 0; j < column_num; ++j)
			globe_map[i][j] = 0;

	for(int i = 0; i < vec_cell_counter; ++i){
		if(!(vec_cell + i)->direction){
			int row_t = (vec_cell + i)->y_pos;
			int column_t = (vec_cell + i)->x_pos;
			int width_t = (vec_cell + i)->width;
			int height_t = (vec_cell + i)->height;

			//cout<<"y_pos: "<<row_t<<" x_pos: "<<column_t<<" width: "<<width_t<<" height: "<<height_t<<endl;

			for(int hi = 0; hi < height_t; ++hi){
				for(int wi = 0; wi < width_t; ++wi){
					++globe_map[row_t + hi][column_t + wi];
					if(globe_map[row_t + hi][column_t + wi] > 1){
						for(int i = 0; i < row_num; ++i)
							delete[] globe_map[i];
						delete[] globe_map;
						return false;
					}
				}
			}
		}
	}
	for(int i = 0; i < row_num; ++i)
		delete[] globe_map[i];
	delete[] globe_map;
	return true;
}


double total_hpwl(){
	double result = 0;
	for(int i = 0; i < net_num; ++i)
		result += (vec_net + i)->get_hpwl_now();
	return result;
}

void display_time(){
	time_t t=time(0);
	struct tm* now=localtime(&t);
	cout<<"now   time: "<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<endl;
}


#endif /* RESULT_FUNC_H_ */
