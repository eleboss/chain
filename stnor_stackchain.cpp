#include "stnor_stackchain.h"


  //x width y height
  // 0--------------------->X
  // |
  // |
  // |
  // |
  // v
  // Y
  //2 5 10 12 15 20 30
#define FL_WIDTH_R 30
#define FL_HEIGHT_R 30
#define FL_WIDTH_2R (FL_WIDTH_R * 2) //要能被图像宽整除
#define FL_HEIGHT_2R (FL_HEIGHT_R * 2) //要能被图像高整除
#define IMAGE_PIXELS ((FL_WIDTH_2R+1) * (FL_HEIGHT_2R+1)) //要能被图像高整除
#define FL_CENTER_PIXELS (FL_WIDTH_2R * FL_HEIGHT_2R) //要能被图像高整除
#define FL_WIDTH  (FL_WIDTH_2R * 2)
#define FL_HEIGHT (FL_HEIGHT_2R * 2)
// #define WEIGHT_ARRAY_NUM ((FL_WIDTH - (FL_WIDTH_2R-1) + 1 )* (FL_HEIGHT - (FL_HEIGHT_2R-1) + 1))
#define RECEPTIVE_PIXELS (FL_WIDTH * FL_HEIGHT)
#define PER_WEIGHT 1.0/IMAGE_PIXELS
#define IMAGE_HEIGHT 180
#define IMAGE_WIDTH 240
#define NUM_PIXELS (IMAGE_HEIGHT * IMAGE_WIDTH)
#define LAYER_X_NUM (IMAGE_WIDTH/FL_WIDTH_2R-1)
#define LAYER_Y_NUM (IMAGE_HEIGHT/FL_HEIGHT_2R-1)

#define FRAME_RATE 1000
#define FRAME_TIME (1.0/FRAME_RATE)
#define PROCESS_EVENT_NUM 5000000

// Parameter for visualize
#define VISUALIZE 0

using namespace std;
using namespace cv;
using namespace Eigen;



// A linked list node  
class EV_Node 
{  
  public:
    double *wp;
    bool active;
    EV_Node *next;  
    EV_Node *prev;  
    EV_Node()
    {
      wp = NULL;
      active = false;
      next = NULL;
      prev = NULL;
    }
};  

class NODE_List 
{
  public:
    EV_Node* tail;
    EV_Node* head;
    EV_Node *node_list;
    int topLeft_x, topLeft_y, bottomRight_x, bottomRight_y;
    NODE_List()
    { 
      node_list = new EV_Node[RECEPTIVE_PIXELS];
      head = &node_list[0];
      tail = &node_list[list_size - 1];
      // construct the list, from head to tail.
      node_list[0].next = &node_list[1];
      node_list[0].prev = NULL;
      node_list[list_size - 1].prev = &node_list[list_size - 2];
      node_list[list_size - 1].next = NULL;
      for(int i = 1; i < list_size - 1; i ++)
      {
        node_list[i].prev = &node_list[i - 1];
        node_list[i].next = &node_list[i + 1];
      }

    }
    ~NODE_List(){}
  private:
    int list_size = RECEPTIVE_PIXELS;
};  

 

//related it to the node_list based on the position of pixels
class LOCAL_List {  
  public:
    NODE_List* local_list[4]; //define 4 entrance to find the box
    int position_id[4];
    int box_counter;
    int output_box_id;
    int relative_position_inbox;
    LOCAL_List()
    {
      for(int i = 0; i < 4; i ++)
      {
        local_list[i] = NULL;
        position_id[i] = -1;
        box_counter = 0;
        output_box_id = -1;
        relative_position_inbox = 0;
      }
    }
    ~LOCAL_List(){}
};  



