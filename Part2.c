/*This program will compute the minimal spanning tree between points placed within an area. It works by comparing
each point that is outside the tree with all points that are currently in the tree. If the distance between two
points outside the tree to the tree are the same, the tie is broken first by which connection has the larger
y component, then by which of the points has a larger x coordinate. If the points are still tied, then most recently
calculated point is selected arbitrarily. After the shortest point is determined, it is added to the tree
and this continues until all points are placed. The program selects the first point listed in the input file 
as the start of the tree. To run the program, make sure to specifiy "-i" has the first parameter and the file
name as the second.
---------------------------------------------------------------
Assignment:           2
Due Date:             March 8, 2016
Name:                 Derek Repka
Unix ID:              repka
Lecture Section:      B1
Lab Section:          H1
Teaching Assistant:   Juehui Fan
---------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int num1, int num2);

int main(void){
	FILE *file;
	char filename[64];
	for (int x = 60; x < 110; x += 10){
		for (int y = 0; y < 10; y++){
			
			sprintf(filename, "instance%d_%d.txt", x, y);
   			int max_x, max_y, points, i, j, distance, low_distance, point, tree_point, shortest_tree; 
    		int shortest = 0, tree_length = 0, placed = 1;
    
   			/* Ensure the file exists. If not, file is created, but will fail the subsequent tests.*/
			if ((file = fopen(filename, "a+")) == NULL){
				printf("File does not exist.\n");
				return -2;
			}
		
			while (fscanf(file, "%d", &max_x) != 1) {
				if (ferror(file)) {
				/* read error */
					fclose(file);
					return -3;
				}
				if (feof(file)) {
				/* no integer to read */
					fclose(file);
					return -4;
				}
				fscanf(file, "%*[^\n]"); /* skip the rest of line */
			}
			if (fscanf(file, "%d", &max_y) != 1) {
				/* max_y not following max_x */
				fclose(file);
				return -5;
			}
			while (fscanf(file, "%d", &points) != 1) {
				if (ferror(file)) {
				/* read error */
					fclose(file);
					return -6;
				}
				if (feof(file)) {
				/* no integer to read */
					fclose(file);
					return -7;
				}
				fscanf(file, "%*[^\n]"); /* skip the rest of line */
			}
		
			/* Initialize the arrays needed.*/
			int coords[2][points], in_tree[points], not_tree[points-1];
	
			/* Read in the coordinates, reporting an error if x or y is greater than max_x or max_y respectively.*/
			for (i = 0; i < points; i++){
				while (fscanf(file, "%d", &coords[0][i]) != 1) {
					if (ferror(file)) {
					/* read error */
						fclose(file);
						return -8;
					}
					if (feof(file)) {
					/* no integer to read */
						fclose(file);
						return -9;
					}
					fscanf(file, "%*[^\n]"); /* skip the rest of line */
				}
				if (fscanf(file, "%d", &coords[1][i]) != 1) {
				/* y_coordinate not following x_coordinate */
					fclose(file);
					return -10;
				}
			}
			/* Place the first point into the tree.*/
			in_tree[0] = 1;
			for (i = 1; i < points; i++) in_tree[i] = 0;
	
			/* Place all other points into another array.*/
			for (i = 0; i < points-1; i++) not_tree[i] = i + 1;
	
			/* Print to file the number of edges for Prim's algorithim */
			fprintf(file, "# edges of the MST by Prim's algorithm: %d\n", points - 1);
	
			/* While all points are not in the tree, loop until they are.*/	
			while (placed < points){
				/* Reset the lowest distance to ensure connections are made.*/
				low_distance = max_x + max_y;
				for (i = 0; i < points - 1; i++){
					/* Go through all the points not already in the tree, skipping those that are.*/
					if (not_tree[i] == 0) continue;
					/* Get the index of the point not in the tree.*/
					point = not_tree[i];
					for(j = 0; j < points; j++){
						/* Go through all the points in the tree, skipping those that are not in the tree.*/
						if (in_tree[j] != j + 1) continue;
						/* Get the index of the point not in the tree.*/
						tree_point = in_tree[j] - 1;
						/* Calculate the distance between the points.*/
						distance = abs(coords[0][point] - coords[0][tree_point]) + abs(coords[1][point] - coords[1][tree_point]);
						/* If this distance is the lowest of all edges, set the points as the lowest.*/
						if (distance == 0) continue;
						if (distance < low_distance){
							low_distance = distance;
							shortest = point;
							shortest_tree = tree_point;
						}
				
						/* If the distance is the same, break the tie.*/
						else if (low_distance == distance){
						/* Compare the y distance, choosing the one with the larger y component.*/
							if (abs(coords[1][point] - coords[1][tree_point]) > abs(coords[1][shortest] - coords[1][shortest_tree])){
								shortest = point;
								shortest_tree = tree_point;
							}
							/* If the y components are the same, choose the one with the larger x coordinate.*/
							else if (abs(coords[1][point] - coords[1][tree_point]) == abs(coords[1][shortest] - coords[1][shortest_tree])){
								if (max(coords[0][tree_point], coords[0][point]) > max(coords[0][shortest], coords[0][shortest_tree])){
									shortest = point;
									shortest_tree = tree_point;
								}
								/* If both points are equal in there y component, and max x coordinate, arbitrarily choose the new point.*/
								else if (coords[0][shortest] == coords[0][point]){
									shortest = point;
									shortest_tree = tree_point;
								}	
							}
						}
					}	
				}
				/* Put the connection into the file.*/
				fprintf(file, "%d %d %d\n", shortest + 1, shortest_tree + 1, low_distance);
				/* Move the connected point from being not in the tree to being in the tree.*/
				in_tree[shortest] = shortest + 1;
				not_tree[shortest - 1] = 0;
				/* Increase the tree length and increment the points placed.*/
				tree_length += low_distance;
				placed += 1;
			}
			/* Put the total weight of the MST into the file.*/
			fprintf(file, "# total weight of the MST by Prim's algorithm is %d.\n", tree_length);
			/* Close the file/*/
			fclose(file);
		}
	}
	return 0;
}

int max(int num1, int num2) {
	/* Return the larger of two integers*/
	if (num1 > num2) return num1;
	else return num2;
}
