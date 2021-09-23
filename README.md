# TAMS UR5 setup

This repo provides all necessary drivers and descriptions for tams' full ur5 setup in the lab including a matching moveit configuration.

---

## Usage with the hardware

To bring up the whole arm configuration with all drivers, the corner urdf, moveit and the camera:

```
roslaunch tams_ur5_setup_bringup tams_ur5_setup_floor_camera.launch
```

This launch file is included in the tams_ur5_setup.launch. You can set the `use_kinect` argument to `true` to bringup the kinect drivers, to `false` to bringup the mechmind camera.

You can also use this launch file to directly bring up the arm without a camera:
```
roslaunch tams_ur5_setup_bringup tams_ur5_setup_ur_robot_driver.launch
```

---

## Usage with moveit demo mode

To start the setup without hardware, use the moveit demo mode:

```
roslaunch tams_ur5_setup_moveit_config demo.launch
```

This will bring up the same setup as `tams_ur5_setup.launch` and `rviz` but replaces the hardware drivers by moveits fake execution interfaces.

You can start this with a gui for the joint states of the ur5 and the gripper:

```
roslaunch tams_ur5_setup_moveit_config demo.launch use_gui:=true
```
