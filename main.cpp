#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
using namespace std;

const int n = 10000;

// Structure to hold regression results
struct RegressionResults {
	float slope;
	float yIntercept;
	float xIntercept;
	float inverseSlope;
	int numXValues;
	int maxYReplicates;
	int totalValues;
	int missingValues;
};

double regressionLineCPP(RegressionResults* results = nullptr){

	// Create and populate coordinate arrays
	int x[n];
	int y[n];

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		x[i] = rand() % 10;
		y[i] = rand() % 10;
	}
	
	// Variable declarations for statistical calculations
	float meanY, meanX, covarianceXY, varianceX;
	// Initialize all statistical accumulators to zero
	meanX = meanY = covarianceXY = varianceX = 0.0;

	int startTime = clock();

	// Calculate sums for statistical measures
	for (int i = 0; i < n; i++)	{
		// Accumulate sum for mean of X
		meanX += x[i];
		// Accumulate sum for mean of Y
		meanY += y[i];
		// Accumulate sum of squares for variance of X
		varianceX += x[i] * x[i];
		// Accumulate sum of products for covariance of XY
		covarianceXY += x[i] * y[i];
	}
	// Compute means by dividing sums by sample size
	meanX /= n;
	meanY /= n;

	// Calculate variance: Var(X) = E[X²] - E[X]²
	varianceX = (varianceX / n) - (meanX * meanX);
	// Calculate covariance: Cov(X,Y) = E[XY] - E[X]E[Y]
	covarianceXY = (covarianceXY / n) - (meanX * meanY);

	// REGRESSION LINE FORMULA: y - meanY = (covarianceXY / varianceX)*(x - meanX)
	//                          y = slope*x + intercept
	float slope = covarianceXY / varianceX;
	float intercept = ((covarianceXY / varianceX) * (-meanX)) + meanY;

	// Store results if requested
	if (results != nullptr) {
		results->slope = slope;
		results->yIntercept = intercept;
		results->xIntercept = -intercept / slope; // When Y=0, X = -intercept/slope
		results->inverseSlope = 1.0f / slope;
		results->numXValues = n;
		results->maxYReplicates = 1;
		results->totalValues = n;
		results->missingValues = 0;
	}

	int endTime = clock();
	return (double(endTime - startTime) / ((clock_t)1000));
}


double regressionLineAssembly() {
	
	// Create and populate coordinate arrays
	int xCoordinates[n];
	int yCoordinates[n];

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		xCoordinates[i] = rand() % 10;
		yCoordinates[i] = rand() % 10;
	}
	
	int sumX, sumY, sumXSquared, sumXY;
	sumX = sumY = sumXSquared = sumXY = 0;

	float tempValue, meanX, meanY, varianceX, covarianceXY, slope, intercept;
	tempValue = meanX = meanY = varianceX = covarianceXY = slope = intercept = 0.0;

	clock_t startTime = clock();
	__asm {
		
		initialize_loop:
			// Initialize array index counter
			mov esi, 0;

		accumulation_loop:
			// Check if counter >= n -> terminate
			cmp esi, 10000;
			jae compute_statistics;

			// Accumulate sumX with x[i]
			mov edx, [xCoordinates + 4*esi];
			mov eax, sumX;
			add eax, edx;
			mov sumX, eax;

			// Accumulate sumY with y[i]
			mov ebx, [yCoordinates + 4 * esi];
			mov eax, sumY;
			add eax, ebx;
			mov sumY, eax;

			// Accumulate sumXSquared with x[i] * x[i]
			mov eax, edx;
			mul edx;	// After multiplication edx resets to 0
			mov ecx, sumXSquared;
			add eax, ecx;
			mov sumXSquared, eax;
			
			// Accumulate sumXY with x[i] * y[i]
			mov edx, [xCoordinates + 4 * esi];
			mov eax, edx;
			mul ebx;
			mov ecx, sumXY;
			add eax, ecx;
			mov sumXY, eax;

			// Increment loop counter
			inc esi;

			// Unconditional jump to next iteration
			jmp accumulation_loop;

		compute_statistics:
			// Load integer (converts to float) onto FPU stack top
			fild sumX;
			// Divide stack top by 'n' to compute mean
			fidiv n;
			// Store stack top to memory location
			fstp meanX;

			fild sumY;
			fidiv n;
			fstp meanY;

			// VARIANCE = (sumXSquared / n) - (meanX * meanX)
			fild sumXSquared;
			fidiv n;
			fstp varianceX;

			// tempValue = (meanX * meanX)
			fld meanX;
			fmul meanX;
			fstp tempValue;

			fld varianceX;
			fsub tempValue;
			fstp varianceX;

			// COVARIANCE = (sumXY / n) - (meanX * meanY)
			fild sumXY;
			fidiv n;
			fstp covarianceXY;

			// tempValue = (meanX * meanY)
			fld meanX;
			fmul meanY;
			fstp tempValue;

			fld covarianceXY;
			fsub tempValue;
			fstp covarianceXY;

			// slope = covarianceXY / varianceX
			fld covarianceXY;
			fdiv varianceX;
			fstp slope;

			// intercept = ((covarianceXY / varianceX) * (-meanX)) + meanY
			fld intercept;
			fsub meanX;
			fmul covarianceXY;
			fdiv varianceX;
			fadd meanY;
			fstp intercept;
	}
	
	
	clock_t endTime = clock();

	return (double(endTime - startTime) / ((clock_t)1000));
}

