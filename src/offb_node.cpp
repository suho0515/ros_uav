//initially include all header files necessary for compiling the subscribers, publishers and services 
//which are going to be used in the code

 #include <ros/ros.h>

 #include <geometry_msgs/PoseStamped.h>
 #include <mavros_msgs/CommandBool.h>
 #include <mavros_msgs/SetMode.h>
 #include <mavros_msgs/State.h>

 #include <mavros_msgs/CommandLong.h>
 #include <mavros_msgs/CommandTOL.h>
 #include <mavros_msgs/PositionTarget.h>

 //creating a callback 'state_cb' to store the current state of the copter in variable 'current_state' 
 //this will be called by subscriber 'state_sub' when a message arrives in the 'mavros/state' topic 
//which is of data type 'mavros_msgs/State'

mavros_msgs::State current_state;              //this sets the variable type for 'current_state' 
                                                                          //variable
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

  //the main ros function with all subscribers, publishers and services

int main(int argc, char **argv)
{

   ros::init(argc, argv, "drone_nav_node");  //initialising ros, with node name 'offn_node'
   ros::NodeHandle nh;                  //main accesspoint for this node to communicate with the system 

  ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>      //subscriber 'state_sub' is subscribing to topic 'mavros/state of type mavros_msgs/State by calling state_cb
     ("mavros/state", 10, state_cb);
   ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped> //publisher 'local_pos_pub' topic 'maros/.../local' type 'geo../PoseStamped'
     ("mavros/setpoint_attitude/thrust", 10);
   ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>  //serviceclient 'arming_client' type(header) 'mav../Com..Bool' service 'mavros/cmd/arming'
     ("mavros/cmd/arming");
       ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>   //serviceclient 'set_mode_client' type(header) 'mav../SetMode' service 'mavros/set_mode'
     ("mavros/set_mode");

  //the setpoint publishing rate MUST be faster than 2Hz(=>1cycle/0.5sec); since pxhawk has a 
  //timeout of 0.5sec
      ros::Rate rate(20.0);                               //here we give 20Hz

 // wait for FCU connection, as soon as the bool 'cuttent_state.connected' becomes true while loop 
//will be exited(!true=false)
while(ros::ok() && !current_state.connected){
 ros::spinOnce();
 rate.sleep();
}
ROS_INFO("Connection with FCU has been establised.");

//initialising pose variable of datatype geo../PoseStamped and assigning (from mavros doc 
//pose.position.)x,y & z values in ros framesystem(ENU)
//this will be converted to pixhawk NED frame by mavros
 geometry_msgs::PoseStamped pose;
 pose.pose.position.x = 0.0;
 pose.pose.position.y = 0.0;
 pose.pose.position.z = 0.0;

 //send a few setpoints before starting, here we send 100 times
 for(int i = 100; ros::ok() && i > 0; --i){
 local_pos_pub.publish(pose);                   //calling the publisher 'local_pos_pub' to publish the pose
 ros::spinOnce();                             
 rate.sleep();                                  //at the rate of 20Hz
  }

//initialise/define variable of type(header) mavros_msgs/SetMode to set modes of the copter
//this variable will be called by the serviceclient 'set_mode_client'
 mavros_msgs::SetMode offb_set_mode;
 offb_set_mode.request.custom_mode = "GUIDED_NOGPS";    //OFFBOARD node since we are 
                                                                                                      //using px4 flight stack
 offb_set_mode.request.base_mode = 0.0;

//initialise variables of type mavros_msgs/CommandBool to arm&disarm the copter
//thes variables will be called by the serviceclient 'arming_client'
   mavros_msgs::CommandBool arm_cmd;
   arm_cmd.request.value = true;                       //true=> armig the copter
   mavros_msgs::CommandBool disarm_cmd;
   disarm_cmd.request.value = false;                   //fals=> disarmig the copter


//initialise a variable to store the current time (used below to have a control over the time gap 
//between requests/commands send to the pixhawk from RPi)
ros::Time last_request = ros::Time::now();

 //making 5sec delay between adjacscent requests
//first sets the mode of the copter by calling seviceclient 'set_mode_client' and also make sure it is 
//sent by using service response - bool 'response.mode_sent'
//then arms the copter by calling service 'arming_client' and also make sure this was 
//communicated successfully using service response - bool 'response.success'
//after that keep sending the pose(set_points) to the drone
 
while(ros::ok() && !current_state.armed){

 if( current_state.mode != "GUIDED_NOGPS" &&
     (ros::Time::now() - last_request > ros::Duration(5.0))){
     if( set_mode_client.call(offb_set_mode) &&
         offb_set_mode.response.mode_sent){
         ROS_INFO("GUIDED_NOGPS enabled");
     }
     last_request = ros::Time::now();

 } else {
     if( !current_state.armed &&
         (ros::Time::now() - last_request > ros::Duration(5.0))){
         if( arming_client.call(arm_cmd) &&
             arm_cmd.response.success){
             ROS_INFO("Vehicle Armed");
         }
         last_request = ros::Time::now();
     }
 }

 local_pos_pub.publish(pose);
 ROS_INFO("Startposition z=%f", pose.pose.position.z);

 ros::spinOnce();
 rate.sleep();
 }

last_request = ros::Time::now();

    //new takeoff command
    //request takeoff
    ros::ServiceClient takeoff_cl = nh.serviceClient<mavros_msgs::CommandTOL> 
                      ("/mavros/cmd/takeoff");
    mavros_msgs::CommandTOL srv_takeoff;
    srv_takeoff.request.altitude = 1.5;

    if(takeoff_cl.call(srv_takeoff)){
        ROS_INFO("takeoff sent %d", srv_takeoff.response.success);
        ROS_INFO("takeoff result %d", srv_takeoff.response.result);
    }
    else{
        ROS_ERROR("Failed Takeoff");
        return -1;
    }




    //after 5sec giving command to drone to takeoff to an altitude of 1.5m
    while(current_state.armed){

        if( ros::Time::now() - last_request > ros::Duration(5.0)){
            ROS_INFO("Initiating Takeoff");

            pose.pose.position.z = 1.5;

        for(int i = 0; ros::ok() && i < 10*20; ++i){   // sending altitude command continuously for around 3.33min(200sec)  
            local_pos_pub.publish(pose);
            ros::spinOnce();
            rate.sleep();
        }
            ROS_INFO("Takeoff done to an altitude of z=%f !", pose.pose.position.z);                
        }
    }
    return 0;
}
