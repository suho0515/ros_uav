#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <stdio.h>

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Vector3Stamped.h"

int count = 1;
int proc = 0;
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& pose_msg,ros::Publisher _pub) {
   float x = pose_msg->pose.position.x; 
   float y = pose_msg->pose.position.y; 
   float z = pose_msg->pose.position.z; 
   //ROS_INFO("x = %f", x);
   //ROS_INFO("y = %f", y);
   //ROS_INFO("z = %f", z);

   geometry_msgs::PoseStamped msg;
   
   if(!proc) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 0.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > -0.1 && x < 0.1 && y > -0.1 && y < 0.1 && z > 0.9 && z < 1.1) proc++;
   }
   else if(proc == 1) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > 4.9 && x < 5.1 && y > -0.1 && y < 0.1 && z > 0.9 && z < 1.1) proc++;
   }
   else if(proc == 2) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 10.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > 4.9 && x < 5.1 && y > 9.9 && y < 10.1 && z > 0.9 && z < 1.1) proc++;
   }
   else if(proc == 3) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = -5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 10.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x < -4.9 && x > -5.1 && y > 9.9 && y < 10.1 && z > 0.9 && z < 1.1) proc++;
   }   
   else if(proc == 4) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = -5.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x < -4.9 && x > -5.1 && y > -0.1 && y < 0.1 && z > 0.9 && z < 1.1) proc++;
   }   
   else if(proc == 5) {
      msg.header.stamp = ros::Time::now();
      msg.header.seq=count;
      msg.header.frame_id = 1;
      msg.pose.position.x = 0.0;//0.001*some_object.position_x;
      msg.pose.position.y = 0.0;//0.001*some_object.position_y;
      msg.pose.position.z = 1.0;//0.001*some_object.position_z;
      msg.pose.orientation.x = 0;
      msg.pose.orientation.y = 0;
      msg.pose.orientation.z = 0;
      msg.pose.orientation.w = 1;

      if(x > -0.1 && x < 0.1 && y > -0.1 && y < 0.1 && z > 0.9 && z < 1.1) proc=0;
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

   // ros subscriber
   ros::Subscriber pose_sub = n.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 100, boost::bind(poseCallback, _1, chatter_pub));

   ros::spin();
   //ros::Rate loop_rate(100);
   //ros::spinOnce();

   //geometry_msgs::PoseStamped msg;
   //int count = 1;
	
		//PositionReciever qp;:
		//Body some_object;
		//qp.connect_to_server();

	
   //while(ros::ok()){
	   //some_object = qp.getStatus();
		// some_object.print();
		//printf("%f\n",some_object.position_x);
       //msg.header.stamp = ros::Time::now();
       //msg.header.seq=count;
       //msg.header.frame_id = 1;
       //msg.pose.position.x = 0.0;//0.001*some_object.position_x;
       //msg.pose.position.y = 0.0;//0.001*some_object.position_y;
       //msg.pose.position.z = 1.0;//0.001*some_object.position_z;
       //msg.pose.orientation.x = 0;
       //msg.pose.orientation.y = 0;
       //msg.pose.orientation.z = 0;
       //msg.pose.orientation.w = 1;

       //chatter_pub.publish(msg);
       //ros::spinOnce();
       //count++;
       //loop_rate.sleep();
   //}
   
      
   return 0;
}
