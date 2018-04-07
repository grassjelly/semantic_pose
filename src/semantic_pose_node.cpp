#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <std_msgs/String.h>

#include "places.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "semantic_pose");

  ros::NodeHandle node;

  ros::Publisher location_pub = node.advertise<std_msgs::String>("location", 10);

  tf::TransformListener listener;

  Places places(node, node, "places");

  ros::Rate rate(10.0);

  while (node.ok()){
    tf::StampedTransform transform;
    ros::Time t = ros::Time(0);

    try{
      listener.waitForTransform("/map", "/base_footprint", t, ros::Duration(10.0));
      listener.lookupTransform("/map", "/base_footprint", t, transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }

    geometry_msgs::Point pt;
  
    pt.x = transform.getOrigin().getX();
    pt.y = transform.getOrigin().getY();

    std_msgs::String loc_msg;
    loc_msg.data = places.where_am_i(pt);
    location_pub.publish(loc_msg);
          
    rate.sleep();
  }
  return 0;
};