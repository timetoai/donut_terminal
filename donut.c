#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

int main()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    const int window_width = w.ws_col, window_height = w.ws_row;

    const int R1 = 1, R2 = 2, K2 = 5;
    const double theta_spacing = 0.07, phi_spacing = 0.02, A_spacing = 0.01, B_spacing = 0.01;
    double K1 = 1.1 * 3 * K2 / (8 * (R1 + R2)), A = 0, B = 0;
    if (window_height < window_width) 
    	{K1 *= window_height;}
    else 
    	{K1 *= window_width;}
    int xp, yp, l_ind;
    char lights[12] = ".,-~:;=!*#$@";
    char arr[window_height][window_width];
    double z_buffer[window_height][window_width];

	// main routine
	while (1)
	{
		double sin_A = sin(A), cos_A = cos(A), sin_B = sin(B), cos_B = cos(B);
		
		// clearing
	    for (int i = 0; i < window_height; i++)
	    {
	    	for (int j = 0; j < window_width; j++)
	    	{
	    		z_buffer[i][j] = 0;
	    		arr[i][j] = ' ';
	    	}
	    }

	    // calculating
		for (double phi = 0; phi < 2 * M_PI; phi += phi_spacing)
		{
			double sin_phi = sin(phi), cos_phi = cos(phi);
			for (double theta = 0; theta < 2 * M_PI; theta += theta_spacing)
			{
				double sin_theta = sin(theta), cos_theta = cos(theta);

				double helper1 = R2 + R1 * cos_theta, helper2 = R1 * sin_theta;

				double x = helper1 * (cos_B * cos_phi + sin_A * sin_B * sin_phi) - helper2 * cos_A * sin_B;
				double y = helper1 * (cos_phi * sin_B - cos_B * sin_A * sin_phi) + helper2 * cos_A * cos_B;
				double z = cos_A * helper1 * sin_phi + helper2 * sin_A;
				double zp = z + K2;
				double rev_z = 1 / zp;

				xp = (int)(window_width / 2 + K1 * x * rev_z);
				yp = (int)(window_height / 2 - K1 * y * rev_z);
				double L = cos_phi * cos_theta * sin_B - cos_A * cos_theta * sin_phi - sin_A * sin_theta + cos_B * (cos_A * sin_theta - cos_theta * sin_A * sin_phi);
				// L in range(- sqrt(2), sqrt(2))
				if (L > 0 && rev_z > z_buffer[yp][xp])
				{
					z_buffer[yp][xp] = rev_z;
					l_ind = floor(L * 8);
					arr[yp][xp] = lights[l_ind];
				}
			}
		}

		// outputing graphics
		printf("\x1b[H");
		for (int i = 0; i < window_height; i++)
		{
			for (int j = 0; j < window_width; j++)
				{putchar(arr[i][j]);}
			printf("\n");
		}
		//sleep(0.1);

		A += A_spacing;
		if (A > 2 * M_PI)
			{A -= 2 * M_PI;}
		B += B_spacing;
		if (B > 2 * M_PI)
			{B -= 2 * M_PI;}
	}

    return 0;
}