class Local_NOR{
  public:
    // define the image plane
    LOCAL_List sae_plane[IMAGE_HEIGHT][IMAGE_WIDTH];
    // define the layer plane
    NODE_List layer_plane[LAYER_Y_NUM][LAYER_X_NUM];
    MatrixXd Local_Weight;
    double **bitmask;
    double *weight_array;
    Local_NOR()
    {
      Local_Weight.resize(FL_HEIGHT_2R+1, FL_WIDTH_2R+1);
      Local_Weight = MatrixXd::Zero(FL_HEIGHT_2R+1, FL_WIDTH_2R+1);
      weight_array = new double[RECEPTIVE_PIXELS]; 

      bitmask = new double*[RECEPTIVE_PIXELS]; 
      for(int i = 0; i < RECEPTIVE_PIXELS; ++i)
          bitmask[i] = new double[RECEPTIVE_PIXELS];//clo_count -> which position in an weight_array
      //init to 0, NA value
      for(int i = 0; i < RECEPTIVE_PIXELS; i ++)
      {
        for(int j = 0; j < RECEPTIVE_PIXELS; j ++)
        {
           bitmask[i][j] = 0;
        }
      }
      // init some temp variable
      int g_box_top_x, g_box_top_y, g_box_bot_x, g_box_bot_y, g_box_x, g_box_y, l_box_1d;
      int g_pix_x, g_pix_y;
      //prepare a bitmask
      for(int k = 0; k < FL_CENTER_PIXELS; k ++)
      {
        g_box_x = k%FL_WIDTH_2R + FL_WIDTH_R;
        g_box_y = k/FL_WIDTH_2R + FL_HEIGHT_R;
        l_box_1d = g_box_x + g_box_y * FL_WIDTH;

        g_box_top_x = g_box_x - FL_WIDTH_R;
        g_box_top_y = g_box_y - FL_HEIGHT_R;
        g_box_bot_x = g_box_x + FL_WIDTH_R;//2R+1
        g_box_bot_y = g_box_y + FL_HEIGHT_R;
        // cout<<" BoxX:"<<g_box_x<<" BoxY:"<<g_box_y<<" 1d:"<<l_box_1d<<" topx:"<< g_box_top_x<<" topy:"<<g_box_top_y<<" botx:"<< g_box_bot_x<<" boty:"<<g_box_bot_y<<endl; 
        for(int j = 0; j < RECEPTIVE_PIXELS; j ++)
        {
          g_pix_x = j%FL_WIDTH;
          g_pix_y = j/FL_WIDTH;
          if(g_pix_x >= g_box_top_x and g_pix_y >= g_box_top_y and g_pix_x <= g_box_bot_x and g_pix_y <= g_box_bot_y)
          {
            // cout<<" PixX:"<<g_pix_x<<" PixY:"<<g_pix_y<<" topx:"<< g_box_top_x<<" topy:"<<g_box_top_y<<" botx:"<< g_box_bot_x<<" boty:"<<g_box_bot_y<<endl; 
            bitmask[l_box_1d][j] = PER_WEIGHT; //process 1, leave 0 out
          }
        }
      }


      for(int y = 0; y < LAYER_Y_NUM; y++)
      {
        for(int x = 0; x < LAYER_X_NUM; x++)
        {
          for(int j = 0; j < RECEPTIVE_PIXELS; j ++)
          {
            layer_plane[y][x].node_list[j].wp = &weight_array[j];
          }
        }
      }

      // arrange the layer id
      for(int y = 0; y < LAYER_Y_NUM; y++)
      {
        for(int x = 0; x < LAYER_X_NUM; x++)
        {
          int topLeft_x = x * FL_WIDTH_2R;
          int topLeft_y = y * FL_HEIGHT_2R;
          int bottomRight_x = x * FL_WIDTH_2R + (FL_WIDTH - 1);
          int bottomRight_y = y * FL_HEIGHT_2R + (FL_HEIGHT - 1);
          // cout<<"TL XY:"<<topLeft_x<<" "<<topLeft_y<<" BR XY:"<<bottomRight_x<<" "<<bottomRight_y<<endl;
          layer_plane[y][x].topLeft_x = topLeft_x;
          layer_plane[y][x].topLeft_y = topLeft_y;
          layer_plane[y][x].bottomRight_x = bottomRight_x;
          layer_plane[y][x].bottomRight_y = bottomRight_y;
        }
      }
      // link each pixel with layer id
      for(int image_y = 0; image_y < IMAGE_HEIGHT; image_y++)
      {
        for(int image_x = 0; image_x < IMAGE_WIDTH; image_x++)
        {
          for(int layer_y = 0; layer_y < LAYER_Y_NUM; layer_y++)
          {
            for(int layer_x = 0; layer_x < LAYER_X_NUM; layer_x++)
            {        
              // if the pixel in the layer box    
              if(image_x >= layer_plane[layer_y][layer_x].topLeft_x and image_y >= layer_plane[layer_y][layer_x].topLeft_y)
              {
                if(image_x <= layer_plane[layer_y][layer_x].bottomRight_x and image_y <= layer_plane[layer_y][layer_x].bottomRight_y)
                {
                  // link the pixel to layer box
                  sae_plane[image_y][image_x].local_list[sae_plane[image_y][image_x].box_counter] = &layer_plane[layer_y][layer_x];
                  int temp_position = (image_x - layer_plane[layer_y][layer_x].topLeft_x) + (image_y - layer_plane[layer_y][layer_x].topLeft_y) * FL_WIDTH;
                  sae_plane[image_y][image_x].position_id[sae_plane[image_y][image_x].box_counter] = temp_position;
                  sae_plane[image_y][image_x].box_counter += 1;
                  
                  // cout<<temp_position<<" LayX:"<<layer_x<<" LayY:"<<layer_y<<" ImX:"<<image_x<<" ImY:"<<image_y<<" TopX:"<<layer_plane[layer_y][layer_x].topLeft_x<<" TopY:"<<layer_plane[layer_y][layer_x].topLeft_y<<" BotX:"<<layer_plane[layer_y][layer_x].bottomRight_x<<" BotY:"<<layer_plane[layer_y][layer_x].bottomRight_y<<" BOXID:"<<sae_plane[image_y][image_x].box_counter<<endl;

                }
              }
            }
          }
        }
      }

      // locate the closest box
      for(int image_y = 0; image_y < IMAGE_HEIGHT; image_y++)
      {
        for(int image_x = 0; image_x < IMAGE_WIDTH; image_x++)
        {
          int loop = sae_plane[image_y][image_x].box_counter;
          float last_e_distance_2 = LONG_MAX;
          for (size_t i = 0; i != loop; ++i) 
          {
            float middle_x = sae_plane[image_y][image_x].local_list[i]->topLeft_x + FL_WIDTH_2R - 0.5;
            float middle_y = sae_plane[image_y][image_x].local_list[i]->topLeft_y + FL_HEIGHT_2R - 0.5;
            float e_distance_2 = pow((middle_x - float(image_x)),2) + pow(middle_y - float(image_y),2); 
            if(e_distance_2 <= last_e_distance_2)
            {
              sae_plane[image_y][image_x].output_box_id = i;
              last_e_distance_2 = e_distance_2;
            }
          }
        }
      }
      
      for(int image_y = 0; image_y < IMAGE_HEIGHT; image_y++)
      {
        for(int image_x = 0; image_x < IMAGE_WIDTH; image_x++)
        {
          int output_box_id = sae_plane[image_y][image_x].output_box_id;
          sae_plane[image_y][image_x].relative_position_inbox = (image_x - sae_plane[image_y][image_x].local_list[output_box_id]->topLeft_x) + (image_y - sae_plane[image_y][image_x].local_list[output_box_id]->topLeft_y) * FL_WIDTH;

          // cout<<" x:"<<image_x<<" y:"<<image_y<<" topx:"<<sae_plane[image_y][image_x].local_list[output_box_id]->topLeft_x<<" topy:"<<sae_plane[image_y][image_x].local_list[output_box_id]->topLeft_y<<" botx:"<<sae_plane[image_y][image_x].local_list[output_box_id]->bottomRight_x<<" boty:"<<sae_plane[image_y][image_x].local_list[output_box_id]->bottomRight_y<<" boxID:"<<output_box_id<<" 1d:"<<sae_plane[image_y][image_x].relative_position_inbox<<endl;
        }
      }
    }
    ~Local_NOR(){}

