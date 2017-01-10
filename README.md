tams_ur5_setup
======

This repo provides all necessary drivers and descriptions for tams' full ur5 setup in the lab including a matching moveit configuration.

---

__Usage with the hardware__

To bring up the whole arm configuration with all drivers, the corner urdf, moveit and the xtion camera
you can use the following command:

```roslaunch tams_ur5_setup_bringup tams_ur5_setup.launch```


To bring up just the xtion camera with the apriltags_ros node and the camera positioner, use the following launch file:

```roslaunch tams_ur5_setup_bringup camera.launch```

This launch file is included in the tams_ur5_setup.launch. You can use the use_kinect argument to bringup
the kinect drivers instead, in case the xtion is replaced by a kinect camera.

You can also use this launch file to directly bring up the setup with a kinect.

```roslaunch tams_ur5_setup_bringup tams_ur5_setup_floor_kinect.launch```

---

__Usage with moveit demo mode__

To start the setup without hardware, use the moveit demo mode:

```roslaunch tams_ur5_setup_moveit_config demo.launch```

This will bring up the same setup as tams_ur5_setup.launch and rviz but replaces the hardware drivers by moveits fake execution interfaces.

You can start this with a gui for the joint states of the ur5 and the gripper:

```roslaunch tams_ur5_setup_moveit_config demo.launch use_gui:=true```
