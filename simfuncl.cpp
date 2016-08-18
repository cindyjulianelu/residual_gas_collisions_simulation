//test of vector roation code.
#include <iostream>
#include <math.h>
#include <limits>
#include <fstream>
#include <time.h>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include "linalg.h"
#include "particle.h"
#include "geoms.h"
#include "simutils.h"
#include "randomNums.h"
#include "piecewiseLinear.h"
using namespace std;

double s = 0.3;
double l = 10.*s;
double d = s+0.025;
string sstr = to_string(s);
double rand01();
double cdf(double vf);
vector<double> Vs;

Vec3 sim_pos(int nsim,bool write, int ntherm = 1000, string fbase = "junk")
{
  //initialize sim
  time_t timer = time(NULL);
  int seed = timer + getpid();
  //cout << seed << endl;
srand(seed);
  Rotater r;
  
  //geoms
  Box b(-l, l, -l, l, -l, l);
  Vec3 cent(0, 0, 0);
  Sphere s(cent, 0.025);
  Cantilever c(-l, l, d, l, -0.05, 0.05);

  //particle
  Particle p;
  p.x0.set_values(0, 0, 1);
  p.v.set_values(0, 0, -1);

  //thermalize
  for (int n=ntherm; n>0; n--)
    {

      int randomNumber = getRandomInt();
      p = collision(p, b, s, c, r, Vs[randomNumber]);	  
    }
  p.t = 0;
  p.smom.set_values(0, 0, 0);
	
  if (write)
    {
      //allocate memory for saving poistions
      double xarr[nsim];
      double yarr[nsim];
      double zarr[nsim];
      
      //loop over collisions
      for (int n=nsim; n>0; n--)
	{
	  *(xarr + nsim-n) = p.x0.x; *(yarr + nsim-n) = p.x0.y; *(zarr + nsim-n) = p.x0.z;

	  //Good place to try? LET'S DDO THIS SHIT!
	  int randInt = getRandomInt();
	  double vf_i = Vs[randInt];

	  p = collision(p, b, s, c, r, vf_i);	  
	}

    
      write_arr(xarr, fbase + "x.txt", nsim);
      write_arr(yarr, fbase + "y.txt", nsim);
      write_arr(zarr, fbase + "z.txt", nsim);
    }
  else
    {
      for (int n=nsim; n>0; n--)
	{
	  int randInt2 = getRandomInt();
	  double vf_set = Vs[randInt2];
	  p = collision(p, b, s, c, r, vf_set);	  
	}
	  
    }
  Vec3 vret = p.smom;
  vret.scale(1/p.t);
  return vret;
}

void rep_sim(int nreps, string fbase, int ncollisions = 1000)
{
  double xarr[nreps];
  double yarr[nreps];
  double zarr[nreps];
  Vec3 fret;
  for (int n = 0; n < nreps; n++)
    {
      fret  = sim_pos(ncollisions, 0);
       *(xarr + n) = fret.x; *(yarr + n) = fret.y; *(zarr + n) = fret.z;
       cout<<"round " << n << " done"<<endl;
    }
  write_arr(xarr, fbase + "x.txt", nreps);
  write_arr(yarr, fbase + "y.txt", nreps);
  write_arr(zarr, fbase + "z.txt", nreps);
}

int main(int argc, char* argv[])
{
  // Good place to try? LET'S DO THIS SHIT

  vector<double> Parr; 
  vector<double> Vfarr;

   // Vfarr: very small intervals, cut 0 to 20 into 10,000 partitions
   for(int i = 0; i <= 99999; i++){
     Vfarr.push_back(i * 1.0 * (20.0000000000/99999.000000000));
   }

   // Parr: vector containing all the cdf values for each Parr value
   for(int i = 0 ; i < Vfarr.size(); i++){
     double cdf_val = cdf(Vfarr[i]);
     Parr.push_back(cdf_val);
   }

  solve_sys(Vfarr, Parr);

 // store these values into a Ps vector
    std::vector<double> Ps;

   // generate random numbers and store into Vs
       for(int i = 0; i < 1e7; i++){
           double randnum = rand01();
          Ps.push_back(randnum);
       }

   // Get Vs values to store in vector from using s()
   //     std::vector<double> Vs;

   //int counter = 0;

      for(int i = 0; i < Ps.size(); i++){
     
          double Ps_i = Ps[i];

	  double value = linear_extrapolate(Ps_i, Parr);
  
      Vs.push_back(value);
     
      }
      cout<<"It's a wrap"<<endl;

      rep_sim(5, argv[1]);//10
  return 0;
  }

double cdf(double vf){
  // return 0.5*(2.0-(exp(-pow(vf,2.0)/2.0)*(2.0+pow(vf,2.0))));
  double result;
  double A;
  //A = erf(vf / sqrt(2.0));
  A = -exp(-pow(vf, 2.0)/2.0);
  double B;
  B = pow(vf, 2.0)+ 2.00000000;
  //B = sqrt(2.0/3.14159265) * vf * exp(-pow(vf, 2.0)/2.0);
  result = (2.00000000 + A*B)*0.5;
  return result;
}

double rand01(){
   double randn = rand();
   return randn/RAND_MAX;
}





//test of vector roation code.
//#include <iostream>
//#include <math.h>
//#include <limits>
//#include <fstream>
//#include <time.h>
//#include <iomanip>
//#include <sstream>
//#include <unistd.h>
//#include <stdio.h>
//#include "linalg.h"
//#include "particle.h"
//#include "geoms.h"
//#include "simutils.h"
//using namespace std;

