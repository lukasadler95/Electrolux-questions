#include <stdio.h>
#include <stdlib.h>

void analyzeArray(int *array, int n, double *average, int *maximum, int *minimum, int **evenArray, int *evenSize) {
    int sum = 0;
    *maximum = array[0];
    *minimum = array[0];
    *evenSize = 0;

    for (int i = 0; i < n; i++) {
        sum += array[i];

        if (array[i] > *maximum) {
            *maximum = array[i];
        }

        if (array[i] < *minimum) {
            *minimum = array[i];
        }

        if (array[i] % 2 == 0) {
            (*evenSize)++;
        }
    }

    *average = (double)sum / n;

    *evenArray = (int *)malloc(*evenSize * sizeof(int));
    if (*evenArray == NULL) {
        printf("Error allocating memory.\n");
        return;
    }

    int j = 0;
    for (int i = 0; i < n; i++) {
        if (array[i] % 2 == 0) {
            (*evenArray)[j] = array[i];
            j++;
        }
    }
}

int readPositiveInteger(double m) {
    double y = m;

    while (1) {
        if (y == (int)y && y > 0) {
            return (int)y;
        } else {
            printf("Invalid input. The number must be a positive and integer: ");
            scanf("%lf", &y);
            while (getchar() != '\n');
        }
    }
}

int main() {
    int n;
    double element, m;

    printf("Enter the size of the array (a positive integer): ");
    scanf("%lf", &m);
    n = readPositiveInteger(m);

    int *array = (int *)malloc(n * sizeof(int));
    if (array == NULL) {
        printf("Error allocating memory for the array.\n");
        return 1;
    }

    printf("Enter the elements of the array:\n");
    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i + 1);
        scanf("%lf", &element);
        array[i] = readPositiveInteger(element);
    }

    double average;
    int maximum, minimum, *evenArray, evenSize;

    analyzeArray(array, n, &average, &maximum, &minimum, &evenArray, &evenSize);

    printf("\nAverage: %.2f\n", average);
    printf("Maximum: %d\n", maximum);
    printf("Minimum: %d\n", minimum);
    printf("Even Numbers Array: ");
    for (int i = 0; i < evenSize; i++) {
        printf("%d ", evenArray[i]);
    }
    printf("\nSize of the new even array: %d\n", evenSize);

    free(array);
    free(evenArray);

    return 0;
}