    void checking()
    {
      for(int y = 0; y < IMAGE_HEIGHT; y++)
      {
        for(int x = 0; x < IMAGE_WIDTH; x++)
        {
          cout<<sae_plane[y][x].output_box_id;
        }
      }
    }


    void loop_checking(int &x, int &y, double &timestamp, int &por)
    {

      int output_box_id = sae_plane[y][x].output_box_id;
      double w = 0.0;
      cout<<output_box_id<<" box"<<"x "<<x<<"y"<<y<<endl;
      EV_Node* loop_pointer = sae_plane[y][x].local_list[output_box_id]->head;
      while (loop_pointer->next != NULL) 
      {  
        w += 1;
        loop_pointer = loop_pointer->next;    
      }
      cout<<w<<endl; 
    }

    void loop_checking_weight()
    {
      for(int y = 0; y < IMAGE_HEIGHT; y++)
      {
        for(int x = 0; x < IMAGE_WIDTH; x++)
        {
          int output_box_id = sae_plane[y][x].output_box_id;
          int rel_pos_inbox = sae_plane[y][x].relative_position_inbox;
          double w_1 = 0.0;
          double w_2 = 0.0;
          cout<<" box:"<<output_box_id<<" x:"<<x<<" y:"<<y<<" rel:"<<rel_pos_inbox<<endl;
          EV_Node* loop_pointer = sae_plane[y][x].local_list[output_box_id]->head;
          if (x >= FL_HEIGHT_R and y >= FL_HEIGHT_R and y < (IMAGE_HEIGHT - FL_HEIGHT_R) and x < (IMAGE_WIDTH - FL_WIDTH_R))
          {
            while (loop_pointer != NULL) 
            { 
              // if(loop_pointer->wp_list[rel_pos_inbox] != NULL)
              // {
              //   // cout<<" "<<rel_pos_inbox<<" "<<*loop_pointer->wp_list[rel_pos_inbox]<<endl;
              //   // *loop_pointer->wp_list[rel_pos_inbox] = w_1;
              //   w_1 += 1;
              // }
              // else
              // {
              //   w_2 += 1;
              // }  
              
              loop_pointer = loop_pointer->next;
              // cout<<w_1<<" "<<w_2<<endl;    
            } 
          }
          cout<<w_1<<" "<<w_2<<endl;
        }
      } 
    }

