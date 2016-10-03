#pragma once

class BezierCurve
{
private:
	double FactorialLookup[33];
	double factorial(int n);
	void CreateFactorialTable();
	double Ni(int n, int i);
	double Bernstein(int n, int i, double t);

public:
	inline BezierCurve()
	{
		CreateFactorialTable();
	}
	
	void Bezier2D(double b[], int lengthOfB, int cpts, double p[]);
};