//double s = 0.3;
//double l = 10.*s;
//double d = s+0.025;
//string sstr = to_string(s);

//Vec3 sim_pos(int nsim,bool write, int ntherm = 1000, string fbase = "junk")
//{
  //initialize sim
// time_t timer = time(NULL);
//int seed = timer + getpid();
  //cout << seed << endl;
//srand(seed);
//Rotater r;
  
  //geoms
  //Box b(-l, l, -l, l, -l, l);
  //Vec3 cent(0, 0, 0);
  //Sphere s(cent, 0.025);
  //Cantilever c(-l, l, d, l, -0.05, 0.05);

  //particle
  //Particle p;
  //p.x0.set_values(0, 0, 1);
  //p.v.set_values(0, 0, -1);

  //thermalize
  //for (int n=ntherm; n>0; n--)
// {
//   p = collision(p, b, s, c, r);	  
// }
//p.t = 0;
//p.smom.set_values(0, 0, 0);
	
//if (write)
//  {
      //allocate memory for saving poistions
//  double xarr[nsim];
// double yarr[nsim];
// double zarr[nsim];
      
      //loop over collisions
      //for (int n=nsim; n>0; n--)
//	{
//	  *(xarr + nsim-n) = p.x0.x; *(yarr + nsim-n) = p.x0.y; *(zarr + nsim-n) = p.x0.z;

//  p = collision(p, b, s, c, r);	  
//	}

    
//  write_arr(xarr, fbase + "x.txt", nsim);
// write_arr(yarr, fbase + "y.txt", nsim);
//  write_arr(zarr, fbase + "z.txt", nsim);
// }
//else
// {
//  for (int n=nsim; n>0; n--)
//	{
//	  p = collision(p, b, s, c, r);	  
//	}
	  
// }
//Vec3 vret = p.smom;
// vret.scale(1/p.t);
//return vret;
//}

//void rep_sim(int nreps, string fbase, int ncollisions = 100000000)
//{
//  double xarr[nreps];
//  double yarr[nreps];
// double zarr[nreps];
// Vec3 fret;
//  for (int n = 0; n < nreps; n++)
//   {
//     fret  = sim_pos(ncollisions, 0);
//       *(xarr + n) = fret.x; *(yarr + n) = fret.y; *(zarr + n) = fret.z;
//    }
//  write_arr(xarr, fbase + "x.txt", nreps);
//  write_arr(yarr, fbase + "y.txt", nreps);
//  write_arr(zarr, fbase + "z.txt", nreps);
//}

//int main(int argc, char* argv[])
//{
// Vec3 sp = sim_pos(10, 1);
  //rep_sim(10, argv[1]);
  //return 0;
  //}

//test of vector roation code.
//#include <iostream>
//#include <math.h>
//#include <limits>
//#include <fstream>
//#include <time.h>
//#include <iomanip>
//#include <sstream>
//#include <unistd.h>
//#include <stdio.h>
//#include <chrono>
//#include "linalg.h"
//#include "particle.h"
//#include "geoms.h"
//#include "simutils.h"
//#include "cdf.h"

//typedef std::chrono::high_resolution_clock Clock;

//using namespace std;

//double s = 0.3;
//double l = 10.*s;
//double d = s+0.025;
//string sstr = to_string(s);

//Vec3 sim_pos(int nsim,bool write, int ntherm = 10, string fbase = "junk")
//{ // ntherm = 1000
  //initialize sim
  //time_t timer = time(NULL);
  //int seed = timer + getpid();
  //cout << seed << endl;
  //srand(seed);
//Rotater r;
  
  //geoms
  //Box b(-l, l, -l, l, -l, l);
  //Vec3 cent(0, 0, 0);
  //Sphere s(cent, 0.025);
  //Cantilever c(-l, l, d, l, -0.05, 0.05);

  //particle
  //Particle p;
  //p.x0.set_values(0, 0, 1);
  //p.v.set_values(0, 0, -1);

  //thermalize
  // for (int n=ntherm; n>0; n--)
    //  {
      //   p = collision(p, b, s, c, r);	  
      //  }
  // p.t = 0;
  // p.smom.set_values(0, 0, 0);
	
  //if (write)
    // {
      //allocate memory for saving poistions
      // double xarr[nsim];
      // double yarr[nsim];
      // double zarr[nsim];
      
      //loop over collisions
      // for (int n=nsim; n>0; n--)
      //	{
      //	  *(xarr + nsim-n) = p.x0.x; *(yarr + nsim-n) = p.x0.y; *(zarr + nsim-n) = p.x0.z;

	  //	  p = collision(p, b, s, c, r);	  
	  //	}

    
      //     write_arr(xarr, fbase + "x.txt", nsim);
      //     write_arr(yarr, fbase + "y.txt", nsim);
      //     write_arr(zarr, fbase + "z.txt", nsim);
      //    }
  //  else
    //    {
      //      for (int n=nsim; n>0; n--)
	//	{
	  //	  p = collision(p, b, s, c, r);	  
	  //	}
	  
      //   }
  // Vec3 vret = p.smom;
  // vret.scale(1/p.t);
  //  return vret;
  //}


//int main(int argc, char* argv[])
//{
  //auto t1 = Clock::now();
  //clock_t t1,t2;
  // t1=clock();

  // Vec3 sp = sim_pos(100,1);

  //t2=clock();
  // float diff ((float)t2-(float)t1);
  // cout<< diff/CLOCKS_PER_SEC <<endl;

  //auto t2 = Clock::now();
  //std::cout <<  std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()<< " nanoseconds" << std::endl;
  
  //  return 0;
  //  }  
