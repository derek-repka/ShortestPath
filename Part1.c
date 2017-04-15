/* This program checks files for valid instances of an RST problem, or creates an instance. When launched with a given file after the parameter "-i", the file is checked to ensure that it is a valid file. If no errors are encountered, the program prints the test file to stdout. If launched with an output file following the paramter "-o", the program does as above, but prints the test file to the target output file, if it is valid, or reports an error to stdout if one is encountered. If no parameters are given, the program instead generates valid instances of an RST problem given input from the user. When checking the input files for validity, it is assumed they are in the format specified in the assignment outline.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main(int argc, char *argv[])
{
	/* Initiate all the necessarily variables.*/
    FILE *file;
    int max_x, max_y, points, i, j, k, instances;
    int *x_coords, *y_coords;
    int infile = 0, outfile = 0, coordinates = 0;
    char filename[64];
    /* Check for the correct number of arguments.*/
    if ((argc % 2) != 0){
		for (i = 1; i < argc; i++) {
			/* Locations of input and output file names are determined.*/
	    	if (strcmp (argv[i], "-i") == 0)
				infile = i+1;
	    	else if (strcmp (argv[i], "-o") == 0)
				outfile = i+1;
		}
    }
    
    /* Check if there are errors in the parameters entered.*/
    else if (argc % 2 == 0){
    	printf("Mismatched parameters. Ensure the parameters are in the correct format.\n");
    	return 0;
    }
    
    /* If there is an input file, it is read and checked for errors.*/
    if (infile != 0){
        file = fopen(argv[infile], "r");
		if (file == NULL)
		/* If the file fails to open, report an error.*/
		{
	    	printf("Error opening file!\n");
	    	return 0;
		}

		while (fscanf(file, "%d", &max_x) != 1)
			/* Skip lines until max_x and max_y is reached.*/
	    	fscanf(file, "%*[^\n]");
	    /* Store the value for max_y.*/
		fscanf(file, "%d", &max_y);
		
		while (fscanf(file, "%d", &points) != 1)
			/* Skip lines until NUM_PT is reached, and then store it.*/
	    	fscanf(file, "%*[^\n]");

		/* If any there are more points then possible unique coordinates, report an error.*/
		if (points > ((max_x + 1) * (max_y + 1))){
	    	printf("%d unique points cannot fit in a [0,%d]x[0,%d] area. This is not a valid instance.\n", points, max_x, max_y);
	    	return 0;
		}
		
		/* Allocate the necessary memory for storing the coordinates.*/
		x_coords = (int *)malloc(points * 4);
		y_coords = (int *)malloc(points * 4);
		/* Locates the first instance of coordinates and then stores them.*/
	    while ( fscanf(file, "%d", &x_coords[0]) != 1)
	    	fscanf(file, "%*[^\n]");
	    fscanf(file, "%d", &y_coords[0]);
	    coordinates ++;
	    /* If either the x or y coordinate is greater than the maximum, report an error.*/
	    if (x_coords[0] > max_x || y_coords[0] > max_y){
			printf("At least one coordinate in the area defined is invalid. This is not a valid instance.\n");
			return 0;
	    }
	    
	    for (i = 1; i < points; i++){
	    	if (fscanf(file, "%d", &x_coords[i]) != 1){
	    		printf("Not enough consecutive coordinate pairs in specified file. This is not a valid instance.\n");
	    		return 0;
	    	}
			fscanf(file, "%d", &y_coords[i]);

	    	/* If either the x or y coordinate is greater than the maximum, report an error.*/
	    	if (x_coords[i] > max_x || y_coords[i] > max_y){
				printf("At least one coordinate in the area defined is invalid. This is not a valid instance.\n");
				return 0;
	    	}
	    	else
				coordinates++;
		}
		
		/* Check if there are more coordinates than NUM_PT.*/
		if (fscanf(file, "%d", &x_coords[i]) == 1){
			fscanf(file, "%d", &y_coords[i]);
			coordinates++;
		}
			
		/* Check if the correct number of coordinates were entered.*/
		if (coordinates != points){
	    	printf("Too many coordinate pairs in specified file. This is not a valid instance.\n");
	    	return 0;
		}
		if (outfile == 0){
			/* Print to stdout. */
			printf("#This is a legal instance\n");
        	printf("-------------------------------------\n");
			printf("#%s\n", argv[infile]);
			printf("#area [0, MAX_X] x [0, MAX_Y]\n");
			printf("%d\t%d\n", max_x, max_y);
			printf("#number of points NUM_PT\n");
			printf("%d\n", points);
			printf("#coordinates\n");
			for (i = 0; i < points; i++)
	    		printf("%d\t%d\n", x_coords[i], y_coords[i]);
			printf("#end of instance\n");
		}
		else if (outfile != 0){
			/* Print to the output file specified. */
			fclose(file);
			file = fopen(argv[outfile], "w");
			if (file == NULL){
				printf("Error opening output file.");
				return 0;
			}
			else
			{
				fprintf(file, "#This is a legal instance\n");
        		fprintf(file, "-------------------------------------\n");
				fprintf(file, "#%s\n", argv[infile]);
				fprintf(file, "#area [0, MAX_X] x [0, MAX_Y]\n");
				fprintf(file, "%d\t%d\n", max_x, max_y);
				fprintf(file, "#number of points NUM_PT\n");
				fprintf(file, "%d\n", points);
				fprintf(file, "#coordinates\n");
				for (i = 0; i < points; i++)
					/* Print the coordinates to stdout. */
	    			fprintf(file, "%d\t%d\n", x_coords[i], y_coords[i]);
				fprintf(file, "#end of instance\n");
			}
		}
		
		/* Free the memory and close the file.*/	
		free(x_coords);
    	free(y_coords);
		fclose(file);
    }


    else if (argc == 1){
    	/* Get the required parameters from stdin.*/
        printf("Generating Random Instances...\n");
		printf("Enter the circuit board size MAX_X MAX_Y seperated by a space: ");
		scanf("%d %d", &max_x, &max_y);

		printf("Enter the number of points: ");
		scanf("%d", &points);
		
		/* If any there are more points then possible unique coordinates, report an error.*/
		if (points > ((max_x + 1) * (max_y + 1))){
	    	printf("%d unique points cannot fit in a [0,%d]x[0,%d] area. This is not a valid instance.\n", points, max_x, max_y);
	    	return 0;
		}

		printf("Enter the number of random instances to be generated: ");
		scanf("%d", &instances);

		/* Initalize the number generator. */
		srand(time(NULL));
		for (k = 10; k < 110; k += 10){
			for (i = 0; i < instances; i++)
			{
				/* Print the data to each instance.*/
				sprintf(filename, "instance%d_%d.txt", k, i);
				file = fopen(filename, "w");
				fprintf(file, "#instance%d_%d.txt\n", k, i);
				fprintf(file, "#area [0, MAX_X] x [0, MAX_Y]\n");
				fprintf(file, "%d\t%d\n", max_x, max_y);
				fprintf(file, "#number of points NUM_PT\n");
				fprintf(file, "%d\n", k);
				fprintf(file, "#coordinates\n");
				for (j = 0; j < k; j++)
					/* Generate the random numbers and add them to the output file.*/
				    fprintf(file, "%-4d\t%-4d\n", rand()%(max_x + 1), rand()%(max_y + 1));
				fprintf(file, "#end of instance\n");
				fclose(file);
			}
		}
    }
    return 0;	
}
