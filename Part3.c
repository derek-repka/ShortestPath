/*---------------------------------------------------------------
Assignment:           3
Due Date:             April 7, 2016
Name:                 Derek Repka
Unix ID:              repka
Lecture Section:      B1
Lab Section:          H1
Teaching Assistant:   Juehui Fan
---------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int num1, int num2);
void place(int points, int start, int dest, int orin, int coords[2][points], int edges[3][points-1], int result[points-1], int overlap[points-1][points-1]);

int main(int argc, char *argv[]){
	FILE *file;
	int max_x, max_y, points, i, j, initial;
	int total_overlap = 0, weight = 0;
	float reduction;

	/* Ensure the file exists. If not, file is created, but will fail the subsequent tests.*/
	if ((file = fopen(argv[1], "a+")) == NULL){
		printf("File does not exist.\n");
		return -1;
	}

	while (fscanf(file, "%d", &max_x) != 1) {
		fscanf(file, "%*[^\n]"); /* skip the rest of line */
	}
	if (fscanf(file, "%d", &max_y) != 1) {
		printf("Max_y not following max_x\n");
		fclose(file);
		return -2;
	}
	while (fscanf(file, "%d", &points) != 1) {
		fscanf(file, "%*[^\n]"); /* skip the rest of line */
	}

	/* Initialize the arrays needed.*/
	int coords[2][points], edges[3][points-1], result[points-1], overlap[points-1][points-1];
	/*reset arrays*/
	for (i = 0; i < (points-1); i++){
		for (j = 0; j < (points-1); j++){
			overlap[i][j] = 0;
		result[i] = 0;
		overlap[0][i] = 0;
		}
	}

	/* Read in the coordinates, reporting an error if x or y is greater than max_x or max_y respectively.*/
	for (i = 0; i < points; i++){
		while (fscanf(file, "%d", &coords[0][i]) != 1) {
			fscanf(file, "%*[^\n]"); /* skip the rest of line */
		}
		if (fscanf(file, "%d", &coords[1][i]) != 1) {
			printf("y_coordinate not following x_coordinate\n");
			fclose(file);
			return -3;
		}
	}
	
	for (i = 0; i < (points - 1); i++){
		while (fscanf(file, "%d", &edges[0][i]) != 1) {
			fscanf(file, "%*[^\n]"); /* skip the rest of line */
		}
		if (fscanf(file, "%d", &edges[1][i]) != 1) {
			printf("Second point not following first\n");
			fclose(file);
			return -4;
		}
		if (fscanf(file, "%d", &edges[2][i]) != 1) {
			printf("Distance not following second point\n");
			fclose(file);
			return -5;
		}
	}
	
	initial = edges[1][0];	/*get the root of the tree*/
	place(points, initial, 0, 0, coords, edges, result, overlap); /*call recursive function*/
	fprintf(file, "# layouts of the edges of the MST by Prim's Algorithm:\n");
	for (i = 0; i < (points-1); i++){
		if (result[i] == 1 || result[i] == 0){
			if (coords[0][edges[0][i]-1] == coords[0][edges[1][i]-1] || coords[1][edges[0][i]-1] == coords[1][edges[1][i]-1]){
				fprintf(file, "(%d, %d) --> (%d, %d)\n", coords[0][edges[0][i]-1], coords[1][edges[0][i]-1], coords[0][edges[1][i]-1], coords[1][edges[1][i]-1]);
			}
			else fprintf(file, "(%d, %d) --> (%d, %d) --> (%d, %d)\n", coords[0][edges[0][i]-1], coords[1][edges[0][i]-1], coords[0][edges[0][i]-1], coords[1][edges[1][i]-1], coords[0][edges[1][i]-1], coords[1][edges[1][i]-1]);
		}
		else if (result[i] == 2){
			if (coords[0][edges[0][i]-1] == coords[0][edges[1][i]-1] || coords[1][edges[0][i]-1] == coords[1][edges[1][i]-1]){
				fprintf(file, "(%d, %d) --> (%d, %d)\n", coords[0][edges[0][i]-1], coords[1][edges[0][i]-1], coords[0][edges[1][i]-1], coords[1][edges[1][i]-1]);
			}
			else fprintf(file, "(%d, %d) --> (%d, %d) --> (%d, %d)\n", coords[0][edges[0][i]-1], coords[1][edges[0][i]-1], coords[0][edges[1][i]-1], coords[1][edges[0][i]-1], coords[0][edges[1][i]-1], coords[1][edges[1][i]-1]);
		}
	}
	
	for (i = 0; i < (points-1); i++) {
		for (j = 0; j <(points-1); j++){
			total_overlap += overlap[j][i];
		}
		weight += edges[2][i];
	}
	fprintf(file, "# total weight of the MST is %d\n", weight);
	fprintf(file, "# total overlap of the layout is %d\n", total_overlap);
	reduction = weight;
	fprintf(file, "# reduction is %.2f\n", total_overlap/reduction);
	fclose(file);
	return 0;
}

