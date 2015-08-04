
// ros libs
#include "ros/ros.h"
#include "std_msgs/String.h"

// vrep libs
#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosStartSimulation.h"
#include "vrep_common/simRosStopSimulation.h"
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/JointSetStateData.h"
#include "../include/v_repConst.h"

bool simulationRunning=true;
float simulationTime=0.0f;

void infoCallback(const vrep_common::VrepInfo::ConstPtr& info)
{
	simulationTime=info->simulationTime.data;
	simulationRunning=(info->simulatorState.data&1)!=0;
}

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

	int *getMotorHandles(ros::NodeHandle n){

		int* motorHandlePtr = new int[4];

		ros::ServiceClient client_objectHandle = n.serviceClient<vrep_common::simRosGetObjectHandle>("/vrep/simRosGetObjectHandle");
		vrep_common::simRosGetObjectHandle srv_objectHandle;

		srv_objectHandle.request.objectName = "OmniWheel1";
		client_objectHandle.call(srv_objectHandle);
		motorHandlePtr[0] = srv_objectHandle.response.handle;

		srv_objectHandle.request.objectName = "OmniWheel2";
		client_objectHandle.call(srv_objectHandle);
		motorHandlePtr[1] = srv_objectHandle.response.handle;

		srv_objectHandle.request.objectName = "OmniWheel3";
		client_objectHandle.call(srv_objectHandle);
		motorHandlePtr[2] = srv_objectHandle.response.handle;

		srv_objectHandle.request.objectName = "OmniWheel4";
		client_objectHandle.call(srv_objectHandle);
		motorHandlePtr[3] = srv_objectHandle.response.handle;

		return motorHandlePtr;

	}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "sim_start");
  ros::NodeHandle n;
  std_msgs::String topicName;
  topicName.data = "omni";
	int motorHandles[4];

	ros::Subscriber subInfo=n.subscribe("/vrep/info",1,infoCallback);
  startStopSim(n,0);
	getMotorHandles(n);

  ros::ServiceClient client_enableSubscriber=n.serviceClient<vrep_common::simRosEnableSubscriber>("/vrep/simRosEnableSubscriber");
  vrep_common::simRosEnableSubscriber srv_enableSubscriber;

  srv_enableSubscriber.request.topicName="/"+topicName.data+"/wheels"; // the topic name
  srv_enableSubscriber.request.queueSize=1; // the subscriber queue size (on V-REP side)
  srv_enableSubscriber.request.streamCmd=simros_strmcmd_set_joint_state; // the subscriber type
  if (client_enableSubscriber.call(srv_enableSubscriber)&&(srv_enableSubscriber.response.subscriberID!=-1)) {


    ros::Publisher motorSpeedPub=n.advertise<vrep_common::JointSetStateData>("omni/wheels",1);
    while (ros::ok() && simulationRunning) {
      vrep_common::JointSetStateData motorSpeeds;
			int* motorHandlePtr;
			motorHandlePtr = getMotorHandles(n);


      // motorSpeeds.handles.data.push_back(motor1Handle);
			// motorSpeeds.handles.data.push_back(motor2Handle);
			// motorSpeeds.handles.data.push_back(motor3Handle);
			motorSpeeds.handles.data.push_back(motorHandlePtr[1]);
      // motorSpeeds.setModes.data.push_back(2); // 2 is the speed mode
      // motorSpeeds.setModes.data.push_back(2); // 2 is the speed mode
      // motorSpeeds.setModes.data.push_back(2); // 2 is the speed mode
      motorSpeeds.setModes.data.push_back(2); // 2 is the speed mode
      // motorSpeeds.values.data.push_back(1.0);
			// motorSpeeds.values.data.push_back(1.0);
			// motorSpeeds.values.data.push_back(1.0);
			motorSpeeds.values.data.push_back(1.0);
      motorSpeedPub.publish(motorSpeeds);

    }

	ros::shutdown();
	printf("simulation terminated!\n");
	return(0);
  }







  //  ros::Rate loop_rate(10);

  ros::spinOnce();

  //    loop_rate.sleep();



  return 0;
}
