#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <stdio.h>

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Vector3Stamped.h"
#include "geometry_msgs/Twist.h"

int count = 1;
int proc = 0;
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& pose_msg,ros::Publisher _pub, ros::Publisher _vel_pub) {
   float x = pose_msg->pose.position.x; 
   float y = pose_msg->pose.position.y; 
   float z = pose_msg->pose.position.z; 
   //ROS_INFO("x = %f", x);
   //ROS_INFO("y = %f", y);
   //ROS_INFO("z = %f", z);

   geometry_msgs::Twist vel_msg;
   vel_msg.linear.x = 0.0;
   vel_msg.linear.y = 0.0;
   vel_msg.linear.z = 0.0;
   vel_msg.angular.x = 0.0;
   vel_msg.angular.y = 0.0;
   vel_msg.angular.z = 0.0;
   _vel_pub.publish(vel_msg);
   ros::spinOnce();

   geometry_msgs::PoseStamped msg;

   if(!proc) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 0.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > -0.5 && x < 0.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   }
   else if(proc == 1) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > 4.5 && x < 5.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   }
   else if(proc == 2) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 10.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > 4.5 && x < 5.5 && y > 9.5 && y < 10.5 && z > 1.5 && z < 2.5) proc++;
   }
   else if(proc == 3) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = -5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 10.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x < -4.5 && x > -5.5 && y > 9.5 && y < 10.5 && z > 1.5 && z < 2.5) proc++;
   }   
   else if(proc == 4) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = -5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x < -4.5 && x > -5.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc++;
   }   
   else if(proc == 5) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 0.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 2.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > -0.5 && x < 0.5 && y > -0.5 && y < 0.5 && z > 1.5 && z < 2.5) proc=0;
   }   


   _pub.publish(msg);
   ros::spinOnce();
   count++;   

   ROS_INFO("proc = %d", proc);
}

int main(int argc, char **argv)
{
   // ros initialize
   ros::init(argc, argv, "nav_route");
   ros::NodeHandle n;

   // ros publisher
   ros::Publisher chatter_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",100);
   ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/mavros/setpoint_velocity/cmd_vel",100);

   // ros subscriber
   ros::Subscriber pose_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 100, boost::bind(poseCallback, _1, chatter_pub, vel_pub));

   ros::spin();
      
   return 0;
}
