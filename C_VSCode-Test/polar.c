#include <stdio.h>
#include <math.h>

int main()
{
	float r;
	float theta;

	printf("Please enter the distance(in meters) and the angle(in degrees). \n");
	scanf("%f", &r);
	scanf("%f", &theta);

	if (r < 0)
	{
		printf("Incorrect distance.\n"); //reject negative distance
	}
	else
	{
		float x = r * cos(theta / 180 * M_PI);
		float y = r * sin(theta / 180 * M_PI);
		printf("Your rectangular coordinate is ( %f , %f ). \n", x, y);
	}
	return 0;
}
