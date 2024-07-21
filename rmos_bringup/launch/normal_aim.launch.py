import os
import sys
import yaml
from ament_index_python.packages import get_package_share_directory
from launch.substitutions import Command
sys.path.append(os.path.join(os.getcwd(), 'rmos_bringup', 'launch'))

def get_params(name):
    return os.path.join(os.getcwd(), 'rmos_bringup', 'config', 'node_params', '{}_params.yaml'.format(name))

def generate_launch_description():
    from launch_ros.descriptions import ComposableNode
    from launch_ros.actions import ComposableNodeContainer, Node, SetParameter, PushRosNamespace
    from launch.actions import TimerAction, Shutdown
    from launch import LaunchDescription

    launch_params = yaml.safe_load(open(os.path.join(
    os.getcwd(), 'rmos_bringup', 'config', 'launch_params.yaml')))

    if launch_params['video_play']: 
        daheng_node_  = Node(
            package='rmos_cam',
            executable='virtual_camera',
            output='screen',
            parameters=[get_params('camera')],
            # extra_arguments=[{'use_video': True}]
        )
    else:
        daheng_node_  = Node(
            package='rmos_cam',
            executable='daheng_camera',
            output='screen',
            parameters=[get_params('camera')],
            # extra_arguments=[{'use_intra_process_comms': True}]
        )
    
    basic_armor_detector_node_ = Node(
        package='rmos_detector',
        namespace= 'rmos_detector',
        executable='basic_detector',
        name='basic_detector',
        output='screen',
        parameters=[get_params('detector')],
    )

    processer_node_ = Node(
        package='rmos_processer',
        namespace= 'rmos_processer',
        executable='processer',
        name='processer',
        output='screen',
        parameters=[get_params('processer')],
    )


    rune_node_ = Node(
        package='rmos_rune',
        namespace= 'rmos_rune',
        executable='rune_detector',
        name='rune_detector',
        output='screen',
        parameters=[get_params('rune')],
    )


    if launch_params['virtual_serial']: 
        communicate_node_ = Node(
            package='rmos_ec',
            namespace= 'rmos_ec',
            executable='virtual_ec_node',
            name='rmos_ec',
            output='screen',
            parameters=[get_params('transporter')],
        )
    else:
        communicate_node_ = Node(
            package='rmos_transporter',
            namespace= 'rmos_transporter',
            executable='usb_comm',
            name='usb_comm',
            output='screen',
            parameters=[get_params('transporter')],
        )

    push_namespace = PushRosNamespace(launch_params['namespace'])

    # 延迟启动
    delay_basic_armor_detector_node_ = TimerAction(
        period=0.0,
        actions=[basic_armor_detector_node_],
    )
    
    delay_processer_node_ = TimerAction(
        period=3.0,
        actions=[processer_node_],
    )

    launch_description_list = [
        push_namespace,
        daheng_node_,
        delay_basic_armor_detector_node_,
        delay_processer_node_,
        communicate_node_
        ]

    if launch_params['rune_enable']:
        launch_description_list.append(rune_node_)
        launch_description_list.append(solver_)

    # Done
    return LaunchDescription(launch_description_list)