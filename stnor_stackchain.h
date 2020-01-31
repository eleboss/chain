#ifndef UTILS_LOAD_
#define UTILS_LOAD_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "timer.h"
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>     
#include <opencv2/core/eigen.hpp>

using namespace std;
using namespace cv;
using namespace Eigen;

class EV_Array
{
  public:
    long double ev_time = 0;
    int ev_x;
    int ev_y;
    int ev_polarity;
};


// Input:
// file_path : the path of your txt file, like "./events.txt"
// ali_time: 1->align event to start at time 0s, 0->do nothing.
// read_number: how many events you want to read?
// map_por: 1->map event from {1,0} to {1,-1}
vector<EV_Array> loading_from_file(string file_path, int ali_time = 1, int read_number = 99999999, int map_por = 1)
{
  char sep = ' ';
  EV_Array event;
  string line;
  vector<EV_Array> ev_array;
  long double ev_time = 0;
  int ev_x;
  int ev_y;
  int ev_polarity;
  long double begin_time = 0.0;
  ifstream myfile (file_path);

  int count = 0;
  int event_counter = 0;
  bool trig = true;

  if (myfile.is_open())
  {
    while ( getline (myfile,line) and event_counter < read_number)
    {
      event_counter += 1;
      count = 0;
      for(size_t p=0, q=0; p!=line.npos; p=q)
      {

        string::size_type sz;     // alias of size_t
        if(count == 0)
        {
          ev_time = stold(line.substr(p+(p!=0), (q=line.find(sep, p+1))-p-(p!=0)),&sz);
        }
        if(count == 1)
        {
          ev_x = stoi(line.substr(p+(p!=0), (q=line.find(sep, p+1))-p-(p!=0)),&sz);
        }
        if(count == 2)
        {
          ev_y = stoi(line.substr(p+(p!=0), (q=line.find(sep, p+1))-p-(p!=0)),&sz);
        }
        if(count == 3)
        {
          ev_polarity = stoi(line.substr(p+(p!=0), (q=line.find(sep, p+1))-p-(p!=0)),&sz);
        }
        count += 1; 
      }
      if(trig)
      {
        begin_time = ev_time;
        trig = false;
      }
      if(ali_time == 1)
      {
        ev_time = ev_time - begin_time;
      }
      if(map_por == 1)
      {
        if(ev_polarity==0)
        {
          ev_polarity = -1;
        } 
      }
      event.ev_time = ev_time;
      event.ev_x = ev_x;
      event.ev_y = ev_y;
      event.ev_polarity = ev_polarity;
      ev_array.push_back(event);

      // cout.precision(30);
      // cout<<ev_array.size()<<endl;
      // std::cout <<"out "<<ev_time <<" "<<ev_x<< " "<<ev_y<<" "<<ev_polarity<< std::endl;
      // cout<<"line " <<endl;
      // cout <<line << '\n';

    }
    // cout<<"loading finished"<<endl;
    myfile.close();
  }
  else 
  {
    cout << "Unable to open file"<<endl;
    while(1);
  }

  return ev_array;
}


#endif