    void updata_time(int &x, int &y, double &timestamp, int &por)
    { 
      uint pos=0;
      int loop = sae_plane[y][x].box_counter;
      for (size_t i = 0; i != loop; ++i) 
      {
      
        pos = sae_plane[y][x].position_id[i];
        // cout<< "x"<<x<<"y"<<y<<" "<<i<<" pos:"<<pos<<endl;

        // indicate this place has been changed
        sae_plane[y][x].local_list[i]->node_list[pos].active = true;

        if(sae_plane[y][x].local_list[i]->node_list[pos].next != NULL) //not tail
        {
          if(sae_plane[y][x].local_list[i]->node_list[pos].prev != NULL) // not head
          {
            // break
            sae_plane[y][x].local_list[i]->node_list[pos].prev->next = sae_plane[y][x].local_list[i]->node_list[pos].next;
            sae_plane[y][x].local_list[i]->node_list[pos].next->prev = sae_plane[y][x].local_list[i]->node_list[pos].prev;
            // put it to the end
            sae_plane[y][x].local_list[i]->tail->next = &sae_plane[y][x].local_list[i]->node_list[pos]; 
            sae_plane[y][x].local_list[i]->node_list[pos].prev = sae_plane[y][x].local_list[i]->tail;
            sae_plane[y][x].local_list[i]->node_list[pos].next = NULL;
            //change the tail
            sae_plane[y][x].local_list[i]->tail = &sae_plane[y][x].local_list[i]->node_list[pos];
          }
          else
          {
            // cout<<"head"<<endl;
            // change the head
            sae_plane[y][x].local_list[i]->head = sae_plane[y][x].local_list[i]->node_list[pos].next; 
            // break
            sae_plane[y][x].local_list[i]->node_list[pos].next->prev = NULL;
            // put it to the end
            sae_plane[y][x].local_list[i]->tail->next = &sae_plane[y][x].local_list[i]->node_list[pos]; 
            sae_plane[y][x].local_list[i]->node_list[pos].prev = sae_plane[y][x].local_list[i]->tail;
            sae_plane[y][x].local_list[i]->node_list[pos].next = NULL;
            //change the tail
            sae_plane[y][x].local_list[i]->tail = &sae_plane[y][x].local_list[i]->node_list[pos]; 
          }
        }
      }
    }
    
    void update_weight(int &x, int &y, double &timestamp, int &por)
    {
      if (x >= FL_HEIGHT_R and y >= FL_HEIGHT_R and y < (IMAGE_HEIGHT - FL_HEIGHT_R) and x < (IMAGE_WIDTH - FL_WIDTH_R))
      {
        int output_box_id = sae_plane[y][x].output_box_id;
        int rel_pos_inbox = sae_plane[y][x].relative_position_inbox;
        double w_1 = 0.0; 
        memcpy(weight_array,bitmask[rel_pos_inbox],RECEPTIVE_PIXELS*8);
        EV_Node* loop_pointer = sae_plane[y][x].local_list[output_box_id]->head;

        while (loop_pointer != NULL) 
        { 
          if(loop_pointer->active == true)
          {
            w_1 += *loop_pointer->wp;
            *loop_pointer->wp = w_1; 

          }
          loop_pointer = loop_pointer->next; 
        }
      }
    }



