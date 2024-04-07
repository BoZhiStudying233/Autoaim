import os

from unicodedata import name

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch.substitutions import PythonExpression, LaunchConfiguration
from launch.conditions import IfCondition
import yaml

def generate_launch_description():

    Id = LaunchDescription()

    # add node
    daheng_node_ = Node(
        package='rmos_cam',
        namespace='rmos_cam',
        executable='daheng_camera',
        name='daheng_camera',
        output='screen',
    )
    # basic_armor_detector_node_ = Node(
    #     package='rmos_detector',
    #     namespace= 'rmos_detector',
    #     executable='basic_detector',
    #     name='basic_detector',
    #     output='screen',
    # )

    
    # dl_armor_detector_node_ = Node(
    #     package='rmos_detector',
    #     namespace= 'rmos_detector',
    #     executable='dl_detector',
    #     name='dl_detector',
    #     output='screen',

    # )

    # communicate_node_ = Node(
    #     package='rmos_transporter',
    #     namespace= 'rmos_transporter',
    #     executable='can_comm',
    #     name='can_comm',
    #     output='screen',
    # )


    communicate_node_ = Node(
        package='rmos_transporter',
        namespace= 'rmos_transporter',
        executable='usb_comm',
        name='usb_comm',
        parameters=[os.path.join(get_package_share_directory('rmos_bringup'), 
                   'config', 'hardware_settings.yaml')],
        output='screen',
    )

    processer_node = Node(
        package='rmos_processer',
        namespace= 'rmos_processer',
        executable='processer',
        name='processer',
        output='screen',
    )



    rune_detector_node = Node(
        package='rmos_detector',
        namespace= 'rmos_detector',
        executable= 'rune_detector',
        name='rune_detector',
        output='screen',
    )

    # Done

    # Id.add_action(mix_detector_node)
    # Id.add_action(rune_detector_node)
    # Id.add_action(processer_node)
    # Id.add_action(communicate_node_)
    # Id.add_action(basic_armor_detector_node_)
    # Id.add_action(daheng_node_)

    return LaunchDescription([

        # dl_armor_detector_node_,
        daheng_node_,
        rune_detector_node,
        communicate_node_,
        processer_node,


    ])
    # return LaunchDescription
