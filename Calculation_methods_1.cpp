#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>



using namespace std;

double S(double array_x[], double array_y[], double array_m[], double x, double h) {
	int i = 0;
	while (x > array_x[i])
		i++;

	double x1 = array_x[i - 1];
	double y1 = array_y[i - 1];
	double x2 = array_x[i];
	double y2 = array_y[i];

	double S = ((x2 - x) * (x2 - x) * (x2 - x) - h * h * (x2 - x)) * array_m[i - 1] / (6 * h) +
			   ((x - x1) * (x - x1) * (x - x1) - h * h * (x - x1)) * array_m[i] / (6 * h) +
		       (x2 - x) * y1 / h + (x - x1) * y2 / h;

	return S;
}



int exponent(double exp) {
	if (exp < 1.5) return 1;
	else if (exp < 3) return 2;
	else if (exp < 6) return 4;
	else if (exp < 12) return 8;
	else return 16;
}



int main() {
	setlocale(LC_ALL, "Russian");

	cout << "n\t" << "d_max\t\t" << "d_оц\t\t" << "K_d\n";
	for (int i = 0; i < 50; i++) cout << "-";
	cout << "\n";



	double* array_d_max = new double[0];

	double prev_d_max = 0;
	double d_max_n = 0;
	double d_ev = 0;
	double K_d = 0;



	for (int count_segments = 5; count_segments < 11000; count_segments *= 2) {
		double h = M_PI / count_segments;



		double* array_x = new double[count_segments + 1];
		double* array_y = new double[count_segments + 1];

		double* array_a = new double[count_segments + 1];
		double* array_b = new double[count_segments + 1];
		double* array_c = new double[count_segments + 1];
		double* array_d = new double[count_segments + 1];

		double* array_l = new double[count_segments + 1];
		double* array_w = new double[count_segments + 1];

		double* array_m = new double[count_segments + 1];

		

		for (int i = 0; i <= count_segments; i++) {
			array_x[i] = h * i;
			array_y[i] = sin(h * i);
		}



		array_a[0] = array_a[count_segments] = 1;
		array_b[0] = 0;
		array_c[0] = 0;
		array_d[0] = array_d[count_segments] = 0;

		for (int i = 1; i < count_segments; i++) {
			array_a[i] = 2 * h / 3;
			array_b[i] = h / 6;
			array_c[i] = h / 6;
			array_d[i] = (array_y[i + 1] - 2 * array_y[i] + array_y[i - 1]) / h;
		}



		array_l[0] = -array_b[0] / array_a[0];
		array_w[0] = array_d[0] / array_a[0];

		for (int i = 1; i < count_segments; i++) {
			array_l[i] = -array_b[i] / (array_a[i] + array_c[i] * array_l[i - 1]);
			array_w[i] = (array_d[i] - array_c[i] * array_w[i - 1]) / (array_a[i] + array_c[i] * array_l[i - 1]);
		}



		array_m[0] = array_m[count_segments] = 0;

		for (int i = count_segments - 1; i > 0; i--)
			array_m[i] = array_w[i] + array_l[i] * array_m[i + 1];



		if (prev_d_max != 0) {
			d_max_n = 0;

			for (int n = 1; n < count_segments; n += 2) {
				int m = (n - 1) / 2;

				double x = h * n;

				double s = S(array_x, array_y, array_m, x, h);

				if (abs(s - array_d_max[m]) > d_max_n)
					d_max_n = abs(s - array_d_max[m]);
			}
		}

		delete[] array_d_max;
		double* new_array_d_max = new double[count_segments];
		array_d_max = new_array_d_max;



		double d_max = 0;

		for (int n = 0; n < count_segments; n++) {
			double x = h * n + h / 2;

			double s = S(array_x, array_y, array_m, x, h);

			array_d_max[n] = s;

			if (abs(s - sin(x)) > d_max)
				d_max = abs(s - sin(x));
		}



		if (prev_d_max != 0) {
			K_d = prev_d_max / d_max;

			d_ev = d_max_n / exponent(K_d);
		}

		prev_d_max = d_max;



		if (d_ev != 0)
			cout << count_segments << "\t" << d_max << "\t" << d_ev << "\t" << K_d << "\n";
		else
			cout << count_segments << "\t" << d_max << "\t" << "-\t\t" << "-\n";



		delete[] array_x;
		delete[] array_y;

		delete[] array_a;
		delete[] array_b;
		delete[] array_c;
		delete[] array_d;

		delete[] array_l;
		delete[] array_w;

		delete[] array_m;
	}
}
