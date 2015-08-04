#include "ros/ros.h"

#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosStartSimulation.h"
#include "vrep_common/simRosStopSimulation.h"

void startStopSim(ros::NodeHandle n, int s){

if (s == 0){

ros::ServiceClient client_simStart = n.serviceClient<vrep_common::simRosStartSimulation>("/vrep/simRosStartSimulation");
vrep_common::simRosStartSimulation srv_simStart;
if(client_simStart.call(srv_simStart)){
ROS_INFO("Started"); // TODO check if service call response is correct
}

}

else if (s == 1){

ros::ServiceClient client_simStop = n.serviceClient<vrep_common::simRosStopSimulation>("/vrep/simRosStopSimulation");
vrep_common::simRosStopSimulation srv_simStop;
if(client_simStop.call(srv_simStop)){
ROS_INFO("Started"); // TODO check if service call response is correct

}

}



}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "sim_start");
  ros::NodeHandle n;


//  ros::Rate loop_rate(10);

	startStopSim(n,0);


    ros::spinOnce();

//    loop_rate.sleep();
  


  return 0;
}
