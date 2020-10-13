#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>



#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Vector3Stamped.h"
#include "geometry_msgs/Twist.h"
#include "linux_kbhit.h"

int count = 1;
int proc = 0;


int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
  //std::cout << "ch : " << ch << std::endl;
  int _ch = ch;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    //ungetc(ch, stdin);
    return ch;
  }
 
  return 0;
}


double p_x = 0.0;
double p_y = 0.0;
double p_z = 2.0;

void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& pose_msg,ros::Publisher _pub, ros::Publisher _vel_pub) {
   float x = pose_msg->pose.position.x; 
   float y = pose_msg->pose.position.y; 
   float z = pose_msg->pose.position.z; 
   //ROS_INFO("x = %f", x);
   //ROS_INFO("y = %f", y);
   //ROS_INFO("z = %f", z);

   geometry_msgs::Twist vel_msg;


   geometry_msgs::PoseStamped msg;

   

   if(!proc) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = p_x;//0.001*some_object.position_x;
      msg.pose.position.y = p_y;//0.001*some_object.position_y;
      msg.pose.position.z = p_z;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      _pub.publish(msg);
      ros::spinOnce();
        

      if(x > -0.5 && x < 0.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   }
   else if(proc == 1) {
      int ch;
      if(x > x-0.1 && x < x+0.1 && y > y-0.1 && y < y+0.1 && z > z-0.1 && z < z+0.1) {
         if(ch = kbhit()) {
            printf("You pressed '%d'!\n", ch);
            if(ch == 'a') p_x += 0.1;
            else if(ch == 'd') p_x -= 0.1;
            else if(ch == 'x') p_y += 0.1;
            else if(ch == 'w') p_y -= 0.1;
            
         }
      }

      
      
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = p_x;//0.001*some_object.position_x;
      msg.pose.position.y = p_y;//0.001*some_object.position_y;
      msg.pose.position.z = p_z;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      _pub.publish(msg);
      ros::spinOnce();
   }
   // else if(proc == 1) {
   //    msg.header.stamp = ros::Time::now();
   //    msg.header.seq=count;
   //    msg.header.frame_id = 1;
   //    msg.pose.position.x = 5.0;//0.001*some_object.position_x;
   //    msg.pose.position.y = 0.0;//0.001*some_object.position_y;
   //    msg.pose.position.z = 2.0;//0.001*some_object.position_z;
   //    msg.pose.orientation.x = 0;
   //    msg.pose.orientation.y = 0;
   //    msg.pose.orientation.z = 0;
   //    msg.pose.orientation.w = 1;

   //    if(x > 4.5 && x < 5.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   // }
   // else if(proc == 2) {
   //    msg.header.stamp = ros::Time::now();
   //    msg.header.seq=count;
   //    msg.header.frame_id = 1;
   //    msg.pose.position.x = 5.0;//0.001*some_object.position_x;
   //    msg.pose.position.y = 10.0;//0.001*some_object.position_y;
   //    msg.pose.position.z = 2.0;//0.001*some_object.position_z;
   //    msg.pose.orientation.x = 0;
   //    msg.pose.orientation.y = 0;
   //    msg.pose.orientation.z = 0;
   //    msg.pose.orientation.w = 1;

   //    if(x > 4.5 && x < 5.5 && y > 9.5 && y < 10.5 && z > 1.5 && z < 2.5) proc++;
   // }
   // else if(proc == 3) {
   //    msg.header.stamp = ros::Time::now();
   //    msg.header.seq=count;
   //    msg.header.frame_id = 1;
   //    msg.pose.position.x = -5.0;//0.001*some_object.position_x;
   //    msg.pose.position.y = 10.0;//0.001*some_object.position_y;
   //    msg.pose.position.z = 2.0;//0.001*some_object.position_z;
   //    msg.pose.orientation.x = 0;
   //    msg.pose.orientation.y = 0;
   //    msg.pose.orientation.z = 0;
   //    msg.pose.orientation.w = 1;

   //    if(x < -4.5 && x > -5.5 && y > 9.5 && y < 10.5 && z > 1.5 && z < 2.5) proc++;
   // }   
   // else if(proc == 4) {
   //    msg.header.stamp = ros::Time::now();
   //    msg.header.seq=count;
   //    msg.header.frame_id = 1;
   //    msg.pose.position.x = -5.0;//0.001*some_object.position_x;
   //    msg.pose.position.y = 0.0;//0.001*some_object.position_y;
   //    msg.pose.position.z = 2.0;//0.001*some_object.position_z;
   //    msg.pose.orientation.x = 0;
   //    msg.pose.orientation.y = 0;
   //    msg.pose.orientation.z = 0;
   //    msg.pose.orientation.w = 1;

   //    if(x < -4.5 && x > -5.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   // }   
   // else if(proc == 5) {
   //    msg.header.stamp = ros::Time::now();
   //    msg.header.seq=count;
   //    msg.header.frame_id = 1;
   //    msg.pose.position.x = 0.0;//0.001*some_object.position_x;
   //    msg.pose.position.y = 0.0;//0.001*some_object.position_y;
   //    msg.pose.position.z = 2.0;//0.001*some_object.position_z;
   //    msg.pose.orientation.x = 0;
   //    msg.pose.orientation.y = 0;
   //    msg.pose.orientation.z = 0;
   //    msg.pose.orientation.w = 1;

   //    if(x > -0.5 && x < 0.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc=0;
   // }   


   count++; 

   ROS_INFO("proc = %d", proc);
}

int main(int argc, char **argv)
{
   // ros initialize
   ros::init(argc, argv, "mavros_teleop_key");
   ros::NodeHandle n;

   // ros publisher
   ros::Publisher chatter_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",100);
   ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/mavros/setpoint_velocity/cmd_vel",100);

   // ros subscriber
   ros::Subscriber pose_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 100, boost::bind(poseCallback, _1, chatter_pub, vel_pub));

   ros::spin();
      
   return 0;
}