double regressionLineSSE() {
	int *xCoordinates = new int[n];
	int *yCoordinates = new int[n];

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		xCoordinates[i] = rand() % 10;
		yCoordinates[i] = rand() % 10;
	}

	

	int sumX, sumY, sumXSquared, sumXY;
	sumX = sumY = sumXSquared = sumXY = 0;

	float tempValue, meanX, meanY, varianceX, covarianceXY, slope, intercept;
	tempValue = meanX = meanY = varianceX = covarianceXY = slope = intercept = 0.0;

	int vectorIterations = n / 4;  // Process 4 elements per SIMD iteration
	clock_t startTime = clock();
	__asm {
	initialize_registers:
			mov ebx, 0;
			mov edx, 0;
			mov ecx, 0;
			mov eax, 0;
			mov esi, xCoordinates;
			mov edi, yCoordinates;

		simd_loop:
			cmp ecx, vectorIterations;
			jae reduce_accumulations;
			
			// Load and accumulate X values into xmm2
			movdqu xmm0, [esi]; 
			paddd xmm2, xmm0;

			// Load and accumulate Y values into xmm3
			movdqu xmm1, [edi];
			paddd xmm3, xmm1;

			// Compute X² and accumulate into xmm4
			pmulld xmm0, xmm0;
			paddd xmm4, xmm0;

			movdqu xmm0, [esi]; // RELOAD X values
			// Compute X*Y and accumulate into xmm5
			pmulld xmm1, xmm0;
			paddd xmm5, xmm1;

			add esi, 16;  // Advance X pointer by 16 bytes (4 ints)
			add edi, 16;  // Advance Y pointer by 16 bytes (4 ints)
			
			inc ecx;
			jmp simd_loop;

		reduce_accumulations:
			// Horizontal reduction of sumX accumulator
			phaddd xmm2, xmm2;
			phaddd xmm2, xmm2;
			movdqu sumX, xmm2;
			mov ebx, sumX;

			// Horizontal reduction of sumY accumulator
			phaddd xmm3, xmm3;
			phaddd xmm3, xmm3;
			movdqu sumY, xmm3;
			mov edx, sumY;

			// Horizontal reduction of sumXSquared accumulator
			phaddd xmm4, xmm4;
			phaddd xmm4, xmm4;
			movdqu sumXSquared, xmm4;
			mov eax, sumXSquared;

			// Horizontal reduction of sumXY accumulator
			phaddd xmm5, xmm5;
			phaddd xmm5, xmm5;
			movdqu sumXY, xmm5;
			mov ecx, sumXY;

			mov sumX, ebx;
			mov sumY, edx;
			mov sumXSquared, eax;
			mov sumXY, ecx;

			// Load integer (converts to float) onto FPU stack top
			fild sumX;
			// Divide stack top by 'n' to compute mean
			fidiv n;
			// Store stack top to memory location
			fstp meanX;

			fild sumY;
			fidiv n;
			fstp meanY;

			// VARIANCE = (sumXSquared / n) - (meanX * meanX)
			fild sumXSquared;
			fidiv n;
			fstp varianceX;

			// tempValue = (meanX * meanX)
			fld meanX;
			fmul meanX;
			fstp tempValue;

			fld varianceX;
			fsub tempValue;
			fstp varianceX;

			// COVARIANCE = (sumXY / n) - (meanX * meanY)
			fild sumXY;
			fidiv n;
			fstp covarianceXY;

			// tempValue = (meanX * meanY)
			fld meanX;
			fmul meanY;
			fstp tempValue;

			fld covarianceXY;
			fsub tempValue;
			fstp covarianceXY;

			// slope = covarianceXY / varianceX
			fld covarianceXY;
			fdiv varianceX;
			fstp slope;

			// intercept = ((covarianceXY / varianceX) * (-meanX)) + meanY
			fld intercept;
			fsub meanX;
			fmul covarianceXY;
			fdiv varianceX;
			fadd meanY;
			fstp intercept;
	}
	clock_t endTime = clock();

	return (double(endTime - startTime) / ((clock_t)1000));
}

