#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <apriltags_ros/AprilTagDetectionArray.h>

class CameraPositioner {
private:
   ros::Subscriber sub;

   // TF communication channels
   tf::TransformListener listener;
   tf::TransformBroadcaster br;

   // constant transforms
   tf::StampedTransform optical_transform;
   tf::StampedTransform world_tag_transform;

   // latest measured position of the camera
   tf::Transform world_camera_transform;
   ros::Time latest_detection_time;

   // for successful initialization the apriltag has to be detected _once_
   bool initialized;

public:
   CameraPositioner() : initialized(false)
   {
      ros::NodeHandle node;
      getConstantTransforms();
      sub = node.subscribe("tag_detections", 1, &CameraPositioner::callback, this);
   }

   void getConstantTransforms(){
      while(true){
         try {
            listener.waitForTransform("/world", "/april_tag_ur5", ros::Time(0), ros::Duration(5.0) );
            listener.lookupTransform("/world", "/april_tag_ur5", ros::Time(0), world_tag_transform);
            tf::Quaternion q;
            q.setRPY(1.57079,0,1.57079);
            world_tag_transform.setRotation(q);
            break;
         }
         catch(...){}
         ROS_WARN_THROTTLE(10, "Waiting for world->april_tag_ur5 transform");
      }

      while(true){
         try {
            listener.waitForTransform("/camera_rgb_optical_frame", "/camera_link", ros::Time(0), ros::Duration(5.0) );
            listener.lookupTransform("/camera_rgb_optical_frame", "/camera_link",  ros::Time(0), optical_transform);
            break;
         }
         catch(...){}
         ROS_WARN_THROTTLE(10, "Waiting for camera_rgb_optical_frame->camera_link transform");
      }
   }

   void callback(const apriltags_ros::AprilTagDetectionArray& msg){
      // if we got a valid tag detection, update world_camera_transform
      if(msg.detections.size() == 1 && msg.detections[0].id == 0){
         tf::Transform tag_transform;
         tf::poseMsgToTF(msg.detections[0].pose.pose, tag_transform);
         world_camera_transform= world_tag_transform * tag_transform.inverse() * optical_transform;
         if(!initialized){
            ROS_INFO("camera positioner is running");
            initialized = true;
         }
         latest_detection_time = msg.detections[0].pose.header.stamp;
      }

      if(ros::Time::now() - latest_detection_time > ros::Duration(20.0)){
         ROS_WARN_THROTTLE(5, "Didn't detect apriltag for camera position update in 20 seconds. The camera might have moved in the meanwhile.");
      }

      // if we measured the camera's position successfully, publish it
      if(initialized){
         br.sendTransform(tf::StampedTransform(world_camera_transform, ros::Time::now(), "/world", "/camera_link"));
      }
   }
};

int main(int argc, char** argv){
  ros::init(argc, argv, "camera_position_node");
  CameraPositioner cam_pos;
  ros::spin();
  return 0;
};
