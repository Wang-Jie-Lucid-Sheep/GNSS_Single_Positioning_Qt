#include "Function.h"
int Find_best_satellite(int middle, MatrixXd *comp_mat)
{
	double min = (*comp_mat)(1,0);
	int best_satellite = 0 ;
	for (int i = 0; i <= middle - 1; i++)
	{
		if ((*comp_mat)(1, i) < min)
		{
			min = (*comp_mat)(1, i);
			best_satellite = i;
		}
	}
	return best_satellite;

}