    void mat_saver(int x, int y, string file_name = "./sis_nor.tif", string type = "img", int ChangeSize = 0)
    {
      if (x >= FL_HEIGHT_R and y >= FL_HEIGHT_R and y < (IMAGE_HEIGHT - FL_HEIGHT_R) and x < (IMAGE_WIDTH - FL_WIDTH_R))
      {
        if(type == "img")
        {
          int output_box_id = sae_plane[y][x].output_box_id;
          int rel_pos_inbox = sae_plane[y][x].relative_position_inbox;
          int height = 0;
          int width = 0;
          for(int i = 0; i < FL_HEIGHT; i++)
          {
            for(int j = 0; j < FL_WIDTH; j++)
            {
              if(bitmask[rel_pos_inbox][i * FL_WIDTH + j] != 0)
              {
                Local_Weight(height,width) = weight_array[i * FL_WIDTH + j]-PER_WEIGHT;
                width += 1;
                if(width > FL_WIDTH_2R)
                {
                  height += 1;
                  width = 0;
                }

              }
            }
          }
          cv::Mat tempMat;
          MatrixXd Local_Weight65535 = Local_Weight.array()*65535.0;
          eigen2cv(Local_Weight65535,tempMat);
          tempMat.convertTo(tempMat,CV_16U);
          if(ChangeSize)
          {
            resize(tempMat, tempMat, cv::Size(224,224), 0, 0, CV_INTER_CUBIC);
          }
          imwrite(file_name, tempMat);
          Local_Weight = MatrixXd::Zero(FL_HEIGHT, FL_WIDTH);
        }
      }
    }
  private:
};

int main (int argc, char *argv[])
{

  TicToc timer_0, timer_1;

  string save_path = "../result/";


  string file_path = "/home/eleboss/dvs_dataset/davis240c/shapes_translation/events.txt";

  timer_0.tic();
  vector<EV_Array> ev_array;
  ev_array = loading_from_file(file_path, 1, PROCESS_EVENT_NUM);

  int process_event_num = PROCESS_EVENT_NUM;
  process_event_num = ev_array.size();

  cout<<"loading time: "<<timer_0.toc()<<" ms.";
  cout<<"Total: "<<ev_array.size()<<" events.";
  cout<<"Duration: "<<ev_array.back().ev_time - ev_array[0].ev_time  <<" s."<<endl;


  timer_1.tic();

  Local_NOR Local_Normalization;
  timer_1.toc();
  cout<<"Construct time:"<<timer_1.toc()<<" ms"<<endl;
  cout<<"Running the check program..."<<endl;
  // Local_Normalization.checking();
  cout<<"Checking over"<<endl;

  //looping all events.
  double consume_time_chain = 0;
  double consume_time_weight = 0;
  int event_x;
  int event_y;
  double event_time; //make it in ms
  int event_polarity;
  float frame_time_step = 0.0;
  for(int i = 0; i < process_event_num; i++)
  {
    // do sth about event
    event_x = ev_array[i].ev_x;
    event_y = ev_array[i].ev_y;
    event_time = ev_array[i].ev_time; //make it in ms
    event_polarity = ev_array[i].ev_polarity;

    // cout<<i<<endl;
    if(event_time > frame_time_step)
    {
      timer_1.tic();
      Local_Normalization.updata_time(event_x, event_y, event_time, event_polarity);
      consume_time_chain += timer_1.toc();
      frame_time_step += FRAME_TIME;

      timer_1.tic();
      // cout<<"!!!"<<endl;
      Local_Normalization.update_weight(event_x, event_y, event_time, event_polarity);
      consume_time_weight += timer_1.toc();
    }
    else
    {
      timer_1.tic();
      Local_Normalization.updata_time(event_x, event_y, event_time, event_polarity);
      consume_time_chain += timer_1.toc();
    }
  }
  double process_frame_num = (frame_time_step/FRAME_TIME);
  cout<<"Overall proceee time chain "<<consume_time_chain<<" ms"<<endl;
  cout<<"Average proceee time chain "<<consume_time_chain / process_event_num<<" ms"<<endl;
  cout<<"Overall proceee time weight "<<consume_time_weight<<" ms"<<endl;
  cout<<"Average proceee time weight "<<consume_time_weight / process_frame_num<<" ms"<<endl;
  cout<<"-----------------------------------------"<<endl;

  return 0;
}