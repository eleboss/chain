#include "timer.h"
#include "chainnor.h"


int main (int argc, char *argv[])
{
  TicToc timer_0, timer_1;

  int id_trans_mat[IMAGE_HEIGHT][IMAGE_WIDTH];
  int image_1d_pos = 0;
  for(int i = 0; i < IMAGE_HEIGHT; i++)
  {
    for(int j = 0; j < IMAGE_WIDTH; j++)
    {
      id_trans_mat[i][j] = i * IMAGE_WIDTH + j;  // init the id_trans_plane
    }
  }

  // reading interface
  string file_path = "/home/eleboss/dvs_dataset/davis240c/outdoors_running/events.txt"; //which file you want to process?
  double process_data_num = READING_EVENT; //how many event you want to process?

  timer_0.tic();
  vector<EV_Array> ev_array;
  cout<<"Starting loading file....."<<endl;
  ev_array = loading_from_file(file_path, 1, process_data_num);
  cout<<"The file loading spend "<<timer_0.toc()<<" ms"<<endl;
  cout<<"The program has read "<<ev_array.size()<<" events."<<endl;
  cout<<"Duration of event_data "<<ev_array.back().ev_time - ev_array[0].ev_time  <<" s"<<endl;
  cout<<"Est process "<<process_data_num<<endl;
  cout<<"Normalization start... "<<endl;
  
  EV_Node node_list[NUM_PIXELS]; 
  double global_weight[NUM_PIXELS];
  fill_n(global_weight, NUM_PIXELS, 0);

  //init the list
  for(int i = 0;i < NUM_PIXELS - 1; i++)
  {
    node_list[i].next = &node_list[i+1];
    node_list[i+1].prev = &node_list[i];
    node_list[i].wp = &global_weight[i];
    node_list[i].time = 0.0;
  }
  node_list[NUM_PIXELS-1].next = NULL;
  node_list[NUM_PIXELS-1].wp = &global_weight[NUM_PIXELS-1];
  node_list[NUM_PIXELS-1].time = 0.0;
  node_list[0].prev = NULL;

  //init tail
  EV_Node* tail;
  EV_Node* head;
  EV_Node* loop_pointer;
  EV_Node *cur_position;
  tail = &node_list[NUM_PIXELS-1];
  head = &node_list[0];

  chrono::duration<double> elapsed_seconds_per;
  chrono::duration<double> elapsed_seconds;
  elapsed_seconds = chrono::duration<double>::zero();

  for(int i=0; i<process_data_num; i++)
  {
    int event_x = ev_array[i].ev_x;
    int event_y = ev_array[i].ev_y;
    double event_time = ev_array[i].ev_time;
    int event_polarity = ev_array[i].ev_polarity;

    timer_1.tic();// start timing
    image_1d_pos = id_trans_mat[event_y][event_x];
    cur_position = &node_list[image_1d_pos];
    cur_position->time = event_time;
    //if tail, do noting
    if(cur_position->next == NULL)
    {
      // cout<<"tail!";
    }
    else
    { 
      // head, worse case
      if(cur_position->prev == NULL)
      {
        // cout<<"head";
        // before: NULL <-- A <--> B
        // after: NULL <-- B
        cur_position->next->prev = NULL;
        head = cur_position->next;
      }
      else
      {
        // before: A <--> B <--> C
        // after:  A <----> C
        cur_position->next->prev = cur_position->prev;
        cur_position->prev->next = cur_position->next;

      }
      //change tail
      cur_position->next = NULL;
      cur_position->prev = tail;
      //point tail next to the new one
      tail->next = &node_list[image_1d_pos];
      //modify the tail to current position
      tail = &node_list[image_1d_pos];

      //order updating
      loop_pointer = head;
      double w = 0;
      while (loop_pointer->next != NULL) {  
        if(loop_pointer->next->time != loop_pointer->time)
        {
          w += PER_WEIGHT;
        } 
        *loop_pointer->wp = w;
        loop_pointer = loop_pointer->next;
      }
      *tail->wp = w + PER_WEIGHT;

    }
    timer_1.toc();

    elapsed_seconds += (timer_1.end - timer_1.start);
    elapsed_seconds_per = (timer_1.end - timer_1.start);


  }
  array_saver(IMAGE_HEIGHT, IMAGE_WIDTH ,"Nor", "chain_nor.xml", global_weight);
  cout<<"Overall proceee time "<<elapsed_seconds.count() * 1000.0 <<" ms"<<endl;
  cout<<"Average proceee time "<<elapsed_seconds.count() * 1000.0 / process_data_num<<" ms"<<endl;


  return 0;
}