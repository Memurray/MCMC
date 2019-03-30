#include <time.h>
#include <iostream>

using namespace std;
//All conditional probabilities tables interpretted as an array of doubles mapped to index by 
//the binary number associated with T=1 F=0 at that spatial position
//B Table is ACEF thus if all true this is 1111 or 15 in binary, thus index 15
const double BLANKETA[4] = {(double)224/233, (double)112/139, (double)14/15, (double)28/55}; //

const double BLANKETB[16] = { (double)16/17, (double)1/2, (double)8/11,(double)1/7,
							  (double)20/23, (double)20/27, (double)10/19, (double)10/31, 
							  (double)9/10, (double)9/25, (double)2/5, (double)1/25,
							  (double)15/19, (double) 45/73, (double)5/23, (double)5/47};

const double BLANKETC[4] =  { (double)3/11,(double) 7/39,(double) 5/37,(double) 5/13};
const double BLANKETE[8] =  { (double)18/19,(double)9/13,(double)3/4,(double)3/11,
							  (double)3/4,(double)3/11,(double)2/11,(double)1/37};
const double BLANKETF[8]=	{ (double) 8/9,(double) 1/2,(double) 14/17,(double) 7/19,
							  (double) 1/3,(double)1/17,(double)2/3,(double)1/5 };
		

//takes a value from 0-1 and returns 1 (used as true) when random value from (0-1) is <= input
//.2 should return 1  20% of the time
int biasedGen(double breakpoint) {
	double result = double(rand()) / (double(RAND_MAX) + 1.0);
	if(result <= breakpoint)
		return 1;
	return 0;
}

//forms index value based on binary conversion of related assignment, maps to location in conditional prob table
void evalA(int assignment[]) {
	int index = assignment[1]*2 + assignment[3];  //map relevant assignments to CP table
	assignment[0] = biasedGen(BLANKETA[index]);  //new value of A = return (0 or 1) from biased gen using CP table lookup as bias
}

void evalB(int assignment[]) {
	int index = assignment[0] * 8 + assignment[2] * 4 + assignment[3] * 2 + assignment[4]; //map relevant assignments to CP table
	assignment[1] = biasedGen(BLANKETB[index]); //new value of B = return (0 or 1) from biased gen using CP table lookup as bias
}

void evalC(int assignment[]) {
	int index = assignment[1] * 2 + assignment[4];//map relevant assignments to CP table
	assignment[2] = biasedGen(BLANKETC[index]); //new value of C = return (0 or 1) from biased gen using CP table lookup as bias
}

void evalE(int assignment[]) {
	int index = assignment[0] * 4 + assignment[1] * 2 + assignment[4];//map relevant assignments to CP table
	assignment[3] = biasedGen(BLANKETE[index]);//new value of E = return (0 or 1) from biased gen using CP table lookup as bias
}

void evalF(int assignment[]) {
	int index = assignment[1] * 4 + assignment[2] * 2 + assignment[3];//map relevant assignments to CP table
	assignment[4] = biasedGen(BLANKETF[index]);//new value of F = return (0 or 1) from biased gen using CP table lookup as bias
}

void printArray(int array[]) {  //only used for debugging for printing 5 element array to screen
	for (int i = 0; i < 5; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}

void genAssignment(int assignment[]) {  //create the base assignment 
	for (int i = 0; i < 5; i++) {
		assignment[i] = biasedGen(.5);  //technically just want it to be 0 half the time and 1 half the time so why not just reuse the biased function with .5 as arg
	}
}

int main(){
	srand(time(NULL));
	int assignment[5];	
	//We want to do 5 runs
	for(int k =1; k <=5; k++){  //for each of the 5 runs
		cout << "Run " << k << endl;	//print the run number
		genAssignment(assignment);		//Generate the base assignment
		double bCount = 0;				//Set number of b=true found to 0
		
		for(int i = 1; i <= 2000; i++){  //each walk through non-evidence vars consists of 5 instances therefore 10,000 instances is 2,000 loops
			if (i % 200 == 0) {  //every 200 loops or 1000 instances
				cout << (double)bCount/i << endl;  //print number of B=True assignments
			}
			bCount = bCount + assignment[1];  //I only update bCount once per loop since only evalB will change the value, but it's important when calculating the percent to not divide by instances but by loops
			evalA(assignment);  //update the value of A based on biased RNG determined by CP tables
			evalB(assignment); //update the value of B based on biased RNG determined by CP tables
			evalC(assignment); //update the value of C based on biased RNG determined by CP tables
			evalE(assignment); //update the value of E based on biased RNG determined by CP tables
			evalF(assignment); //update the value of F based on biased RNG determined by CP tables
		}	
	}
	return 0;
}
