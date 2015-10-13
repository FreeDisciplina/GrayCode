#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef long int int32;
typedef long long int int64;
typedef long double double64;

#define BASE 3
#define SIZE_IN_BIT (1ULL<<10ULL)
#define SIZE_IN_BYTE (SIZE_IN_BIT>>3ULL)

#define N 8

void split(int32 a[])
{
	ofstream fout("enum.txt");

	int32 a1[N], a2[N], d[N], f[N+1], x[N], j;

	int32 count = 0;

	for (int32 i = 0; i < N; i++)
	{
		if (a[i] >= 1)
		{
			x[count] = i; count++;
		}
		a1[i] = 0; a2[i] = a[i];
		d[i] = 1; f[i] = i;
	} f[N] = N;
	j = 0;

	// The first case: do something
	for (int32 i = 0; i < N; i++)
	{
		fout << setw(4) << a1[i] << ",";
	}
	fout << endl;

	if (count > 0)
	{
		while(true)
		{
			j = f[0]; f[0] = 0;
			if (j == count) break;
			else
			{
				a1[x[j]] = a1[x[j]] + d[j];
				a2[x[j]] = a[x[j]] - a1[x[j]];
			}
			if ((a1[x[j]] == 0) || (a1[x[j]] == a[x[j]]))
			{
				d[j] = -d[j];	f[j] = f[j+1];	f[j+1] = j+1;
			}

			// Other cases: do something
			for (int32 i = 0; i < N; i++)
			{
				fout << setw(4) << a1[i] << ",";
			}
			fout << endl;
		}
	}

	fout.close();
}

#define set1(A, idx) (A[(idx)>>3] |= ((u8)1 << (u8)((idx)&0x7)))
#define set0(A, idx) (A[(idx)>>3] &= (~((u8)1 << (u8)((idx)&0x7))))

inline int32 A2I(u8 A[], int32 d)
{
	int32 idx = 0;

	for (int32 i = 0; i < d; i++)
	{
		idx += A[i] * (int32)pow((float)BASE, (float)i);
	}
	return idx;
}

int32 I2A(u8 A[], int32 idx)
{
	for (int32 i = 0; i < N; i++)
	{
		A[i] = 0;
	}

	int32 d = (int32)floor(log((double)idx)/log((double)BASE)) + 1;

	int32 i = d - 1;
	while ( i >= 0)
	{
		int32 p = (int32)pow((float)BASE, (float)(i));
		A[i] = idx / p;
		idx = idx % p;
		i = (int32)floor(log((double)idx)/log((double)BASE));
	}

	return d;
}

void printA(ofstream &file, u8 A[], int32 d)
{
	for (int32 i = 0; i < d; i++)
	{
		file
			<< ((A[i]>>0)&1)
			<< ((A[i]>>1)&1)
			<< ((A[i]>>2)&1)
			<< ((A[i]>>3)&1)
			<< ((A[i]>>4)&1)
			<< ((A[i]>>5)&1)
			<< ((A[i]>>6)&1)
			<< ((A[i]>>7)&1) << " ";
	}
	file << endl;
}

int main()
{
	int32 a[N];
	for (int32 i = 0; i < N; i++)
	{
		a[i] = i;
	}
	split(a);

	u8 * idcArray = new u8[SIZE_IN_BYTE];
	u64 sum = 0ULL;

	idcArray[0] = 0;
	idcArray[1] = 1;
	for (u64 i = 2; i < SIZE_IN_BYTE; i++)
	{
		idcArray[i] = 0;
		//idcArray[i] = idcArray[i - 1] + idcArray[i - 2];
		//sum += idcArray[i];
	}

	ofstream fout;
	fout.open("Array.txt");

	printA(fout, idcArray, SIZE_IN_BYTE);

	set1(idcArray, 2); set1(idcArray, 64);
	printA(fout, idcArray, SIZE_IN_BYTE);

	set0(idcArray, 2); set0(idcArray, 64);
	printA(fout, idcArray, SIZE_IN_BYTE);


	int32 idx = 96;
	u8 Ax[N];
	int32 d = I2A(Ax, idx);
	for (int32 i = 0; i < d; i++)
	{
		fout << Ax[i] + '\0' << ",";
	}
	fout << endl;

	fout << "Idx = " << idx << "  Base = " << BASE  << "  d = " << d << endl;
	fout << "I2A(Ax, idx): Ax = ";
	for (int32 i = 0; i < d; i++)
	{
		fout << Ax[i] + '\0' << ",";
	}
	fout << endl;
	fout << "A2I(A, idx): idx = " << A2I(Ax, d);

	fout.close();
	
	cout << sum << endl;
	delete [] idcArray;

	return 0;
}