int min(int num1, int num2) {
	if (num1 > num2) return num2;
	else return num1;
}

void place(int points, int start, int dest, int orin, int coords[2][points], int edges[3][points-1], int result[points-1], int overlap[points-1][points-1]){
	int overlap_temp, i, j, pos;
	int num = 0, xi = 0, yi = 0, xj = 0, yj = 0, xk=0, yk = 0;
	int comp[3][points-1];
	
	for (i = 0; i < (points-1); i++){
		comp[0][i] = 0;
		comp[1][i] = 0;
		comp[2][i] = 0;
	}
		
	for (i = 0; i < (points-1); i++){
		if (edges[0][i] == dest) pos = i;
		if (edges[1][i] == start){
			comp[0][num] = edges[0][i];
			comp[1][num] = start;
			comp[2][num] = i;
			num += 1;
			place(points, edges[0][i], edges[1][i], 1, coords, edges, result, overlap);
			place(points, edges[0][i], edges[1][i], 2, coords, edges, result, overlap);
		}
	}
	if (num == 0) return;
	if (num > 1){
		for (i = 0; i < num; i++){
			for (j = i + 1; j < num; j++){
				overlap_temp = 0;
				if (i == j) continue;
				xi = coords[0][start - 1];
				yi = coords[1][start - 1];
				xj = coords[0][comp[0][i]-1];
				yj = coords[1][comp[0][i]-1];
				xk = coords[0][comp[0][j]-1];
				yk = coords[1][comp[0][j]-1];
				if (((yj >= yi) && (yk >= yi))||((yj <= yi) && (yk <= yi))){
					overlap_temp = min(abs(yj - yi), abs(yk - yi));
					if (overlap_temp > overlap[dest-1][start-1]){
						overlap[dest-1][start-1] = overlap_temp;
						result[comp[2][j]] = 2;
						result[comp[2][i]] = 2;

					}
				}		
				if (((xj >= xi) && (xk >= xi)) || ((xj <= xi) && (xk <= xi))){
					overlap_temp = min(abs(xj - xi), abs(xk - xi));
					if (overlap_temp > overlap[dest-1][start-1]){
						overlap[dest-1][start-1] = overlap_temp;
						result[comp[2][j]] = 1;
						result[comp[2][i]] = 1;
					}
				}
			}
		}
	}
	
	for (i = 0; i < num; i++){
		overlap_temp = 0;
		xi = coords[0][start-1];
		xj = coords[0][comp[0][i]-1];
		xk = coords[0][dest-1];
		yi = coords[1][start-1];
		yj = coords[1][comp[0][i]-1];
		yk = coords[1][dest-1];
		if (((xj >= xi) && (xk >= xi)) || ((xj <= xi) && (xk <= xi))){
			overlap_temp = min(abs(xj - xi), abs(xk - xi));
			if (overlap_temp > overlap[start-1][dest-1]){
				overlap[start-1][dest-1] = overlap_temp;
				result[pos] = orin;
			}
		}
		if (((yj >= yi) && (yk >= yi)) || ((yj <= yi) && (yk <= yi))){
			overlap_temp = min(abs(yj - yi), abs(yk - yi));
			if (overlap_temp > overlap[start-1][dest-1]){
				overlap[start-1][dest-1] = overlap_temp;
				result[pos] = orin;
			}
		}
	}
	return;
}
