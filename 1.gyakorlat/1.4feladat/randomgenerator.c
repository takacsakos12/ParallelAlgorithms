#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
srand((unsigned int)time(NULL));

printf("5 Random Floating-Point Numbers between 500 and 1000:\n");

for (int i = 0; i < 5; i++) {
float random_number = 500 +((float)rand() / (float)RAND_MAX) * (1000-500);
printf("%f\n", random_number);
}

return 0;
}