void displayBenchmarkResults(double cppTime1000, double asmTime1000, double sseTime1000, 
                             double cppTime3000, double asmTime3000, double sseTime3000) {
	cout << "> EXECUTION TIME RESULTS TABLE" << endl
		<< "\t| C++ Time\t| Assembly Time      | SSE Time" << endl
		<< "--------|---------------|--------------------|------------" << endl
		<< "1000 it.| " << cppTime1000 << "\t\t| " << asmTime1000 << "\t\t     | " << sseTime1000 << endl
		<< "--------|---------------|--------------------|------------" << endl
		<< "3000 it.| " << cppTime3000 << "\t\t| " << asmTime3000 << "\t\t     | " << sseTime3000 << endl;
}

void displayRegressionResults(const RegressionResults& results) {
	cout << "\n##########################################################" << endl
		 << "REGRESSION LINE RESULTS" << endl
		 << "##########################################################" << endl
		 << "Best-fit values\tPerfect line" << endl
		 << "Slope\t" << results.slope << endl
		 << "Y-intercept\t" << results.yIntercept << endl
		 << "X-intercept\t" << results.xIntercept << endl
		 << "1/Slope\t" << results.inverseSlope << endl
		 << "Equation\tY = " << results.slope << "*X + " << results.yIntercept << endl
		 << "Data\t" << endl
		 << "Number of X values\t" << results.numXValues << endl
		 << "Maximum number of Y replicates\t" << results.maxYReplicates << endl
		 << "Total number of values\t" << results.totalValues << endl
		 << "Number of missing values\t" << results.missingValues << endl
		 << "##########################################################" << endl;
}

int main() {

	cout << "##########################################################" << endl
		 << "REDUCED BENCHMARK - Linear Regression Line" << endl
		 << "##########################################################" << endl;

	double cppTotalTime1000 = 0;
	double asmTotalTime1000 = 0;
	double sseTotalTime1000 = 0;

	int progressPercentage = 10;

	cout << "TEST 1: Random point cloud (1000 iterations)" << endl
		 << "EXECUTION: [";
	
	// First benchmark loop: 1000 iterations
	for (int i = 1; i <= 1000; i++) {
		// Accumulate execution times from each iteration
		cppTotalTime1000 += regressionLineCPP();
		asmTotalTime1000 += regressionLineAssembly();
		sseTotalTime1000 += regressionLineSSE();

		if (progressPercentage * 1000 / 100 == i) {
			cout << "=";
			progressPercentage += 10;
		}

	}
	cout << "]" << endl
		 << "----------------------------------------------------------" << endl
		 << "TEST 2: Random point cloud (3000 iterations)" << endl
		 << "EXECUTION: [";

	double cppTotalTime3000 = 0;
	double asmTotalTime3000 = 0;
	double sseTotalTime3000 = 0;

	progressPercentage = 10;

	// Second benchmark loop: 3000 iterations
	for (int i = 1; i <= 3000; i++) {
		cppTotalTime3000 += regressionLineCPP();
		asmTotalTime3000 += regressionLineAssembly();
		sseTotalTime3000 += regressionLineSSE();

		if (progressPercentage * 3000 / 100 == i) {
			cout << "=";
			progressPercentage += 10;
		}
	}
	cout << "]" << endl;
	cout << "##########################################################" << endl;
	

	displayBenchmarkResults(cppTotalTime1000, asmTotalTime1000, sseTotalTime1000, 
	                        cppTotalTime3000, asmTotalTime3000, sseTotalTime3000);
	
	// Calculate and display regression results from a single run
	RegressionResults results;
	regressionLineCPP(&results);
	displayRegressionResults(results);
	
	// Keep console window open
	system("pause");

	return 0;
}

